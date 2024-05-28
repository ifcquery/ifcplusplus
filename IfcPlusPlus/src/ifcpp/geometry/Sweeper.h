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

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <earcut/include/mapbox/earcut.hpp>
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
	virtual ~Sweeper(){}

	/*\brief Extrudes a set of cross sections along a direction
	  \param[in] paths Set of cross sections to extrude
	  \param[in] dir Extrusion vector
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] itemData Container to add result polyhedron or polyline
	**/
	void extrude(const std::vector<std::vector<vec2> >& faceLoopsInput, const vec3 extrusionVector, shared_ptr<ItemShapeData>& itemData, GeomProcessingParams& params)
	{
		// TODO: complete and test
		if( faceLoopsInput.size() == 0 )
		{
			messageCallback("faceLoopsInput.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
			return;
		}

		// loops and indexes
		//  3----------------------------2
		//  |                            |
		//  |   1-------------------2    |3---------2
		//  |   |                   |    |          |
		//  |   |                   |    |          |face_loops[2]
		//  |   0---face_loops[1]---3    |0---------1
		//  |                            |
		//  0-------face_loops[0]--------1

		double eps = m_geom_settings->getEpsilonMergePoints();
		double epsCoplanarAngle = m_geom_settings->getEpsilonCoplanarAngle();
		if( extrusionVector.length2() < eps*eps*100 )
		{
#ifdef _DEBUG
			std::cout << "extrusionVector.length2() == 0" << std::endl;
#endif
			// maybe still use it as flat surface instead of extruded volume
			return;
		}

		struct FaceLoop
		{
			std::vector<array2d> m_loop;
			double area = 0;
		};

		struct FaceLoopSet
		{
			std::vector<shared_ptr<FaceLoop> > m_faceLoops;

			void insertFaceLoopOrCreateNewFaceLoopSet(shared_ptr<FaceLoop>& newFaceLoop, shared_ptr<FaceLoopSet>& newFaceLoopSet, double eps)
			{
				if (m_faceLoops.size() == 0)
				{
					m_faceLoops.push_back(newFaceLoop);
					return;
				}

				// the first loop is the one with the biggest area. So the new loop has to be checked if it is inside or outside
				// of the first loop
				shared_ptr<FaceLoop>& firstFaceLoop = m_faceLoops.front();
				double existingLoopArea = firstFaceLoop->area;

				std::vector<array2d> newLoopArray = newFaceLoop->m_loop;
				std::vector<array2d> existingLoopArray = firstFaceLoop->m_loop;
				size_t numPointsInExistingPolygon = 0;
				size_t numPointsOnExistingPolygon = 0;
				size_t numPointsOutsideExistingPolygon = 0;
				for (auto it = newLoopArray.begin(); it != newLoopArray.end(); ++it)
				{
					const array2d& point = *it;
					GeomUtils::PointPosition pointIsInseExistingPolygon = GeomUtils::pointInPolySimple(point, existingLoopArray, eps);
					if (pointIsInseExistingPolygon == GeomUtils::INSIDE)
					{
#ifdef _DEBUG
						std::vector<vec2> carveLoop;
						for (auto polygonPoint : existingLoopArray)
						{
							carveLoop.push_back(carve::geom::VECTOR(polygonPoint[0], polygonPoint[1]));
						}
						carve::geom::aabb<2> bboxPolygon;
						bboxPolygon.fit(carveLoop.begin(), carveLoop.end());
						bool pointInBBox = bboxPolygon.containsPoint(carve::geom::VECTOR(point[0], point[1]));
						if (!pointInBBox)
						{
							vec4 color(0.3, 0.4, 0.5, 1.0);
							GeomDebugDump::dumpPolyline(existingLoopArray, color, 0, false, false);
							std::string label = "x";
							vec3 point3 = carve::geom::VECTOR(point[0], point[1], 0);
							GeomDebugDump::dumpVertex(point3, color, label);
							GeomDebugDump::moveOffset(bboxPolygon.extent.y * 2);
						}
#endif
						++numPointsInExistingPolygon;
					}
					else if (pointIsInseExistingPolygon == GeomUtils::ON_EDGE)
					{
#ifdef _DEBUG
						if (false)
						{
							vec4 color(0.3, 0.4, 0.5, 1.0);
							GeomDebugDump::dumpPolyline(existingLoopArray, color, 0, true, false);
							std::string label = "x";
							vec3 point3 = carve::geom::VECTOR(point[0], point[1], 0);
							GeomDebugDump::dumpVertex(point3, color, label);
						}
#endif
						++numPointsOnExistingPolygon;
					}
					else
					{
						++numPointsOutsideExistingPolygon;
					}
				}

				// triangulation with several polygons works only when one is completely inside the other
				bool validForCombiningPolygons = numPointsInExistingPolygon > 0;
				if (numPointsOutsideExistingPolygon > 0)
				{
					// if at least one point of new polygon is outside, it can't be combined
					validForCombiningPolygons = false;
				}
				if (numPointsOnExistingPolygon > 0)
				{
					// ok if all other points are inside
					// could be fully inside, but all points on edges of outer polygon:
					//  -------------------------
					//  |                       |
					//  |                       |
					//  |      poly1            |  poly2 fully overlaps poly1
					//  |-----------------------|
					//  |      poly2            |
					//  -------------------------

					validForCombiningPolygons = false;
				}
				if (validForCombiningPolygons)
				{
					// insert into existing face loop set
					if (newFaceLoop->area > existingLoopArea)
					{
						// new loop is biggest loop
						m_faceLoops.insert(m_faceLoops.begin(), newFaceLoop);

					}
					else
					{
						m_faceLoops.push_back(newFaceLoop);
					}
				}
				else
				{
					// new polygon is not inside, at least intersecting other polygon, so can't be combined
					newFaceLoopSet = make_shared<FaceLoopSet>();
					newFaceLoopSet->m_faceLoops.push_back(newFaceLoop);
				}
			}
		};
		std::vector<shared_ptr<FaceLoopSet> > faceLoopsTriangulate;
		shared_ptr<FaceLoopSet> currentFaceLoopSet;
		std::vector<double> polygon3DArea;
		bool face_loop_reversed = false;
		bool warning_small_loop_detected = false;
		bool errorOccured = false;
		GeomUtils::ProjectionPlane face_plane = GeomUtils::ProjectionPlane::UNDEFINED;
		vec3 normal = carve::geom::VECTOR(0, 0, 1);

		for( auto it_bounds = faceLoopsInput.begin(); it_bounds != faceLoopsInput.end(); ++it_bounds )
		{
			std::vector<vec2> loopPointsInput = *it_bounds;

			if( loopPointsInput.size() < 3 )
			{
				if( it_bounds == faceLoopsInput.begin() )
				{
					break;
				}
				else
				{
					continue;
				}
			}

			bool mergeAlignedEdges = true;
			GeomUtils::simplifyPolygon(loopPointsInput, mergeAlignedEdges, eps, epsCoplanarAngle);
			GeomUtils::unClosePolygon(loopPointsInput, eps);

			normal = GeomUtils::computePolygon2DNormal(loopPointsInput);
			if (normal.z < 0)
			{
				std::reverse(loopPointsInput.begin(), loopPointsInput.end());
				normal = -normal;
			}

			shared_ptr<FaceLoop> fl = make_shared<FaceLoop>();
			std::vector<array2d >& path_loop_2d = fl->m_loop;
			for( size_t i = 0; i < loopPointsInput.size(); ++i )
			{
				const vec2& point = loopPointsInput[i];
				path_loop_2d.push_back({ point.x, point.y });
			}

			if( path_loop_2d.size() < 3 )
			{
				//std::cout << __FUNC__ << ": #" << face_id <<  "=IfcFace: path_loop.size() < 3" << std::endl;
				continue;
			}

			double loop_area = std::abs(GeomUtils::signedArea(path_loop_2d));
			double min_loop_area = m_geom_settings->getEpsilonMergePoints();
			if( loop_area < min_loop_area )
			{
				warning_small_loop_detected = true;
				continue;
			}
			fl->area = loop_area;

			if (!currentFaceLoopSet)
			{
				currentFaceLoopSet = make_shared<FaceLoopSet>();
				faceLoopsTriangulate.push_back(currentFaceLoopSet);
			}
			if (faceLoopsTriangulate.size() > 1)
			{
				// try if new face loop is connected or inside one of the existing loop sets
				shared_ptr<FaceLoopSet> detachedLoop;
				bool attachmentToExistingLoopFound = false;
				for (shared_ptr<FaceLoopSet>& existingLoopSet : faceLoopsTriangulate)
				{
					
					existingLoopSet->insertFaceLoopOrCreateNewFaceLoopSet(fl, detachedLoop, eps);

					if (!detachedLoop)
					{
						attachmentToExistingLoopFound = true;
					}
				}
				if (!attachmentToExistingLoopFound)
				{
					if (detachedLoop)
					{
						faceLoopsTriangulate.push_back(detachedLoop);
					}
					else
					{
						std::cout << "unexpected detachedLoop missing";
					}
				}
			}
			else
			{
				shared_ptr<FaceLoopSet> detachedLoop;
				currentFaceLoopSet->insertFaceLoopOrCreateNewFaceLoopSet(fl, detachedLoop, eps);

				if (detachedLoop)
				{
					faceLoopsTriangulate.push_back(detachedLoop);
				}
			}
		}

		if( warning_small_loop_detected )
		{
			std::stringstream err;
			err << "std::abs( signed_area ) < 1.e-10";
			messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, params.ifc_entity);
		}

		if( faceLoopsTriangulate.size() == 0 )
		{
#ifdef _DEBUG
			std::cout << "faceLoopsTriangulate.size() == 0" << std::endl;
#endif
			return;
		}

		for (shared_ptr<FaceLoopSet>& existingLoopSet : faceLoopsTriangulate)
		{
			std::vector<std::vector<array2d>> loopsForEarcut;
			for (size_t iiLoop = 0; iiLoop < existingLoopSet->m_faceLoops.size(); ++iiLoop)
			{
				shared_ptr<FaceLoop>& existingLoop = existingLoopSet->m_faceLoops[iiLoop];
				std::vector<array2d>& loop = existingLoop->m_loop;
				glm::dvec3 normal = GeomUtils::computePolygon2DNormal(loop, eps);
				if (normal.z <= 0 && iiLoop == 0)
				{
					std::reverse(loop.begin(), loop.end());
					//normal = -normal;
				}
				else if (normal.z >= 0 && iiLoop > 0)
				{
					std::reverse(loop.begin(), loop.end());
					//normal = -normal;
				}

				loopsForEarcut.push_back(loop);
			}

			std::vector<uint32_t> triangulated = mapbox::earcut<uint32_t>(loopsForEarcut);
			std::vector<array2d> polygons2dFlatVector;
			GeomUtils::polygons2flatVec(loopsForEarcut, polygons2dFlatVector);
			size_t numPointsInAllLoops = polygons2dFlatVector.size();

#ifdef _DEBUG
			if (params.ifc_entity)
			{
				if (params.ifc_entity->m_tag == 9817817 && false )
				{
					vec4 color(0.3, 0.4, 0.5, 1.0);
					shared_ptr<carve::input::PolyhedronData> poly_data(new carve::input::PolyhedronData());

					// add points bottom
					for (size_t ii = 0; ii < polygons2dFlatVector.size(); ++ii)
					{
						array2d& point2D = polygons2dFlatVector[ii];
						vec3 point3D = carve::geom::VECTOR(point2D[0], point2D[1], 0);
						poly_data->addVertex(point3D);
					}

					// front and back cap
					for (int ii = 0; ii < triangulated.size(); ii += 3)
					{
						size_t idxA = triangulated[ii + 0];
						size_t idxB = triangulated[ii + 1];
						size_t idxC = triangulated[ii + 2];

						if (extrusionVector.z < 0)
						{
							poly_data->addFace(idxA, idxB, idxC);
						}
						else
						{
							poly_data->addFace(idxA, idxC, idxB);
						}
					}

					GeomDebugDump::moveOffset(0.5);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts(), eps));
					GeomDebugDump::dumpMeshset(meshset, color, 0, true, false);
				}
			}
