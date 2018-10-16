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

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge2d.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <GC_MakeEllipse.hxx>
#include <gp_Circ.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_TWire.hxx>

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

#include "SplineConverterOCC.h"
#include "GeomUtilsOCC.h"
#include "GeomDebugDumpOCC.h"
#include "PlacementConverterOCC.h"
#include "PointConverterOCC.h"
#include "SplineConverterOCC.h"

//\brief class to convert different types of IFC curve representations into OCC input geometry
class IFCQUERY_EXPORT CurveConverterOCC : public StatusCallback
{
public:	
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;
	shared_ptr<SplineConverterOCC>		m_spline_converter;
	
	CurveConverterOCC( shared_ptr<GeometrySettings>& gs, shared_ptr<UnitConverter>& uc, shared_ptr<SplineConverterOCC>& sc )
		: m_geom_settings( gs ), m_unit_converter( uc ), m_spline_converter( sc )
	{
	}

	virtual ~CurveConverterOCC(){}

	void convertIfcCurve( const shared_ptr<IfcCurve>& ifc_curve, TopoDS_Wire& target_wire ) const
	{
		std::vector<shared_ptr<IfcTrimmingSelect> > trim1_vec;
		std::vector<shared_ptr<IfcTrimmingSelect> > trim2_vec;
		convertIfcCurve( ifc_curve, target_wire, trim1_vec, trim2_vec, true );
	}

