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

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/UnitConverter.h>

#include <IfcAdvancedFace.h>
#include <IfcCurveBoundedPlane.h>
#include <IfcCurveBoundedSurface.h>
#include <IfcCylindricalSurface.h>
#include <IfcFace.h>
#include <IfcFaceBound.h>
#include <IfcPlane.h>
#include <IfcRationalBSplineSurfaceWithKnots.h>
#include <IfcRectangularTrimmedSurface.h>
#include <IfcSurfaceOfLinearExtrusion.h>
#include <IfcSurfaceOfRevolution.h>
#include <IfcSweptSurface.h>

#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "CurveConverter.h"
#include "SplineConverter.h"
#include "PolyInputCache3D.h"
#include "ProfileCache.h"
#include "Sweeper.h"

class SurfaceProxy
{
public:
	virtual void computePointOnSurface(const vec3& point_in, vec3& point_out) = 0;
};

class SurfaceProxyLinear : public SurfaceProxy
{
public:
	virtual void computePointOnSurface(const vec3& point_in, vec3& point_out)
	{
		point_out = m_surface_matrix*point_in;
	}
	carve::math::Matrix m_surface_matrix;
};

class FaceConverter : public StatusCallback
{
public:
	enum ShellType { SHELL_TYPE_UNKONWN, OPEN_SHELL, CLOSED_SHELL };

	shared_ptr<GeometrySettings>	m_geom_settings;
	shared_ptr<UnitConverter>		m_unit_converter;
	shared_ptr<CurveConverter>		m_curve_converter;
	shared_ptr<SplineConverter>		m_spline_converter;
	shared_ptr<Sweeper>				m_sweeper;
	shared_ptr<ProfileCache>		m_profile_cache;

	FaceConverter( shared_ptr<GeometrySettings>& gs, shared_ptr<UnitConverter>& uc, shared_ptr<CurveConverter>& cc, shared_ptr<SplineConverter>& sc, shared_ptr<Sweeper>& sw, shared_ptr<ProfileCache>&	profile_cache )
		: m_geom_settings( gs ), m_unit_converter( uc ), m_curve_converter( cc ), m_spline_converter( sc ), m_sweeper( sw ), m_profile_cache(profile_cache)
	{
	}

	virtual ~FaceConverter(){}