#endif

			shared_ptr<carve::input::PolyhedronData> polyhedronResult(new carve::input::PolyhedronData());

			// add points bottom
			for (size_t ii = 0; ii < polygons2dFlatVector.size(); ++ii)
			{
				array2d& point2D = polygons2dFlatVector[ii];
				vec3 point3D = carve::geom::VECTOR(point2D[0], point2D[1], 0);
				polyhedronResult->addVertex(point3D);
			}

			// add points top
			for (size_t ii = 0; ii < polygons2dFlatVector.size(); ++ii)
			{
				array2d& point2D = polygons2dFlatVector[ii];
				vec3 point3D = carve::geom::VECTOR(point2D[0], point2D[1], 0);
				vec3 point3D_top = point3D + extrusionVector;
				polyhedronResult->addVertex(point3D_top);
			}


			// triangles along the extruded loops
			size_t idxLoopOffset = 0;
			for (size_t ii = 0; ii < loopsForEarcut.size(); ++ii)
			{
				std::vector<array2d>& loop2D = loopsForEarcut[ii];

#ifdef _DEBUG
				glm::dvec3 loopNormal_glm = GeomUtils::computePolygon2DNormal(loop2D, eps);
				vec3 loopNormal = carve::geom::VECTOR(loopNormal_glm.x, loopNormal_glm.y, loopNormal_glm.z);

				if (ii == 0)
				{
					// normal should point up
					if (loopNormal_glm.z <= 0)
					{
						std::cout << "normal should point up" << std::endl;
					}
				}
				else
				{
					if (loopNormal_glm.z > 0)
					{
						std::cout << "normal should point down" << std::endl;
					}
				}
#endif

				bool createQuadsIfPossible = false;
				bool flipFaces = false;

				if (extrusionVector.z < 0)
				{
					flipFaces = !flipFaces;
				}

				const size_t numLoopPoints = loop2D.size();
				for (size_t jj = 0; jj < numLoopPoints; ++jj)
				{
					array2d& point2D = loop2D[jj];
					array2d& point2D_next = loop2D[(jj + 1) % numLoopPoints];

					vec3 point3D = carve::geom::VECTOR(point2D[0], point2D[1], 0);
					vec3 point3D_next = carve::geom::VECTOR(point2D_next[0], point2D_next[1], 0);

					vec3 point3D_top = point3D + extrusionVector;
					vec3 point3D_top_next = point3D_next + extrusionVector;

					size_t idx = jj + idxLoopOffset;
					size_t idx_next = (jj + 1) % numLoopPoints + idxLoopOffset;
					size_t idx_top = idx + numPointsInAllLoops;
					size_t idx_top_next = idx_next + numPointsInAllLoops;

					if (createQuadsIfPossible)
					{
						if (flipFaces)
						{
							polyhedronResult->addFace(idx, idx_top, idx_top_next, idx_next);
						}
						else
						{
							polyhedronResult->addFace(idx, idx_next, idx_top_next, idx_top);
						}
					}
					else
					{
						if (flipFaces)
						{
							polyhedronResult->addFace(idx, idx_top_next, idx_next);
							polyhedronResult->addFace(idx_top_next, idx, idx_top);
						}
						else
						{
							polyhedronResult->addFace(idx, idx_next, idx_top_next);
							polyhedronResult->addFace(idx_top_next, idx_top, idx);
						}
					}
				}

				idxLoopOffset += numLoopPoints;
			}

			// front and back cap
			for (int ii = 0; ii < triangulated.size(); ii += 3)
			{
				size_t idxA = triangulated[ii + 0];
				size_t idxB = triangulated[ii + 1];
				size_t idxC = triangulated[ii + 2];

				size_t idxAtop = idxA + numPointsInAllLoops;
				size_t idxBtop = idxB + numPointsInAllLoops;
				size_t idxCtop = idxC + numPointsInAllLoops;

				if (extrusionVector.z < 0)
				{
					polyhedronResult->addFace(idxA, idxB, idxC);
					polyhedronResult->addFace(idxAtop, idxCtop, idxBtop);
				}
				else
				{
					polyhedronResult->addFace(idxA, idxC, idxB);
					polyhedronResult->addFace(idxAtop, idxBtop, idxCtop);
				}
			}

