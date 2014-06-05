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

#include "ifcpp/IFC4/include/IfcParameterValue.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcPositiveLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcCartesianPoint.h"
#include "ifcpp/IFC4/include/IfcDirection.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement3D.h"
#include "ifcpp/IFC4/include/IfcPlane.h"
#include "ifcpp/IFC4/include/IfcCurveBoundedPlane.h"

#include "ifcpp/IFC4/include/IfcRationalBSplineSurfaceWithKnots.h"
#include "ifcpp/IFC4/include/IfcCurveBoundedSurface.h"
#include "ifcpp/IFC4/include/IfcRectangularTrimmedSurface.h"
#include "ifcpp/IFC4/include/IfcCylindricalSurface.h"
#include "ifcpp/IFC4/include/IfcSweptSurface.h"
#include "ifcpp/IFC4/include/IfcSurfaceOfLinearExtrusion.h"
#include "ifcpp/IFC4/include/IfcSurfaceOfRevolution.h"
#include "ifcpp/IFC4/include/IfcFaceBound.h"
#include "ifcpp/IFC4/include/IfcFace.h"
#include "ifcpp/IFC4/include/IfcPolyLoop.h"
#include "ifcpp/IFC4/include/IfcEdgeLoop.h"
#include "ifcpp/IFC4/include/IfcEdge.h"
#include "ifcpp/IFC4/include/IfcOrientedEdge.h"
#include "ifcpp/IFC4/include/IfcVertexPoint.h"
#include "ifcpp/IFC4/include/IfcVertexLoop.h"

#include "ifcpp/model/UnitConverter.h"
#include "ifcpp/model/IfcPPException.h"
#include "GeometrySettings.h"
#include "GeomUtils.h"
#include "GeometryException.h"
#include "ProfileConverter.h"
#include "PlacementConverter.h"
#include "CurveConverter.h"
#include "FaceConverter.h"

FaceConverter::FaceConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> uc, shared_ptr<CurveConverter>	cc )
	: m_geom_settings(geom_settings), m_unit_converter( uc ), m_curve_converter( cc )
{
}

FaceConverter::~FaceConverter()
{
}