	void convertIfcSurface( const shared_ptr<IfcSurface>& surface, shared_ptr<ItemShapeData>& item_data, shared_ptr<SurfaceProxy>& surface_proxy, double plane_span_default = -1)
	{
		//ENTITY IfcSurface ABSTRACT SUPERTYPE OF(ONEOF(IfcBoundedSurface, IfcElementarySurface, IfcSweptSurface))
		shared_ptr<IfcBoundedSurface> bounded_surface = dynamic_pointer_cast<IfcBoundedSurface>( surface );
		if( bounded_surface )
		{
			// ENTITY IfcBoundedSurface ABSTRACT SUPERTYPE OF(ONEOF(IfcBSplineSurface, IfcCurveBoundedPlane, IfcCurveBoundedSurface, IfcRectangularTrimmedSurface))
			if( dynamic_pointer_cast<IfcBSplineSurface>( bounded_surface ) )
			{
				if( dynamic_pointer_cast<IfcRationalBSplineSurfaceWithKnots>( bounded_surface ) )
				{
					shared_ptr<IfcRationalBSplineSurfaceWithKnots> nurbs_surface = dynamic_pointer_cast<IfcRationalBSplineSurfaceWithKnots>( bounded_surface );
					if( nurbs_surface )
					{
						shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
						m_spline_converter->convertIfcBSplineSurface( nurbs_surface, polyline_data );
						if( polyline_data->getVertexCount() > 1 )
						{
							item_data->m_polylines.push_back( polyline_data );
						}
					}
				}
			}
			else if( dynamic_pointer_cast<IfcCurveBoundedPlane>( bounded_surface ) )
			{
				// ENTITY IfcCurveBoundedPlane SUBTYPE OF IfcBoundedSurface;
				shared_ptr<IfcCurveBoundedPlane> curve_bounded_plane = dynamic_pointer_cast<IfcCurveBoundedPlane>( bounded_surface );
				shared_ptr<TransformData> curve_bounded_plane_transform;
				shared_ptr<IfcPlane>& basis_surface = curve_bounded_plane->m_BasisSurface;
				if( basis_surface )
				{
					shared_ptr<IfcAxis2Placement3D>& basis_surface_placement = basis_surface->m_Position;

					if( basis_surface_placement )
					{
						m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D( basis_surface_placement, curve_bounded_plane_transform);
					}
				}

				// convert outer boundary
				shared_ptr<IfcCurve>& outer_boundary = curve_bounded_plane->m_OuterBoundary;

				std::vector<CurveConverter::CurveSegment> segments;
				std::vector<std::vector<vec3> > face_loops;
				face_loops.push_back( std::vector<vec3>() );
				std::vector<vec3>& outer_boundary_loop = face_loops.back();
				m_curve_converter->convertIfcCurve( outer_boundary, segments, true );
				for (auto& seg : segments)
				{
					std::copy(seg.m_points.begin(), seg.m_points.end(), std::back_inserter(outer_boundary_loop));
				}

				// convert inner boundaries
				std::vector<shared_ptr<IfcCurve> >& vec_inner_boundaries = curve_bounded_plane->m_InnerBoundaries;			//optional
				for( auto& inner_boundary : vec_inner_boundaries )
				{
					if( !inner_boundary )
					{
						continue;
					}
					face_loops.push_back( std::vector<vec3>() );
					std::vector<CurveConverter::CurveSegment> segmentsInner;
					std::vector<vec3>& inner_boundary_loop = face_loops.back();
					m_curve_converter->convertIfcCurve( inner_boundary, segmentsInner, true );
					for (auto& seg : segmentsInner)
					{
						std::copy(seg.m_points.begin(), seg.m_points.end(), std::back_inserter(inner_boundary_loop));
					}
				}

				double eps = m_geom_settings->getEpsilonMergePoints();
				PolyInputCache3D poly_cache(eps);
				bool mergeAlignedEdges = true;
				GeomProcessingParams params( m_geom_settings, outer_boundary.get(),  this );
				createTriangulated3DFace( face_loops, poly_cache, params, false );
				item_data->addOpenPolyhedron( poly_cache.m_poly_data, params );
				item_data->applyTransformToItem(curve_bounded_plane_transform->m_matrix, eps, false );
			}
			else if( dynamic_pointer_cast<IfcCurveBoundedSurface>( bounded_surface ) )
			{
				shared_ptr<IfcCurveBoundedSurface> curve_bounded_surface = dynamic_pointer_cast<IfcCurveBoundedSurface>( bounded_surface );
				shared_ptr<IfcSurface>& basis_surface = curve_bounded_surface->m_BasisSurface;
				if( basis_surface )
				{
					convertIfcSurface( basis_surface, item_data, surface_proxy );
				}

				// TODO: implement
#ifdef _DEBUG
				std::cout << "IfcCurveBoundedSurface boundaries not implemented." << std::endl;
#endif
			}
			else if( dynamic_pointer_cast<IfcRectangularTrimmedSurface>( bounded_surface ) )
			{
				shared_ptr<IfcRectangularTrimmedSurface> rectengular_trimmed_surface = dynamic_pointer_cast<IfcRectangularTrimmedSurface>( bounded_surface );

				shared_ptr<IfcSurface>& basis_surface = rectengular_trimmed_surface->m_BasisSurface;
				if( basis_surface )
				{
					convertIfcSurface( basis_surface, item_data, surface_proxy );
				}

				//shared_ptr<IfcParameterValue>& u1 = rectengular_trimmed_surface->m_U1;
				//shared_ptr<IfcParameterValue>& v1 = rectengular_trimmed_surface->m_V1;
				//shared_ptr<IfcParameterValue>& u2 = rectengular_trimmed_surface->m_U2;
				//shared_ptr<IfcParameterValue>& v2 = rectengular_trimmed_surface->m_V2;
				//bool u_sense = rectengular_trimmed_surface->m_Usense;
				//bool v_sense = rectengular_trimmed_surface->m_Vsense;
				// TODO: implement
#ifdef _DEBUG
				std::cout << "IfcRectangularTrimmedSurface U1, V1, U2, V2 not implemented." << std::endl;
#endif
			}
			return;
		}

		shared_ptr<IfcElementarySurface> elementary_surface = dynamic_pointer_cast<IfcElementarySurface>( surface );
		if( elementary_surface )
		{
			//ENTITY IfcElementarySurface	ABSTRACT SUPERTYPE OF(ONEOF(IfcCylindricalSurface, IfcPlane))
			shared_ptr<IfcAxis2Placement3D>& elementary_surface_placement = elementary_surface->m_Position;

			shared_ptr<TransformData> elementary_surface_transform;
			if( elementary_surface_placement )
			{
				m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D( elementary_surface_placement, elementary_surface_transform );
			}

			shared_ptr<SurfaceProxyLinear> proxy_linear( new SurfaceProxyLinear() );
			if( elementary_surface_transform )
			{
				proxy_linear->m_surface_matrix = elementary_surface_transform->m_matrix;
			}
			surface_proxy = proxy_linear;

			shared_ptr<IfcPlane> elementary_surface_plane = dynamic_pointer_cast<IfcPlane>( elementary_surface );
			if( elementary_surface_plane )
			{
				//  1----0     create big rectangular plane
				//  |    |     ^ y
				//  |    |     |
				//  2----3     ---> x
				{
					double plane_span = HALF_SPACE_BOX_SIZE*m_unit_converter->getCustomLengthFactor();
					if( plane_span_default > 0 )
					{
						plane_span = plane_span_default;
					}
					shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
					polyline_data->beginPolyline();
					polyline_data->addVertex( proxy_linear->m_surface_matrix*carve::geom::VECTOR( plane_span, plane_span, 0.0 ) );
					polyline_data->addVertex( proxy_linear->m_surface_matrix*carve::geom::VECTOR( -plane_span, plane_span, 0.0 ) );
					polyline_data->addVertex( proxy_linear->m_surface_matrix*carve::geom::VECTOR( -plane_span, -plane_span, 0.0 ) );
					polyline_data->addVertex( proxy_linear->m_surface_matrix*carve::geom::VECTOR( plane_span, -plane_span, 0.0 ) );

					polyline_data->addPolylineIndex( 0 );
					polyline_data->addPolylineIndex( 1 );
					polyline_data->addPolylineIndex( 2 );
					polyline_data->addPolylineIndex( 3 );
					item_data->m_polylines.push_back( polyline_data );
				}
				return;
			}

			shared_ptr<IfcCylindricalSurface> cylindrical_surface = dynamic_pointer_cast<IfcCylindricalSurface>( elementary_surface );
			if( cylindrical_surface )
			{
				shared_ptr<IfcPositiveLengthMeasure> cylindrical_surface_radius = cylindrical_surface->m_Radius;
				double circle_radius = cylindrical_surface_radius->m_value;

				int num_segments = m_geom_settings->getNumVerticesPerCircleWithRadius(circle_radius);
				double start_angle = 0.0;
				double opening_angle = M_PI*2.0;
				const double circle_center_x = 0.0;
				const double circle_center_y = 0.0;

				std::vector<vec2> circle_points;
				GeomUtils::addArcWithEndPoint( circle_points, circle_radius, start_angle, opening_angle, circle_center_x, circle_center_y, num_segments );

				// apply position and insert points
				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				polyline_data->beginPolyline();
				for( size_t i = 0; i < circle_points.size(); ++i )
				{
					vec2& point = circle_points[i];
					vec3 point3d( carve::geom::VECTOR( point.x, point.y, 0 ) );
					if( elementary_surface_transform )
					{
						polyline_data->addVertex( elementary_surface_transform->m_matrix*point3d );
					}
					polyline_data->addPolylineIndex( i );
				}
				item_data->m_polylines.push_back( polyline_data );
				return;
			}

			throw UnhandledRepresentationException( surface );
		}

		shared_ptr<IfcSweptSurface> swept_surface = dynamic_pointer_cast<IfcSweptSurface>( surface );
		if( dynamic_pointer_cast<IfcSweptSurface>( surface ) )
		{
			// ENTITY IfcSweptSurface	ABSTRACT SUPERTYPE OF(ONEOF(IfcSurfaceOfLinearExtrusion, IfcSurfaceOfRevolution))
			shared_ptr<IfcProfileDef>& swept_surface_profile = swept_surface->m_SweptCurve;
			shared_ptr<ProfileConverter> profileCon = m_profile_cache->getProfileConverter(swept_surface_profile, true);
			if (profileCon)
			{
				const std::vector<std::vector<vec2> >& swept_profile = profileCon->getCoordinates();

				shared_ptr<IfcAxis2Placement3D>& swept_surface_placement = swept_surface->m_Position;

				shared_ptr<TransformData> swept_surface_transform;
				if (swept_surface_placement)
				{
					m_curve_converter->getPlacementConverter()->convertIfcAxis2Placement3D(swept_surface_placement, swept_surface_transform);
				}

				shared_ptr<IfcSurfaceOfLinearExtrusion> linear_extrusion = dynamic_pointer_cast<IfcSurfaceOfLinearExtrusion>(swept_surface);
				if (linear_extrusion)
				{
					shared_ptr<IfcDirection>& ifc_extrusion_direction = linear_extrusion->m_ExtrudedDirection;
					if (ifc_extrusion_direction)
					{
						vec3 extrusion_direction = carve::geom::VECTOR(ifc_extrusion_direction->m_DirectionRatios[0]->m_value, ifc_extrusion_direction->m_DirectionRatios[1]->m_value, ifc_extrusion_direction->m_DirectionRatios[2]->m_value);

						double factor = 1.0;
						if (linear_extrusion->m_Depth)
						{
							factor = linear_extrusion->m_Depth->m_value;
							extrusion_direction *= factor;
						}

						GeomProcessingParams params(m_geom_settings, surface.get(), this);
						m_sweeper->extrude(swept_profile, extrusion_direction, item_data, params);
					}

					return;
				}
			}
			shared_ptr<IfcSurfaceOfRevolution> suface_of_revolution = dynamic_pointer_cast<IfcSurfaceOfRevolution>( swept_surface );
			if( suface_of_revolution )
			{
				// TODO: implement
#ifdef _DEBUG
				std::cout << "IfcSurfaceOfRevolution not implemented." << std::endl;
#endif
				return;
			}

			throw UnhandledRepresentationException( surface );
		}
		throw UnhandledRepresentationException( surface );
	}

