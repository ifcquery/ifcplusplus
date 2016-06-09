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
#include <ifcpp/model/UnitConverter.h>

#include <ifcpp/IFC4/include/IfcCurveBoundedPlane.h>
#include <ifcpp/IFC4/include/IfcCurveBoundedSurface.h>
#include <ifcpp/IFC4/include/IfcCylindricalSurface.h>
#include <ifcpp/IFC4/include/IfcFace.h>
#include <ifcpp/IFC4/include/IfcFaceBound.h>
#include <ifcpp/IFC4/include/IfcPlane.h>
#include <ifcpp/IFC4/include/IfcRationalBSplineSurfaceWithKnots.h>
#include <ifcpp/IFC4/include/IfcRectangularTrimmedSurface.h>
#include <ifcpp/IFC4/include/IfcSurfaceOfLinearExtrusion.h>
#include <ifcpp/IFC4/include/IfcSurfaceOfRevolution.h>
#include <ifcpp/IFC4/include/IfcSweptSurface.h>

#include "IncludeCarveHeaders.h"
#include "GeometryInputData.h"
#include "GeometrySettings.h"
#include "CurveConverter.h"
#include "SplineConverter.h"
#include "Sweeper.h"

class SurfaceProxy
{
public:
	virtual void computePointOnSurface(const carve::geom::vector<3>& point_in, carve::geom::vector<3>& point_out) = 0;
};

class SurfaceProxyLinear : public SurfaceProxy
{
public:
	virtual void computePointOnSurface(const carve::geom::vector<3>& point_in, carve::geom::vector<3>& point_out)
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

	FaceConverter( shared_ptr<GeometrySettings>& gs, shared_ptr<UnitConverter>& uc, shared_ptr<CurveConverter>& cc, shared_ptr<SplineConverter>& sc, shared_ptr<Sweeper>& sw )
		: m_geom_settings( gs ), m_unit_converter( uc ), m_curve_converter( cc ), m_spline_converter( sc ), m_sweeper( sw )
	{
	}

	virtual ~FaceConverter(){}