void FaceConverter::convertIfcSurface( const shared_ptr<IfcSurface>& surface, shared_ptr<carve::input::PolylineSetData>& polyline_data )
{
	//ENTITY IfcSurface ABSTRACT SUPERTYPE OF(ONEOF(IfcBoundedSurface, IfcElementarySurface, IfcSweptSurface))

	double length_factor = m_unit_converter->getLengthInMeterFactor();
	shared_ptr<IfcBoundedSurface> bounded_surface = dynamic_pointer_cast<IfcBoundedSurface>(surface);
	if( bounded_surface )
	{
		// ENTITY IfcBoundedSurface ABSTRACT SUPERTYPE OF(ONEOF(IfcBSplineSurface, IfcCurveBoundedPlane, IfcCurveBoundedSurface, IfcRectangularTrimmedSurface))
		if( dynamic_pointer_cast<IfcBSplineSurface>(bounded_surface) )
		{
			if( dynamic_pointer_cast<IfcRationalBSplineSurfaceWithKnots>(bounded_surface) )
			{
				shared_ptr<IfcRationalBSplineSurfaceWithKnots> nurbs_surface = dynamic_pointer_cast<IfcRationalBSplineSurfaceWithKnots>(bounded_surface);
				convertIfcBSplineSurface( nurbs_surface, polyline_data );
			}
		}
		else if( dynamic_pointer_cast<IfcCurveBoundedPlane>(bounded_surface) )
		{
			// ENTITY IfcCurveBoundedPlane SUBTYPE OF IfcBoundedSurface;
			shared_ptr<IfcCurveBoundedPlane> curve_bounded_plane = dynamic_pointer_cast<IfcCurveBoundedPlane>(bounded_surface);
			carve::math::Matrix curve_bounded_plane_matrix;
			shared_ptr<IfcPlane>& basis_surface = curve_bounded_plane->m_BasisSurface;
			if( basis_surface )
			{
				shared_ptr<IfcAxis2Placement3D>& basis_surface_placement = basis_surface->m_Position;

				if( basis_surface_placement )
				{
					PlacementConverter::convertIfcAxis2Placement3D( basis_surface_placement, curve_bounded_plane_matrix, length_factor );
					//curve_bounded_plane_matrix = pos*curve_bounded_plane_matrix;
				}
			}
			shared_ptr<IfcCurve>& outer_boundary = curve_bounded_plane->m_OuterBoundary;
			if( outer_boundary )
			{
				//convertIfcCurve( outer_boundary, target,  );
				// TODO: implement boundary
			}
			std::vector<shared_ptr<IfcCurve> >& vec_inner_boundaries = curve_bounded_plane->m_InnerBoundaries;
			for( unsigned int i=0; i<vec_inner_boundaries.size(); ++i )
			{
				shared_ptr<IfcCurve>& inner_curve = vec_inner_boundaries[i];
				//convertIfcCurve( outer_boundary)
				// TODO: implement boundary
			}
			std::cout << "IfcCurveBoundedPlane not implemented." << std::endl;
		}
		else if( dynamic_pointer_cast<IfcCurveBoundedSurface>(bounded_surface) )
		{
			shared_ptr<IfcCurveBoundedSurface> curve_bounded_surface = dynamic_pointer_cast<IfcCurveBoundedSurface>(bounded_surface);
			shared_ptr<IfcSurface>& basis_surface = curve_bounded_surface->m_BasisSurface;
			if( basis_surface )
			{
				convertIfcSurface( basis_surface, polyline_data );
			}

			std::vector<shared_ptr<IfcBoundaryCurve> >& vec_boundaries = curve_bounded_surface->m_Boundaries;
			bool implicit_outer = curve_bounded_surface->m_ImplicitOuter;
			// TODO: implement
			std::cout << "IfcCurveBoundedSurface not implemented." << std::endl;
		}
		else if( dynamic_pointer_cast<IfcRectangularTrimmedSurface>(bounded_surface) )
		{
			shared_ptr<IfcRectangularTrimmedSurface> rectengular_trimmed_surface = dynamic_pointer_cast<IfcRectangularTrimmedSurface>(bounded_surface);

			shared_ptr<IfcSurface>& basis_surface = rectengular_trimmed_surface->m_BasisSurface;
			if( basis_surface )
			{
				convertIfcSurface( basis_surface, polyline_data );
			}

			shared_ptr<IfcParameterValue>& u1 = rectengular_trimmed_surface->m_U1;
			shared_ptr<IfcParameterValue>& v1 = rectengular_trimmed_surface->m_V1;
			shared_ptr<IfcParameterValue>& u2 = rectengular_trimmed_surface->m_U2;
			shared_ptr<IfcParameterValue>& v2 = rectengular_trimmed_surface->m_V2;
			bool u_sense = rectengular_trimmed_surface->m_Usense;
			bool v_sense = rectengular_trimmed_surface->m_Vsense;
			// TODO: implement
			std::cout << "IfcRectangularTrimmedSurface not implemented." << std::endl;
		}
		return;
	}

	shared_ptr<IfcElementarySurface> elementary_surface = dynamic_pointer_cast<IfcElementarySurface>(surface);
	if( elementary_surface )
	{
		//ENTITY IfcElementarySurface	ABSTRACT SUPERTYPE OF(ONEOF(IfcCylindricalSurface, IfcPlane))
		shared_ptr<IfcAxis2Placement3D>& elementary_surface_placement = elementary_surface->m_Position;

		carve::math::Matrix elementary_surface_matrix;
		if( elementary_surface_placement )
		{
			PlacementConverter::convertIfcAxis2Placement3D( elementary_surface_placement, elementary_surface_matrix, length_factor );
			//elementary_surface_matrix = pos*elementary_surface_matrix;
		}

		shared_ptr<IfcPlane> elementary_surface_plane = dynamic_pointer_cast<IfcPlane>(elementary_surface);
		if( elementary_surface_plane )
		{
			//  1----0     create big rectangular plane
			//  |    |     ^ y
			//  |    |     |
			//  2----3     ---> x
			{
				double plane_span = HALF_SPACE_BOX_SIZE;
				polyline_data->beginPolyline();
				polyline_data->addVertex( elementary_surface_matrix*carve::geom::VECTOR( plane_span,  plane_span, 0.0 ));
				polyline_data->addVertex( elementary_surface_matrix*carve::geom::VECTOR(-plane_span,  plane_span, 0.0 ));
				polyline_data->addVertex( elementary_surface_matrix*carve::geom::VECTOR(-plane_span, -plane_span, 0.0 ));
				polyline_data->addVertex( elementary_surface_matrix*carve::geom::VECTOR( plane_span, -plane_span, 0.0 ));

				polyline_data->addPolylineIndex(0);
				polyline_data->addPolylineIndex(1);
				polyline_data->addPolylineIndex(2);
				polyline_data->addPolylineIndex(3);
			}
			return;
		}

		shared_ptr<IfcCylindricalSurface> cylindrical_surface = dynamic_pointer_cast<IfcCylindricalSurface>(elementary_surface);
		if( cylindrical_surface )
		{
			shared_ptr<IfcPositiveLengthMeasure> cylindrical_surface_radius = cylindrical_surface->m_Radius;
			double circle_radius = cylindrical_surface_radius->m_value;

			int num_segments = m_geom_settings->m_num_vertices_per_circle;	// TODO: adapt to model size and complexity
			double start_angle = 0.0;
			double opening_angle = M_PI*2.0;
			const double circle_center_x = 0.0;
			const double circle_center_y = 0.0;

			std::vector<carve::geom::vector<2> > circle_points;
			ProfileConverter::addArcWithEndPoint( circle_points, circle_radius, start_angle, opening_angle, circle_center_x, circle_center_y, num_segments );

			// apply position and insert points
			polyline_data->beginPolyline();
			for( int i=0; i<circle_points.size(); ++i )
			{
				carve::geom::vector<2>& point = circle_points[i];
				carve::geom::vector<3> point3d( carve::geom::VECTOR( point.x, point.y, 0 ) );
				polyline_data->addVertex( elementary_surface_matrix*point3d );
				polyline_data->addPolylineIndex(i);
			}
			return;
		}

		throw UnhandledRepresentationException(surface);
	}

	shared_ptr<IfcSweptSurface> swept_surface = dynamic_pointer_cast<IfcSweptSurface>(surface);
	if( dynamic_pointer_cast<IfcSweptSurface>(surface) )
	{
		// ENTITY IfcSweptSurface	ABSTRACT SUPERTYPE OF(ONEOF(IfcSurfaceOfLinearExtrusion, IfcSurfaceOfRevolution))
		shared_ptr<IfcProfileDef>& swept_surface_profile = swept_surface->m_SweptCurve;
		shared_ptr<IfcAxis2Placement3D>& swept_surface_placement = swept_surface->m_Position;

		carve::math::Matrix swept_surface_matrix;
		if( swept_surface_placement )
		{
			PlacementConverter::convertIfcAxis2Placement3D( swept_surface_placement, swept_surface_matrix, length_factor );
			//swept_surface_matrix = pos*swept_surface_matrix;
		}

		shared_ptr<IfcSurfaceOfLinearExtrusion> linear_extrusion = dynamic_pointer_cast<IfcSurfaceOfLinearExtrusion>(swept_surface);
		if( linear_extrusion )
		{
			shared_ptr<IfcDirection>& linear_extrusion_direction = linear_extrusion->m_ExtrudedDirection;
			shared_ptr<IfcLengthMeasure>& linear_extrusion_depth = linear_extrusion->m_Depth;
			// TODO: implement
			std::cout << "IfcSurfaceOfLinearExtrusion not implemented." << std::endl;
			return;
		}

		shared_ptr<IfcSurfaceOfRevolution> suface_of_revolution = dynamic_pointer_cast<IfcSurfaceOfRevolution>(swept_surface);
		if( suface_of_revolution )
		{
			// TODO: implement
			std::cout << "IfcSurfaceOfRevolution not implemented." << std::endl;
			return;
		}

		throw UnhandledRepresentationException(surface);
	}
	throw UnhandledRepresentationException(surface);
}

