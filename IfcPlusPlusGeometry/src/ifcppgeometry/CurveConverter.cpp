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


#define _USE_MATH_DEFINES 
#include <math.h>

#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/UnitConverter.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPUtil.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement2D.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement3D.h"
#include "ifcpp/IFC4/include/IfcBoundedCurve.h"
#include "ifcpp/IFC4/include/IfcCompositeCurve.h"
#include "ifcpp/IFC4/include/IfcCompositeCurveSegment.h"
#include "ifcpp/IFC4/include/IfcCartesianPoint.h"
#include "ifcpp/IFC4/include/IfcPolyline.h"
#include "ifcpp/IFC4/include/IfcTrimmedCurve.h"
#include "ifcpp/IFC4/include/IfcTrimmingSelect.h"
#include "ifcpp/IFC4/include/IfcParameterValue.h"
#include "ifcpp/IFC4/include/IfcBSplineCurve.h"
#include "ifcpp/IFC4/include/IfcPolyLoop.h"
#include "ifcpp/IFC4/include/IfcEdgeLoop.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcPositiveLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcPcurve.h"
#include "ifcpp/IFC4/include/IfcCircle.h"
#include "ifcpp/IFC4/include/IfcConic.h"
#include "ifcpp/IFC4/include/IfcEllipse.h"
#include "ifcpp/IFC4/include/IfcLine.h"
#include "ifcpp/IFC4/include/IfcVector.h"
#include "ifcpp/IFC4/include/IfcDirection.h"
#include "ifcpp/IFC4/include/IfcOffsetCurve2D.h"
#include "ifcpp/IFC4/include/IfcOffsetCurve3D.h"
#include "ifcpp/IFC4/include/IfcEdge.h"
#include "ifcpp/IFC4/include/IfcOrientedEdge.h"
#include "ifcpp/IFC4/include/IfcVertexPoint.h"

#include "GeometrySettings.h"
#include "GeomUtils.h"
#include "GeometryException.h"
#include "PlacementConverter.h"
#include "ProfileConverter.h"
#include "CurveConverter.h"

CurveConverter::CurveConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> uc )
	: m_geom_settings(geom_settings), m_unit_converter( uc )
{
}

CurveConverter::~CurveConverter()
{
}

void CurveConverter::convertIfcCurve2D( const shared_ptr<IfcCurve>& ifc_curve, std::vector<carve::geom::vector<2> >& loops, std::vector<carve::geom::vector<2> >& segment_start_points ) const
{
	std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
	std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
	convertIfcCurve2D( ifc_curve, loops, segment_start_points, trim1_vec, trim2_vec, true );
}


void CurveConverter::convertIfcCurve2D( const shared_ptr<IfcCurve>& ifc_curve, std::vector<carve::geom::vector<2> >& target_vec, std::vector<carve::geom::vector<2> >& segment_start_points,
	std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool sense_agreement ) const
{
	std::vector<carve::geom::vector<3> > target_vec_3d;
	std::vector<carve::geom::vector<3> > segment_start_points_3d;
	convertIfcCurve( ifc_curve, target_vec_3d, segment_start_points_3d, trim1_vec, trim2_vec, sense_agreement );

	for( int i=0; i<target_vec_3d.size(); ++i )
	{
		carve::geom::vector<3>& point_3d = target_vec_3d[i];
		target_vec.push_back( carve::geom::VECTOR( point_3d.x, point_3d.y ) );
	}
	for( int i=0; i<segment_start_points_3d.size(); ++i )
	{
		carve::geom::vector<3>& point_3d = segment_start_points_3d[i];
		segment_start_points.push_back( carve::geom::VECTOR( point_3d.x, point_3d.y ) );
	}
}


void CurveConverter::convertIfcCurve( const shared_ptr<IfcCurve>& ifc_curve, std::vector<carve::geom::vector<3> >& loops, std::vector<carve::geom::vector<3> >& segment_start_points ) const
{
	std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
	std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
	convertIfcCurve( ifc_curve, loops, segment_start_points, trim1_vec, trim2_vec, true );
}