	void convertIfcSurface( const shared_ptr<IfcSurface>& surface, shared_ptr<ItemShapeInputData>& item_data, shared_ptr<SurfaceProxy>& surface_proxy )
	{
		//ENTITY IfcSurface ABSTRACT SUPERTYPE OF(ONEOF(IfcBoundedSurface, IfcElementarySurface, IfcSweptSurface))

		double length_factor = m_unit_converter->getLengthInMeterFactor();
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
				carve::math::Matrix curve_bounded_plane_matrix;
				shared_ptr<IfcPlane>& basis_surface = curve_bounded_plane->m_BasisSurface;
				if( basis_surface )
				{
					shared_ptr<IfcAxis2Placement3D>& basis_surface_placement = basis_surface->m_Position;

					if( basis_surface_placement )
					{
						PlacementConverter::convertIfcAxis2Placement3D( basis_surface_placement, length_factor, curve_bounded_plane_matrix );
					}
				}

				// convert outer boundary
				shared_ptr<IfcCurve>& outer_boundary = curve_bounded_plane->m_OuterBoundary;
				std::vector<std::vector<carve::geom::vector<3> > > face_loops;
				face_loops.push_back( std::vector<carve::geom::vector<3> >() );
				std::vector<carve::geom::vector<3> >& outer_boundary_loop = face_loops.back();
				std::vector<carve::geom::vector<3> > segment_start_points;
				m_curve_converter->convertIfcCurve( outer_boundary, outer_boundary_loop, segment_start_points );

				// convert inner boundaries
				std::vector<shared_ptr<IfcCurve> >& vec_inner_boundaries = curve_bounded_plane->m_InnerBoundaries;			//optional
				for( auto& inner_boundary : vec_inner_boundaries )
				{
					if( !inner_boundary )
					{
						continue;
					}
					face_loops.push_back( std::vector<carve::geom::vector<3> >() );
					std::vector<carve::geom::vector<3> >& inner_boundary_loop = face_loops.back();
					std::vector<carve::geom::vector<3> > segment_start_points;
					m_curve_converter->convertIfcCurve( inner_boundary, inner_boundary_loop, segment_start_points );
				}

				PolyInputCache3D poly_cache;
				m_sweeper->createTriangulated3DFace( face_loops, outer_boundary.get(), poly_cache );
				item_data->addOpenPolyhedron( poly_cache.m_poly_data );
				item_data->applyPosition( curve_bounded_plane_matrix );
			}
			else if( dynamic_pointer_cast<IfcCurveBoundedSurface>( bounded_surface ) )
			{
				shared_ptr<IfcCurveBoundedSurface> curve_bounded_surface = dynamic_pointer_cast<IfcCurveBoundedSurface>( bounded_surface );
				shared_ptr<IfcSurface>& basis_surface = curve_bounded_surface->m_BasisSurface;
				if( basis_surface )
				{
					convertIfcSurface( basis_surface, item_data, surface_proxy );
				}


				//std::vector<shared_ptr<IfcBoundaryCurve> >& vec_boundaries = curve_bounded_surface->m_Boundaries;
				//bool implicit_outer = curve_bounded_surface->m_ImplicitOuter;

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

			carve::math::Matrix elementary_surface_matrix;
			if( elementary_surface_placement )
			{
				PlacementConverter::convertIfcAxis2Placement3D( elementary_surface_placement, length_factor, elementary_surface_matrix );
				//elementary_surface_matrix = pos*elementary_surface_matrix;
			}

			shared_ptr<SurfaceProxyLinear> proxy_linear( new SurfaceProxyLinear() );
			proxy_linear->m_surface_matrix = elementary_surface_matrix;
			surface_proxy = proxy_linear;

			shared_ptr<IfcPlane> elementary_surface_plane = dynamic_pointer_cast<IfcPlane>( elementary_surface );
			if( elementary_surface_plane )
			{
				//  1----0     create big rectangular plane
				//  |    |     ^ y
				//  |    |     |
				//  2----3     ---> x
				{
					double plane_span = HALF_SPACE_BOX_SIZE;
					shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
					polyline_data->beginPolyline();
					polyline_data->addVertex( elementary_surface_matrix*carve::geom::VECTOR( plane_span, plane_span, 0.0 ) );
					polyline_data->addVertex( elementary_surface_matrix*carve::geom::VECTOR( -plane_span, plane_span, 0.0 ) );
					polyline_data->addVertex( elementary_surface_matrix*carve::geom::VECTOR( -plane_span, -plane_span, 0.0 ) );
					polyline_data->addVertex( elementary_surface_matrix*carve::geom::VECTOR( plane_span, -plane_span, 0.0 ) );

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

				int num_segments = m_geom_settings->getNumVerticesPerCircle();	// TODO: adapt to model size and complexity
				double start_angle = 0.0;
				double opening_angle = M_PI*2.0;
				const double circle_center_x = 0.0;
				const double circle_center_y = 0.0;

				std::vector<carve::geom::vector<2> > circle_points;
				GeomUtils::addArcWithEndPoint( circle_points, circle_radius, start_angle, opening_angle, circle_center_x, circle_center_y, num_segments );

				// apply position and insert points
				shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
				polyline_data->beginPolyline();
				for( size_t i = 0; i < circle_points.size(); ++i )
				{
					carve::geom::vector<2>& point = circle_points[i];
					carve::geom::vector<3> point3d( carve::geom::VECTOR( point.x, point.y, 0 ) );
					polyline_data->addVertex( elementary_surface_matrix*point3d );
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
			//shared_ptr<IfcProfileDef>& swept_surface_profile = swept_surface->m_SweptCurve;
			shared_ptr<IfcAxis2Placement3D>& swept_surface_placement = swept_surface->m_Position;

			carve::math::Matrix swept_surface_matrix;
			if( swept_surface_placement )
			{
				PlacementConverter::convertIfcAxis2Placement3D( swept_surface_placement, length_factor, swept_surface_matrix );
				//swept_surface_matrix = pos*swept_surface_matrix;
			}

			shared_ptr<IfcSurfaceOfLinearExtrusion> linear_extrusion = dynamic_pointer_cast<IfcSurfaceOfLinearExtrusion>( swept_surface );
			if( linear_extrusion )
			{
				//shared_ptr<IfcDirection>& linear_extrusion_direction = linear_extrusion->m_ExtrudedDirection;
				//shared_ptr<IfcLengthMeasure>& linear_extrusion_depth = linear_extrusion->m_Depth;
				// TODO: implement
#ifdef _DEBUG
				std::cout << "IfcSurfaceOfLinearExtrusion not implemented." << std::endl;
#endif
				return;
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

	void convertIfcFaceList( const std::vector<shared_ptr<IfcFace> >& vec_faces, shared_ptr<ItemShapeInputData> item_data, ShellType st )
	{
		PolyInputCache3D poly_cache;
		IfcPPEntity* report_entity = nullptr;
		for( const shared_ptr<IfcFace>& ifc_face : vec_faces )
		{
			if( !ifc_face )
			{
				continue;
			}
			const std::vector<shared_ptr<IfcFaceBound> >& vec_bounds = ifc_face->m_Bounds;
			std::vector<std::vector<carve::geom::vector<3> > > face_loops;
			report_entity = ifc_face.get();

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

				face_loops.push_back( std::vector<carve::geom::vector<3> >() );
				std::vector<carve::geom::vector<3> >& loop_points = face_loops.back();
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
			m_sweeper->createTriangulated3DFace( face_loops, report_entity, poly_cache );
		}

		// IfcFaceList can be a closed or open shell
		if( st == SHELL_TYPE_UNKONWN )
		{
			item_data->addOpenOrClosedPolyhedron( poly_cache.m_poly_data );
		}
		else if( st == OPEN_SHELL )
		{
			item_data->addOpenPolyhedron( poly_cache.m_poly_data );
		}
		else if( st == CLOSED_SHELL )
		{
			try
			{
				item_data->addClosedPolyhedron( poly_cache.m_poly_data );
			}
			catch( IfcPPException& e )
			{
				// not a fatal error, just mesh is not closed
				messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_MINOR_WARNING, "", report_entity );  // calling function already in e.what()
			}
		}
	}
};
