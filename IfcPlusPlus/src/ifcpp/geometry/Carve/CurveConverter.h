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

#include <ifcpp/geometry/GeometryException.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/IFC4/include/IfcBoolean.h>
#include <ifcpp/IFC4/include/IfcBSplineCurve.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcCircle.h>
#include <ifcpp/IFC4/include/IfcConic.h>
#include <ifcpp/IFC4/include/IfcCompositeCurve.h>
#include <ifcpp/IFC4/include/IfcCompositeCurveSegment.h>
#include <ifcpp/IFC4/include/IfcCurve.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcEdgeLoop.h>
#include <ifcpp/IFC4/include/IfcEllipse.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLine.h>
#include <ifcpp/IFC4/include/IfcLoop.h>
#include <ifcpp/IFC4/include/IfcOffsetCurve2D.h>
#include <ifcpp/IFC4/include/IfcOffsetCurve3D.h>
#include <ifcpp/IFC4/include/IfcOrientedEdge.h>
#include <ifcpp/IFC4/include/IfcParameterValue.h>
#include <ifcpp/IFC4/include/IfcPcurve.h>
#include <ifcpp/IFC4/include/IfcPolyline.h>
#include <ifcpp/IFC4/include/IfcPolyLoop.h>
#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcTrimmedCurve.h>
#include <ifcpp/IFC4/include/IfcTrimmingSelect.h>
#include <ifcpp/IFC4/include/IfcVector.h>
#include <ifcpp/IFC4/include/IfcVertexPoint.h>

#include "SplineConverter.h"
#include "GeomUtils.h"
#include "GeomDebugDump.h"
#include "PlacementConverter.h"
#include "PointConverter.h"
#include "SplineConverter.h"
#include "IncludeCarveHeaders.h"


//\brief class to convert different types of IFC curve representations into carve input geometry
class CurveConverter : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings>	m_geom_settings;
	shared_ptr<PlacementConverter>	m_placement_converter;
	shared_ptr<PointConverter>		m_point_converter;
	shared_ptr<SplineConverter>		m_spline_converter;