	void convertIfcFaceList( const std::vector<shared_ptr<IfcFace> >& vec_faces, shared_ptr<ItemShapeData> item_data, ShellType st )
	{
		if( vec_faces.size() == 0 )
		{
			return;
		}
		double eps = m_geom_settings->getEpsilonMergePoints();
		PolyInputCache3D poly_cache(eps);
		GeomProcessingParams params( m_geom_settings, nullptr,  this );

		if (item_data)
		{
			// some linux debugging
			std::string tagString = "";
			if(!item_data->m_ifc_representation.expired() )
			{
				shared_ptr<IfcRepresentation> representation( item_data->m_ifc_representation );
				tagString = ", representation tag: " + std::to_string(representation->m_tag);
			}
			if (!item_data->m_product.expired())
			{
				shared_ptr<ProductShapeData> shapeData(item_data->m_product);
				tagString = ", product guid: " + shapeData->m_entity_guid;
			}
			
			printToDebugLog(__FUNC__, "num faces: " + std::to_string(vec_faces.size()) + tagString);
		}
		
		for( size_t ii = 0; ii < vec_faces.size(); ++ii )
		{
			const shared_ptr<IfcFace>& ifc_face = vec_faces[ii];
			if( !ifc_face )
			{
				continue;
			}

			const std::vector<shared_ptr<IfcFaceBound> >& vec_bounds = ifc_face->m_Bounds;
			std::vector<std::vector<vec3> > face_loops;
			params.ifc_entity = ifc_face.get();

			for( auto it_bounds = vec_bounds.begin(); it_bounds != vec_bounds.end(); ++it_bounds )
			{
				const shared_ptr<IfcFaceBound>& face_bound = ( *it_bounds );

				if( !face_bound )
				{
					continue;
				}

				// ENTITY IfcLoop SUPERTYPE OF(ONEOF(IfcEdgeLoop, IfcPolyLoop, IfcVertexLoop))
				const shared_ptr<IfcLoop>& loop = face_bound->m_Bound;
				if( !loop )
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

				face_loops.push_back( std::vector<vec3>() );
				std::vector<vec3>& loop_points = face_loops.back();

				m_curve_converter->convertIfcLoop( loop, loop_points );

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

				bool orientation = true;
				if( face_bound->m_Orientation )
				{
					orientation = face_bound->m_Orientation->m_value;
				}
				if( !orientation )
				{
					std::reverse( loop_points.begin(), loop_points.end() );
				}
			}

			for( size_t iiLoop = 0; iiLoop < face_loops.size(); ++iiLoop )
			{
				std::vector<vec3>& loop = face_loops[iiLoop];
				GeomUtils::unClosePolygon(loop, params.epsMergePoints);
			}
			
			createTriangulated3DFace( face_loops, poly_cache, params, false );

#ifdef _DEBUG
			if( ifc_face->m_tag == 279929)
			{
				params.debugDump = true;
			}

			shared_ptr<IfcAdvancedFace> advancedFace = dynamic_pointer_cast<IfcAdvancedFace>( ifc_face );

			if( params.debugDump )//|| advancedFace )
			{
				vec4 color(0.5, 0.6, 0.7, 1.0);
				for( size_t iiLoop = 0; iiLoop < face_loops.size(); ++iiLoop )
				{
					std::vector<vec3>& loop = face_loops[iiLoop];
					GeomDebugDump::dumpPolyline(loop, color, 0, false, false);
				}

				//if( ii == 34 )
				{
					//PolyInputCache3D poly_cache_dump(eps);
					//createTriangulated3DFace(face_loops, poly_cache_dump, params);
					//std::map<std::string, std::string> mesh_input_options;
					//shared_ptr<carve::mesh::MeshSet<3> > meshset(poly_cache_dump.m_poly_data->createMesh(mesh_input_options, eps));
					//bool drawNormals = false;
					//GeomDebugDump::dumpMeshset(meshset, color, drawNormals, false, false);
				}
				GeomDebugDump::moveOffset(0.0001);
			}
#endif
		}

		// IfcFaceList can be a closed or open shell
		if( st == SHELL_TYPE_UNKONWN )
		{
			item_data->addOpenOrClosedPolyhedron( poly_cache.m_poly_data, params );
		}
		else if( st == OPEN_SHELL )
		{
			item_data->addOpenPolyhedron( poly_cache.m_poly_data, params );
		}
		else if( st == CLOSED_SHELL )
		{
			item_data->addClosedPolyhedron(poly_cache.m_poly_data, params);
		}
	}

