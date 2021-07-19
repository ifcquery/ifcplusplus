/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <array>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <earcut.hpp/include/mapbox/earcut.hpp>

#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "GeomDebugDump.h"
#include "CSG_Adapter.h"

class GeometrySettings;
class UnitConverter;

class Sweeper : public StatusCallback
{
public:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;

	Sweeper(shared_ptr<GeometrySettings>& settings, shared_ptr<UnitConverter>& uc) : m_geom_settings(settings), m_unit_converter(uc) {}
	virtual ~Sweeper() {}

	/*\brief Extrudes a set of cross sections along a direction
	  \param[in] paths Set of cross sections to extrude
	  \param[in] dir Extrusion vector
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] item_data Container to add result polyhedron or polyline
	**/
	void extrude(const std::vector<std::vector<vec2> >& face_loops_input, const vec3 extrusion_vector, BuildingEntity* ifc_entity, shared_ptr<ItemShapeData>& item_data)
	{
		// TODO: complete and test
		if (face_loops_input.size() == 0)
		{
			messageCallback("profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			return;
		}

		// figure 1: loops and indexes
		//  3----------------------------2
		//  |                            |
		//  |   1-------------------2    |3---------2
		//  |   |                   |    |          |
		//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
		//  |   0---face_loops[1]---3    |0---------1
		//  |                            |
		//  0-------face_loops[0]--------1

		std::vector<std::vector<std::vector<vec2> > > profile_paths_enclosed;
		findEnclosedLoops(face_loops_input, profile_paths_enclosed);

		for (size_t ii_profile_paths = 0; ii_profile_paths < profile_paths_enclosed.size(); ++ii_profile_paths)
		{
			const std::vector<std::vector<vec2> >& face_loops_enclosed = profile_paths_enclosed[ii_profile_paths];

			size_t num_points_in_loops = 0;
			for (size_t ii = 0; ii < face_loops_enclosed.size(); ++ii)
			{
				num_points_in_loops += face_loops_enclosed[ii].size();
			}
			if (num_points_in_loops == 0)
			{
				continue;
			}

			shared_ptr<carve::input::PolyhedronData> poly_data(new carve::input::PolyhedronData());
			poly_data->points.resize(num_points_in_loops * 2);

			std::vector<vec3>& polyhedron_points = poly_data->points;
			size_t polyhedron_point_index = 0;
			for (size_t ii = 0; ii < face_loops_enclosed.size(); ++ii)
			{
				const std::vector<vec2>& loop = face_loops_enclosed[ii];
				for (size_t jj = 0; jj < loop.size(); ++jj)
				{
					const vec2& vec_2d = loop[jj];
					// cross section is defined in XY plane
					polyhedron_points[polyhedron_point_index] = carve::geom::VECTOR(vec_2d.x, vec_2d.y, 0);
					polyhedron_points[polyhedron_point_index + num_points_in_loops] = carve::geom::VECTOR(vec_2d.x, vec_2d.y, 0) + extrusion_vector;
					++polyhedron_point_index;
				}
			}

			bool flip_faces = false;
			vec3 normal_first_loop = GeomUtils::computePolygon2DNormal(face_loops_enclosed[0]);
			double extrusion_dot_normal = dot(extrusion_vector, normal_first_loop);
			if (extrusion_dot_normal < 0)
			{
				flip_faces = true;
			}


			// add face loops for all sections
			const size_t num_poly_points = polyhedron_points.size();
			size_t loop_offset = 0;
			for (size_t ii = 0; ii < face_loops_enclosed.size(); ++ii)
			{
				const std::vector<vec2>& loop = face_loops_enclosed[ii];

				for (size_t jj = 0; jj < loop.size(); ++jj)
				{
					size_t tri_idx_a = jj + loop_offset;

					size_t tri_idx_next = tri_idx_a + 1;
					if (jj == loop.size() - 1)
					{
						tri_idx_next -= loop.size();
					}
					size_t tri_idx_up = tri_idx_a + num_points_in_loops;
					size_t tri_idx_next_up = tri_idx_next + num_points_in_loops;

					if (flip_faces)
					{
						poly_data->addFace(tri_idx_a, tri_idx_next_up, tri_idx_next);
						poly_data->addFace(tri_idx_next_up, tri_idx_a, tri_idx_up);
					}
					else
					{
						poly_data->addFace(tri_idx_a, tri_idx_next, tri_idx_next_up);
						poly_data->addFace(tri_idx_next_up, tri_idx_up, tri_idx_a);
					}
				}
				loop_offset += loop.size();
			}

			// add bottom and top cap
			std::vector<int> cap_triangle_indexes;
			triangulateLoops(face_loops_enclosed, cap_triangle_indexes, ifc_entity);
			if (cap_triangle_indexes.size() > 2)
			{
				// triangles are clockwise, which is correct for bottom cap (opposite of extrusion vector)
				bool flip_cap_faces = extrusion_vector.z > 0;
				for (size_t ii = 2; ii < cap_triangle_indexes.size(); ii += 3)
				{
					int idx0 = cap_triangle_indexes[ii-2];
					int idx1 = cap_triangle_indexes[ii-1];
					int idx2 = cap_triangle_indexes[ii];
					if (flip_cap_faces)
					{
						std::swap(idx0, idx2);
					}

#ifdef _DEBUG
					std::vector<vec2> triangle;
					triangle.push_back(carve::geom::VECTOR(polyhedron_points[idx0].x, polyhedron_points[idx0].y));
					triangle.push_back(carve::geom::VECTOR(polyhedron_points[idx1].x, polyhedron_points[idx1].y));
					triangle.push_back(carve::geom::VECTOR(polyhedron_points[idx2].x, polyhedron_points[idx2].y));
					vec3 normal = GeomUtils::computePolygon2DNormal(triangle);

					double extrusion_dot_normal = dot(extrusion_vector, normal);
					if (extrusion_dot_normal > 0)
					{
						std::cout << "normal vector of triangle and extrusion vector should point in opposite direction"<< std::endl;
					}
#endif
					// bottom cap
					poly_data->addFace(idx0, idx1, idx2);

					// top cap
					idx0 = idx0 + num_poly_points - num_points_in_loops;
					idx1 = idx1 + num_poly_points - num_points_in_loops;
					idx2 = idx2 + num_poly_points - num_points_in_loops;
					poly_data->addFace(idx2, idx1, idx0);
				}
			}

			try
			{
#ifdef _DEBUG
				shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts()));
				bool valid = CSG_Adapter::checkMeshSetValidAndClosed(meshset, this, ifc_entity);
				if (!valid)
				{
					carve::geom::vector<4> color = carve::geom::VECTOR(0.3, 0.4, 0.5, 1.0);
					GeomDebugDump::dumpMeshset(meshset, color, true);
					GeomDebugDump::clearMeshsetDump();
				}
#endif

				item_data->addClosedPolyhedron(poly_data);

			}
			catch (BuildingException& exception)
			{
#ifdef _DEBUG
				std::cout << exception.what() << std::endl;
				shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts()));
				carve::geom::vector<4> color = carve::geom::VECTOR(0.3, 0.4, 0.5, 1.0);
				GeomDebugDump::dumpMeshset(meshset, color, true);
#endif
				messageCallback(exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity);  // calling function already in e.what()
			}
		}
	}

	/*\brief Extrudes a circle cross section along a path. At turns, the points are placed in the bisecting plane
	  \param[in] curve_points Path along which the circle is swept
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] item_data Container to add result polyhedron or polyline
	  \param[in] nvc Number of vertices per circle
	  \param[in] radius_inner If positive value is given, the swept disk becomes a pipe
	**/
	void sweepDisk(const std::vector<vec3>& curve_points, BuildingEntity* ifc_entity, shared_ptr<ItemShapeData>& item_data, const size_t nvc, const double radius, const double radius_inner = -1)
	{
		const size_t num_curve_points = curve_points.size();
		if (num_curve_points < 2)
		{
			messageCallback("num curve points < 2", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			return;
		}

		if (!item_data)
		{
			messageCallback("!item_data", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			return;
		}

		if (radius < 0.001)
		{
			// Cross section is just a point. Create a polyline
			shared_ptr<carve::input::PolylineSetData> polyline_data(new carve::input::PolylineSetData());
			if (!polyline_data)
			{
				throw OutOfMemoryException(__FUNC__);
			}
			polyline_data->beginPolyline();
			for (size_t i_polyline = 0; i_polyline < curve_points.size(); ++i_polyline)
			{
				const vec3& curve_pt = curve_points[i_polyline];
				polyline_data->addVertex(curve_pt);
				polyline_data->addPolylineIndex(0);
				polyline_data->addPolylineIndex(i_polyline);
			}
			item_data->m_polylines.push_back(polyline_data);
			return;
		}

		double use_radius_inner = radius_inner;
		if (radius_inner > radius)
		{
			messageCallback("radius_inner > radius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			use_radius_inner = radius;
		}

		vec3 local_z(carve::geom::VECTOR(0, 0, 1));
		vec3 curve_point_first = curve_points[0];
		vec3 curve_point_second = curve_points[1];

		bool bend_found = false;
		if (num_curve_points > 3)
		{
			// compute local z vector by dot product of the first bend of the reference line
			vec3 vertex_back2 = curve_point_first;
			vec3 vertex_back1 = curve_point_second;
			for (size_t i = 2; i<num_curve_points; ++i)
			{
				const vec3& vertex_current = curve_points[i];
				vec3 section1 = vertex_back1 - vertex_back2;
				vec3 section2 = vertex_current - vertex_back1;
				section1.normalize();
				section2.normalize();

				double dot_product = dot(section1, section2);
				double dot_product_abs = std::abs(dot_product);

				// if dot == 1 or -1, then points are colinear
				if (dot_product_abs < (1.0-0.0001) || dot_product_abs >(1.0+0.0001))
				{
					// bend found, compute cross product
					vec3 lateral_vec = cross(section1, section2);
					local_z = cross(lateral_vec, section1);
					local_z.normalize();
					bend_found = true;
					break;
				}
			}
		}

		if (!bend_found)
		{
			// sweeping curve is linear. assume any local z vector
			vec3 sweep_dir = curve_point_second - curve_point_first;
			if (sweep_dir.length2() > 0.1)
			{
				sweep_dir.normalize();

				double dot_sweep_dir = dot(sweep_dir, carve::geom::VECTOR(0, 0, 1));
				if (std::abs(dot_sweep_dir-1.0) > 0.0001)
				{
					local_z = cross(carve::geom::VECTOR(0, 0, 1), sweep_dir);
					if (local_z.length2() < 0.001)
					{
						local_z = cross(carve::geom::VECTOR(0, 1, 0), sweep_dir);
						local_z.normalize();
					}
					else
					{
						local_z.normalize();
					}
					double dot_normal_local_z = dot(sweep_dir, local_z);
					if (std::abs(dot_normal_local_z-1.0) < 0.0001)
					{
						local_z = cross(carve::geom::VECTOR(0, 1, 0), sweep_dir);
						local_z.normalize();

						dot_normal_local_z = dot(sweep_dir, local_z);
						if (std::abs(dot_normal_local_z-1.0) < 0.0001)
						{
							local_z = cross(carve::geom::VECTOR(1, 0, 0), sweep_dir);
							local_z.normalize();
						}
					}
				}
			}
		}

		// rotate disk into first direction
		vec3  section_local_y = local_z;
		vec3  section_local_z = curve_point_first - curve_point_second;
		vec3  section_local_x = carve::geom::cross(section_local_y, section_local_z);
		section_local_y = carve::geom::cross(section_local_x, section_local_z);
		std::vector<vec3> inner_shape_points;

		section_local_x.normalize();
		section_local_y.normalize();
		section_local_z.normalize();

		carve::math::Matrix matrix_first_direction = carve::math::Matrix(
			section_local_x.x, section_local_y.x, section_local_z.x, 0,
			section_local_x.y, section_local_y.y, section_local_z.y, 0,
			section_local_x.z, section_local_y.z, section_local_z.z, 0,
			0, 0, 0, 1);

		double angle = 0;
		double delta_angle = 2.0*M_PI/double(nvc);	// TODO: adapt to model size and complexity
		std::vector<vec3> circle_points(nvc);
		std::vector<vec3> circle_points_inner(nvc);
		for (size_t ii = 0; ii < nvc; ++ii)
		{
			// cross section (circle) is defined in XY plane
			double x = sin(angle);
			double y = cos(angle);
			vec3 vertex(carve::geom::VECTOR(x*radius, y*radius, 0.0));
			vertex = matrix_first_direction*vertex + curve_point_first;
			circle_points[ii] = vertex;

			if (use_radius_inner > 0)
			{
				vec3 vertex_inner(carve::geom::VECTOR(x*use_radius_inner, y*use_radius_inner, 0.0));
				vertex_inner = matrix_first_direction*vertex_inner + curve_point_first;
				circle_points_inner[ii] = vertex_inner;
			}
			angle += delta_angle;
		}

		shared_ptr<carve::input::PolyhedronData> poly_data(new carve::input::PolyhedronData());
		if (!poly_data)
		{
			throw OutOfMemoryException(__FUNC__);
		}

		for (size_t ii = 0; ii<num_curve_points; ++ii)
		{
			const vec3& vertex_current = curve_points[ii];
			vec3 vertex_next;
			vec3 vertex_before;
			if (ii == 0)
			{
				// first point
				vertex_next = curve_points[ii+1];
				vec3 delta_element = vertex_next - vertex_current;
				vertex_before = vertex_current - (delta_element);
			}
			else if (ii == num_curve_points-1)
			{
				// last point
				vertex_before = curve_points[ii-1];
				vec3 delta_element = vertex_current - vertex_before;
				vertex_next = vertex_before + (delta_element);
			}
			else
			{
				// inner point
				vertex_next = curve_points[ii+1];
				vertex_before = curve_points[ii-1];
			}

			vec3 bisecting_normal;
			GeomUtils::bisectingPlane(vertex_before, vertex_current, vertex_next, bisecting_normal);

			vec3 section1 = vertex_current - vertex_before;
			vec3 section2 = vertex_next - vertex_current;
			if (section1.length2() > 0.0001)
			{
				section1.normalize();
				if (section2.length2() > 0.0001)
				{
					section2.normalize();
					double dot_product = dot(section1, section2);
					double dot_product_abs = std::abs(dot_product);

					if (dot_product_abs < (1.0-0.0001) || dot_product_abs >(1.0+0.0001))
					{
						// bend found, compute next local z vector
						vec3 lateral_vec = cross(section1, section2);
						if (lateral_vec.length2() > 0.0001)
						{
							local_z = cross(lateral_vec, section1);
							local_z.normalize();
						}
					}
				}
			}

			if (ii == num_curve_points -1)
			{
				bisecting_normal *= -1.0;
			}

			carve::geom::plane<3> bisecting_plane(bisecting_normal, vertex_current);
			for (size_t jj = 0; jj < nvc; ++jj)
			{
				vec3& vertex = circle_points[jj];

				vec3 v;
				double t;
				carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection(bisecting_plane, vertex, vertex + section1, v, t);
				if (intersect > 0)
				{
					vertex = v;
				}
				else
				{
					messageCallback("no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
				}

				poly_data->addVertex(vertex);
			}

			if (use_radius_inner > 0)
			{
				for (size_t jj = 0; jj < nvc; ++jj)
				{
					vec3& vertex = circle_points_inner[jj];

					vec3 v;
					double t;
					carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection(bisecting_plane, vertex, vertex + section1, v, t);
					if (intersect > 0)
					{
						vertex = v;
					}
					else
					{
						messageCallback("no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
					}

					//inner_shape_points[jj] = vertex;
					inner_shape_points.push_back(vertex);
				}
			}
		}

		// outer shape
		size_t num_vertices_outer = poly_data->getVertexCount();
		for (size_t i = 0; i<num_curve_points- 1; ++i)
		{
			size_t i_offset = i*nvc;
			size_t i_offset_next = (i + 1)*nvc;
			for (size_t jj = 0; jj < nvc; ++jj)
			{
				size_t current_loop_pt1 = jj + i_offset;
				size_t current_loop_pt2 = (jj + 1) % nvc + i_offset;

				size_t next_loop_pt1 = jj + i_offset_next;
				size_t next_loop_pt2 = (jj + 1) % nvc + i_offset_next;
#ifdef _DEBUG
				if (current_loop_pt1 >= num_vertices_outer || current_loop_pt2 >= num_vertices_outer || next_loop_pt1 >= num_vertices_outer || next_loop_pt2 >= num_vertices_outer)
				{
					std::cout << "current_loop_pt1 >= num_vertices_outer" << std::endl;
				}
#endif
				poly_data->addFace(current_loop_pt1, next_loop_pt1, next_loop_pt2);
				poly_data->addFace(next_loop_pt2, current_loop_pt2, current_loop_pt1);
			}
		}

		if (use_radius_inner > 0)
		{
			if (inner_shape_points.size() != num_vertices_outer)
			{
				messageCallback("inner_shape_points.size() != num_vertices_outer", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			}

			// add points for inner shape
			for (size_t i = 0; i<inner_shape_points.size(); ++i)
			{
				poly_data->addVertex(inner_shape_points[i]);
			}

			// faces of inner shape
			for (size_t i = 0; i<num_curve_points- 1; ++i)
			{
				size_t i_offset = i*nvc + num_vertices_outer;
				size_t i_offset_next = (i + 1)*nvc + num_vertices_outer;
				for (size_t jj = 0; jj < nvc; ++jj)
				{
					size_t current_loop_pt1 = jj + i_offset;
					size_t current_loop_pt2 = (jj + 1) % nvc + i_offset;

					size_t next_loop_pt1 = jj + i_offset_next;
					size_t next_loop_pt2 = (jj + 1) % nvc + i_offset_next;

					poly_data->addFace(current_loop_pt1, current_loop_pt2, next_loop_pt2);
					poly_data->addFace(next_loop_pt2, next_loop_pt1, current_loop_pt1);
				}
			}

			// front cap
			for (size_t jj = 0; jj < nvc; ++jj)
			{
				size_t outer_rim_next = (jj + 1) % nvc;
				size_t inner_rim_next = outer_rim_next + num_vertices_outer;

				poly_data->addFace(jj, outer_rim_next, num_vertices_outer+jj);
				poly_data->addFace(outer_rim_next, inner_rim_next, num_vertices_outer+jj);
			}

			// back cap
			size_t back_offset = (num_curve_points - 1)*nvc;
			for (size_t jj = 0; jj < nvc; ++jj)
			{
				size_t outer_rim_next = (jj + 1) % nvc + back_offset;
				size_t inner_rim_next = outer_rim_next + num_vertices_outer;

				poly_data->addFace(jj+back_offset, num_vertices_outer+jj+back_offset, outer_rim_next);
				poly_data->addFace(outer_rim_next, num_vertices_outer+jj+back_offset, inner_rim_next);
			}
		}
		else
		{
#ifdef _DEBUG
			if (nvc >= num_vertices_outer)
			{
				std::cout << "nvc >= num_vertices_outer" << std::endl;
			}
#endif
			// front cap, full pipe, create triangle fan
			for (size_t jj = 0; jj < nvc - 2; ++jj)
			{
				poly_data->addFace(0, jj+1, jj+2);
			}

			// back cap
			size_t back_offset = (num_curve_points - 1)*nvc;
			for (size_t jj = 0; jj < nvc - 2; ++jj)
			{
				poly_data->addFace(back_offset, back_offset+jj+2, back_offset+jj+1);
			}
		}

		try
		{
			item_data->addClosedPolyhedron(poly_data);
		}
		catch (BuildingException& exception)
		{
			messageCallback(exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity);  // calling function already in e.what()
#ifdef _DEBUG
			shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts()));
			carve::geom::vector<4> color = carve::geom::VECTOR(0.7, 0.7, 0.7, 1.0);
			GeomDebugDump::dumpMeshset(meshset, color, true);
#endif
		}

#ifdef _DEBUG
		shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts()));
		CSG_Adapter::checkMeshSetValidAndClosed(meshset, this, ifc_entity);
#endif
	}

	void findEnclosedLoops(const std::vector<std::vector<vec2> >& face_loops_input, std::vector<std::vector<std::vector<vec2> > >& profile_paths_enclosed)
	{
		if (face_loops_input.size() > 1)
		{
			const std::vector<vec2>& loop1 = face_loops_input[0];
			std::vector<std::vector<vec2> > enclosed_in_loop1;
			std::vector<std::vector<vec2> > separate_loops;
			enclosed_in_loop1.push_back(loop1);

			for (size_t ii = 1; ii < face_loops_input.size(); ++ii)
			{
				const std::vector<vec2>& loop = face_loops_input[ii];
				bool loop_enclosed_in_loop1 = GeomUtils::isEnclosed(loop, loop1);

				if (loop_enclosed_in_loop1)
				{
					enclosed_in_loop1.push_back(loop);
				}
				else
				{
					separate_loops.push_back(loop);
				}
			}

			profile_paths_enclosed.push_back(enclosed_in_loop1);
			if (separate_loops.size() > 0)
			{
				profile_paths_enclosed.push_back(separate_loops);
			}
		}
		else
		{
			profile_paths_enclosed.push_back(face_loops_input);
		}
	}

	void triangulateLoops(const std::vector<std::vector<vec2> >& profile_paths_input, std::vector<int>& face_indexes_out, BuildingEntity* ifc_entity)
	{
		if (profile_paths_input.size() == 0)
		{
			messageCallback("profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			return;
		}

		using N = uint32_t;
		using PolyPoint2D = std::array<double, 2>;
		std::vector<std::vector<PolyPoint2D> > polygons2d;

		// make sure the loops are closed
		for (size_t loop_ii = 0; loop_ii < profile_paths_input.size(); ++loop_ii)
		{
			const std::vector<vec2>& path_loop_2d = profile_paths_input[loop_ii];
			std::vector<PolyPoint2D> poly2d;

			for (size_t loop_jj = 0; loop_jj < path_loop_2d.size(); ++loop_jj)
			{
				const vec2& p2d = path_loop_2d[loop_jj];
				poly2d.push_back({ p2d.x, p2d.y });
			}

			polygons2d.push_back(poly2d);
		}

		// Run tessellation
		// Returns array of indices that refer to the vertices of the input polygon.
		// Three subsequent indices form a triangle. Output triangles are clockwise.
		std::vector<N> triangulated_indices = mapbox::earcut<N>(polygons2d);

		for (size_t point_i = 0; point_i < triangulated_indices.size(); ++point_i)
		{
			N& idx = triangulated_indices[point_i];
			face_indexes_out.push_back(idx);
		}
	}

	/*\brief Extrudes a cross section along a path. At turns, the points are placed in the bisecting plane
	  \param[in] curve_points Path along which the cross section is swept
	  \param[in] profile_paths Set of cross sections to sweep
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] item_data Container to add result polyhedron or polyline
	**/
	void sweepArea(const std::vector<vec3>& curve_points, const std::vector<std::vector<vec2> >& profile_paths_input, BuildingEntity* ifc_entity, shared_ptr<ItemShapeData>& item_data)
	{
		if (profile_paths_input.size() == 0)
		{
			messageCallback("profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			return;
		}
		const size_t num_curve_points = profile_paths_input.size();
		if (num_curve_points < 2)
		{
			messageCallback("num curve points < 2", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			return;
		}

		// figure 1: loops and indexes
		//  3----------------------------2
		//  |                            |
		//  |   1-------------------2    |3---------2
		//  |   |                   |    |          |
		//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
		//  |   0---face_loops[1]---3    |0---------1
		//  |                            |
		//  0-------face_loops[0]--------1


		std::vector<std::vector<std::vector<vec2> > > profile_paths_enclosed;
		findEnclosedLoops(profile_paths_input, profile_paths_enclosed);

		for (size_t ii_profile_paths = 0; ii_profile_paths < profile_paths_enclosed.size(); ++ii_profile_paths)
		{
			const std::vector<std::vector<vec2> >& profile_paths = profile_paths_enclosed[ii_profile_paths];

			for (size_t ii = 0; ii < profile_paths.size(); ++ii)
			{
				std::vector<int> face_indexes;

				triangulateLoops(profile_paths, face_indexes, ifc_entity);

				size_t num_points_in_all_loops = 0;
				for (size_t ii = 0; ii < profile_paths.size(); ++ii)
				{
					const std::vector<vec2>& loop = profile_paths[ii];
					num_points_in_all_loops += loop.size();
				}

				shared_ptr<carve::input::PolyhedronData> poly_data(new carve::input::PolyhedronData());
				if (!poly_data)
				{
					throw OutOfMemoryException(__FUNC__);
				}
				poly_data->points.resize(num_points_in_all_loops * curve_points.size());

				const vec3& curve_point_first = curve_points[0];
				const vec3& curve_point_second = curve_points[1];
				const vec3 curve_normal = GeomUtils::computePolygonNormal(curve_points);

				// rotate face loops into first direction
				vec3  section_local_y = curve_normal;
				vec3  section_local_z = curve_point_first - curve_point_second;
				vec3  section_local_x = carve::geom::cross(section_local_y, section_local_z);
				section_local_y = carve::geom::cross(section_local_x, section_local_z);
				section_local_x.normalize();
				section_local_y.normalize();
				section_local_z.normalize();

				carve::math::Matrix matrix_first_direction = carve::math::Matrix(
					section_local_x.x, section_local_y.x, section_local_z.x, 0,
					section_local_x.y, section_local_y.y, section_local_z.y, 0,
					section_local_x.z, section_local_y.z, section_local_z.z, 0,
					0, 0, 0, 1);

				std::vector<vec3>& polyhedron_points = poly_data->points;
				size_t polyhedron_point_index = 0;
				for (size_t ii = 0; ii < profile_paths.size(); ++ii)
				{
					const std::vector<vec2>& loop = profile_paths[ii];
					for (size_t jj = 0; jj < loop.size(); ++jj)
					{
						const vec2& vec_2d = loop[jj];
						vec3  vec_3d(carve::geom::VECTOR(vec_2d.x, vec_2d.y, 0));

						// cross section is defined in XY plane
						vec_3d = matrix_first_direction * vec_3d + curve_point_first;
						polyhedron_points[polyhedron_point_index++] = vec_3d;
					}
				}

				for (size_t ii = 1; ii < num_curve_points; ++ii)
				{
					vec3 curve_point_current = curve_points[ii];
					vec3 curve_point_next;
					vec3 curve_point_before;
					if (ii == 0)
					{
						// first point
						curve_point_next = curve_points[ii + 1];
						vec3 delta_element = curve_point_next - curve_point_current;
						curve_point_before = curve_point_current - (delta_element);
					}
					else if (ii == num_curve_points - 1)
					{
						// last point
						curve_point_before = curve_points[ii - 1];
						vec3 delta_element = curve_point_current - curve_point_before;
						curve_point_next = curve_point_before + (delta_element);
					}
					else
					{
						// inner point
						curve_point_next = curve_points[ii + 1];
						curve_point_before = curve_points[ii - 1];
					}

					vec3 bisecting_normal;
					GeomUtils::bisectingPlane(curve_point_before, curve_point_current, curve_point_next, bisecting_normal);

					vec3 section1 = curve_point_current - curve_point_before;
					section1.normalize();
					if (ii == num_curve_points - 1)
					{
						bisecting_normal *= -1.0;
					}

					carve::geom::plane<3> bisecting_plane(bisecting_normal, curve_point_current);
					for (size_t jj = 0; jj < num_points_in_all_loops; ++jj)
					{
						vec3& section_point_3d = polyhedron_points[polyhedron_point_index];
						vec3& previous_section_point_3d = polyhedron_points[polyhedron_point_index - num_points_in_all_loops];

						polyhedron_point_index++;

						vec3 v;
						double t;
						carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection(bisecting_plane, previous_section_point_3d, previous_section_point_3d + section1, v, t);
						if (intersect > 0)
						{
							section_point_3d = v;
						}
						else
						{
							messageCallback("no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
						}
					}
				}

				// add face loops for all sections
				const size_t num_poly_points = polyhedron_points.size();
				size_t loop_offset = 0;
				for (size_t ii = 0; ii < profile_paths.size(); ++ii)
				{
					const std::vector<vec2>& loop = profile_paths[ii];

					for (size_t jj = 0; jj < loop.size(); ++jj)
					{
						for (size_t kk = 0; kk < num_curve_points - 1; ++kk)
						{
							size_t tri_idx_a = num_points_in_all_loops * kk + jj + loop_offset;

							size_t tri_idx_next = tri_idx_a + 1;
							if (jj == loop.size() - 1)
							{
								tri_idx_next -= loop.size();
							}
							size_t tri_idx_up = tri_idx_a + num_points_in_all_loops;  // next section
							size_t tri_idx_next_up = tri_idx_next + num_points_in_all_loops;  // next section


							if (tri_idx_a >= num_poly_points || tri_idx_next >= num_poly_points || tri_idx_up >= num_poly_points || tri_idx_next_up >= num_poly_points)
							{
								messageCallback("invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
								continue;
							}

							poly_data->addFace(tri_idx_a, tri_idx_next, tri_idx_next_up);
							poly_data->addFace(tri_idx_next_up, tri_idx_up, tri_idx_a);
						}
					}

					loop_offset += loop.size();
				}

				// add front and back cap
				for (size_t ii = 0; ii < face_indexes.size(); ++ii)
				{
					size_t num_face_vertices = face_indexes[ii];

					if (num_face_vertices == 3)
					{
						size_t tri_idx_a = face_indexes[ii + 1];
						size_t tri_idx_b = face_indexes[ii + 2];
						size_t tri_idx_c = face_indexes[ii + 3];
						if (tri_idx_a >= num_poly_points || tri_idx_b >= num_poly_points || tri_idx_c >= num_poly_points)
						{
							messageCallback("invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
							ii += num_face_vertices;
							continue;
						}

						poly_data->addFace(tri_idx_a, tri_idx_c, tri_idx_b);


						size_t tri_idx_a_back_cap = tri_idx_a + num_poly_points - num_points_in_all_loops;
						size_t tri_idx_b_back_cap = tri_idx_b + num_poly_points - num_points_in_all_loops;
						size_t tri_idx_c_back_cap = tri_idx_c + num_poly_points - num_points_in_all_loops;
						if (tri_idx_a_back_cap >= num_poly_points || tri_idx_b_back_cap >= num_poly_points || tri_idx_c_back_cap >= num_poly_points)
						{
							messageCallback("invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
							ii += num_face_vertices;
							continue;
						}

						poly_data->addFace(tri_idx_a_back_cap, tri_idx_b_back_cap, tri_idx_c_back_cap);
					}
					else if (num_face_vertices == 2)
					{
						// add polyline
						//poly_data->addFace( face_indexes[ii+1], face_indexes[ii+2] );
					}
					else if (num_face_vertices == 1)
					{
						// add polyline
						//poly_data->addFace( face_indexes[ii+1], face_indexes[ii+2] );
					}
					else
					{
						messageCallback("num_face_vertices != 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
					}
					ii += num_face_vertices;

				}

				try
				{
					item_data->addClosedPolyhedron(poly_data);
				}
				catch (BuildingException& exception)
				{
					messageCallback(exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity);  // calling function already in e.what()
#ifdef _DEBUG
					carve::geom::vector<4> color = carve::geom::VECTOR(0.7, 0.7, 0.7, 1.0);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts()));
					GeomDebugDump::dumpMeshset(meshset, color, true);
#endif
				}

#ifdef _DEBUG
				shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts()));
				CSG_Adapter::checkMeshSetValidAndClosed(meshset, this, ifc_entity);
#endif
			}
		}
	}

#ifdef _DEBUG
	static inline bool pointInPolySimple(const std::vector<vec2>& points, const vec2& p) {
		if (points.size() < 1)
		{
			return false;
		}
		size_t l = points.size();
		double s = 0.0;
		double rp, r0, d;

		rp = r0 = ::atan2(points[0].y - p.y, points[0].x - p.x);

		for (size_t i = 1; i < l; i++) {
			double r = atan2(points[i].y - p.y, points[i].x - p.x);
			d = r - rp;
			if (d > M_PI) {
				d -= M_TWOPI;
			}
			if (d < -M_PI) {
				d += M_TWOPI;
			}
			s = s + d;
			rp = r;
		}

		d = r0 - rp;
		if (d > M_PI) {
			d -= M_TWOPI;
		}
		if (d < -M_PI) {
			d += M_TWOPI;
		}
		s = s + d;

		bool is_zero = fabs(s) < carve::CARVE_EPSILON;
		return !is_zero;
	}

	void checkTriangleIdx(int idx, const PolyInputCache3D& poly_cache)
	{
		if (idx >= poly_cache.m_poly_data->points.size())
		{
			std::cout << "invalid idx " << idx << '\n';
		}
	}

#endif

	/*\brief method createTriangulated3DFace: Creates a triangulated face
	\param[in] vec_bounds: Curves as face boundaries. The first input curve is the outer boundary, succeeding curves are inner boundaries
	\param[in] ifc_entity: Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	\param[out] poly_cache: Result input object
	**/
	static void createTriangulated3DFace(std::vector<std::vector<vec3> >& vec_bounds, BuildingEntity* ifc_entity, PolyInputCache3D& poly_cache)
	{
		std::vector<std::vector<vec2> > face_loops_2d;
		std::vector<std::vector<vec3> > face_loops_3d;
		std::map<size_t, size_t> map_merged_idx;
		bool face_loop_reversed = false;
		bool warning_small_loop_detected = false;
		GeomUtils::ProjectionPlane face_plane = GeomUtils::UNDEFINED;

		for (auto it_bounds = vec_bounds.begin(); it_bounds != vec_bounds.end(); ++it_bounds)
		{
			std::vector<vec3>& loop_points = *it_bounds;

			if (loop_points.size() < 3)
			{
				if (it_bounds == vec_bounds.begin())
				{
					break;
				}
				else
				{
					continue;
				}
			}

			if (loop_points.size() == 3 && vec_bounds.size() == 1)
			{
				// simple triangle
				const vec3& v0 = loop_points[0];
				const vec3& v1 = loop_points[1];
				const vec3& v2 = loop_points[2];
				size_t vertex_id0 = poly_cache.addPoint(v0);
				size_t vertex_id1 = poly_cache.addPoint(v1);
				size_t vertex_id2 = poly_cache.addPoint(v2);
				poly_cache.m_poly_data->addFace(vertex_id0, vertex_id1, vertex_id2);
				return;
			}

			if (loop_points.size() == 4 && vec_bounds.size() == 1)
			{
				// two triangles
				const vec3& v0 = loop_points[0];
				const vec3& v1 = loop_points[1];
				const vec3& v2 = loop_points[2];
				const vec3& v3 = loop_points[3];
				size_t vertex_id0 = poly_cache.addPoint(v0);
#ifdef _DEBUG
				const vec3& v0_check = poly_cache.m_poly_data->points[vertex_id0];
				vec3 delta_zero = v0_check - v0;
				if (delta_zero.length2() > 0.0000001)
				{
					std::cout << "incorrect index";
				}
#endif
				size_t vertex_id1 = poly_cache.addPoint(v1);
				size_t vertex_id2 = poly_cache.addPoint(v2);
				size_t vertex_id3 = poly_cache.addPoint(v3);
				poly_cache.m_poly_data->addFace(vertex_id0, vertex_id1, vertex_id2);
				poly_cache.m_poly_data->addFace(vertex_id0, vertex_id2, vertex_id3);
				return;
			}

			carve::geom3d::Vector normal = GeomUtils::computePolygonNormal(loop_points);
			if (it_bounds == vec_bounds.begin())
			{
				double nx = std::abs(normal.x);
				double ny = std::abs(normal.y);
				double nz = std::abs(normal.z);
				if (nz > nx && nz >= ny)
				{
					face_plane = GeomUtils::XY_PLANE;
				}
				else if (nx >= ny && nx >= nz)
				{
					face_plane = GeomUtils::YZ_PLANE;
				}
				else if (ny > nx && ny >= nz)
				{
					face_plane = GeomUtils::XZ_PLANE;
				}
				else
				{
#ifdef _DEBUG
					std::cout << __FUNC__ << ": " << "unable to project to plane: nx" << nx << " ny " << ny << " nz " << nz << ", entity_id: " << ifc_entity << std::endl;
#endif
					continue;
				}
			}

			// project face into 2d plane
			std::vector<vec2> single_loop2d;
			std::vector<vec3> single_loop3d;

			for (size_t i = 0; i < loop_points.size(); ++i)
			{
				const vec3& point = loop_points[i];
				single_loop3d.push_back(point);
				vec2 projected_point;
				if (face_plane == GeomUtils::XY_PLANE)
				{
					projected_point = carve::geom::VECTOR(point.x, point.y);
				}
				else if (face_plane == GeomUtils::YZ_PLANE)
				{
					projected_point = carve::geom::VECTOR(point.y, point.z);
				}
				else if (face_plane == GeomUtils::XZ_PLANE)
				{
					projected_point = carve::geom::VECTOR(point.x, point.z);
				}

				single_loop2d.push_back(projected_point);

#ifdef _DEBUG
				if (it_bounds != vec_bounds.begin())
				{
					bool inside_first_loop = pointInPolySimple(face_loops_2d[0], projected_point);
					if (!inside_first_loop)
					{
						std::cout << "second loop is not in first loop"<< std::endl;
					}
				}
#endif
			}

			// check winding order
			carve::geom3d::Vector normal_2d = GeomUtils::computePolygon2DNormal(single_loop2d);
			if (it_bounds == vec_bounds.begin())
			{
				if (normal_2d.z < 0)
				{
					std::reverse(single_loop2d.begin(), single_loop2d.end());
					std::reverse(single_loop3d.begin(), single_loop3d.end());
					face_loop_reversed = true;
				}
			}
			else
			{
				if (normal_2d.z > 0)
				{
					std::reverse(single_loop2d.begin(), single_loop2d.end());
					std::reverse(single_loop3d.begin(), single_loop3d.end());
				}
			}

			if (single_loop2d.size() < 3)
			{
				//std::cout << __FUNC__ << ": #" << face_id <<  "=IfcFace: path_loop.size() < 3" << std::endl;
				continue;
			}

			double signed_area = carve::geom2d::signedArea(single_loop2d);
			double min_loop_area = 1.e-10;//m_geom_settings->m_min_face_area
			if (std::abs(signed_area) < min_loop_area)
			{
				warning_small_loop_detected = true;
				continue;
			}

			face_loops_2d.push_back(single_loop2d);
			face_loops_3d.push_back(single_loop3d);
		}

		if (warning_small_loop_detected)
		{
#ifdef _DEBUG
			std::cout << __FUNC__ << ": abs( signed_area ) < 1.e-10" << ", ifc_entity: " << ifc_entity << std::endl;
#endif
		}

		if (face_loops_2d.size() > 0)
		{
			try
			{
				// triangle index type. uint16_t for up to 65536 vertices. Use uint32_t for more
				using triangleIndexType = uint16_t;
				using PolygonPoint2D = std::array<double, 2>;
				using PolygonPoint3D = std::array<double, 3>;
				std::vector<std::vector<PolygonPoint2D> > polygons2d;
				std::vector<std::vector<PolygonPoint3D> > polygons3d;
				std::vector<PolygonPoint3D> polyloops_combined;

				for (size_t loop_ii = 0; loop_ii < face_loops_2d.size(); ++loop_ii)
				{
					std::vector<vec2>& path_loop_2d = face_loops_2d[loop_ii];
					std::vector<vec3>& path_loop_3d = face_loops_3d[loop_ii];

					std::vector<PolygonPoint2D> poly2d;
					std::vector<PolygonPoint3D> poly3d;
					for (size_t loop_jj = 0; loop_jj < path_loop_2d.size(); ++loop_jj)
					{
						const vec2& p2d = path_loop_2d[loop_jj];
						const vec3& p3d = path_loop_3d[loop_jj];
						poly2d.push_back({ p2d.x, p2d.y });
						poly3d.push_back({ p3d.x, p3d.y, p3d.z });
						polyloops_combined.push_back({ p3d.x, p3d.y, p3d.z });
					}
					polygons2d.push_back(poly2d);
					polygons3d.push_back(poly3d);
				}


				// Run tessellation. Three subsequent indices form a triangle. Output triangles are clockwise.
				std::vector<triangleIndexType> triangulated_indices = mapbox::earcut<triangleIndexType>(polygons2d);

				if (triangulated_indices.size() > 2)
				{
					std::map<size_t, size_t> map_idx;
					for (size_t point_i = 0; point_i < polyloops_combined.size(); ++point_i)
					{
						PolygonPoint3D& v = polyloops_combined[point_i];
						size_t pointIndex = poly_cache.addPoint(carve::geom::VECTOR(v[0], v[1], v[2]));
						map_idx[point_i] = pointIndex;
					}

					for (size_t idx_ii = 2; idx_ii < triangulated_indices.size(); idx_ii += 3)
					{
						triangleIndexType idx0 = triangulated_indices[idx_ii-2];
						triangleIndexType idx1 = triangulated_indices[idx_ii-1];
						triangleIndexType idx2 = triangulated_indices[idx_ii];

						idx0 = map_idx[idx0];
						idx1 = map_idx[idx1];
						idx2 = map_idx[idx2];

						poly_cache.m_poly_data->addFace(idx0, idx1, idx2);
					}
				}
			}
			catch (...)
			{
#ifdef _DEBUG
				std::cout << __FUNC__ << ": triangulate failed, ifc_entity: " << ifc_entity << std::endl;
				carve::geom::vector<4> color = carve::geom::VECTOR(0.3, 0.4, 0.5, 1.0);
				GeomDebugDump::dumpPolylineSet(face_loops_2d, color, true, true);

				{
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_cache.m_poly_data->createMesh(carve::input::opts()));
					carve::geom::aabb<3> bbox = meshset->getAABB();
					if (bbox.pos.x + bbox.extent.x > 1000000 || bbox.pos.y + bbox.extent.y > 1000000 || bbox.pos.z + bbox.extent.z > 1000000)
					{
						for (size_t i = 0; i < meshset->vertex_storage.size(); ++i) {
							meshset->vertex_storage[i].v -= bbox.pos;
						}
					}
					carve::geom::vector<4> color = carve::geom::VECTOR(0.3, 0.4, 0.5, 1.0);
					GeomDebugDump::clearMeshsetDump();
					GeomDebugDump::dumpMeshset(meshset, color, true);
				}
#endif
				return;
			}
		}
	}
};