void CurveConverter::convertIfcCurve( const shared_ptr<IfcCurve>& ifc_curve, std::vector<carve::geom::vector<3> >& target_vec, std::vector<carve::geom::vector<3> >& segment_start_points,
	std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool sense_agreement ) const
{
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	double plane_angle_factor = m_unit_converter->getAngleInRadianFactor();

	//	ENTITY IfcCurve ABSTRACT SUPERTYPE OF	(ONEOF(IfcBoundedCurve, IfcConic, IfcLine, IfcOffsetCurve2D, IfcOffsetCurve3D, IfcPCurve))
	shared_ptr<IfcBoundedCurve> bounded_curve = dynamic_pointer_cast<IfcBoundedCurve>(ifc_curve);
	if( bounded_curve )
	{
		shared_ptr<IfcCompositeCurve> composite_curve = dynamic_pointer_cast<IfcCompositeCurve>(bounded_curve);
		if( composite_curve )
		{
			// ENTITY IfcBoundedCurve ABSTRACT SUPERTYPE OF	(ONEOF(IfcCompositeCurve, IfcPolyline, IfcTrimmedCurve, IfcBSplineCurve))
			std::vector<shared_ptr<IfcCompositeCurveSegment> > segements = composite_curve->m_Segments;
			std::vector<shared_ptr<IfcCompositeCurveSegment> >::iterator it_segments = segements.begin();
			for(; it_segments!=segements.end(); ++it_segments )
			{
				shared_ptr<IfcCompositeCurveSegment> segement = (*it_segments);
				shared_ptr<IfcCurve> segement_curve = segement->m_ParentCurve;

				std::vector<carve::geom::vector<3> > segment_vec;
				convertIfcCurve( segement_curve, segment_vec, segment_start_points );
				if( segment_vec.size() > 0 )
				{
					GeomUtils::appendPointsToCurve( segment_vec, target_vec );
				}
			}
			return;
		}

		shared_ptr<IfcPolyline> poly_line = dynamic_pointer_cast<IfcPolyline>(ifc_curve);
		if( poly_line )
		{
			std::vector<shared_ptr<IfcCartesianPoint> >& points = poly_line->m_Points;
			if( points.size() > 0 )
			{
				convertIfcCartesianPointVector( points, target_vec );
				segment_start_points.push_back( carve::geom::VECTOR( points[0]->m_Coordinates[0]->m_value*length_factor, points[0]->m_Coordinates[1]->m_value*length_factor, 0 ) );
			}
			return;
		}

		shared_ptr<IfcTrimmedCurve> trimmed_curve = dynamic_pointer_cast<IfcTrimmedCurve>(bounded_curve);
		if( trimmed_curve )
		{
			shared_ptr<IfcCurve> basis_curve = trimmed_curve->m_BasisCurve;
			std::vector<carve::geom::vector<3> > basis_curve_points;

			std::vector<shared_ptr<IfcTrimmingSelect> > curve_trim1_vec = trimmed_curve->m_Trim1;
			std::vector<shared_ptr<IfcTrimmingSelect> > curve_trim2_vec = trimmed_curve->m_Trim2;
			bool trimmed_sense_agreement = trimmed_curve->m_SenseAgreement;

			convertIfcCurve( basis_curve, basis_curve_points, segment_start_points, curve_trim1_vec, curve_trim2_vec, trimmed_sense_agreement );
			GeomUtils::appendPointsToCurve( basis_curve_points, target_vec );
			return;
		}

		shared_ptr<IfcBSplineCurve> bspline_curve = dynamic_pointer_cast<IfcBSplineCurve>(bounded_curve);
		if( bspline_curve )
		{
			std::vector<shared_ptr<IfcCartesianPoint> >&	points = bspline_curve->m_ControlPointsList;


			// TODO: compute bspline curve
			convertIfcCartesianPointVector( points, target_vec );
			return;
		}
		throw UnhandledRepresentationException(bounded_curve);
	}

	shared_ptr<IfcConic> conic = dynamic_pointer_cast<IfcConic>(ifc_curve);
	if( conic )
	{
		// ENTITY IfcConic ABSTRACT SUPERTYPE OF(ONEOF(IfcCircle, IfcEllipse))
		shared_ptr<IfcAxis2Placement> conic_placement = conic->m_Position;
		carve::math::Matrix conic_position_matrix( carve::math::Matrix::IDENT() );

		shared_ptr<IfcAxis2Placement2D> axis2placement2d = dynamic_pointer_cast<IfcAxis2Placement2D>( conic_placement );
		if( axis2placement2d )
		{
			PlacementConverter::convertIfcAxis2Placement2D( axis2placement2d, conic_position_matrix, length_factor );
		}
		else if( dynamic_pointer_cast<IfcAxis2Placement3D>( conic_placement ) )
		{
			shared_ptr<IfcAxis2Placement3D> axis2placement3d = dynamic_pointer_cast<IfcAxis2Placement3D>( conic_placement );
			PlacementConverter::convertIfcAxis2Placement3D( axis2placement3d, conic_position_matrix, length_factor );
		}

		shared_ptr<IfcCircle> circle = dynamic_pointer_cast<IfcCircle>(conic);
		if( circle )
		{
			double circle_radius = 0.0;
			if( circle->m_Radius )
			{
				circle_radius = circle->m_Radius->m_value*length_factor;
			}

			carve::geom::vector<3> circle_center = conic_position_matrix*carve::geom::VECTOR( 0, 0, 0 );

			double trim_angle1 = 0.0;
			double trim_angle2 = M_PI*2.0;

			// check for trimming begin
			shared_ptr<IfcParameterValue> trim_par1;
			if( trim1_vec.size() > 0 )
			{
				if( findFirstInVector( trim1_vec, trim_par1 ) )
				{
					trim_angle1 = trim_par1->m_value*plane_angle_factor;
				}
				else
				{
					shared_ptr<IfcCartesianPoint> trim_point1;
					if( findFirstInVector( trim1_vec, trim_point1 ) )
					{
						carve::geom::vector<3> trim_point;
						convertIfcCartesianPoint( trim_point1, trim_point );
						trim_angle1 = getAngleOnCircle( circle_center, circle_radius, trim_point );
					}
				}
			}

			if( trim2_vec.size() > 0 )
			{
				// check for trimming end
				shared_ptr<IfcParameterValue> trim_par2;
				if( findFirstInVector( trim2_vec, trim_par2 ) )
				{
					trim_angle2 = trim_par2->m_value*plane_angle_factor;
				}
				else
				{
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if( findFirstInVector( trim2_vec, ifc_trim_point ) )
					{
						carve::geom::vector<3> trim_point;
						convertIfcCartesianPoint( ifc_trim_point, trim_point );
						trim_angle2 = getAngleOnCircle( circle_center, circle_radius, trim_point );
					}
				}
			}

			double start_angle = trim_angle1;
			double opening_angle = 0;

			if( sense_agreement )
			{
				if( trim_angle1 < trim_angle2 )
				{
					opening_angle = trim_angle2 - trim_angle1;
				}
				else
				{
					// circle passes 0 angle
					opening_angle = trim_angle2 - trim_angle1 + 2.0*M_PI;
				}
			}
			else
			{
				if( trim_angle1 > trim_angle2 )
				{
					opening_angle = trim_angle2 - trim_angle1;
				}
				else
				{
					// circle passes 0 angle
					opening_angle = trim_angle2 - trim_angle1 - 2.0*M_PI;
				}
			}

			if( opening_angle > 0 )
			{
				while( opening_angle > 2.0*M_PI )
				{
					opening_angle -= 2.0*M_PI;
				}
			}
			else
			{
				while( opening_angle < -2.0*M_PI )
				{
					opening_angle += 2.0*M_PI;
				}
			}

			int num_segments = m_geom_settings->m_num_vertices_per_circle*(std::abs(opening_angle)/(2.0*M_PI));
			if( num_segments < m_geom_settings->m_min_num_vertices_per_arc ) num_segments = m_geom_settings->m_min_num_vertices_per_arc;
			const double circle_center_x = 0.0;
			const double circle_center_y = 0.0;
			std::vector<carve::geom::vector<2> > circle_points;
			ProfileConverter::addArcWithEndPoint( circle_points, circle_radius, start_angle, opening_angle, circle_center_x, circle_center_y, num_segments );

			if( circle_points.size() > 0 )
			{
				// apply position
				for( unsigned int i=0; i<circle_points.size(); ++i )
				{
					carve::geom::vector<2>&  point = circle_points[i];
					carve::geom::vector<3>  point3d( carve::geom::VECTOR( point.x, point.y, 0 ) );
					point3d = conic_position_matrix * point3d;
					point.x = point3d.x;
					point.y = point3d.y;
				}

				GeomUtils::appendPointsToCurve( circle_points, target_vec );
				segment_start_points.push_back( carve::geom::VECTOR( circle_points[0].x, circle_points[0].y, 0 ) );
			}

			return;
		}

		shared_ptr<IfcEllipse> ellipse = dynamic_pointer_cast<IfcEllipse>(conic);
		if( ellipse )
		{
			if( ellipse->m_SemiAxis1 )
			{
				if( ellipse->m_SemiAxis2 )
				{
					double xRadius = ellipse->m_SemiAxis1->m_value*length_factor;
					double yRadius = ellipse->m_SemiAxis2->m_value*length_factor;

					double radiusMax = std::max(xRadius, yRadius);
					int num_segments = m_geom_settings->m_num_vertices_per_circle;	// TODO: adapt to model size and complexity

					// todo: implement clipping

					std::vector<carve::geom::vector<3> > circle_points;
					double angle=0;
					for(int i = 0; i < num_segments; ++i) 
					{
						circle_points.push_back( carve::geom::vector<3>( carve::geom::VECTOR( xRadius * cos(angle), yRadius * sin(angle), 0 ) ) );
						angle += 2.0*M_PI / double(num_segments);
					}

					// apply position
					for( unsigned int i=0; i<circle_points.size(); ++i )
					{
						carve::geom::vector<3>& point = circle_points[i];
						point = conic_position_matrix * point;
					}
					GeomUtils::appendPointsToCurve( circle_points, target_vec );

					//if( segment_start_points != nullptr )
					{
						carve::geom::vector<3> pt0 = circle_points[0];
						segment_start_points.push_back( pt0 );
					}
				}
			}
			return;
		}
		throw UnhandledRepresentationException(conic);
	}

	shared_ptr<IfcLine> line = dynamic_pointer_cast<IfcLine>(ifc_curve);
	if( line )
	{
		shared_ptr<IfcCartesianPoint> ifc_line_point = line->m_Pnt;
		carve::geom::vector<3> line_origin;
		convertIfcCartesianPoint( ifc_line_point, line_origin );
		
		// line: lambda(u) = line_point + u*line_direction
		shared_ptr<IfcVector> line_vec = line->m_Dir;
		if( !line_vec )
		{
			return;
		}
		shared_ptr<IfcDirection> ifc_line_direction = line_vec->m_Orientation;

		std::vector<double>& direction_ratios = ifc_line_direction->m_DirectionRatios;
		carve::geom::vector<3> line_direction;
		if( direction_ratios.size() > 1 )
		{
			if( direction_ratios.size() > 2 )
			{
				line_direction = carve::geom::VECTOR( direction_ratios[0], direction_ratios[1], direction_ratios[2] );
			}
			else
			{
				line_direction = carve::geom::VECTOR( direction_ratios[0], direction_ratios[1], 0 );
			}
		}
		line_direction.normalize();

		shared_ptr<IfcLengthMeasure> line_magnitude = line_vec->m_Magnitude;
		double line_magnitude_value = line_magnitude->m_value*length_factor;

		// check for trimming at beginning of line
		double start_parameter = 0.0;
		shared_ptr<IfcParameterValue> trim_par1;
		if( findFirstInVector( trim1_vec, trim_par1 ) )
		{
			start_parameter = trim_par1->m_value;
			line_origin = line_origin + line_direction*start_parameter;
		}
		else
		{
			shared_ptr<IfcCartesianPoint> ifc_trim_point;
			if( findFirstInVector( trim1_vec, ifc_trim_point ) )
			{
				carve::geom::vector<3> trim_point;
				convertIfcCartesianPoint( ifc_trim_point, trim_point );

				carve::geom::vector<3> closest_point_on_line;
				GeomUtils::closestPointOnLine( trim_point, line_origin, line_direction, closest_point_on_line );

				if( (closest_point_on_line-trim_point).length() < 0.0001 )
				{
					// trimming point is on the line
					line_origin = trim_point;
				}
			}
		}
		// check for trimming at end of line
		carve::geom::vector<3> line_end;
		shared_ptr<IfcParameterValue> trim_par2;
		if( findFirstInVector( trim2_vec, trim_par2 ) )
		{
			line_magnitude_value = trim_par2->m_value*length_factor;
			line_end = line_origin + line_direction*line_magnitude_value;
		}
		else
		{
			shared_ptr<IfcCartesianPoint> ifc_trim_point;
			if( findFirstInVector( trim2_vec, ifc_trim_point ) )
			{
				carve::geom::vector<3> trim_point;
				convertIfcCartesianPoint( ifc_trim_point, trim_point );

				carve::geom::vector<3> closest_point_on_line;
				GeomUtils::closestPointOnLine( trim_point, line_origin, line_direction, closest_point_on_line );

				if( (closest_point_on_line-trim_point).length() < 0.0001 )
				{
					// trimming point is on the line
					line_end = trim_point;
				}
			}
		}

		std::vector<carve::geom::vector<3> > points_vec;
		points_vec.push_back( line_origin );
		points_vec.push_back( line_end );

		GeomUtils::appendPointsToCurve( points_vec, target_vec );

		//if( segment_start_points != nullptr )
		{
			segment_start_points.push_back( line_origin );
		}
		return;
	}

	shared_ptr<IfcOffsetCurve2D> offset_curve_2d = dynamic_pointer_cast<IfcOffsetCurve2D>(ifc_curve);
	if( offset_curve_2d )
	{
		// TODO: implement
		return;
	}

	shared_ptr<IfcOffsetCurve3D> offset_curve_3d = dynamic_pointer_cast<IfcOffsetCurve3D>(ifc_curve);
	if( offset_curve_3d )
	{
		// TODO: implement
		return;
	}

	shared_ptr<IfcPcurve> pcurve = dynamic_pointer_cast<IfcPcurve>(ifc_curve);
	if( pcurve )
	{
		// TODO: implement
		return;
	}

	throw UnhandledRepresentationException(ifc_curve);
}