	static void triangulateCurvedPolygon(std::vector<vec3>& loopPoints3D, PolyInputCache3D& meshOut, GeomProcessingParams& params, 
		double maxAllowedDistanceFromPlane)
	{
		// find corner with smallest angle
		if (loopPoints3D.size() > params.generalSettings->m_maxNumFaceEdges)
		{
			return;
		}

		size_t numPoints = loopPoints3D.size();
		double eps = params.epsMergePoints;
		double smallestAngle = DBL_MAX;
		size_t smallestAngleIdx = numPoints + 1;
		for (size_t i = 0; i < numPoints; ++i)
		{
			size_t idxB = (i + 1) % numPoints;
			const vec3& pointA = loopPoints3D[i];
			const vec3& pointB = loopPoints3D[idxB];
			const vec3& pointC = loopPoints3D[(i + 2) % numPoints];
			const vec3 BA = (pointA - pointB).normalized();
			const vec3 BC = (pointC - pointB).normalized();
			double dotProduct = dot(BA, BC);
			if (std::isnan(dotProduct))
			{
#if defined(_DEBUG) || defined(_DEBUG_RELEASE)
				std::cout << __FUNC__ << ": dotProduct is nan" << std::endl;
#endif
				continue;
			}
			double openingAngle = std::acos(dotProduct);
			
			if (openingAngle < smallestAngle)
			{
				smallestAngle = openingAngle;
				smallestAngleIdx = idxB;
			}
		}

		if (smallestAngleIdx < numPoints)
		{
			// collect in-plane triangles from smallest angle corner

			std::vector<size_t> currentFlatPolygon;
			int indexBack = smallestAngleIdx - 1;
			int indexForward = smallestAngleIdx + 1;

			if (indexBack < 0)
			{
				indexBack += numPoints;
			}
			if (indexBack >= numPoints)
			{
				indexBack -= numPoints;
			}
			if (indexForward < 0)
			{
				indexForward += numPoints;
			}
			if (indexForward >= numPoints)
			{
				indexForward -= numPoints;
			}

			currentFlatPolygon.push_back(indexBack);
			currentFlatPolygon.push_back(smallestAngleIdx);
			currentFlatPolygon.push_back(indexForward);

			const vec3& pointA = loopPoints3D[currentFlatPolygon[0]];
			const vec3& pointB = loopPoints3D[currentFlatPolygon[1]];
			const vec3& pointC = loopPoints3D[currentFlatPolygon[2]];
			vec3 normal = GeomUtils::computeNormal(pointA, pointB, pointC, eps);
			vec3 centroid = GeomUtils::computePolygonCentroid({ pointA, pointB, pointC });

			if (normal.length2() > 0.5*0.5)
			{
				// go forward in loop until the point is not in the plane any more
				for (size_t i = 0; i < numPoints; ++i)
				{
					int indexA = (smallestAngleIdx + i + 2)%numPoints;
					if (indexA < 0)
					{
						indexA += numPoints;
					}
					const vec3& pointA = loopPoints3D[indexA];

					double distance = GeomUtils::distancePointPlane(pointA, normal, centroid);
					if (std::abs(distance) > maxAllowedDistanceFromPlane)
					{
						break;
					}

					currentFlatPolygon.push_back(indexA);
				}

				// go backward in loop until the point is not in the plane any more
				for (size_t i = 0; i < numPoints; ++i)
				{
					int indexA = smallestAngleIdx - i - 2;
					if (indexA < 0)
					{
						indexA += numPoints;
					}
					if (indexA >= numPoints )
					{
						indexA -= numPoints;
					}
					if (indexA < 0)
					{
						continue;
					}
					if (indexA >= loopPoints3D.size())
					{
						continue;
					}
					const vec3& pointA = loopPoints3D[indexA];

					double distance = GeomUtils::distancePointPlane(pointA, normal, centroid);
					if (std::abs(distance) > maxAllowedDistanceFromPlane)
					{
						break;
					}

					currentFlatPolygon.insert(currentFlatPolygon.begin(), indexA);
				}
			}
			else
			{
				if (params.callbackFunc)
				{
					params.callbackFunc->messageCallback("degenerated triangle", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
				}
			}

			std::vector<vec3> currentFlatPolygonPoints;
			for (size_t i = 0; i < currentFlatPolygon.size(); ++i)
			{
				size_t index = currentFlatPolygon[i];
				currentFlatPolygonPoints.push_back(loopPoints3D[index]);
			}
			std::vector<std::vector<vec3> > vectorFlatPolygon = { currentFlatPolygonPoints };
			createTriangulated3DFace(vectorFlatPolygon, meshOut, params, false);

			currentFlatPolygon.pop_back();
			currentFlatPolygon.erase(currentFlatPolygon.begin());

			std::set<size_t> currentFlatPolygonIndexes;
			for (size_t i = 0; i < currentFlatPolygon.size(); ++i)
			{
				size_t index = currentFlatPolygon[i];
				currentFlatPolygonIndexes.insert(index);
			}

#ifdef _DEBUG
			if (params.debugDump)
			{
				vec4 color(0.5, 0.5, 0.3, 0.5);
				GeomDebugDump::dumpPolyline(loopPoints3D, color, 2.0, false, false);
			}
#endif

			for (auto itIndex = currentFlatPolygonIndexes.rbegin(); itIndex != currentFlatPolygonIndexes.rend(); ++itIndex)
			{
				size_t index = *itIndex;
				auto it = loopPoints3D.begin();
				if (index < loopPoints3D.size())
				{
#ifdef _DEBUG
					if (currentFlatPolygonIndexes.size() > 1 && params.debugDump)
					{
						vec4 color2(0.5, 0.3, 0.9, 0.3);
						//GeomDebugDump::dumpPolyline(loopPoints3D, color2, 3.0, true);
					}
#endif

					std::advance(it, index);
					loopPoints3D.erase(it);

#ifdef _DEBUG
					if (currentFlatPolygonIndexes.size() > 1 && params.debugDump)
					{
						vec4 color2(0.5, 0., 0.3, 0.3);
						//GeomDebugDump::dumpPolyline(loopPoints3D, color2, 3.0, true);
					}
#endif
				}
			}

#ifdef _DEBUG
			if (params.debugDump)
			{
				PolyInputCache3D polyDebug(eps);
				createTriangulated3DFace(vectorFlatPolygon, polyDebug, params, false);
				shared_ptr<carve::mesh::MeshSet<3> > meshset(polyDebug.m_poly_data->createMesh(carve::input::opts(), eps));
				bool drawNormals = false;
				bool drawOpenEdges = false;
				vec4 color1(0.5, 0.5, 0.5, 1.);
				GeomDebugDump::dumpMeshset(meshset, color1, drawNormals, drawOpenEdges, true);
				GeomDebugDump::moveOffset(0.1);
			}
#endif
		}
	}

	///\brief method createTriangulated3DFace: Creates a triangulated face
	///\param[in] inputBounds3D: Curves as face boundaries. The first input curve is the outer boundary, succeeding curves are inner boundaries
	///\param[in] ifc_entity: Ifc entity that the geometry belongs to, just for error messages. Pass a nullptr if no entity at hand.
	///\param[out] meshOut: Result mesh
	static void createTriangulated3DFace(const std::vector<std::vector<vec3> >& inputBounds3D, PolyInputCache3D& meshOut, 
		const GeomProcessingParams& params, bool flipFace)
	{
		double eps = params.epsMergePoints;
		if (inputBounds3D.size() == 1)
		{
			const std::vector<vec3>& outerLoop = inputBounds3D[0];
			if (outerLoop.size() < 3)
			{
				return;
			}

			if (outerLoop.size() == 3)
			{
				const vec3& v0 = outerLoop[0];
				const vec3& v1 = outerLoop[1];
				const vec3& v2 = outerLoop[2];
				if (flipFace) {
					meshOut.addFaceCheckIndexes(v0, v2, v1, eps);
				}
				else {
					meshOut.addFaceCheckIndexes(v0, v1, v2, eps);
				}
				return;
			}

			if (outerLoop.size() == 4)
			{
				const vec3& v0 = outerLoop[0];
				const vec3& v1 = outerLoop[1];
				const vec3& v2 = outerLoop[2];
				const vec3& v3 = outerLoop[3];
				if (flipFace) {
					meshOut.addFaceCheckIndexes(v0, v3, v2, v1, eps);
				}
				else {
					meshOut.addFaceCheckIndexes(v0, v1, v2, v3, eps);
				}
				return;
			}
		}

#ifdef _DEBUG
		PolyInputCache3D polyDebug(eps);
#endif

		std::vector<std::vector<std::array<double, 2> > > polygons2d;
		std::vector<std::vector<vec3> > polygons3d;
		std::vector<double> polygon3DArea;
		bool face_loop_reversed = false;
		bool warning_small_loop_detected = false;
		bool errorOccured = false;
		GeomUtils::ProjectionPlane face_plane = GeomUtils::ProjectionPlane::UNDEFINED;
		vec3 normal = carve::geom::VECTOR(0, 0, 1);
		vec3 normalOuterBound = carve::geom::VECTOR(0, 0, 1);

		for (auto it_bounds = inputBounds3D.begin(); it_bounds != inputBounds3D.end(); ++it_bounds)
		{
			std::vector<vec3> currentPointLoop = *it_bounds;

			if (currentPointLoop.size() < 3)
			{
				if (it_bounds == inputBounds3D.begin())
				{
					break;
				}
				else
				{
					continue;
				}
			}

			if (params.mergeAlignedEdges)
			{
				//GeomUtils::simplifyPolygon(loopPoints3Dinput, params.epsMergePoints, params.epsMergeAlignedEdgesAngle);
			}
			GeomUtils::unClosePolygon(currentPointLoop, eps);
			normal = GeomUtils::computePolygonNormal(currentPointLoop, eps);
			vec3 centroid = GeomUtils::computePolygonCentroid(currentPointLoop);

			carve::geom::aabb<3> bbox;
			GeomUtils::polygonBbox(currentPointLoop, bbox);

			if (it_bounds == inputBounds3D.begin())
			{
				normalOuterBound = normal;

				// figure out on which plane to project the 3D points
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
					std::stringstream err;
					err << "unable to project to plane: nx" << nx << " ny " << ny << " nz " << nz << std::endl;
					if (params.callbackFunc)
					{
						params.callbackFunc->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, params.ifc_entity);
					}
					continue;
				}
			}

			// project face into 2d plane
			std::vector<std::array<double, 2> > path_loop_2d;
			std::vector<vec3> path_loop_3d;
			double curvature = 0;
			double maxExtent = std::max(bbox.extent.x, std::max(bbox.extent.y, bbox.extent.z));
			double maxAllowedDistanceFromPlane = params.epsMergePoints * 500 * maxExtent;
			const size_t numPoints = currentPointLoop.size();
			for (size_t i = 0; i < numPoints; ++i)
			{
				const vec3& point = currentPointLoop[i];
				path_loop_3d.push_back(point);
				if (face_plane == GeomUtils::XY_PLANE)
				{
					path_loop_2d.push_back({ point.x, point.y });
				}
				else if (face_plane == GeomUtils::YZ_PLANE)
				{
					path_loop_2d.push_back({ point.y, point.z });
				}
				else if (face_plane == GeomUtils::XZ_PLANE)
				{
					path_loop_2d.push_back({ point.x, point.z });
				}

				double distance = GeomUtils::distancePointPlane(point, normal, centroid);
				if (std::abs(maxExtent) > eps)
				{
					curvature += std::abs(distance) / maxExtent * 0.001;
				}
			}

			if (curvature > eps * 1000)
			{
				GeomProcessingParams paramsDebug(params);
#ifdef _DEBUG
				GeomDebugDump::moveOffset(0.6);
				vec4 color2(0.5, 0.3, 0.9, 0.3);
				for (auto it_bounds = inputBounds3D.begin(); it_bounds != inputBounds3D.end(); ++it_bounds)
				{
					std::vector<vec3> currentPointLoop = *it_bounds;
					GeomDebugDump::dumpPolyline(currentPointLoop, color2, 3.0, false, false);
					color2.g += 0.4f;
					color2.b -= 0.4f;
				}
				GeomDebugDump::moveOffset(0.3);
				paramsDebug.debugDump = true;
#endif
				std::vector<vec3> loopPoints3DinputCopy;
				std::copy(currentPointLoop.begin(), currentPointLoop.end(), std::back_inserter(loopPoints3DinputCopy));
				size_t numPoints = currentPointLoop.size();
				for (size_t jj = 0; jj < numPoints; ++jj)
				{
					triangulateCurvedPolygon(loopPoints3DinputCopy, meshOut, paramsDebug, maxAllowedDistanceFromPlane);
					if (loopPoints3DinputCopy.size() < 3)
					{
						break;
					}
				}

#ifdef _DEBUG
				{
					GeomProcessingParams paramsDebug(params);
					paramsDebug.debugDump = false;
					PolyInputCache3D polyDebug(eps);
					std::vector<vec3> loopPoints3DinputCopy;
					std::copy(currentPointLoop.begin(), currentPointLoop.end(), std::back_inserter(loopPoints3DinputCopy));
					size_t numPoints = currentPointLoop.size();
					for (size_t jj = 0; jj < numPoints; ++jj)
					{
						triangulateCurvedPolygon(loopPoints3DinputCopy, polyDebug, paramsDebug, maxAllowedDistanceFromPlane);
						if (loopPoints3DinputCopy.size() < 3)
						{
							break;
						}
					}

					GeomDebugDump::moveOffset(0.3);
					vec4 color(0.5, 0.8, 0.5, 0.3);
					shared_ptr<carve::mesh::MeshSet<3> > meshset(polyDebug.m_poly_data->createMesh(carve::input::opts(), eps));
					GeomDebugDump::dumpMeshset(meshset, color, false, false, true);
					GeomDebugDump::moveOffset(0.3);
				}
#endif
				return;
			}

			if (path_loop_2d.size() < 3)
			{
				//std::cout << __FUNC__ << ": #" << face_id <<  "=IfcFace: path_loop.size() < 3" << std::endl;
				continue;
			}

			double loop_area = std::abs(GeomUtils::signedArea(path_loop_2d));
			double min_loop_area = params.minFaceArea;
			if (loop_area < min_loop_area)
			{
				warning_small_loop_detected = true;
				continue;
			}

			if (currentPointLoop.size() == 4 && inputBounds3D.size() == 1)
			{
				if (GeomUtils::isQuadConvex(path_loop_2d[0], path_loop_2d[1], path_loop_2d[2], path_loop_2d[3]))
				{
					// add 2 triangles for quad
					vec3 v0 = currentPointLoop[0];
					vec3 v1 = currentPointLoop[1];
					vec3 v2 = currentPointLoop[2];
					vec3 v3 = currentPointLoop[3];
					uint32_t idx0 = meshOut.addPoint(v0);
					uint32_t idx1 = meshOut.addPoint(v1);
					uint32_t idx2 = meshOut.addPoint(v2);
					uint32_t idx3 = meshOut.addPoint(v3);

#ifdef _DEBUG
					uint32_t idx0_dbg = polyDebug.addPoint(v0);
					uint32_t idx1_dbg = polyDebug.addPoint(v1);
					uint32_t idx2_dbg = polyDebug.addPoint(v2);
					uint32_t idx3_dbg = polyDebug.addPoint(v3);
#endif

					vec3 normalTriangle0 = GeomUtils::computePolygonNormal({ v0, v1, v2 }, eps);
					vec3 normalTriangle1 = GeomUtils::computePolygonNormal({ v2, v3, v0 }, eps);
					if (flipFace) {
						std::swap(idx1, idx2);
					}
					if (dot(normalTriangle0, normalOuterBound) > 0)
					{
						// normalTriangle0 and normalOuterBound should point in the same direction" << std::endl;
						meshOut.m_poly_data->addFace(idx0, idx1, idx2);
#ifdef _DEBUG
						polyDebug.m_poly_data->addFace(idx0_dbg, idx1_dbg, idx2_dbg);
#endif
					}
					else
					{
						// normalTriangle0 and normalOuterBound should point in the same direction" << std::endl;
						meshOut.m_poly_data->addFace(idx0, idx2, idx1);
#ifdef _DEBUG
						polyDebug.m_poly_data->addFace(idx0_dbg, idx2_dbg, idx1_dbg);
#endif
					}

					if (dot(normalTriangle1, normalOuterBound) > 0)
					{
						//std::cout << "normalTriangle1 and normalOuterBound should point in the same direction" << std::endl;
						meshOut.m_poly_data->addFace(idx2, idx3, idx0);
#ifdef _DEBUG
						polyDebug.m_poly_data->addFace(idx2_dbg, idx3_dbg, idx0_dbg);
#endif
					}
					else
					{
						meshOut.m_poly_data->addFace(idx2, idx0, idx3);
#ifdef _DEBUG
						polyDebug.m_poly_data->addFace(idx2_dbg, idx0_dbg, idx3_dbg);
#endif
					}

					return;
				}
			}

			// outer loop (biggest area) needs to come first
			bool insertPositionFound = false;
			for (size_t iiArea = 0; iiArea < polygon3DArea.size(); ++iiArea)
			{
				double existingLoopArea = polygon3DArea[iiArea];

				// existingArea[i]  < loop_area < existingArea[i+1]
				if (loop_area > existingLoopArea)
				{
					polygons2d.insert(polygons2d.begin() + iiArea, path_loop_2d);
					polygons3d.insert(polygons3d.begin() + iiArea, path_loop_3d);
					polygon3DArea.insert(polygon3DArea.begin() + iiArea, loop_area);
					insertPositionFound = true;
					break;
				}
			}

			if (!insertPositionFound)
			{
				polygons2d.push_back(path_loop_2d);
				polygons3d.push_back(path_loop_3d);
				polygon3DArea.push_back(loop_area);
			}
		}

#ifdef _DEBUG
		// check descending order
		if (polygon3DArea.size() > 0)
		{
			double previousLoopArea = polygon3DArea[0];
			if (polygon3DArea.size() > 1)
			{
				for (size_t iiArea = 1; iiArea < polygon3DArea.size(); ++iiArea)
				{
					double loopArea = polygon3DArea[iiArea];
					if (loopArea > previousLoopArea)
					{
						std::cout << "polygon3DArea not descending" << std::endl;
					}
				}
			}
		}
#endif
		if (polygons3d.size() > 0)
		{
			std::vector<vec3>& loopOuterBound = polygons3d[0];
			normalOuterBound = GeomUtils::computePolygonNormal(loopOuterBound, eps);
		}