	void convertIfcCurve( const shared_ptr<IfcCurve>& ifc_curve, TopoDS_Wire& target_wire, std::vector<shared_ptr<IfcTrimmingSelect> >& trim1_vec, std::vector<shared_ptr<IfcTrimmingSelect> >& trim2_vec, bool sense_agreement ) const
	{
		double length_factor = m_unit_converter->getLengthInMeterFactor();

		//	ENTITY IfcCurve ABSTRACT SUPERTYPE OF	(ONEOF(IfcBoundedCurve, IfcConic, IfcLine, IfcOffsetCurve2D, IfcOffsetCurve3D, IfcPCurve))
		shared_ptr<IfcBoundedCurve> bounded_curve = dynamic_pointer_cast<IfcBoundedCurve>(ifc_curve);
		if( bounded_curve )
		{
			shared_ptr<IfcCompositeCurve> composite_curve = dynamic_pointer_cast<IfcCompositeCurve>(bounded_curve);
			if( composite_curve )
			{
				// ENTITY IfcBoundedCurve ABSTRACT SUPERTYPE OF	(ONEOF(IfcCompositeCurve, IfcPolyline, IfcTrimmedCurve, IfcBSplineCurve))
				std::vector<shared_ptr<IfcCompositeCurveSegment> >& vec_segements = composite_curve->m_Segments;
				for( size_t i_segments = 0; i_segments < vec_segements.size(); ++i_segments )
				{
					shared_ptr<IfcCompositeCurveSegment> segement = vec_segements[i_segments];
					shared_ptr<IfcCurve> segement_curve = segement->m_ParentCurve;

					TopoDS_Wire segment_wire;
					convertIfcCurve( segement_curve, segment_wire );
					if( !segment_wire.IsNull() )
					{
						GeomUtilsOCC::appendWireToWire( segment_wire, target_wire );

#ifdef _DEBUG
						//GeomDebugDumpOCC::dumpShape(segment_wire, vec4(0.8, 0.9, 0.9, 1.0), true, false);
						//GeomDebugDumpOCC::dumpShape(segment_wire, vec4(0.8, 0.9, 0.9, 1.0), true, true);
						//GeomDebugDumpOCC::dumpShape(target_wire, vec4(0.8, 0.9, 0.9, 1.0), true, false);
#endif
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
					PointConverterOCC::convertIfcCartesianPointVector( points, target_wire, length_factor );
				}
				return;
			}

			shared_ptr<IfcTrimmedCurve> trimmed_curve = dynamic_pointer_cast<IfcTrimmedCurve>(bounded_curve);
			if( trimmed_curve )
			{
				shared_ptr<IfcCurve> basis_curve = trimmed_curve->m_BasisCurve;
				TopoDS_Wire basis_curve_wire;
				std::vector<shared_ptr<IfcTrimmingSelect> >& curve_trim1_vec = trimmed_curve->m_Trim1;
				std::vector<shared_ptr<IfcTrimmingSelect> >& curve_trim2_vec = trimmed_curve->m_Trim2;
				bool trimmed_sense_agreement = true;
				if( trimmed_curve->m_SenseAgreement )
				{
					trimmed_sense_agreement = trimmed_curve->m_SenseAgreement->m_value;
				}

				convertIfcCurve( basis_curve, basis_curve_wire, curve_trim1_vec, curve_trim2_vec, trimmed_sense_agreement );
				GeomUtilsOCC::appendWireToWire( basis_curve_wire, target_wire );

#ifdef _DEBUG
				//GeomDebugDumpOCC::dumpShape(basis_curve_wire, vec4(0.8, 0.9, 0.9, 1.0), true, true);
				//GeomDebugDumpOCC::dumpShape(target_wire, vec4(0.8, 0.9, 0.9, 1.0), true, false);
#endif
				return;
			}

			shared_ptr<IfcBSplineCurve> bspline_curve = dynamic_pointer_cast<IfcBSplineCurve>(bounded_curve);
			if( bspline_curve )
			{
				m_spline_converter->convertBSplineCurve( bspline_curve, target_wire, length_factor );
				return;
			}
			throw UnhandledRepresentationException( bounded_curve );
		}

		shared_ptr<IfcConic> conic = dynamic_pointer_cast<IfcConic>(ifc_curve);
		if( conic )
		{
			gp_Trsf conic_position_matrix;
			shared_ptr<IfcAxis2Placement> conic_placement_select = conic->m_Position;
			if( conic_placement_select )
			{
				// TYPE IfcAxis2Placement = SELECT	(IfcAxis2Placement2D	,IfcAxis2Placement3D);
				shared_ptr<IfcAxis2Placement2D> axis2placement2d = dynamic_pointer_cast<IfcAxis2Placement2D>(conic_placement_select);
				if( axis2placement2d )
				{
					PlacementConverterOCC::convertIfcAxis2Placement2D( axis2placement2d, length_factor, conic_position_matrix );
				}
				else if( dynamic_pointer_cast<IfcAxis2Placement3D>(conic_placement_select) )
				{
					shared_ptr<IfcAxis2Placement3D> axis2placement3d = dynamic_pointer_cast<IfcAxis2Placement3D>(conic_placement_select);
					PlacementConverterOCC::convertIfcAxis2Placement3D( axis2placement3d, length_factor, conic_position_matrix );
				}
			}

			double circle_center_x = conic_position_matrix.TranslationPart().X();
			double circle_center_y = conic_position_matrix.TranslationPart().Y();
			gp_Pnt circle_center(circle_center_x, circle_center_y, 0);
			conic_position_matrix.SetTranslation(gp_Vec(0, 0, 0));

			double plane_angle_factor = m_unit_converter->getAngleInRadiantFactor();
			UnitConverter::AngularUnit angular_unit = m_unit_converter->getAngularUnit();
			std::vector<shared_ptr<IfcParameterValue> > trim_params;
			std::vector<gp_Pnt> trim_points;

			// check for trimming begin
			shared_ptr<IfcParameterValue> trim_par1;
			if( trim1_vec.size() > 0 )
			{
				if( GeomUtilsOCC::findFirstInVector( trim1_vec, trim_par1 ) )
				{
					trim_params.push_back( trim_par1 );
				}
				else
				{
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if( GeomUtilsOCC::findFirstInVector( trim1_vec, ifc_trim_point ) )
					{
						gp_Vec trim_point;
						PointConverterOCC::convertIfcCartesianPoint( ifc_trim_point, trim_point, length_factor );
						trim_points.push_back( gp_Pnt( trim_point.X(), trim_point.Y(), trim_point.Z() ) );
					}
				}
			}

			if( trim2_vec.size() > 0 )
			{
				// check for trimming end
				shared_ptr<IfcParameterValue> trim_par2;
				if( GeomUtilsOCC::findFirstInVector( trim2_vec, trim_par2 ) )
				{
					trim_params.push_back( trim_par2 );
				}
				else
				{
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if( GeomUtilsOCC::findFirstInVector( trim2_vec, ifc_trim_point ) )
					{
						gp_Vec trim_point;
						PointConverterOCC::convertIfcCartesianPoint( ifc_trim_point, trim_point, length_factor );
						trim_points.push_back( gp_Pnt( trim_point.X(), trim_point.Y(), trim_point.Z() ) );
					}
				}
			}

			bool trim = false;
			double trim_angle1 = 0.0;
			double trim_angle2 = M_PI*2.0;

			// check for trimming begin
			if( trim1_vec.size() > 0 )
			{
				if( GeomUtilsOCC::findFirstInVector(trim1_vec, trim_par1) )
				{
					if( angular_unit == UnitConverter::UNDEFINED )
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
					if( GeomUtilsOCC::findFirstInVector(trim1_vec, trim_point1) )
					{
						// get direction of trim_point to circle_center, then compute angle. This is more robust in case the trim_point is not exactly on the circle
						gp_Vec trim_point_vec;
						PointConverterOCC::convertIfcCartesianPoint(trim_point1, trim_point_vec, length_factor );
						gp_Pnt trim_point(trim_point_vec.X(), trim_point_vec.Y(), trim_point_vec.Z() );
						trim_angle1 = PointConverterOCC::getAngleOnCircle(circle_center, trim_point);
					}
				}
			}

			if( trim2_vec.size() > 0 )
			{
				trim = true;
				// check for trimming end
				shared_ptr<IfcParameterValue> trim_par2;
				if( GeomUtilsOCC::findFirstInVector(trim2_vec, trim_par2) )
				{
					if( angular_unit == UnitConverter::UNDEFINED )
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
					if( GeomUtilsOCC::findFirstInVector(trim2_vec, ifc_trim_point) )
					{
						// get direction of trim_point to circle_center, then compute angle. This is more robust in case the trim_point is not exactly on the circle
						gp_Vec trim_point_vec;
						PointConverterOCC::convertIfcCartesianPoint(ifc_trim_point, trim_point_vec, length_factor);
						gp_Pnt trim_point(trim_point_vec.X(), trim_point_vec.Y(), trim_point_vec.Z());
						trim_angle2 = PointConverterOCC::getAngleOnCircle(circle_center, trim_point);
					}
				}
			}

			double start_angle = trim_angle1;
			double opening_angle = trim_angle2 - trim_angle1;

			if( !sense_agreement )
			{
				std::swap(trim_angle1, trim_angle2);
			}
			
			// ENTITY IfcConic ABSTRACT SUPERTYPE OF(ONEOF(IfcCircle, IfcEllipse))
			shared_ptr<IfcCircle> ifc_circle = dynamic_pointer_cast<IfcCircle>(conic);
			if( ifc_circle )
			{
				double circle_radius = 0.0;
				if( ifc_circle->m_Radius )
				{
					circle_radius = ifc_circle->m_Radius->m_value*length_factor;
				}

				gp_Circ circle( gp_Ax2( circle_center, gp_Dir(0, 0, 1)), circle_radius );
				double dx = cos(trim_angle1)*circle_radius;
				double dy = sin(trim_angle1)*circle_radius;
				gp_Pnt trimPoint1(circle_center_x +dx, circle_center_y + dy, 0);
				dx = cos(trim_angle2)*circle_radius;
				dy = sin(trim_angle2)*circle_radius;
				gp_Pnt trimPoint2(circle_center_x +dx, circle_center_y + dy, 0);

				try
				{
					if( trim )
					{
						TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(circle, trimPoint1, trimPoint2);
						GeomUtilsOCC::applyMatrixToShape(edge, conic_position_matrix);
						GeomUtilsOCC::appendEdgeToWire(edge, target_wire);
					}
					else
					{
						TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(circle);
						GeomUtilsOCC::applyMatrixToShape(edge, conic_position_matrix);
						GeomUtilsOCC::appendEdgeToWire(edge, target_wire);
					}

#ifdef _DEBUG
					//GeomDebugDumpOCC::dumpShape(target_wire, vec4(0.5, 0.5, 0.4, 1.0), true, false);
#endif
				}
				catch( Standard_Failure& sf )
				{
					std::cout << sf.GetMessageString() << std::endl;
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
						double x_radius = ellipse->m_SemiAxis1->m_value*length_factor;
						double y_radius = ellipse->m_SemiAxis2->m_value*length_factor;

						try
						{
							gp_Dir dir(0, 0, 1);
							if( !sense_agreement )
							{
								dir = gp_Dir(0, 0, -1);
							}

							gp_Ax2 ellipse_axis(gp_Pnt(0, 0, 0), dir);
							GC_MakeEllipse mk_ellipse(ellipse_axis, x_radius, y_radius);

							BRepBuilderAPI_MakeEdge mk_edge;
							if( trim_points.size() > 1 )
							{
								mk_edge.Init(mk_ellipse.Value(), trim_points[0], trim_points[1]);
							}
							else if( trim_params.size() > 1 )
							{
								mk_edge.Init(mk_ellipse.Value(), trim_params[0]->m_value, trim_params[1]->m_value);
							}
							TopoDS_Edge edge = mk_edge.Edge();

							// TODO: implement

							GeomUtilsOCC::applyMatrixToShape(edge, conic_position_matrix);
							GeomUtilsOCC::appendEdgeToWire(edge, target_wire);
						}
						catch( Standard_Failure& sf )
						{
							std::cout << sf.GetMessageString() << std::endl;
						}
					}
				}
				return;
			}
			throw UnhandledRepresentationException( conic );
		}

		shared_ptr<IfcLine> line = dynamic_pointer_cast<IfcLine>(ifc_curve);
		if( line )
		{
			shared_ptr<IfcCartesianPoint> ifc_line_point = line->m_Pnt;
			gp_Vec line_origin;
			if( PointConverterOCC::convertIfcCartesianPoint( ifc_line_point, line_origin, length_factor ) )
			{
				// line: lambda(u) = line_point + u*line_direction
				const shared_ptr<IfcVector>& line_vec = line->m_Dir;
				if( !line_vec )
				{
					return;
				}
				const shared_ptr<IfcDirection>& ifc_line_direction = line_vec->m_Orientation;
				gp_Vec line_direction;
				PointConverterOCC::convertIfcDirection( ifc_line_direction, line_direction );

				double line_magnitude_value = 0;
				if( line_vec->m_Magnitude )
				{
					line_magnitude_value = line_vec->m_Magnitude->m_value*length_factor;
				}

				// check for trimming at beginning of line
				double start_parameter = 0.0;
				shared_ptr<IfcParameterValue> trim_par1;
				if( GeomUtilsOCC::findFirstInVector( trim1_vec, trim_par1 ) )
				{
					start_parameter = trim_par1->m_value;
					line_origin = line_origin + line_direction*start_parameter;
				}
				else
				{
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if( GeomUtilsOCC::findFirstInVector( trim1_vec, ifc_trim_point ) )
					{
						gp_Vec trim_point;
						PointConverterOCC::convertIfcCartesianPoint( ifc_trim_point, trim_point, length_factor );

						gp_Vec closest_point_on_line;
						GeomUtilsOCC::closestPointOnLine( trim_point, line_origin, line_direction, closest_point_on_line );

						if( (closest_point_on_line - trim_point).Magnitude() < 0.0001 )
						{
							// trimming point is on the line
							line_origin = trim_point;
						}
					}
				}
				// check for trimming at end of line
				gp_Vec line_end;
				shared_ptr<IfcParameterValue> trim_par2;
				if( GeomUtilsOCC::findFirstInVector( trim2_vec, trim_par2 ) )
				{
					line_magnitude_value = trim_par2->m_value*length_factor;
					line_end = line_origin + line_direction*line_magnitude_value;
				}
				else
				{
					shared_ptr<IfcCartesianPoint> ifc_trim_point;
					if( GeomUtilsOCC::findFirstInVector( trim2_vec, ifc_trim_point ) )
					{
						gp_Vec trim_point;
						PointConverterOCC::convertIfcCartesianPoint( ifc_trim_point, trim_point, length_factor );

						gp_Vec closest_point_on_line;
						GeomUtilsOCC::closestPointOnLine( trim_point, line_origin, line_direction, closest_point_on_line );

						if( (closest_point_on_line - trim_point).Magnitude() < 0.0001 )
						{
							// trimming point is on the line
							line_end = trim_point;
						}
					}
				}

				TopoDS_Edge edge = BRepBuilderAPI_MakeEdge( gp_Pnt( line_origin.X(), line_origin.Y(), line_origin.Z() ), gp_Pnt( line_end.X(), line_end.Y(), line_end.Z() ) );
				GeomUtilsOCC::appendEdgeToWire( edge, target_wire );
			}
			return;
		}

		shared_ptr<IfcOffsetCurve2D> offset_curve_2d = dynamic_pointer_cast<IfcOffsetCurve2D>(ifc_curve);
		if( offset_curve_2d )
		{
			// TODO: implement
			std::cout << "IfcOffsetCurve2D not implemented" << std::endl;
			return;
		}

		shared_ptr<IfcOffsetCurve3D> offset_curve_3d = dynamic_pointer_cast<IfcOffsetCurve3D>(ifc_curve);
		if( offset_curve_3d )
		{
			// TODO: implement
			std::cout << "IfcOffsetCurve3D not implemented" << std::endl;
			return;
		}

		shared_ptr<IfcPcurve> pcurve = dynamic_pointer_cast<IfcPcurve>(ifc_curve);
		if( pcurve )
		{
			// TODO: implement
			std::cout << "IfcPcurve not implemented" << std::endl;
			return;
		}

		throw UnhandledRepresentationException( ifc_curve );
	}