void CurveConverter::convertIfcPolyline( const shared_ptr<IfcPolyline>& poly_line, std::vector<carve::geom::vector<3> >& loop ) const
{
	convertIfcCartesianPointVector( poly_line->m_Points, loop );
}

void CurveConverter::convertIfcLoop( const shared_ptr<IfcLoop>& loop,	std::vector<carve::geom::vector<3> >& loop_points ) const
{
	const shared_ptr<IfcPolyLoop> poly_loop = dynamic_pointer_cast<IfcPolyLoop>( loop );
	if( poly_loop )
	{
		const std::vector<shared_ptr<IfcCartesianPoint> >&    ifc_points = poly_loop->m_Polygon;
		convertIfcCartesianPointVectorSkipDuplicates( ifc_points, loop_points );

		// if first and last point have same coordinates, remove last point
		while( loop_points.size() > 2 )
		{
			carve::geom3d::Vector& first = loop_points.front();
			carve::geom3d::Vector& last = loop_points.back();

			if( std::abs(first.x-last.x) < 0.00000001 )
			{
				if( std::abs(first.y-last.y) < 0.00000001 )
				{
					if( std::abs(first.z-last.z) < 0.00000001 )
					{
						loop_points.pop_back();
						continue;
					}
				}
			}
			break;
		}

		return;

	}

	shared_ptr<IfcEdgeLoop> edge_loop = dynamic_pointer_cast<IfcEdgeLoop>( loop );
	if( edge_loop )
	{
		std::vector<shared_ptr<IfcOrientedEdge> >& edge_list = edge_loop->m_EdgeList;
		std::vector<shared_ptr<IfcOrientedEdge> >::iterator it_edge;
		for( it_edge=edge_list.begin(); it_edge!=edge_list.end(); ++it_edge )
		{
			shared_ptr<IfcOrientedEdge> oriented_edge = (*it_edge);
			shared_ptr<IfcEdge> edge = oriented_edge->m_EdgeElement;

			shared_ptr<IfcVertex> edge_start = edge->m_EdgeStart;
			shared_ptr<IfcVertexPoint> edge_start_point = dynamic_pointer_cast<IfcVertexPoint>(edge_start);
			if( edge_start_point )
			{
				if( edge_start_point->m_VertexGeometry )
				{
					shared_ptr<IfcPoint> edge_start_point_geometry = edge_start_point->m_VertexGeometry;
					shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>(edge_start_point_geometry);
					if( !ifc_point )
					{
						// TODO: could be also  IfcPointOnCurve, IfcPointOnSurface
						continue;
					}



					// TODO: implement

				}
			}
			shared_ptr<IfcVertex> edge_end = edge->m_EdgeEnd;
			shared_ptr<IfcVertexPoint> edge_end_point = dynamic_pointer_cast<IfcVertexPoint>(edge_end);
			if( edge_end_point )
			{
				if( edge_end_point->m_VertexGeometry )
				{
					shared_ptr<IfcPoint> edge_point_geometry = edge_end_point->m_VertexGeometry;
					shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>(edge_point_geometry);

					if( !ifc_point )
					{
						// TODO: could be also  IfcPointOnCurve, IfcPointOnSurface
						continue;
					}

					// TODO: implement
				}
			}
		}
	}
}

