/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#pragma once

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/StatusCallback.h>

#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "CSG_Adapter.h"

class GeometrySettings;
class UnitConverter;

//#define MAP_T boost::unordered_map
#define MAP_T std::map

class Sweeper : public StatusCallback
{
public:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;

	Sweeper(shared_ptr<GeometrySettings>& settings, shared_ptr<UnitConverter>& uc) : m_geom_settings(settings), m_unit_converter(uc) {}
	~Sweeper(){}

	/*\brief Extrudes a set of cross sections along a direction
	  \param[in] paths Set of cross sections to extrude
	  \param[in] dir Extrusion vector
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] item_data Container to add result polyhedron or polyline
	**/
	void extrude( const std::vector<std::vector<carve::geom::vector<2> > >& face_loops_input, const carve::geom::vector<3> extrusion_vector, IfcPPEntity* ifc_entity, shared_ptr<ItemShapeInputData>& item_data )
	{
		// TODO: complete and test
		if( face_loops_input.size() == 0 )
		{
			messageCallback( "profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

#ifdef _DEBUG
		//shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		//if( !polyline_data )
		//{
		//	throw IfcPPOutOfMemoryException( __FUNC__ );
		//}
		//polyline_data->beginPolyline();
		//polyline_data->addVertex( carve::geom::VECTOR( 0, 0, 0 ) );
		//polyline_data->addVertex( extrusion_vector );
		//polyline_data->addPolylineIndex( 0 );
		//polyline_data->addPolylineIndex( 1 );
		//item_data->m_polylines.push_back( polyline_data );
#endif

		// figure 1: loops and indexes
		//  3----------------------------2
		//  |                            |
		//  |   1-------------------2    |3---------2
		//  |   |                   |    |          |
		//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
		//  |   0---face_loops[1]---3    |0---------1
		//  |                            |
		//  0-------face_loops[0]--------1

		std::vector<int> face_indexes;
		std::vector<std::vector<carve::geom::vector<2> > > face_loops_used_for_triangulation;
		triangulateLoops( face_loops_input, face_loops_used_for_triangulation, face_indexes, ifc_entity, this );

		size_t num_points_in_all_loops = 0;
		for( size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii )
		{
			const std::vector<carve::geom::vector<2> >&  loop = face_loops_used_for_triangulation[ii];
			num_points_in_all_loops += loop.size();
		}

		shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData() );
		if( !poly_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		poly_data->points.resize( num_points_in_all_loops*2 );

		std::vector<carve::geom::vector<3> >& polyhedron_points = poly_data->points;
		size_t polyhedron_point_index = 0;
		for( size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii )
		{
			const std::vector<carve::geom::vector<2> >&  loop = face_loops_used_for_triangulation[ii];
			for( size_t jj = 0; jj < loop.size(); ++jj )
			{
				const carve::geom::vector<2>&  vec_2d = loop[jj];
				// cross section is defined in XY plane
				polyhedron_points[polyhedron_point_index] = carve::geom::VECTOR( vec_2d.x, vec_2d.y, 0 );
				polyhedron_points[polyhedron_point_index + num_points_in_all_loops] = carve::geom::VECTOR( vec_2d.x, vec_2d.y, 0 ) + extrusion_vector;
				++polyhedron_point_index;
			}
		}

		bool flip_faces = false;
		if( face_loops_used_for_triangulation.size() > 0 )
		{
			carve::geom::vector<3>  normal_first_loop = GeomUtils::computePolygon2DNormal( face_loops_used_for_triangulation[0] );
			double extrusion_dot_normal = dot( extrusion_vector, normal_first_loop );
			if( extrusion_dot_normal < 0 )
			{
				flip_faces = true;
			}
		}
		
		// add face loops for all sections
		const size_t num_poly_points = polyhedron_points.size();
		size_t loop_offset = 0;
		for( size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii )
		{
			const std::vector<carve::geom::vector<2> >&  loop = face_loops_used_for_triangulation[ii];

			for( size_t jj = 0; jj < loop.size(); ++jj )
			{
				size_t tri_idx_a = jj + loop_offset;

				size_t tri_idx_next = tri_idx_a + 1;
				if( jj == loop.size() - 1 )
				{
					tri_idx_next -= loop.size();
				}
				size_t tri_idx_up = tri_idx_a + num_points_in_all_loops;
				size_t tri_idx_next_up = tri_idx_next + num_points_in_all_loops;

				if( tri_idx_a >= num_poly_points || tri_idx_next >= num_poly_points || tri_idx_up >= num_poly_points || tri_idx_next_up >= num_poly_points )
				{
					messageCallback( "invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}

				if( flip_faces )
				{
					poly_data->addFace( tri_idx_a, tri_idx_next_up, tri_idx_next );
					poly_data->addFace( tri_idx_next_up, tri_idx_a, tri_idx_up );
				}
				else
				{
					poly_data->addFace( tri_idx_a, tri_idx_next, tri_idx_next_up );
					poly_data->addFace( tri_idx_next_up, tri_idx_up, tri_idx_a );
				}
			}

			loop_offset += loop.size();
		}

		// add front and back cap
		for( size_t ii = 0; ii < face_indexes.size(); ++ii )
		{
			size_t num_face_vertices = face_indexes[ii];
			if( ii + num_face_vertices >= face_indexes.size() )
			{
				messageCallback( "ii + num_face_vertices >= face_indexes.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				break;
			}

			if( num_face_vertices == 3 )
			{
				size_t tri_idx_a = face_indexes[ii+1];
				size_t tri_idx_b = face_indexes[ii+2];
				size_t tri_idx_c = face_indexes[ii+3];
				if( tri_idx_a < num_poly_points && tri_idx_b < num_poly_points && tri_idx_c < num_poly_points )
				{
					if( flip_faces )
					{
						poly_data->addFace( tri_idx_a, tri_idx_b, tri_idx_c );
					}
					else
					{
						poly_data->addFace( tri_idx_a, tri_idx_c, tri_idx_b );
					}
				}
				else
				{
					messageCallback( "invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}

				size_t tri_idx_a_back_cap = tri_idx_a + num_poly_points - num_points_in_all_loops;
				size_t tri_idx_b_back_cap = tri_idx_b + num_poly_points - num_points_in_all_loops;
				size_t tri_idx_c_back_cap = tri_idx_c + num_poly_points - num_points_in_all_loops;
				if( tri_idx_a_back_cap < num_poly_points && tri_idx_b_back_cap < num_poly_points && tri_idx_c_back_cap < num_poly_points )
				{
					if( flip_faces )
					{
						poly_data->addFace( tri_idx_a_back_cap, tri_idx_c_back_cap, tri_idx_b_back_cap );
					}
					else
					{
						poly_data->addFace( tri_idx_a_back_cap, tri_idx_b_back_cap, tri_idx_c_back_cap );
					}
				}
				else
				{
					messageCallback( "invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}
			}
			else if( num_face_vertices == 2 )
			{
				// add polyline
				//poly_data->addFace( face_indexes[ii+1], face_indexes[ii+2] );
			}
			else if( num_face_vertices == 1 )
			{
				// add polyline
				//poly_data->addFace( face_indexes[ii+1], face_indexes[ii+2] );
			}
			else
			{
				messageCallback( "num_face_vertices != 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			}
			ii += num_face_vertices;
		}

		try
		{
			item_data->addClosedPolyhedron( poly_data );
		}
		catch( IfcPPException& exception )
		{
#ifdef _DEBUG
			std::cout << exception.what() << std::endl;
			shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts() ) );
			carve::geom::vector<4> color = carve::geom::VECTOR( 0.3, 0.4, 0.5, 1.0 );
			GeomDebugUtils::dumpMeshset( meshset, color, true );
#endif
			messageCallback( exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity );  // calling function already in e.what()
		}

#ifdef _DEBUG
		shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts() ) );
		CSG_Adapter::checkMeshSetValidAndClosed( meshset, this, ifc_entity );
#endif
	}

	/*\brief Extrudes a circle cross section along a path. At turns, the points are placed in the bisecting plane
	  \param[in] curve_points Path along which the circle is swept
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] item_data Container to add result polyhedron or polyline
	  \param[in] nvc Number of vertices per circle
	  \param[in] radius_inner If positive value is given, the swept disk becomes a pipe
	**/
	void sweepDisk( const std::vector<carve::geom::vector<3> >& curve_points, IfcPPEntity* ifc_entity, shared_ptr<ItemShapeInputData>& item_data, const size_t nvc, const double radius, const double radius_inner = -1 )
	{
		carve::geom::vector<3> local_z( carve::geom::VECTOR( 0, 0, 1 ) );
		const size_t num_curve_points = curve_points.size();
		if( num_curve_points < 2 )
		{
			messageCallback( "num curve points < 2", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		if( !item_data )
		{
			messageCallback( "!item_data", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		if( radius < 0.001 )
		{
			// Cross section is just a point. Create a polyline
			shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
			if( !polyline_data )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}
			polyline_data->beginPolyline();
			for( size_t i_polyline = 0; i_polyline < curve_points.size(); ++i_polyline )
			{
				const carve::geom::vector<3>& curve_pt = curve_points[i_polyline];
				polyline_data->addVertex( curve_pt );
				polyline_data->addPolylineIndex( 0 );
				polyline_data->addPolylineIndex( i_polyline );
			}
			item_data->m_polylines.push_back( polyline_data );
			return;
		}

		double use_radius_inner = radius_inner;
		if( radius_inner > radius )
		{
			messageCallback( "radius_inner > radius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			use_radius_inner = radius;
		}

		carve::geom::vector<3>  curve_point_first = curve_points[0];
		carve::geom::vector<3>  curve_point_second = curve_points[1];

		bool bend_found = false;
		if( num_curve_points > 3 )
		{
			// compute local z vector by dot product of the first bend of the reference line
			carve::geom::vector<3> vertex_back2 = curve_point_first;
			carve::geom::vector<3> vertex_back1 = curve_point_second;
			for( size_t i = 2; i<num_curve_points; ++i )
			{
				const carve::geom::vector<3>& vertex_current = curve_points[i];
				carve::geom::vector<3> section1 = vertex_back1 - vertex_back2;
				carve::geom::vector<3> section2 = vertex_current - vertex_back1;
				section1.normalize();
				section2.normalize();

				double dot_product = dot( section1, section2 );
				double dot_product_abs = std::abs(dot_product);

				// if dot == 1 or -1, then points are colinear
				if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
				{
					// bend found, compute cross product
					carve::geom::vector<3> lateral_vec = cross( section1, section2 );
					local_z = cross( lateral_vec, section1 );
					local_z.normalize();
					bend_found = true;
					break;
				}
			}
		}

		if( !bend_found )
		{
			// sweeping curve is linear. assume any local z vector
			carve::geom::vector<3> sweep_dir = curve_point_second - curve_point_first;
			sweep_dir.normalize();
			local_z = cross( carve::geom::VECTOR( 0, 0, 1 ), sweep_dir );
			if( local_z.length2() < 0.001 )
			{
				local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
				local_z.normalize();
			}
			else
			{
				local_z.normalize();
			}
			double dot_normal_local_z = dot( sweep_dir, local_z );
			if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
			{
				local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
				local_z.normalize();

				dot_normal_local_z = dot( sweep_dir, local_z );
				if( std::abs(dot_normal_local_z-1.0) < 0.0001 )
				{
					local_z = cross( carve::geom::VECTOR( 1, 0, 0 ), sweep_dir );
					local_z.normalize();
				}
			}
		}

		// rotate disk into first direction
		carve::geom::vector<3>  section_local_y = local_z;
		carve::geom::vector<3>  section_local_z = curve_point_first - curve_point_second;
		carve::geom::vector<3>  section_local_x = carve::geom::cross( section_local_y, section_local_z );
		section_local_y = carve::geom::cross( section_local_x, section_local_z );
		std::vector<carve::geom::vector<3> > inner_shape_points(nvc);
	
		section_local_x.normalize();
		section_local_y.normalize();
		section_local_z.normalize();

		carve::math::Matrix matrix_first_direction = carve::math::Matrix(
			section_local_x.x,		section_local_y.x,		section_local_z.x,	0,
			section_local_x.y,		section_local_y.y,		section_local_z.y,	0,
			section_local_x.z,		section_local_y.z,		section_local_z.z,	0,
			0,				0,				0,			1 );

		double angle = 0;
		double delta_angle = 2.0*M_PI/double(nvc);	// TODO: adapt to model size and complexity
		std::vector<carve::geom::vector<3> > circle_points(nvc);
		std::vector<carve::geom::vector<3> > circle_points_inner(nvc);
		for( size_t ii = 0; ii < nvc; ++ii )
		{
			// cross section (circle) is defined in XY plane
			double x = sin(angle);
			double y = cos(angle);
			carve::geom::vector<3> vertex( carve::geom::VECTOR( x*radius, y*radius, 0.0 ) );
			vertex = matrix_first_direction*vertex + curve_point_first;
			circle_points[ii] = vertex;

			if( use_radius_inner > 0 )
			{
				carve::geom::vector<3> vertex_inner( carve::geom::VECTOR( x*use_radius_inner, y*use_radius_inner, 0.0 ) );
				vertex_inner = matrix_first_direction*vertex_inner + curve_point_first;
				circle_points_inner[ii] = vertex_inner;
			}
			angle += delta_angle;
		}

		shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData() );
		if( !poly_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}

		for( size_t ii = 0; ii<num_curve_points; ++ii )
		{
			const carve::geom::vector<3>& vertex_current = curve_points[ii];
			carve::geom::vector<3> vertex_next;
			carve::geom::vector<3> vertex_before;
			if( ii == 0 )
			{
				// first point
				vertex_next	= curve_points[ii+1];
				carve::geom::vector<3> delta_element = vertex_next - vertex_current;
				vertex_before = vertex_current - (delta_element);
			}
			else if( ii == num_curve_points-1 )
			{
				// last point
				vertex_before	= curve_points[ii-1];
				carve::geom::vector<3> delta_element = vertex_current - vertex_before;
				vertex_next = vertex_before + (delta_element);
			}
			else
			{
				// inner point
				vertex_next		= curve_points[ii+1];
				vertex_before	= curve_points[ii-1];
			}

			carve::geom::vector<3> bisecting_normal;
			GeomUtils::bisectingPlane( vertex_before, vertex_current, vertex_next, bisecting_normal );

			carve::geom::vector<3> section1 = vertex_current - vertex_before;
			carve::geom::vector<3> section2 = vertex_next - vertex_current;
			section1.normalize();
			section2.normalize();
			double dot_product = dot( section1, section2 );
			double dot_product_abs = std::abs(dot_product);

			if( dot_product_abs < (1.0-0.0001) || dot_product_abs > (1.0+0.0001) )
			{
				// bend found, compute next local z vector
				carve::geom::vector<3> lateral_vec = cross( section1, section2 );
				local_z = cross( lateral_vec, section1 );
				local_z.normalize();
			}
			if( ii == num_curve_points -1 )
			{
				bisecting_normal *= -1.0;
			}

			carve::geom::plane<3> bisecting_plane( bisecting_normal, vertex_current );
			for( size_t jj = 0; jj < nvc; ++jj )
			{
				carve::geom::vector<3>& vertex = circle_points[jj];

				carve::geom::vector<3> v;
				double t;
				carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t);
				if( intersect > 0 )
				{
					vertex = v;
				}
				else
				{
					messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}

				poly_data->addVertex( vertex );
			}

			if( use_radius_inner > 0 )
			{
				for( size_t jj = 0; jj < nvc; ++jj )
				{
					carve::geom::vector<3>& vertex = circle_points_inner[jj];
					
					carve::geom::vector<3> v;
					double t;
					carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t);
					if( intersect > 0 )
					{
						vertex = v;
					}
					else
					{
						messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					}

					inner_shape_points[jj] = vertex;
				}
			}
		}

		// outer shape
		size_t num_vertices_outer = poly_data->getVertexCount();
		for( size_t i=0; i<num_curve_points- 1; ++i )
		{
			size_t i_offset = i*nvc;
			size_t i_offset_next = ( i + 1 )*nvc;
			for( size_t jj = 0; jj < nvc; ++jj )
			{
				size_t current_loop_pt1 = jj + i_offset;
				size_t current_loop_pt2 = ( jj + 1 ) % nvc + i_offset;

				size_t next_loop_pt1 = jj + i_offset_next;
				size_t next_loop_pt2 = ( jj + 1 ) % nvc + i_offset_next;
				poly_data->addFace( current_loop_pt1,	next_loop_pt1,		next_loop_pt2 );
				poly_data->addFace( next_loop_pt2,		current_loop_pt2,	current_loop_pt1  );
			}
		}

		if( use_radius_inner > 0 )
		{
			if( inner_shape_points.size() != num_vertices_outer )
			{
				messageCallback( "inner_shape_points.size() != num_vertices_outer", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			}

			// add points for inner shape
			for( size_t i=0; i<inner_shape_points.size(); ++i )
			{
				poly_data->addVertex( inner_shape_points[i] );
			}

			// faces of inner shape
			for( size_t i=0; i<num_curve_points- 1; ++i )
			{
				size_t i_offset = i*nvc + num_vertices_outer;
				size_t i_offset_next = ( i + 1 )*nvc + num_vertices_outer;
				for( size_t jj = 0; jj < nvc; ++jj )
				{
					size_t current_loop_pt1 = jj + i_offset;
					size_t current_loop_pt2 = ( jj + 1 ) % nvc + i_offset;

					size_t next_loop_pt1 = jj + i_offset_next;
					size_t next_loop_pt2 = ( jj + 1 ) % nvc + i_offset_next;
					poly_data->addFace( current_loop_pt1,	current_loop_pt2,	next_loop_pt2 );
					poly_data->addFace( next_loop_pt2,		next_loop_pt1,		current_loop_pt1  );
				}
			}

			// front cap
			for( size_t jj = 0; jj < nvc; ++jj )
			{
				size_t outer_rim_next = ( jj + 1 ) % nvc;
				size_t inner_rim_next = outer_rim_next + num_vertices_outer;
				poly_data->addFace( jj,					outer_rim_next,		num_vertices_outer+jj );
				poly_data->addFace( outer_rim_next,		inner_rim_next,		num_vertices_outer+jj );
			}

			// back cap
			size_t back_offset = ( num_curve_points - 1 )*nvc;
			for( size_t jj = 0; jj < nvc; ++jj )
			{
				size_t outer_rim_next = ( jj + 1 ) % nvc + back_offset;
				size_t inner_rim_next = outer_rim_next + num_vertices_outer;
				poly_data->addFace( jj+back_offset,		num_vertices_outer+jj+back_offset,	outer_rim_next );
				poly_data->addFace( outer_rim_next,		num_vertices_outer+jj+back_offset,	inner_rim_next );
			}
		}
		else
		{
			// front cap, full pipe, create triangle fan
			for( size_t jj = 0; jj < nvc - 2; ++jj )
			{
				poly_data->addFace( 0, jj+1, jj+2 );
			}

			// back cap
			size_t back_offset = ( num_curve_points - 1 )*nvc;
			for( size_t jj = 0; jj < nvc - 2; ++jj )
			{
				poly_data->addFace( back_offset, back_offset+jj+2, back_offset+jj+1 );
			}
		}

		try
		{
			item_data->addClosedPolyhedron( poly_data );
		}
		catch( IfcPPException& exception )
		{
			messageCallback( exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity );  // calling function already in e.what()
		}

	#ifdef _DEBUG
		shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh(carve::input::opts()) );
		CSG_Adapter::checkMeshSetValidAndClosed( meshset, this, ifc_entity );
	#endif
	}

	void triangulateLoops( const std::vector<std::vector<carve::geom::vector<2> > >& profile_paths_input,
		std::vector<std::vector<carve::geom::vector<2> > >& face_loops_used_for_triangulation,
		std::vector<int>& face_indexes_out, IfcPPEntity* ifc_entity, StatusCallback* report_callback )
	{
		// TODO: complete and test
		if( profile_paths_input.size() == 0 )
		{
			report_callback->messageCallback( "profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
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

		carve::geom::vector<3> normal_first_loop;
		bool warning_small_loop_detected = false;
		bool polyline_created = false;
		for( size_t i_face_loops = 0; i_face_loops < profile_paths_input.size(); ++i_face_loops )
		{
			const std::vector<carve::geom::vector<2> >& loop_input = profile_paths_input[i_face_loops];
			std::vector<carve::geom::vector<2> > loop_2d;

			GeomUtils::copyClosedLoopSkipDuplicates( loop_input, loop_2d );
			
			if( loop_2d.size() < 3 )
			{
				if( profile_paths_input.size() == 1 )
				{
					// Cross section is just a point or a line. Create a face with one index
					face_indexes_out.push_back( loop_2d.size() );  // num points
					for( size_t ii = 0; ii < loop_2d.size(); ++ii )
					{
						face_indexes_out.push_back( ii );  // point index
					}
					// Nothing to triangulate, so return
					return;
				}
				continue;
			}
			
			// check winding order
			bool reverse_loop = false;
			carve::geom::vector<3>  normal_2d = GeomUtils::computePolygon2DNormal( loop_2d );
			if( i_face_loops == 0 )
			{
				normal_first_loop = normal_2d;
				if( normal_2d.z < 0 )
				{
					reverse_loop = true;
					normal_first_loop = -normal_first_loop;
				}
			}
			else
			{
				if( normal_2d.z > 0 )
				{
					reverse_loop = true;
				}
			}
			if( reverse_loop )
			{
				std::reverse( loop_2d.begin(), loop_2d.end() );
			}

			if( loop_2d.size() < 3 )
			{
				report_callback->messageCallback( "loop_2d.size() < 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			}

			double signed_area = carve::geom2d::signedArea( loop_2d );
			double min_loop_area = 1.e-10;//m_geom_settings->m_min_face_area
			if( std::abs( signed_area ) < min_loop_area )
			{
				warning_small_loop_detected = true;
				continue;
			}

			face_loops_used_for_triangulation.push_back( loop_2d );
		}

		if( warning_small_loop_detected )
		{
			report_callback->messageCallback( "abs( signed_area ) < 1.e-10", StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, ifc_entity );
		}

		if( face_loops_used_for_triangulation.size() == 0 )
		{
			if( polyline_created )
			{
				// already handled as curve
				return;
			}
			report_callback->messageCallback( "face_loops.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		size_t num_vertices_in_loops = 0;
		for( size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii )
		{
			std::vector<carve::geom::vector<2> >&  loop = face_loops_used_for_triangulation[ii];
			num_vertices_in_loops += loop.size();
		}

		// triangulate
		std::vector<carve::geom::vector<2> > merged_path;
		std::vector<carve::triangulate::tri_idx> triangulated;
		std::vector<std::pair<size_t, size_t> > path_incorporated_holes;
		std::vector<size_t> merged_to_unmerged;
		try
		{
			path_incorporated_holes = carve::triangulate::incorporateHolesIntoPolygon( face_loops_used_for_triangulation );
			// figure 2: path wich incorporates holes, described by path_all_loops
			// (0/0) -> (1/3) -> (1/0) -> (1/1) -> (1/2) -> (1/3) -> (0/0) -> (0/1) -> (0/2) -> (0/3)

			//  0/3<----------------------------0/2
			//  |                                ^
			//  |  1/0------------------>1/1     |
			//  |   ^                    |       |
			//  |   |                    v       |  path_incorporated_holes
			//  |  1/3  1/3<-------------1/2     |
			//  v /    /                         |
			//  0/0  0/0----------------------->0/1

			merged_path.reserve( path_incorporated_holes.size() );
			merged_to_unmerged.resize( path_incorporated_holes.size() );
			for( size_t i = 0; i < path_incorporated_holes.size(); ++i )
			{
				size_t loop_number = path_incorporated_holes[i].first;
				size_t index_in_loop = path_incorporated_holes[i].second;

				if( loop_number >= face_loops_used_for_triangulation.size() )
				{
					report_callback->messageCallback( "loop_number >= face_loops_used_for_triangulation.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}
				std::vector<carve::geom::vector<2> >& loop = face_loops_used_for_triangulation[loop_number];

				if( index_in_loop >= loop.size() )
				{
					report_callback->messageCallback( "index_in_loop >= loop.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}
				carve::geom::vector<2>& point_in_loop = loop[index_in_loop];
				merged_path.push_back( point_in_loop );
				merged_to_unmerged[i] = loop_number;
				//path_unmerged[i].second = index_in_loop;
			}
			// figure 3: merged path for triangulation
			//  9<--------------------------8
			//  |                           ^
			//  |  2------------------>3    |
			//  |  ^                   |    |
			//  |  |                   v    |  merged_path
			//  |  1  5<---------------4    |
			//  | /  /                      |
			//  0  6----------------------->7
			carve::triangulate::triangulate( merged_path, triangulated );
			carve::triangulate::improve( merged_path, triangulated );
			// triangles: (3,8,9) (2,0,1) (4,6,7)  (4,5,6)  (9,0,2)  (9,2,3)  (7,8,3)  (3,4,7)
		}
		catch( ... )
		{
			report_callback->messageCallback( "carve::triangulate::incorporateHolesIntoPolygon failed", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		for( size_t i = 0; i != triangulated.size(); ++i )
		{
			carve::triangulate::tri_idx triangle = triangulated[i];
			size_t a = triangle.a;
			size_t b = triangle.b;
			size_t c = triangle.c;

			size_t loop_number_a = path_incorporated_holes[a].first;
			size_t index_in_loop_a = path_incorporated_holes[a].second;

			size_t vertex_id_a = index_in_loop_a;
			for( size_t jj = 0; jj < loop_number_a; ++jj )
			{
				if( face_loops_used_for_triangulation.size() > jj )
				{
					vertex_id_a += face_loops_used_for_triangulation[jj].size();
				}
			}

			size_t loop_number_b = path_incorporated_holes[b].first;
			size_t index_in_loop_b = path_incorporated_holes[b].second;
			size_t vertex_id_b = index_in_loop_b;
			for( size_t jj = 0; jj < loop_number_b; ++jj )
			{
				if( face_loops_used_for_triangulation.size() > jj )
				{
					vertex_id_b += face_loops_used_for_triangulation[jj].size();
				}
			}

			size_t loop_number_c = path_incorporated_holes[c].first;
			size_t index_in_loop_c = path_incorporated_holes[c].second;
			size_t vertex_id_c = index_in_loop_c;
			for( size_t jj = 0; jj < loop_number_c; ++jj )
			{
				if( face_loops_used_for_triangulation.size() > jj )
				{
					vertex_id_c += face_loops_used_for_triangulation[jj].size();
				}
			}

			if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
			{
				continue;
			}

			face_indexes_out.push_back( 3 );
			face_indexes_out.push_back( vertex_id_a );
			face_indexes_out.push_back( vertex_id_b );
			face_indexes_out.push_back( vertex_id_c );
		}
	}

	/*\brief Extrudes a cross section along a path. At turns, the points are placed in the bisecting plane
	  \param[in] curve_points Path along which the cross section is swept
	  \param[in] profile_paths Set of cross sections to sweep
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] item_data Container to add result polyhedron or polyline
	**/
	void sweepArea( const std::vector<carve::geom::vector<3> >& curve_points, const std::vector<std::vector<carve::geom::vector<2> > >& profile_paths, IfcPPEntity* ifc_entity, shared_ptr<ItemShapeInputData>& item_data )
	{
		// TODO: complete and test
		if( profile_paths.size() == 0 )
		{
			messageCallback( "profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		if( !polyline_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		polyline_data->beginPolyline();
		for( size_t ii = 0; ii < curve_points.size(); ++ii )
		{
			polyline_data->addVertex( curve_points[ii] );	
			polyline_data->addPolylineIndex( ii );
		}
		item_data->m_polylines.push_back( polyline_data );

		// figure 1: loops and indexes
		//  3----------------------------2
		//  |                            |
		//  |   1-------------------2    |3---------2
		//  |   |                   |    |          |
		//  |   |                   |    |          |face_loops[2]   // TODO: handle combined profiles
		//  |   0---face_loops[1]---3    |0---------1
		//  |                            |
		//  0-------face_loops[0]--------1

		std::vector<int> face_indexes;
		std::vector<std::vector<carve::geom::vector<2> > > face_loops_used_for_triangulation;
		triangulateLoops( profile_paths, face_loops_used_for_triangulation, face_indexes, ifc_entity, this );

		size_t num_points_in_all_loops = 0;
		for( size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii )
		{
			const std::vector<carve::geom::vector<2> >&  loop = face_loops_used_for_triangulation[ii];
			num_points_in_all_loops += loop.size();
		}

		shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData() );
		if( !poly_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		poly_data->points.resize( num_points_in_all_loops*curve_points.size() );

		carve::geom::vector<3> local_z( carve::geom::VECTOR( 0, 0, 1 ) );
		const size_t num_curve_points = curve_points.size();
		if( num_curve_points < 2 )
		{
			messageCallback( "num curve points < 2", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		const carve::geom::vector<3>& curve_point_first = curve_points[0];
		const carve::geom::vector<3>& curve_point_second = curve_points[1];

		bool bend_found = false;
		if( num_curve_points > 3 )
		{
			// compute local z vector by dot product of the first bend of the reference line
			const carve::geom::vector<3>& vertex_back2 = curve_point_first;
			const carve::geom::vector<3>& vertex_back1 = curve_point_second;
			for( size_t i = 2; i < num_curve_points; ++i )
			{
				const carve::geom::vector<3>& vertex_current = curve_points[i];
				carve::geom::vector<3> section1 = vertex_back1 - vertex_back2;
				carve::geom::vector<3> section2 = vertex_current - vertex_back1;
				section1.normalize();
				section2.normalize();

				double dot_product = dot( section1, section2 );
				double dot_product_abs = std::abs( dot_product );

				// if dot == 1 or -1, then points are colinear
				if( dot_product_abs < ( 1.0 - 0.0001 ) || dot_product_abs >( 1.0 + 0.0001 ) )
				{
					// bend found, compute cross product
					carve::geom::vector<3> lateral_vec = cross( section1, section2 );
					local_z = cross( lateral_vec, section1 );
					local_z.normalize();
					bend_found = true;
					break;
				}
			}
		}

		if( !bend_found )
		{
			// sweeping curve is linear. assume any local z vector
			carve::geom::vector<3> sweep_dir = curve_point_second - curve_point_first;
			sweep_dir.normalize();
			local_z = cross( carve::geom::VECTOR( 0, 0, 1 ), sweep_dir );
			if( local_z.length2() < 0.001 )
			{
				local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
				local_z.normalize();
			}
			else
			{
				local_z.normalize();
			}
			double dot_normal_local_z = dot( sweep_dir, local_z );
			if( std::abs( dot_normal_local_z - 1.0 ) < 0.0001 )
			{
				local_z = cross( carve::geom::VECTOR( 0, 1, 0 ), sweep_dir );
				local_z.normalize();

				dot_normal_local_z = dot( sweep_dir, local_z );
				if( std::abs( dot_normal_local_z - 1.0 ) < 0.0001 )
				{
					local_z = cross( carve::geom::VECTOR( 1, 0, 0 ), sweep_dir );
					local_z.normalize();
				}
			}
		}

		// rotate face loops into first direction
		carve::geom::vector<3>  section_local_y = local_z;
		carve::geom::vector<3>  section_local_z = curve_point_first - curve_point_second;
		carve::geom::vector<3>  section_local_x = carve::geom::cross( section_local_y, section_local_z );
		section_local_y = carve::geom::cross( section_local_x, section_local_z );
		std::vector<carve::geom::vector<3> > inner_shape_points;

		section_local_x.normalize();
		section_local_y.normalize();
		section_local_z.normalize();

		carve::math::Matrix matrix_first_direction = carve::math::Matrix(
			section_local_x.x, section_local_y.x, section_local_z.x, 0,
			section_local_x.y, section_local_y.y, section_local_z.y, 0,
			section_local_x.z, section_local_y.z, section_local_z.z, 0,
			0, 0, 0, 1 );

		std::vector<carve::geom::vector<3> >& polyhedron_points = poly_data->points;
		size_t polyhedron_point_index = 0;
		for( size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii )
		{
			const std::vector<carve::geom::vector<2> >&  loop = face_loops_used_for_triangulation[ii];
			for( size_t jj = 0; jj < loop.size(); ++jj )
			{
				const carve::geom::vector<2>&  vec_2d = loop[jj];
				carve::geom::vector<3>  vec_3d( carve::geom::VECTOR( vec_2d.x, vec_2d.y, 0 ) );

				// cross section is defined in XY plane
				vec_3d = matrix_first_direction*vec_3d + curve_point_first;
				polyhedron_points[polyhedron_point_index++] = vec_3d;
			}
		}

		for( size_t ii = 1; ii < num_curve_points; ++ii )
		{
			carve::geom::vector<3> curve_point_current = curve_points[ii];
			carve::geom::vector<3> curve_point_next;
			carve::geom::vector<3> curve_point_before;
			if( ii == 0 )
			{
				// first point
				curve_point_next = curve_points[ii + 1];
				carve::geom::vector<3> delta_element = curve_point_next - curve_point_current;
				curve_point_before = curve_point_current - ( delta_element );
			}
			else if( ii == num_curve_points - 1 )
			{
				// last point
				curve_point_before = curve_points[ii - 1];
				carve::geom::vector<3> delta_element = curve_point_current - curve_point_before;
				curve_point_next = curve_point_before + ( delta_element );
			}
			else
			{
				// inner point
				curve_point_next = curve_points[ii + 1];
				curve_point_before = curve_points[ii - 1];
			}

			carve::geom::vector<3> bisecting_normal;
			GeomUtils::bisectingPlane( curve_point_before, curve_point_current, curve_point_next, bisecting_normal );

			carve::geom::vector<3> section1 = curve_point_current - curve_point_before;
			carve::geom::vector<3> section2 = curve_point_next - curve_point_current;
			section1.normalize();
			section2.normalize();
			double dot_product = dot( section1, section2 );
			double dot_product_abs = std::abs( dot_product );

			if( dot_product_abs < ( 1.0 - 0.0001 ) || dot_product_abs >( 1.0 + 0.0001 ) )
			{
				// bend found, compute next local z vector
				// TODO: vector with local z vectors as parameter
				carve::geom::vector<3> lateral_vec = cross( section1, section2 );
				local_z = cross( lateral_vec, section1 );
				local_z.normalize();
			}
			if( ii == num_curve_points - 1 )
			{
				bisecting_normal *= -1.0;
			}

			carve::geom::plane<3> bisecting_plane( bisecting_normal, curve_point_current );
			for( size_t jj = 0; jj < num_points_in_all_loops; ++jj )
			{
				carve::geom::vector<3>&  section_point_3d = polyhedron_points[polyhedron_point_index];
				carve::geom::vector<3>&  previous_section_point_3d = polyhedron_points[polyhedron_point_index - num_points_in_all_loops];

				polyhedron_point_index++;

				carve::geom::vector<3> v;
				double t;
				carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, previous_section_point_3d, previous_section_point_3d + section1, v, t );
				if( intersect > 0 )
				{
					section_point_3d = v;
				}
				else
				{
					messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				}
			}
		}

		// add face loops for all sections
		const size_t num_poly_points = polyhedron_points.size();
		size_t loop_offset = 0;
		for( size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii )
		{
			const std::vector<carve::geom::vector<2> >&  loop = face_loops_used_for_triangulation[ii];

			for( size_t jj = 0; jj < loop.size(); ++jj )
			{
				for( size_t kk = 0; kk < num_curve_points - 1; ++kk )
				{
					size_t tri_idx_a = num_points_in_all_loops*kk + jj + loop_offset;

					size_t tri_idx_next = tri_idx_a + 1;
					if( jj == loop.size() - 1 )
					{
						tri_idx_next -= loop.size();
					}
					size_t tri_idx_up = tri_idx_a + num_points_in_all_loops;  // next section
					size_t tri_idx_next_up = tri_idx_next + num_points_in_all_loops;  // next section


					if( tri_idx_a >= num_poly_points || tri_idx_next >= num_poly_points || tri_idx_up >= num_poly_points || tri_idx_next_up >= num_poly_points )
					{
						messageCallback( "invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
						continue;
					}

					poly_data->addFace( tri_idx_a, tri_idx_next, tri_idx_next_up );
					poly_data->addFace( tri_idx_next_up, tri_idx_up, tri_idx_a );
				}
			}

			loop_offset += loop.size();
		}

		// add front and back cap
		for( size_t ii = 0; ii < face_indexes.size(); ++ii )
		{
			size_t num_face_vertices = face_indexes[ii];

			if( num_face_vertices == 3 )
			{
				size_t tri_idx_a = face_indexes[ii+1];
				size_t tri_idx_b = face_indexes[ii+2];
				size_t tri_idx_c = face_indexes[ii+3];
				if( tri_idx_a >= num_poly_points || tri_idx_b >= num_poly_points || tri_idx_c >= num_poly_points )
				{
					messageCallback( "invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					ii += num_face_vertices;
					continue;
				}

				poly_data->addFace( tri_idx_a, tri_idx_c, tri_idx_b );


				size_t tri_idx_a_back_cap = tri_idx_a + num_poly_points - num_points_in_all_loops;
				size_t tri_idx_b_back_cap = tri_idx_b + num_poly_points - num_points_in_all_loops;
				size_t tri_idx_c_back_cap = tri_idx_c + num_poly_points - num_points_in_all_loops;
				if( tri_idx_a_back_cap >= num_poly_points || tri_idx_b_back_cap >= num_poly_points || tri_idx_c_back_cap >= num_poly_points )
				{
					messageCallback( "invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					ii += num_face_vertices;
					continue;
				}

				poly_data->addFace( tri_idx_a_back_cap, tri_idx_b_back_cap, tri_idx_c_back_cap );
			}
			else if( num_face_vertices == 2 )
			{
				// add polyline
				//poly_data->addFace( face_indexes[ii+1], face_indexes[ii+2] );
			}
			else if( num_face_vertices == 1 )
			{
				// add polyline
				//poly_data->addFace( face_indexes[ii+1], face_indexes[ii+2] );
			}
			else
			{
				messageCallback( "num_face_vertices != 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			}
			ii += num_face_vertices;

		}


		try
		{
			item_data->addClosedPolyhedron( poly_data );
		}
		catch( IfcPPException& exception )
		{
			messageCallback( exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", ifc_entity );  // calling function already in e.what()
		}

#ifdef _DEBUG
		shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts() ) );
		CSG_Adapter::checkMeshSetValidAndClosed( meshset, this, ifc_entity );
#endif
	}

	/*\brief Creates a triangulated face
	\param[in] Curves as face boundaries. The first input curve is the outer boundary, succeeding curves are inner boundaries
	\param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	\param[out] poly_data Result input object
	**/

	void createFace( const std::vector<std::vector<carve::geom::vector<3> > >& vec_bounds, IfcPPEntity* ifc_entity, PolyInputCache3D& poly_cache )
	{
		std::vector<std::vector<carve::geom::vector<2> > > face_loops_2d;
		std::vector<std::vector<double> > face_loop_3rd_dim;
		std::map<size_t, size_t> map_merged_idx;
		bool face_loop_reversed = false;
		bool warning_small_loop_detected = false;
		GeomUtils::ProjectionPlane face_plane = GeomUtils::UNDEFINED;

		for( auto it_bounds = vec_bounds.begin(); it_bounds != vec_bounds.end(); ++it_bounds )
		{
			const std::vector<carve::geom::vector<3> >& loop_points = *it_bounds;

			if( loop_points.size() < 3 )
			{
				if( it_bounds == vec_bounds.begin() )
				{
					break;
				}
				else
				{
					continue;
				}
			}

			if( loop_points.size() == 3 && vec_bounds.size() == 1 )
			{
				std::vector<size_t> triangle_indexes;
				for( size_t point_i = 0; point_i < 3; ++point_i )
				{
					carve::geom::vector<3> v = loop_points[point_i];
					size_t vertex_id = poly_cache.addPoint( v );
					triangle_indexes.push_back( vertex_id );
					map_merged_idx[point_i] = vertex_id;
				}

				if( triangle_indexes.size() != 3 )
				{
					messageCallback( "triangle_indexes.size() != 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}

				poly_cache.m_poly_data->addFace( triangle_indexes[0], triangle_indexes[1], triangle_indexes[2] );
				continue;
			}

			carve::geom3d::Vector normal = GeomUtils::computePolygonNormal( loop_points );
			if( it_bounds == vec_bounds.begin() )
			{
				double nx = std::abs( normal.x );
				double ny = std::abs( normal.y );
				double nz = std::abs( normal.z );
				if( nz > nx && nz >= ny )
				{
					face_plane = GeomUtils::XY_PLANE;
				}
				else if( nx >= ny && nx >= nz )
				{
					face_plane = GeomUtils::YZ_PLANE;
				}
				else if( ny > nx && ny >= nz )
				{
					face_plane = GeomUtils::XZ_PLANE;
				}
				else
				{
					std::stringstream err;
					err << "unable to project to plane: nx" << nx << " ny " << ny << " nz " << nz << std::endl;
					messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}
			}

			// project face into 2d plane
			std::vector<carve::geom::vector<2> > path_loop_2d;
			std::vector<double> path_loop_3rd_dim;

			for( size_t i = 0; i < loop_points.size(); ++i )
			{
				const carve::geom::vector<3>& point = loop_points[i];

				if( face_plane == GeomUtils::XY_PLANE )
				{
					path_loop_2d.push_back( carve::geom::VECTOR( point.x, point.y ) );
					path_loop_3rd_dim.push_back( point.z );
				}
				else if( face_plane == GeomUtils::YZ_PLANE )
				{
					path_loop_2d.push_back( carve::geom::VECTOR( point.y, point.z ) );
					path_loop_3rd_dim.push_back( point.x );
				}
				else if( face_plane == GeomUtils::XZ_PLANE )
				{
					path_loop_2d.push_back( carve::geom::VECTOR( point.x, point.z ) );
					path_loop_3rd_dim.push_back( point.y );
				}
			}

			if( loop_points.size() == 4 && vec_bounds.size() == 1 )
			{
				if( carve::geom2d::quadIsConvex( path_loop_2d[0], path_loop_2d[1], path_loop_2d[2], path_loop_2d[3] ) )
				{
					// add 2 triangles for quad
					std::vector<size_t> triangle_indexes;
					for( size_t point_i = 0; point_i < 4; ++point_i )
					{
						carve::geom::vector<3> v = loop_points[point_i];
						size_t vertex_index = poly_cache.addPoint( v );
						map_merged_idx[point_i] = vertex_index;
						triangle_indexes.push_back( vertex_index );
					}

					poly_cache.m_poly_data->addFace( triangle_indexes[0], triangle_indexes[1], triangle_indexes[2] );
					poly_cache.m_poly_data->addFace( triangle_indexes[2], triangle_indexes[3], triangle_indexes[0] );
					continue;
				}
			}

			// check winding order
			carve::geom3d::Vector normal_2d = GeomUtils::computePolygon2DNormal( path_loop_2d );
			if( it_bounds == vec_bounds.begin() )
			{
				if( normal_2d.z < 0 )
				{
					std::reverse( path_loop_2d.begin(), path_loop_2d.end() );
					face_loop_reversed = true;
				}
			}
			else
			{
				if( normal_2d.z > 0 )
				{
					std::reverse( path_loop_2d.begin(), path_loop_2d.end() );
				}
			}

			if( path_loop_2d.size() < 3 )
			{
				//std::cout << __FUNC__ << ": #" << face_id <<  "=IfcFace: path_loop.size() < 3" << std::endl;
				continue;
			}

			double signed_area = carve::geom2d::signedArea( path_loop_2d );
			double min_loop_area = 1.e-10;//m_geom_settings->m_min_face_area
			if( std::abs( signed_area ) < min_loop_area )
			{
				warning_small_loop_detected = true;
				continue;
			}

			face_loops_2d.push_back( path_loop_2d );
			face_loop_3rd_dim.push_back( path_loop_3rd_dim );
		}

		if( warning_small_loop_detected )
		{
			std::stringstream err;
			err << "abs( signed_area ) < 1.e-10";
			messageCallback( err.str().c_str(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, ifc_entity );
		}

		if( face_loops_2d.size() > 0 )
		{
			std::vector<std::pair<size_t, size_t> > result; // first is loop index, second is vertex index in loop
			std::vector<carve::geom::vector<2> > merged;
			std::vector<carve::geom3d::Vector> merged_3d;
			std::vector<carve::triangulate::tri_idx> triangulated;

			try
			{
				result = carve::triangulate::incorporateHolesIntoPolygon( face_loops_2d );
				merged.reserve( result.size() );
				for( size_t i = 0; i < result.size(); ++i )
				{
					size_t loop_number = result[i].first;
					size_t index_in_loop = result[i].second;
					carve::geom::vector<2> & loop_point = face_loops_2d[loop_number][index_in_loop];
					merged.push_back( loop_point );

					// restore 3rd dimension
					if( face_loop_reversed )
					{
						index_in_loop = face_loops_2d[loop_number].size() - index_in_loop - 1;
					}

					carve::geom3d::Vector v;
					if( face_plane == GeomUtils::XY_PLANE )
					{
						double z = face_loop_3rd_dim[loop_number][index_in_loop];
						v = carve::geom::VECTOR( loop_point.x, loop_point.y, z );
					}
					else if( face_plane == GeomUtils::YZ_PLANE )
					{
						double x = face_loop_3rd_dim[loop_number][index_in_loop];
						v = carve::geom::VECTOR( x, loop_point.x, loop_point.y );
					}
					else if( face_plane == GeomUtils::XZ_PLANE )
					{
						double y = face_loop_3rd_dim[loop_number][index_in_loop];
						v = carve::geom::VECTOR( loop_point.x, y, loop_point.y );
					}
					merged_3d.push_back( v );
				}
				carve::triangulate::triangulate( merged, triangulated );
				carve::triangulate::improve( merged, triangulated );

			}
			catch( ... )
			{
				messageCallback( "carve::triangulate::incorporateHolesIntoPolygon failed ", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
				return;
			}

			// now insert points to polygon, avoiding points with same coordinates
			for( size_t i = 0; i != merged.size(); ++i )
			{
				const carve::geom::vector<3>& v = merged_3d[i];
				size_t vertex_index = poly_cache.addPoint( v );
				map_merged_idx[i] = vertex_index;
			}
			for( size_t i = 0; i != triangulated.size(); ++i )
			{
				carve::triangulate::tri_idx triangle = triangulated[i];
				size_t a = triangle.a;
				size_t b = triangle.b;
				size_t c = triangle.c;

				size_t vertex_id_a = map_merged_idx[a];
				size_t vertex_id_b = map_merged_idx[b];
				size_t vertex_id_c = map_merged_idx[c];

				if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
				{
					continue;
				}

#ifdef _DEBUG
				const carve::poly::Vertex<3>& v_a = poly_cache.m_poly_data->getVertex( vertex_id_a );
				const carve::poly::Vertex<3>& v_b = poly_cache.m_poly_data->getVertex( vertex_id_b );

				double dx = v_a.v[0] - v_b.v[0];
				if( std::abs( dx ) < 0.0000001 )
				{
					double dy = v_a.v[1] - v_b.v[1];
					if( std::abs( dy ) < 0.0000001 )
					{
						double dz = v_a.v[2] - v_b.v[2];
						if( std::abs( dz ) < 0.0000001 )
						{
							messageCallback( "abs(dx) < 0.00001 && abs(dy) < 0.00001 && abs(dz) < 0.00001", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
						}
					}
				}
#endif

				if( face_loop_reversed )
				{
					poly_cache.m_poly_data->addFace( vertex_id_a, vertex_id_c, vertex_id_b );
				}
				else
				{
					poly_cache.m_poly_data->addFace( vertex_id_a, vertex_id_b, vertex_id_c );
				}
			}
		}
	}
};