#ifdef _DEBUG
			std::string details = "";
			bool correct = checkPolyhedronData(polyhedronResult, params, details);
			if (!correct) {
				shared_ptr<carve::mesh::MeshSet<3> > meshset(polyhedronResult->createMesh(carve::input::opts(), eps));
				vec4 color(0.3, 0.4, 0.5, 1.0);
				GeomDebugDump::dumpMeshset(meshset, color, 0, true, false);
			}
#endif
			itemData->addClosedPolyhedron(polyhedronResult, params);
		}
	}
	
	/*\brief Extrudes a circle cross section along a path. At turns, the points are placed in the bisecting plane
	  \param[in] curvePoints Path along which the circle is swept
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] itemData Container to add result polyhedron or polyline
	  \param[in] nvc Number of vertices per circle
	  \param[in] radius_inner If positive value is given, the swept disk becomes a pipe
	**/
	void sweepDisk( const std::vector<vec3>& curvePoints, shared_ptr<ItemShapeData>& itemData, GeomProcessingParams& params, const size_t nvc, const double radius, const double radius_inner = -1 )
	{
		const size_t num_curvePoints = curvePoints.size();
		if( num_curvePoints < 2 )
		{
			messageCallback( "num curve points < 2", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity );
			return;
		}

		if( !itemData )
		{
			messageCallback( "!itemData", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity );
			return;
		}

		if( radius < 0.001 )
		{
			// Cross section is just a point. Create a polyline
			shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
			polyline_data->beginPolyline();
			for( size_t i_polyline = 0; i_polyline < curvePoints.size(); ++i_polyline )
			{
				const vec3& curve_pt = curvePoints[i_polyline];
				polyline_data->addVertex( curve_pt );
				polyline_data->addPolylineIndex( 0 );
				polyline_data->addPolylineIndex( i_polyline );
			}
			itemData->m_polylines.push_back( polyline_data );
			return;
		}

		double use_radius_inner = radius_inner;
		if( radius_inner > radius )
		{
			messageCallback( "radius_inner > radius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity );
			use_radius_inner = radius;
		}

		double eps = params.epsMergePoints;
		vec3 local_z( carve::geom::VECTOR( 0, 0, 1 ) );
		vec3 curve_point_first = curvePoints[0];
		vec3 curve_point_second = curvePoints[1];

		bool bend_found = false;
		if( num_curvePoints > 3 )
		{
			// compute local z vector by dot product of the first bend of the reference line
			vec3 vertex_back2 = curve_point_first;
			vec3 vertex_back1 = curve_point_second;
			for( size_t i = 2; i<num_curvePoints; ++i )
			{
				const vec3& vertex_current = curvePoints[i];
				vec3 section1 = vertex_back1 - vertex_back2;
				vec3 section2 = vertex_current - vertex_back1;
				section1.normalize();
				section2.normalize();

				double dot_product = dot( section1, section2 );
				double dot_product_abs = std::abs(dot_product);

				// if dot == 1 or -1, then points are colinear
				if( dot_product_abs < (1.0-eps) || dot_product_abs > (1.0+eps) )
				{
					// bend found, compute cross product
					vec3 lateral_vec = cross( section1, section2 );
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
			vec3 sweep_dir = curve_point_second - curve_point_first;
			if( sweep_dir.length2() > eps )
			{
				sweep_dir.normalize();

				vec3 perpendicularToSweep = carve::geom::VECTOR(0, 0, 1);
				double dot_sweep_dir = dot(sweep_dir, perpendicularToSweep);
				if( std::abs(dot_sweep_dir - 1.0) < eps )
				{
					perpendicularToSweep = carve::geom::VECTOR(0, 1, 0);
					dot_sweep_dir = dot(sweep_dir, perpendicularToSweep);
				}

				if( std::abs(dot_sweep_dir - 1.0) > eps )
				{
					local_z = cross(perpendicularToSweep, sweep_dir);
					if( local_z.length2() < eps )
					{
						local_z = cross(carve::geom::VECTOR(0, 1, 0), sweep_dir);
						local_z.normalize();
					}
					else
					{
						local_z.normalize();
					}
					double dot_normal_local_z = dot(sweep_dir, local_z);
					if( std::abs(dot_normal_local_z-1.0) < eps )
					{
						local_z = cross(carve::geom::VECTOR(0, 1, 0), sweep_dir);
						local_z.normalize();

						dot_normal_local_z = dot(sweep_dir, local_z);
						if( std::abs(dot_normal_local_z-1.0) < eps )
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
		vec3  section_local_x = carve::geom::cross( section_local_y, section_local_z );
		section_local_y = carve::geom::cross( section_local_x, section_local_z );
		std::vector<vec3> inner_shape_points;
	
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
		std::vector<vec3> circle_points(nvc);
		std::vector<vec3> circle_points_inner(nvc);
		for( size_t ii = 0; ii < nvc; ++ii )
		{
			// cross section (circle) is defined in XY plane
			double x = sin(angle);
			double y = cos(angle);
			vec3 vertex( carve::geom::VECTOR( x*radius, y*radius, 0.0 ) );
			vertex = matrix_first_direction*vertex + curve_point_first;
			circle_points[ii] = vertex;

			if( use_radius_inner > 0 )
			{
				vec3 vertex_inner( carve::geom::VECTOR( x*use_radius_inner, y*use_radius_inner, 0.0 ) );
				vertex_inner = matrix_first_direction*vertex_inner + curve_point_first;
				circle_points_inner[ii] = vertex_inner;
			}
			angle += delta_angle;
		}

		shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData() );
		
		for( size_t ii = 0; ii<num_curvePoints; ++ii )
		{
			const vec3& vertex_current = curvePoints[ii];
			vec3 vertex_next;
			vec3 vertex_before;
			if( ii == 0 )
			{
				// first point
				vertex_next	= curvePoints[ii+1];
				vec3 delta_element = vertex_next - vertex_current;
				vertex_before = vertex_current - (delta_element);
			}
			else if( ii == num_curvePoints-1 )
			{
				// last point
				vertex_before	= curvePoints[ii-1];
				vec3 delta_element = vertex_current - vertex_before;
				vertex_next = vertex_current + (delta_element);
			}
			else
			{
				// inner point
				vertex_next		= curvePoints[ii+1];
				vertex_before	= curvePoints[ii-1];
			}

			vec3 bisecting_normal;
			GeomUtils::bisectingPlane( vertex_before, vertex_current, vertex_next, bisecting_normal, eps );

			vec3 section1 = vertex_current - vertex_before;
			vec3 section2 = vertex_next - vertex_current;
			if (section1.length2() > eps)
			{
				section1.normalize();
				if (section2.length2() > eps)
				{
					section2.normalize();
					double dot_product = dot(section1, section2);
					double dot_product_abs = std::abs(dot_product);

					if (dot_product_abs < (1.0-eps) || dot_product_abs >(1.0+eps))
					{
						// bend found, compute next local z vector
						vec3 lateral_vec = cross(section1, section2);
						if (lateral_vec.length2() > eps)
						{
							local_z = cross(lateral_vec, section1);
							local_z.normalize();
						}
					}
				}
			}

			if( ii == num_curvePoints -1 )
			{
				bisecting_normal *= -1.0;
			}

			carve::geom::plane<3> bisecting_plane( bisecting_normal, vertex_current );
			for( size_t jj = 0; jj < nvc; ++jj )
			{
				vec3& vertex = circle_points[jj];

				vec3 v;
				double t;
				carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t, eps);
				if( intersect > 0 )
				{
					vertex = v;
				}
				else
				{
					messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity );
				}

				poly_data->addVertex( vertex );
			}

			if( use_radius_inner > 0 )
			{
				for( size_t jj = 0; jj < nvc; ++jj )
				{
					vec3& vertex = circle_points_inner[jj];
					
					vec3 v;
					double t;
					carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( bisecting_plane, vertex, vertex + section1, v, t, eps);
					if( intersect > 0 )
					{
						vertex = v;
					}
					else
					{
						messageCallback( "no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity );
					}

					//inner_shape_points[jj] = vertex;
					inner_shape_points.push_back( vertex );
				}
			}
		}

		// outer shape
		size_t num_vertices_outer = poly_data->getVertexCount();
		for( size_t i=0; i<num_curvePoints- 1; ++i )
		{
			size_t i_offset = i*nvc;
			size_t i_offset_next = ( i + 1 )*nvc;
			for( size_t jj = 0; jj < nvc; ++jj )
			{
				size_t current_loop_pt1 = jj + i_offset;
				size_t current_loop_pt2 = ( jj + 1 ) % nvc + i_offset;

				size_t next_loop_pt1 = jj + i_offset_next;
				size_t next_loop_pt2 = ( jj + 1 ) % nvc + i_offset_next;
#ifdef _DEBUG
				if( current_loop_pt1 >= num_vertices_outer || current_loop_pt2 >= num_vertices_outer || next_loop_pt1 >= num_vertices_outer || next_loop_pt2 >= num_vertices_outer )
				{
					std::cout << "current_loop_pt1 >= num_vertices_outer" << std::endl;
				}
#endif
				poly_data->addFace( current_loop_pt1,	next_loop_pt1,		next_loop_pt2 );
				poly_data->addFace( next_loop_pt2,		current_loop_pt2,	current_loop_pt1  );
			}
		}

		if( use_radius_inner > 0 )
		{
			if( inner_shape_points.size() != num_vertices_outer )
			{
				messageCallback( "inner_shape_points.size() != num_vertices_outer", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity );
			}

			// add points for inner shape
			for( size_t i=0; i<inner_shape_points.size(); ++i )
			{
				poly_data->addVertex( inner_shape_points[i] );
			}

			// faces of inner shape
			for( size_t i=0; i<num_curvePoints- 1; ++i )
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
			size_t back_offset = ( num_curvePoints - 1 )*nvc;
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
#ifdef _DEBUG
			if( nvc >= num_vertices_outer )
			{
				std::cout << "nvc >= num_vertices_outer" << std::endl;
			}
#endif
			// front cap, full pipe, create triangle fan
			for( size_t jj = 0; jj < nvc - 2; ++jj )
			{
				poly_data->addFace( 0, jj+1, jj+2 );
			}

			// back cap
			size_t back_offset = ( num_curvePoints - 1 )*nvc;
			for( size_t jj = 0; jj < nvc - 2; ++jj )
			{
				poly_data->addFace( back_offset, back_offset+jj+2, back_offset+jj+1 );
			}
		}

		try
		{
			itemData->addClosedPolyhedron( poly_data, params );
		}
		catch( BuildingException& exception )
		{
			messageCallback( exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", params.ifc_entity );  // calling function already in e.what()
#ifdef _DEBUG
			shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts(), eps ) );
			vec4 color( 0.7, 0.7, 0.7, 1.0 );
			bool drawNormals = true;
			GeomDebugDump::dumpMeshset( meshset, color, drawNormals, true );
#endif
		}

	#ifdef _DEBUG
		shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh(carve::input::opts(), eps) );
		MeshSetInfo infoMesh( this, params.ifc_entity );
		MeshOps::checkMeshSetValidAndClosed( meshset, infoMesh, params);
	#endif
	}

	void findEnclosedLoops(const std::vector<std::vector<vec2> >& faceLoopsInput, std::vector<std::vector<std::vector<vec2> > >& profile_paths_enclosed, double eps)
	{
		if (faceLoopsInput.size() > 1)
		{
			const std::vector<vec2>& loop1 = faceLoopsInput[0];
			std::vector<std::vector<vec2> > enclosed_in_loop1;
			std::vector<std::vector<vec2> > separate_loops;
			enclosed_in_loop1.push_back(loop1);

			for (size_t ii = 1; ii < faceLoopsInput.size(); ++ii)
			{
				const std::vector<vec2>& loop = faceLoopsInput[ii];
				bool loop_enclosed_in_loop1 = GeomUtils::isEnclosed(loop, loop1, eps);

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
			profile_paths_enclosed.push_back(faceLoopsInput);
		}
	}

	void triangulateLoops( const std::vector<std::vector<vec2> >& profile_paths_input, std::vector<std::vector<vec2> >& face_loops_used_for_triangulation, std::vector<int>& face_indexes_out, BuildingEntity* ifc_entity )
	{
		// TODO: complete and test
		if( profile_paths_input.size() == 0 )
		{
			messageCallback( "profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
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

		vec3 normal_first_loop;
		bool warning_small_loop_detected = false;
		bool polyline_created = false;
		double eps = m_geom_settings->getEpsilonMergePoints();
		
		for( size_t i_face_loops = 0; i_face_loops < profile_paths_input.size(); ++i_face_loops )
		{
			const std::vector<vec2>& loop_input = profile_paths_input[i_face_loops];
			std::vector<vec2> loop_2d;
			GeomUtils::copyClosedLoopSkipDuplicates( loop_input, loop_2d, eps );
			
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
			vec3  normal_2d = GeomUtils::computePolygon2DNormal( loop_2d );
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
				messageCallback( "loop_2d.size() < 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
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
			messageCallback( "std::abs( signed_area ) < 1.e-10", StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, ifc_entity );
		}

		if( face_loops_used_for_triangulation.size() == 0 )
		{
			if( polyline_created )
			{
				// already handled as curve
				return;
			}
			messageCallback( "face_loops.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
			return;
		}

		size_t num_vertices_in_loops = 0;
		for( size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii )
		{
			std::vector<vec2>&  loop = face_loops_used_for_triangulation[ii];
			num_vertices_in_loops += loop.size();
		}

		// triangulate
		std::vector<vec2> merged_path;
		std::vector<carve::triangulate::tri_idx> triangulated;
		std::vector<std::pair<size_t, size_t> > path_incorporated_holes;
		try
		{
			path_incorporated_holes = carve::triangulate::incorporateHolesIntoPolygon( face_loops_used_for_triangulation );
			// figure 2: path which incorporates holes, described by path_all_loops
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
			for( size_t i = 0; i < path_incorporated_holes.size(); ++i )
			{
				size_t loop_number = path_incorporated_holes[i].first;
				size_t index_in_loop = path_incorporated_holes[i].second;

				if( loop_number >= face_loops_used_for_triangulation.size() )
				{
					messageCallback( "loop_number >= face_loops_used_for_triangulation.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}
				std::vector<vec2>& loop = face_loops_used_for_triangulation[loop_number];

				if( index_in_loop >= loop.size() )
				{
					messageCallback( "index_in_loop >= loop.size()", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity );
					continue;
				}
				vec2& point_in_loop = loop[index_in_loop];
				merged_path.push_back( point_in_loop );
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
			carve::triangulate::triangulate( merged_path, triangulated, eps );
			carve::triangulate::improve( merged_path, triangulated );
			// triangles: (3,8,9) (2,0,1) (4,6,7)  (4,5,6)  (9,0,2)  (9,2,3)  (7,8,3)  (3,4,7)
		}
		catch( ... )
		{
#ifdef _DEBUG
			messageCallback("carve::triangulate failed", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, ifc_entity);
			vec4 color(0.3, 0.4, 0.5, 1.0);
			GeomDebugDump::dumpPolyline(face_loops_used_for_triangulation, color, true, false );
#endif
			return;
		}

		for( size_t i = 0; i != triangulated.size(); ++i )
		{
			const carve::triangulate::tri_idx& triangle = triangulated[i];
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
	  \param[in] curvePoints Path along which the cross section is swept
	  \param[in] profile_paths Set of cross sections to sweep
	  \param[in] e Ifc entity that the geometry belongs to (just for error messages). Pass a nullptr if no entity at hand.
	  \param[out] itemData Container to add result polyhedron or polyline
	**/
	void sweepArea(const std::vector<vec3>& curvePoints, const std::vector<std::vector<vec2> >& profile_paths_input, shared_ptr<ItemShapeData>& itemData, GeomProcessingParams& params)
	{
		if (profile_paths_input.size() == 0)
		{
			messageCallback("profile_paths.size() == 0", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
			return;
		}
		const size_t num_curvePoints = curvePoints.size();
		if (num_curvePoints < 2)
		{
			messageCallback("num curve points < 2", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
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

		double eps = m_geom_settings->getEpsilonMergePoints();
		std::vector<std::vector<std::vector<vec2> > > profile_paths_enclosed;
		findEnclosedLoops(profile_paths_input, profile_paths_enclosed, eps);

		for (size_t ii_profile_paths = 0; ii_profile_paths < profile_paths_enclosed.size(); ++ii_profile_paths)
		{
			const std::vector<std::vector<vec2> >& profile_paths = profile_paths_enclosed[ii_profile_paths];

			for (size_t ii = 0; ii < profile_paths.size(); ++ii)
			{
				std::vector<int> face_indexes;
				std::vector<std::vector<vec2> > face_loops_used_for_triangulation;
				triangulateLoops(profile_paths, face_loops_used_for_triangulation, face_indexes, params.ifc_entity);

				size_t num_points_in_all_loops = 0;
				for (size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii)
				{
					const std::vector<vec2>& loop = face_loops_used_for_triangulation[ii];
					num_points_in_all_loops += loop.size();
				}

				shared_ptr<carve::input::PolyhedronData> poly_data(new carve::input::PolyhedronData());
				poly_data->points.resize(num_points_in_all_loops * curvePoints.size());

				const vec3& curve_point_first = curvePoints[0];
				const vec3& curve_point_second = curvePoints[1];
				const vec3 curve_normal = GeomUtils::computePolygonNormal(curvePoints, eps);

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
				for (size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii)
				{
					const std::vector<vec2>& loop = face_loops_used_for_triangulation[ii];
					for (size_t jj = 0; jj < loop.size(); ++jj)
					{
						const vec2& vec_2d = loop[jj];
						vec3  vec_3d(carve::geom::VECTOR(vec_2d.x, vec_2d.y, 0));

						// cross section is defined in XY plane
						vec_3d = matrix_first_direction * vec_3d + curve_point_first;
						polyhedron_points[polyhedron_point_index++] = vec_3d;
					}
				}

				for (size_t ii = 1; ii < num_curvePoints; ++ii)
				{
					vec3 curve_point_current = curvePoints[ii];
					vec3 curve_point_next;
					vec3 curve_point_before;
					if (ii == 0)
					{
						// first point
						curve_point_next = curvePoints[ii + 1];
						vec3 delta_element = curve_point_next - curve_point_current;
						curve_point_before = curve_point_current - (delta_element);
					}
					else if (ii == num_curvePoints - 1)
					{
						// last point
						curve_point_before = curvePoints[ii - 1];
						vec3 delta_element = curve_point_current - curve_point_before;
						curve_point_next = curve_point_before + (delta_element);
					}
					else
					{
						// inner point
						curve_point_next = curvePoints[ii + 1];
						curve_point_before = curvePoints[ii - 1];
					}

					vec3 bisecting_normal;
					GeomUtils::bisectingPlane(curve_point_before, curve_point_current, curve_point_next, bisecting_normal, eps);

					vec3 section1 = curve_point_current - curve_point_before;
					section1.normalize();
					if (ii == num_curvePoints - 1)
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
						carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection(bisecting_plane, previous_section_point_3d, previous_section_point_3d + section1, v, t, eps);
						if (intersect > 0)
						{
							section_point_3d = v;
						}
						else
						{
							messageCallback("no intersection found", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
						}
					}
				}

				// add face loops for all sections
				const size_t num_poly_points = polyhedron_points.size();
				size_t loop_offset = 0;
				for (size_t ii = 0; ii < face_loops_used_for_triangulation.size(); ++ii)
				{
					const std::vector<vec2>& loop = face_loops_used_for_triangulation[ii];

					for (size_t jj = 0; jj < loop.size(); ++jj)
					{
						for (size_t kk = 0; kk < num_curvePoints - 1; ++kk)
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
								messageCallback("invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
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
							messageCallback("invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
							ii += num_face_vertices;
							continue;
						}

						poly_data->addFace(tri_idx_a, tri_idx_c, tri_idx_b);

						size_t tri_idx_a_back_cap = tri_idx_a + num_poly_points - num_points_in_all_loops;
						size_t tri_idx_b_back_cap = tri_idx_b + num_poly_points - num_points_in_all_loops;
						size_t tri_idx_c_back_cap = tri_idx_c + num_poly_points - num_points_in_all_loops;
						if (tri_idx_a_back_cap >= num_poly_points || tri_idx_b_back_cap >= num_poly_points || tri_idx_c_back_cap >= num_poly_points)
						{
							messageCallback("invalid triangle index", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
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
						messageCallback("num_face_vertices != 3", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
					}
					ii += num_face_vertices;
				}

				try
				{
					itemData->addClosedPolyhedron(poly_data, params);
				}
				catch (BuildingException & exception)
				{
					messageCallback(exception.what(), StatusCallback::MESSAGE_TYPE_WARNING, "", params.ifc_entity);  // calling function already in e.what()
#ifdef _DEBUG
					vec4 color(0.7, 0.7, 0.7, 1.0);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_data->createMesh(carve::input::opts(), eps));
					bool drawNormals = true;
					GeomDebugDump::dumpMeshset(meshset, color, drawNormals, true);
#endif
				}
			}
		}
	}
};
 