	void convertIfcPolyline( const shared_ptr<IfcPolyline>& poly_line, std::vector<gp_Vec>& loop ) const
	{
		double length_factor = m_unit_converter->getLengthInMeterFactor();
		PointConverterOCC::convertIfcCartesianPointVector( poly_line->m_Points, loop, length_factor );
	}

	void convertIfcLoop( const shared_ptr<IfcLoop>& loop, TopoDS_Wire& target_wire ) const
	{
		const shared_ptr<IfcPolyLoop> poly_loop = dynamic_pointer_cast<IfcPolyLoop>(loop);
		if( poly_loop )
		{
			const std::vector<shared_ptr<IfcCartesianPoint> >&    ifc_points = poly_loop->m_Polygon;
			double length_factor = m_unit_converter->getLengthInMeterFactor();
			PointConverterOCC::convertIfcCartesianPointVectorSkipDuplicates( ifc_points, target_wire, true, length_factor );
			GeomUtilsOCC::closeWire( target_wire );

#ifdef _DEBUG
			if( !target_wire.Closed() )
			{
				GeomDebugDumpOCC::dumpShape( target_wire, vec4( 0.5, 0.5, 0.5, 1.0 ), true, true );
			}
#endif
			return;
		}

		shared_ptr<IfcEdgeLoop> edge_loop = dynamic_pointer_cast<IfcEdgeLoop>(loop);
		if( edge_loop )
		{
			std::cout << "IfcEdgeLoop not implemented" << std::endl;

			const std::vector<shared_ptr<IfcOrientedEdge> >& edge_list = edge_loop->m_EdgeList;
			for( size_t i_edge = 0; i_edge < edge_list.size(); ++i_edge )
			{
				const shared_ptr<IfcOrientedEdge>& oriented_edge = edge_list[i_edge];
				const shared_ptr<IfcEdge>& edge = oriented_edge->m_EdgeElement;

				const shared_ptr<IfcVertex>& edge_start = edge->m_EdgeStart;
				const shared_ptr<IfcVertexPoint>& edge_start_point = dynamic_pointer_cast<IfcVertexPoint>(edge_start);
				if( edge_start_point )
				{
					if( edge_start_point->m_VertexGeometry )
					{
						const shared_ptr<IfcPoint>& edge_start_point_geometry = edge_start_point->m_VertexGeometry;
						shared_ptr<IfcCartesianPoint> ifc_point = dynamic_pointer_cast<IfcCartesianPoint>(edge_start_point_geometry);
						if( !ifc_point )
						{
							// TODO: could be also  IfcPointOnCurve, IfcPointOnSurface
							continue;
						}

						// TODO: implement
					}
				}
				const shared_ptr<IfcVertex>& edge_end = edge->m_EdgeEnd;
				shared_ptr<IfcVertexPoint> edge_end_point = dynamic_pointer_cast<IfcVertexPoint>(edge_end);
				if( edge_end_point )
				{
					if( edge_end_point->m_VertexGeometry )
					{
						const shared_ptr<IfcPoint>& edge_point_geometry = edge_end_point->m_VertexGeometry;
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
};