void FaceConverter::convertIfcFaceList( const std::vector<shared_ptr<IfcFace> >& faces, shared_ptr<ItemData> item_data, std::stringstream& strs_err )
{
	PolyInputCache3D poly_cache;
	for( std::vector<shared_ptr<IfcFace> >::const_iterator it_ifc_faces=faces.begin(); it_ifc_faces!=faces.end(); ++it_ifc_faces )
	{
		const shared_ptr<IfcFace>& face = *it_ifc_faces;
		std::vector<shared_ptr<IfcFaceBound> >& vec_bounds = face->m_Bounds;
		std::vector<std::vector<carve::geom::vector<3> > > face_loops;

		for( std::vector<shared_ptr<IfcFaceBound> >::iterator it_bounds=vec_bounds.begin(); it_bounds!=vec_bounds.end(); ++it_bounds )
		{
			shared_ptr<IfcFaceBound> face_bound = (*it_bounds);

			// ENTITY IfcLoop SUPERTYPE OF(ONEOF(IfcEdgeLoop, IfcPolyLoop, IfcVertexLoop))
			shared_ptr<IfcLoop> loop = face_bound->m_Bound;
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

			bool orientation = face_bound->m_Orientation;
			if( !orientation )
			{
				std::reverse( loop_points.begin(), loop_points.end() );
			}

		}
		GeomUtils::createFace( face_loops, poly_cache, strs_err );
	}
	// IfcFaceList can be a closed or open shell, so let the calling function decide where to put it
	item_data->open_or_closed_polyhedrons.push_back( poly_cache.m_poly_data );
}