public:
	CurveConverter( shared_ptr<GeometrySettings>& gs, shared_ptr<PlacementConverter>& placement_converter, shared_ptr<PointConverter>& pc, shared_ptr<SplineConverter>& sc )
		: m_geom_settings( gs ), m_placement_converter(placement_converter), m_point_converter( pc ), m_spline_converter( sc )
	{
	}

	virtual ~CurveConverter(){}

	const shared_ptr<GeometrySettings>&		getGeomSettings() { return m_geom_settings; }
	const shared_ptr<PlacementConverter>&	getPlcamentConverter() { return m_placement_converter; }
	const shared_ptr<PointConverter>&		getPointConverter() { return m_point_converter; }
	const shared_ptr<SplineConverter>&		getSplineConverter() { return m_spline_converter; }

	void convertIfcCurve2D( const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec2>& loops, std::vector<vec2>& segment_start_points ) const
	{
		std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
		std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
		convertIfcCurve2D( ifc_curve, loops, segment_start_points, trim1_vec, trim2_vec, true );
	}

	void convertIfcCurve2D( const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec2>& target_vec, std::vector<vec2>& segment_start_points,
		std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool sense_agreement ) const
	{
		std::vector<vec3> target_vec_3d;
		std::vector<vec3> segment_start_points_3d;
		convertIfcCurve( ifc_curve, target_vec_3d, segment_start_points_3d, trim1_vec, trim2_vec, sense_agreement );

		for( size_t i = 0; i < target_vec_3d.size(); ++i )
		{
			vec3& point_3d = target_vec_3d[i];
			target_vec.push_back( carve::geom::VECTOR( point_3d.x, point_3d.y ) );
		}
		for( size_t i = 0; i < segment_start_points_3d.size(); ++i )
		{
			vec3& point_3d = segment_start_points_3d[i];
			segment_start_points.push_back( carve::geom::VECTOR( point_3d.x, point_3d.y ) );
		}
	}

	void convertIfcCurve( const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec3>& loops, std::vector<vec3>& segment_start_points ) const
	{
		std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
		std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
		convertIfcCurve( ifc_curve, loops, segment_start_points, trim1_vec, trim2_vec, true );
	}

	void convertIfcCurve( const shared_ptr<IfcCurve>& ifc_curve, std::vector<vec3>& target_vec, std::vector<vec3>& segment_start_points,
		std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool sense_agreement ) const
	{
		double length_factor = m_point_converter->getUnitConverter()->getLengthInMeterFactor();

		//	ENTITY IfcCurve ABSTRACT SUPERTYPE OF	(ONEOF(IfcBoundedCurve, IfcConic, IfcLine, IfcOffsetCurve2D, IfcOffsetCurve3D, IfcPCurve))
		shared_ptr<IfcBoundedCurve> bounded_curve = dynamic_pointer_cast<IfcBoundedCurve>( ifc_curve );
		if( bounded_curve )
		{
			shared_ptr<IfcCompositeCurve> composite_curve = dynamic_pointer_cast<IfcCompositeCurve>( bounded_curve );
			if( composite_curve )
			{
				// ENTITY IfcBoundedCurve ABSTRACT SUPERTYPE OF	(ONEOF(IfcCompositeCurve, IfcPolyline, IfcTrimmedCurve, IfcBSplineCurve))
				std::vector<shared_ptr<IfcCompositeCurveSegment> >& vec_segements = composite_curve->m_Segments;
				for( size_t i_segments = 0; i_segments < vec_segements.size(); ++i_segments )
				{
					shared_ptr<IfcCompositeCurveSegment> segement = vec_segements[i_segments];
					shared_ptr<IfcCurve> segement_curve = segement->m_ParentCurve;

					std::vector<vec3> segment_vec;
					convertIfcCurve( segement_curve, segment_vec, segment_start_points );
					if( segment_vec.size() > 0 )
					{
						GeomUtils::appendPointsToCurve( segment_vec, target_vec );
					}
				}
				return;
			}

			shared_ptr<IfcPolyline> poly_line = dynamic_pointer_cast<IfcPolyline>( ifc_curve );
			if( poly_line )
			{
				std::vector<shared_ptr<IfcCartesianPoint> >& points = poly_line->m_Points;
				if( points.size() > 0 )
				{
					m_point_converter->convertIfcCartesianPointVector( points, target_vec );
					shared_ptr<IfcCartesianPoint>& first_point = points[0];
					if( first_point->m_Coordinates.size() > 1 )
					{
						if( first_point->m_Coordinates[0] && first_point->m_Coordinates[1] )
						{
							segment_start_points.push_back( carve::geom::VECTOR( first_point->m_Coordinates[0]->m_value*length_factor, first_point->m_Coordinates[1]->m_value*length_factor, 0 ) );
						}
					}
				}
				return;
			}

			shared_ptr<IfcTrimmedCurve> trimmed_curve = dynamic_pointer_cast<IfcTrimmedCurve>( bounded_curve );
			if( trimmed_curve )
			{
				shared_ptr<IfcCurve> basis_curve = trimmed_curve->m_BasisCurve;
				if( basis_curve )
				{
					std::vector<vec3> basis_curve_points;
					std::vector<shared_ptr<IfcTrimmingSelect> >& curve_trim1_vec = trimmed_curve->m_Trim1;
					std::vector<shared_ptr<IfcTrimmingSelect> >& curve_trim2_vec = trimmed_curve->m_Trim2;
					bool trimmed_sense_agreement = trimmed_curve->m_SenseAgreement ? trimmed_curve->m_SenseAgreement->m_value : true;

					convertIfcCurve( basis_curve, basis_curve_points, segment_start_points, curve_trim1_vec, curve_trim2_vec, trimmed_sense_agreement );
					GeomUtils::appendPointsToCurve( basis_curve_points, target_vec );
				}
				return;
			}

			shared_ptr<IfcBSplineCurve> bspline_curve = dynamic_pointer_cast<IfcBSplineCurve>( bounded_curve );
			if( bspline_curve )
			{
				m_spline_converter->convertBSplineCurve( bspline_curve, target_vec, segment_start_points );
				return;
			}
			throw UnhandledRepresentationException( bounded_curve );
		}

		shared_ptr<IfcConic> conic = dynamic_pointer_cast<IfcConic>( ifc_curve );
		if( conic )
		{
			// ENTITY IfcConic ABSTRACT SUPERTYPE OF(ONEOF(IfcCircle, IfcEllipse))

			shared_ptr<TransformData> conic_position_matrix;
			// TYPE IfcAxis2Placement = SELECT	(IfcAxis2Placement2D	,IfcAxis2Placement3D);
			shared_ptr<IfcAxis2Placement> conic_placement_select = conic->m_Position;
			if( conic_placement_select )
			{
				shared_ptr<IfcAxis2Placement2D> axis2placement2d = dynamic_pointer_cast<IfcAxis2Placement2D>( conic_placement_select );
				if( axis2placement2d )
				{
					m_placement_converter->convertIfcAxis2Placement2D( axis2placement2d, conic_position_matrix );
				}
				else if( dynamic_pointer_cast<IfcAxis2Placement3D>( conic_placement_select ) )
				{
					shared_ptr<IfcAxis2Placement3D> axis2placement3d = dynamic_pointer_cast<IfcAxis2Placement3D>( conic_placement_select );
					m_placement_converter->convertIfcAxis2Placement3D( axis2placement3d, conic_position_matrix );
				}
			}

			shared_ptr<IfcCircle> circle = dynamic_pointer_cast<IfcCircle>( conic );
			if( circle )
			{
				double circle_radius = 0.0;
				if( circle->m_Radius )
				{
					circle_radius = circle->m_Radius->m_value*length_factor;
				}

				vec3 circle_center;
				if( conic_position_matrix )
				{
					circle_center = conic_position_matrix->m_matrix*carve::geom::VECTOR( 0, 0, 0 );
				}

				double trim_angle1 = 0.0;
				double trim_angle2 = M_PI*2.0;

				// check for trimming begin
				shared_ptr<IfcParameterValue> trim_par1;
				if( trim1_vec.size() > 0 )
				{
					if( GeomUtils::findFirstInVector( trim1_vec, trim_par1 ) )
					{
						double plane_angle_factor = m_point_converter->getUnitConverter()->getAngleInRadiantFactor();
						if( m_point_converter->getUnitConverter()->getAngularUnit() == UnitConverter::UNDEFINED )
						{
							// angular unit definition not found in model, default to radian
							plane_angle_factor = 1.0;

							if( trim_par1->m_value > M_PI )
							{
								// assume degree
								plane_angle_factor = M_PI / 180.0;
							}
						}

						trim_angle1 = trim_par1->m_value*plane_angle_factor;
					}
					else
					{
						shared_ptr<IfcCartesianPoint> trim_point1;
						if( GeomUtils::findFirstInVector( trim1_vec, trim_point1 ) )
						{
							vec3 trim_point;
							PointConverter::convertIfcCartesianPoint( trim_point1, trim_point, length_factor );
							// TODO: get direction of trim_point to circle_center, get angle. This is more robust in case the trim_point is not exactly on the circle
							trim_angle1 = m_point_converter->getAngleOnCircle( circle_center, circle_radius, trim_point );
						}
					}
				}

				if( trim2_vec.size() > 0 )
				{
					// check for trimming end
					shared_ptr<IfcParameterValue> trim_par2;
					if( GeomUtils::findFirstInVector( trim2_vec, trim_par2 ) )
					{
						double plane_angle_factor = m_point_converter->getUnitConverter()->getAngleInRadiantFactor();
						if( m_point_converter->getUnitConverter()->getAngularUnit() == UnitConverter::UNDEFINED )
						{
							// angular unit definition not found in model, default to radian
							plane_angle_factor = 1.0;

							if( trim_par2->m_value > M_PI )
							{
								// assume degree
								plane_angle_factor = M_PI / 180.0;
							}
						}
						trim_angle2 = trim_par2->m_value*plane_angle_factor;
					}
					else
					{
						shared_ptr<IfcCartesianPoint> ifc_trim_point;
						if( GeomUtils::findFirstInVector( trim2_vec, ifc_trim_point ) )
						{
							vec3 trim_point;
							PointConverter::convertIfcCartesianPoint( ifc_trim_point, trim_point, length_factor );
							trim_angle2 = m_point_converter->getAngleOnCircle( circle_center, circle_radius, trim_point );
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

				while( opening_angle > 2.0*M_PI )
				{
					opening_angle -= 2.0*M_PI;
				}
				while( opening_angle < -2.0*M_PI )
				{
					opening_angle += 2.0*M_PI;
				}

				int num_segments = m_geom_settings->getNumVerticesPerCircleWithRadius(circle_radius)*( std::abs( opening_angle ) / ( 2.0*M_PI ) );
				if( num_segments < m_geom_settings->getMinNumVerticesPerArc() ) num_segments = m_geom_settings->getMinNumVerticesPerArc();
				const double circle_center_x = 0.0;
				const double circle_center_y = 0.0;
				std::vector<vec2> circle_points;
				std::vector<vec3> circle_points3D;
				if( circle_radius > 0.0 )
				{
					GeomUtils::addArcWithEndPoint( circle_points, circle_radius, start_angle, opening_angle, circle_center_x, circle_center_y, num_segments );
				}
				else
				{
					circle_points.push_back( carve::geom::VECTOR( circle_center_x, circle_center_y ) );
				}

				if( circle_points.size() > 0 )
				{
					// apply position
					for( size_t i = 0; i < circle_points.size(); ++i )
					{
						vec2&  point = circle_points[i];
						vec3  point3D( carve::geom::VECTOR( point.x, point.y, 0 ) );
						if( conic_position_matrix )
						{
							point3D = conic_position_matrix->m_matrix * point3D;
						}
						point.x = point3D.x;
						point.y = point3D.y;
						circle_points3D.push_back(point3D);
					}

					GeomUtils::appendPointsToCurve(circle_points3D, target_vec );
					segment_start_points.push_back(circle_points3D[0]);
				}

				return;
			}

			shared_ptr<IfcEllipse> ellipse = dynamic_pointer_cast<IfcEllipse>( conic );
			if( ellipse )
			{
				if( ellipse->m_SemiAxis1 )
				{
					if( ellipse->m_SemiAxis2 )
					{
						double x_radius = ellipse->m_SemiAxis1->m_value*length_factor;
						double y_radius = ellipse->m_SemiAxis2->m_value*length_factor;
						int num_segments = m_geom_settings->getNumVerticesPerCircleWithRadius(
                                std::max(x_radius, y_radius));

						// todo: implement clipping

						std::vector<vec3> circle_points;
						double angle = 0;
						for( int i = 0; i < num_segments; ++i )
						{
							circle_points.push_back( vec3( carve::geom::VECTOR( x_radius * cos( angle ), y_radius * sin( angle ), 0 ) ) );
							angle += 2.0*M_PI / double( num_segments );
						}

						// apply position
						if( conic_position_matrix )
						{
							for( size_t i = 0; i < circle_points.size(); ++i )
							{
								vec3& point = circle_points[i];
								point = conic_position_matrix->m_matrix * point;
							}
						}
						GeomUtils::appendPointsToCurve( circle_points, target_vec );

						vec3 pt0 = circle_points[0];
						segment_start_points.push_back( pt0 );
					}
				}
				return;
			}
			throw UnhandledRepresentationException( conic );
		}

		shared_ptr<IfcLine> line = dynamic_pointer_cast<IfcLine>( ifc_curve );
		if( line )
		{
			shared_ptr<IfcCartesianPoint> ifc_line_point = line->m_Pnt;
			vec3 line_origin;
			if( PointConverter::convertIfcCartesianPoint( ifc_line_point, line_origin, length_factor ) )
			{
				// line: lambda(u) = line_point + u*line_direction
				shared_ptr<IfcVector> line_vec = line->m_Dir;
				if( !line_vec )
				{
					return;
				}
				shared_ptr<IfcDirection> ifc_line_direction = line_vec->m_Orientation;

				std::vector<shared_ptr<IfcReal> >& direction_ratios = ifc_line_direction->m_DirectionRatios;
				vec3 line_direction;
				if( direction_ratios.size() > 1 )
				{
					if( direction_ratios.size() > 2 )
					{
						line_direction = carve::geom::VECTOR( direction_ratios[0]->m_value, direction_ratios[1]->m_value, direction_ratios[2]->m_value );
					}
					else
					{
						line_direction = carve::geom::VECTOR( direction_ratios[0]->m_value, direction_ratios[1]->m_value, 0 );
					}
				}
				line_direction.normalize();

				shared_ptr<IfcLengthMeasure> line_magnitude = line_vec->m_Magnitude;
				double line_magnitude_value = line_magnitude->m_value*length_factor;

				// check for trimming at beginning of line
				double start_parameter = 0.0;
				shared_ptr<IfcParameterValue> trim_par1;
				if( GeomUtils::findFirstInVector( trim1_vec, trim_par1 ) )
				{
					start_parameter = trim_par1->m_value;
					line_origin = line_origin + line_direction*start_parameter;
				}
				else
				{
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if( GeomUtils::findFirstInVector( trim1_vec, ifc_trim_point ) )
					{
						vec3 trim_point;
						PointConverter::convertIfcCartesianPoint( ifc_trim_point, trim_point, length_factor );

						vec3 closest_point_on_line;
						GeomUtils::closestPointOnLine( trim_point, line_origin, line_direction, closest_point_on_line );

						if( ( closest_point_on_line - trim_point ).length() < 0.0001 )
						{
							// trimming point is on the line
							line_origin = trim_point;
						}
					}
				}
				// check for trimming at end of line
				vec3 line_end;
				shared_ptr<IfcParameterValue> trim_par2;
				if( GeomUtils::findFirstInVector( trim2_vec, trim_par2 ) )
				{
					line_magnitude_value = trim_par2->m_value*length_factor;
					line_end = line_origin + line_direction*line_magnitude_value;
				}
				else
				{
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if( GeomUtils::findFirstInVector( trim2_vec, ifc_trim_point ) )
					{
						vec3 trim_point;
						PointConverter::convertIfcCartesianPoint( ifc_trim_point, trim_point, length_factor );

						vec3 closest_point_on_line;
						GeomUtils::closestPointOnLine( trim_point, line_origin, line_direction, closest_point_on_line );

						if( ( closest_point_on_line - trim_point ).length() < 0.0001 )
						{
							// trimming point is on the line
							line_end = trim_point;
						}
					}
				}

				std::vector<vec3> points_vec;
				points_vec.push_back( line_origin );
				points_vec.push_back( line_end );

				GeomUtils::appendPointsToCurve( points_vec, target_vec );
				segment_start_points.push_back( line_origin );
			}
			return;
		}

		shared_ptr<IfcOffsetCurve2D> offset_curve_2d = dynamic_pointer_cast<IfcOffsetCurve2D>( ifc_curve );
		if( offset_curve_2d )
		{
			// TODO: implement
			return;
		}

		shared_ptr<IfcOffsetCurve3D> offset_curve_3d = dynamic_pointer_cast<IfcOffsetCurve3D>( ifc_curve );
		if( offset_curve_3d )
		{
			// TODO: implement
			return;
		}

		shared_ptr<IfcPcurve> pcurve = dynamic_pointer_cast<IfcPcurve>( ifc_curve );
		if( pcurve )
		{
			// TODO: implement
			return;
		}

		throw UnhandledRepresentationException( ifc_curve );
	}


	void convertIfcPolyline( const shared_ptr<IfcPolyline>& poly_line, std::vector<vec3>& loop ) const
	{
		m_point_converter->convertIfcCartesianPointVector( poly_line->m_Points, loop );
	}

	void convertIfcLoop( const shared_ptr<IfcLoop>& loop, std::vector<vec3>& loop_points ) const
	{
		const shared_ptr<IfcPolyLoop> poly_loop = dynamic_pointer_cast<IfcPolyLoop>( loop );
		if( poly_loop )
		{
			const std::vector<shared_ptr<IfcCartesianPoint> >&    ifc_points = poly_loop->m_Polygon;
			m_point_converter->convertIfcCartesianPointVectorSkipDuplicates( ifc_points, loop_points );

			// if first and last point have same coordinates, remove last point
			while( loop_points.size() > 2 )
			{
				carve::geom3d::Vector& first = loop_points.front();
				carve::geom3d::Vector& last = loop_points.back();

				if( std::abs( first.x - last.x ) < 0.00000001 )
				{
					if( std::abs( first.y - last.y ) < 0.00000001 )
					{
						if( std::abs( first.z - last.z ) < 0.00000001 )
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
			for( size_t i_edge = 0; i_edge < edge_list.size(); ++i_edge )
			{
				shared_ptr<IfcOrientedEdge> oriented_edge = edge_list[i_edge];
				shared_ptr<IfcEdge> edge = oriented_edge->m_EdgeElement;

				shared_ptr<IfcVertex> edge_start = edge->m_EdgeStart;
				shared_ptr<IfcVertexPoint> edge_start_point = dynamic_pointer_cast<IfcVertexPoint>( edge_start );
				if( edge_start_point )
				{
					if( edge_start_point->m_VertexGeometry )
					{
						shared_ptr<IfcPoint> edge_start_point_geometry = edge_start_point->m_VertexGeometry;
						shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>( edge_start_point_geometry );
						if( !ifc_point )
						{
							// TODO: could be also  IfcPointOnCurve, IfcPointOnSurface
							continue;
						}

						// TODO: implement

					}
				}
				shared_ptr<IfcVertex> edge_end = edge->m_EdgeEnd;
				shared_ptr<IfcVertexPoint> edge_end_point = dynamic_pointer_cast<IfcVertexPoint>( edge_end );
				if( edge_end_point )
				{
					if( edge_end_point->m_VertexGeometry )
					{
						shared_ptr<IfcPoint> edge_point_geometry = edge_end_point->m_VertexGeometry;
						shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>( edge_point_geometry );

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
};