void CurveConverter::convertIfcCartesianPoint( const shared_ptr<IfcCartesianPoint>& ifc_point,	carve::geom::vector<3> & point ) const
{
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	std::vector<shared_ptr<IfcLengthMeasure> >& coords1 = ifc_point->m_Coordinates;
	if( coords1.size() > 2 )
	{
		// round to 0.1 mm
		// TODO: round only when digits are noise
#ifdef ROUND_IFC_COORDINATES
		double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double z = round(coords1[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		double x = coords1[0]->m_value*length_factor;
		double y = coords1[1]->m_value*length_factor;
		double z = coords1[2]->m_value*length_factor;
#endif
		point = carve::geom::VECTOR( x, y, z );
	}
	else if( coords1.size() > 1 )
	{
		// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
		double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		double x = coords1[0]->m_value*length_factor;
		double y = coords1[1]->m_value*length_factor;
#endif
		point = carve::geom::VECTOR( x, y, 0.0 );
	}
}

void CurveConverter::convertIfcCartesianPoint( const shared_ptr<IfcCartesianPoint>& ifc_point,	carve::geom::vector<3> & point, double length_factor )
{
	std::vector<shared_ptr<IfcLengthMeasure> >& coords1 = ifc_point->m_Coordinates;
	if( coords1.size() > 2 )
	{
		// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
		double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double z = round(coords1[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		double x = coords1[0]->m_value*length_factor;
		double y = coords1[1]->m_value*length_factor;
		double z = coords1[2]->m_value*length_factor;
#endif
		point = carve::geom::VECTOR( x, y, z );
	}
	else if( coords1.size() > 1 )
	{
		// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
		double x = round(coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
		double y = round(coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
		double x = coords1[0]->m_value*length_factor;
		double y = coords1[1]->m_value*length_factor;
#endif
		point = carve::geom::VECTOR( x, y, 0.0 );
	}
}

void CurveConverter::convertIfcCartesianPointVector( const std::vector<shared_ptr<IfcCartesianPoint> >& points, std::vector<carve::geom::vector<3> >& loop ) const
{
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	const unsigned int num_points = points.size();
	for( unsigned int i_point=0; i_point < num_points; ++i_point )
	{
		const std::vector<shared_ptr<IfcLengthMeasure> >& coords = points[i_point]->m_Coordinates;

		if( coords.size() > 2  )
		{
			// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
			double x = round(coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			double y = round(coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			double z = round(coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
			double x = coords[0]->m_value*length_factor;
			double y = coords[1]->m_value*length_factor;
			double z = coords[2]->m_value*length_factor;
#endif
			loop.push_back( carve::geom::VECTOR( x, y, z ) );
		}
		else if( coords.size() > 1  )
		{
			// round to 0.1 mm
#ifdef ROUND_IFC_COORDINATES
			double x = round(coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			double y = round(coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
			double x = coords[0]->m_value*length_factor;
			double y = coords[1]->m_value*length_factor;
#endif
			loop.push_back( carve::geom::VECTOR( x, y, 0.0 ) );
		}
		else
		{
			std::cout << "convertIfcCartesianPointVector: ifc_pt->m_Coordinates.size() != 2" << std::endl;
		}
	}
}

void CurveConverter::convertIfcCartesianPointVectorSkipDuplicates( const std::vector<shared_ptr<IfcCartesianPoint> >& ifc_points, std::vector<carve::geom::vector<3> >& loop ) const
{
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	std::vector<shared_ptr<IfcCartesianPoint> >::const_iterator it_cp;
	int i=0;
	carve::geom::vector<3>  vertex_previous;
	for( it_cp=ifc_points.begin(); it_cp!=ifc_points.end(); ++it_cp, ++i )
	{
		shared_ptr<IfcCartesianPoint> cp = (*it_cp);
		const int cp_id = cp->getId();
		double x = 0.0, y = 0.0, z = 0.0;
		std::vector<shared_ptr<IfcLengthMeasure> >& coords = cp->m_Coordinates;


		if( coords.size() > 2 )
		{
#ifdef ROUND_IFC_COORDINATES
			x = round(coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			y = round(coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			z = round(coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
			x = coords[0]->m_value*length_factor;
			y = coords[1]->m_value*length_factor;
			z = coords[2]->m_value*length_factor;
#endif
		}
		else if( coords.size() > 1 )
		{
#ifdef ROUND_IFC_COORDINATES
			x = round(coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
			y = round(coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP)*ROUND_IFC_COORDINATES_DOWN;
#else
			x = coords[0]->m_value*length_factor;
			y = coords[1]->m_value*length_factor;
#endif
		}

		carve::geom::vector<3>  vertex( carve::geom::VECTOR( x, y, z ) );

		// skip duplicate vertices
		if( it_cp != ifc_points.begin() )
		{
			if( std::abs(vertex.x-vertex_previous.x) < 0.00000001 )
			{
				if( std::abs(vertex.y-vertex_previous.y) < 0.00000001 )
				{
					if( std::abs(vertex.z-vertex_previous.z) < 0.00000001 )
					{
						// TODO: is it better to report degenerated loops, or to just omit them?
						continue;
					}
				}
			}
		}
		loop.push_back( vertex );
		vertex_previous = vertex;
	}
}

// @brief: returns the corresponding angle (radian, 0 is to the right) if the given point lies on the circle. If the point does not lie on the circle, -1 is returned.
double CurveConverter::getAngleOnCircle( const carve::geom::vector<3>& circle_center, double circle_radius, const carve::geom::vector<3>& trim_point )
{
	double result_angle = -1.0;
	carve::geom::vector<3> center_trim_point = trim_point-circle_center;
	if( std::abs(center_trim_point.length() - circle_radius) < 0.0001 )
	{
		carve::geom::vector<3> center_trim_point_direction = center_trim_point;
		center_trim_point_direction.normalize();
		double cos_angle = carve::geom::dot( center_trim_point_direction, carve::geom::vector<3> ( carve::geom::VECTOR( 1.0, 0, 0 ) ) );

		if( std::abs(cos_angle) < 0.0001 )
		{
			if( center_trim_point.y > 0 )
			{
				result_angle = M_PI_2;
			}
			else if( center_trim_point.y < 0 )
			{
				result_angle = M_PI*1.5;
			}
		}
		else
		{
			if( center_trim_point.y > 0 )
			{
				result_angle = acos( cos_angle );
			}
			else if( center_trim_point.y < 0 )
			{
				result_angle = 2.0*M_PI - acos( cos_angle );
			}
		}
	}
	return result_angle;
}