void convertIfcCartesianPointVector2D( std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& points, double length_factor, osg::Vec3Array* vertices )
{
	std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >::iterator it_cp_outer;
	for( it_cp_outer=points.begin(); it_cp_outer!=points.end(); ++it_cp_outer )
	{
		std::vector<shared_ptr<IfcCartesianPoint> >& points_inner = (*it_cp_outer);
		std::vector<shared_ptr<IfcCartesianPoint> >::iterator it_cp;
		for( it_cp=points_inner.begin(); it_cp!=points_inner.end(); ++it_cp )
		{
			shared_ptr<IfcCartesianPoint> cp = (*it_cp);

			if( !cp )
			{
				continue;
			}

			std::vector<shared_ptr<IfcLengthMeasure> >& coords = cp->m_Coordinates;
			if( coords.size() > 2 )
			{
				vertices->push_back( osg::Vec3( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, coords[2]->m_value*length_factor ) );
			}
			else if( coords.size() > 1 )
			{
				vertices->push_back( osg::Vec3( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 ) );
			}
		}
	}
}


void FaceConverter::convertIfcBSplineSurface( const shared_ptr<IfcRationalBSplineSurfaceWithKnots>& ifc_surface, shared_ptr<carve::input::PolylineSetData>& polyline_data )
{
	std::vector<shared_ptr<IfcParameterValue> >& ifc_u_knots = ifc_surface->m_UKnots;
	std::vector<shared_ptr<IfcParameterValue> >& ifc_v_knots = ifc_surface->m_VKnots;
	std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& ifc_control_points = ifc_surface->m_ControlPointsList;
	std::vector<std::vector<double> > vec_weights = ifc_surface->m_WeightsData;

	osg::ref_ptr<osg::Vec3Array> control_point_array = new osg::Vec3Array();
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	convertIfcCartesianPointVector2D( ifc_control_points, length_factor, control_point_array );
	int degree_u = ifc_surface->m_UDegree;
	int degree_v = ifc_surface->m_VDegree;
	unsigned int numPathU=10;
	unsigned int numPathV=10;

	const unsigned int eta = ifc_control_points.size();
	if( eta < 2 )
	{
		return;
	}
	const unsigned int zeta = ifc_control_points[0].size();

	const int num_points_per_section = eta*zeta;
	// TODO: implement
}