		// check winding order in 2D
		std::vector<std::array<double, 2> > polygons2dFlatVector;
		for (size_t ii = 0; ii < polygons2d.size(); ++ii)
		{
			std::vector<std::array<double, 2> >& loop2D = polygons2d[ii];
			std::vector<vec3>& loop3D = polygons3d[ii];

			glm::dvec3 normal_2d = GeomUtils::computePolygon2DNormal(loop2D, eps);
			if (ii == 0)
			{
				if (normal_2d.z < 0)
				{
					std::reverse(loop2D.begin(), loop2D.end());
					std::reverse(loop3D.begin(), loop3D.end());
					face_loop_reversed = true;
				}
			}
			else
			{
				if (normal_2d.z > 0)
				{
					std::reverse(loop2D.begin(), loop2D.end());
					std::reverse(loop3D.begin(), loop3D.end());
				}
			}
		}

		if (warning_small_loop_detected)
		{
			std::stringstream err;
			err << "std::abs( signed_area ) < 1.e-10";
			if (params.callbackFunc)
			{
				params.callbackFunc->messageCallback(err.str().c_str(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, __FUNC__, params.ifc_entity);
			}
		}

		if (polygons2d.size() > 0)
		{
			std::vector<uint32_t> triangulated = mapbox::earcut<uint32_t>(polygons2d);

			std::vector<vec3> polygons3dFlatVector;
			GeomUtils::polygons2flatVec(polygons3d, polygons3dFlatVector);

			for (int ii = 0; ii < triangulated.size(); ii += 3)
			{
				size_t idxA = triangulated[ii + 0];
				size_t idxB = triangulated[ii + 1];
				size_t idxC = triangulated[ii + 2];

				const vec3& pointA = polygons3dFlatVector[idxA];
				const vec3& pointB = polygons3dFlatVector[idxB];
				const vec3& pointC = polygons3dFlatVector[idxC];

				idxA = meshOut.addPoint(pointA);
				idxB = meshOut.addPoint(pointB);
				idxC = meshOut.addPoint(pointC);

#ifdef _DEBUG
				size_t idxA_dbg = polyDebug.addPoint(pointA);
				size_t idxB_dbg = polyDebug.addPoint(pointB);
				size_t idxC_dbg = polyDebug.addPoint(pointC);
#endif
				if (flipFace) {
					std::swap(idxB, idxC);
				}
				vec3 triangleNormal = GeomUtils::computePolygonNormal({ pointA, pointB, pointC }, eps);
				if (dot(triangleNormal, normalOuterBound) >= 0)
				{

					meshOut.m_poly_data->addFace(idxA, idxB, idxC);
#ifdef _DEBUG
					polyDebug.m_poly_data->addFace(idxA_dbg, idxB_dbg, idxC_dbg);
#endif
				}
				else
				{
					meshOut.m_poly_data->addFace(idxA, idxC, idxB);
#ifdef _DEBUG
					polyDebug.m_poly_data->addFace(idxA_dbg, idxC_dbg, idxB_dbg);
#endif
				}
			}
#ifdef _DEBUG
			if (errorOccured /*|| ifc_entity->m_tag == 64*/)
			{
				vec4 color(0, 1, 1, 1);
				GeomDebugDump::dumpPolyline(polygons2d, color, true, false);
				//shared_ptr<carve::mesh::MeshSet<3> > meshset(meshOut.m_poly_data->createMesh(carve::input::opts()));
				//GeomDebugDump::dumpMeshset(meshset, color, true, true);
			}
#endif
		}

#ifdef _DEBUG
		if (errorOccured && params.debugDump)
		{
			vec4 color(0, 1, 1, 1);
			shared_ptr<carve::mesh::MeshSet<3> > meshset(polyDebug.m_poly_data->createMesh(carve::input::opts(), eps));
			bool drawNormals = true;
			GeomDebugDump::dumpMeshset(meshset, color, drawNormals, true);
		}
#endif
	}
};
