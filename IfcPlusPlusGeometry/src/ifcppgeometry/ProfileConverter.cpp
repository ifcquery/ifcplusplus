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

#include <ifcpp/IFC4/include/IfcProfileDef.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcParameterizedProfileDef.h>
#include <ifcpp/IFC4/include/IfcArbitraryOpenProfileDef.h>
#include <ifcpp/IFC4/include/IfcArbitraryClosedProfileDef.h>
#include <ifcpp/IFC4/include/IfcCompositeProfileDef.h>
#include <ifcpp/IFC4/include/IfcDerivedProfileDef.h>
#include <ifcpp/IFC4/include/IfcCurve.h>
#include <ifcpp/IFC4/include/IfcArbitraryProfileDefWithVoids.h>
#include <ifcpp/IFC4/include/IfcBoundedCurve.h>
#include <ifcpp/IFC4/include/IfcCartesianTransformationOperator2D.h>
#include <ifcpp/IFC4/include/IfcRectangleProfileDef.h>
#include <ifcpp/IFC4/include/IfcCircleProfileDef.h>
#include <ifcpp/IFC4/include/IfcCircleHollowProfileDef.h>
#include <ifcpp/IFC4/include/IfcTrapeziumProfileDef.h>
#include <ifcpp/IFC4/include/IfcCenterLineProfileDef.h>
#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcRectangleHollowProfileDef.h>
#include <ifcpp/IFC4/include/IfcRoundedRectangleProfileDef.h>
#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcNonNegativeLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcEllipseProfileDef.h>
#include <ifcpp/IFC4/include/IfcIShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcAsymmetricIShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcLShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcUShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcCShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcZShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcPlaneAngleMeasure.h>
#include <ifcpp/IFC4/include/IfcTShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement2D.h>
#include <ifcpp/IFC4/include/IfcRationalBSplineSurfaceWithKnots.h>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/IfcPPException.h>

#include "GeometrySettings.h"
#include "GeomUtils.h"
#include "RepresentationConverter.h"
#include "PlacementConverter.h"
#include "CurveConverter.h"
#include "ProfileConverter.h"


ProfileConverter::ProfileConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> unit_converter )
	: m_geom_settings(geom_settings), m_unit_converter(unit_converter)
{
}

ProfileConverter::~ProfileConverter()
{
}

void removeDuplicates( std::vector<std::vector<carve::geom::vector<2> > >&	paths )
{
	for( std::vector<std::vector<carve::geom::vector<2> > >::iterator it = paths.begin(); it != paths.end(); ++it )
	{
		std::vector<carve::geom::vector<2> >& loop = (*it);
		if( loop.size() > 1 )
		{
			std::vector<carve::geom::vector<2> >::iterator it_loop = loop.begin();
			carve::geom::vector<2> previous_point = (*it_loop);
			++it_loop;
			for( ; it_loop != loop.end(); ++it_loop )
			{
				carve::geom::vector<2>& current_point = (*it_loop);
				if( std::abs(current_point.x - previous_point.x ) < 0.00001 )
				{
					if( std::abs(current_point.y - previous_point.y ) < 0.00001 )
					{
						it_loop = loop.erase( it_loop );
						previous_point = (*it_loop);
						continue;
					}
				}
				previous_point = current_point;
			}
		}
	}
}

void ProfileConverter::computeProfile( shared_ptr<IfcProfileDef> profile_def )
{
	// ENTITY IfcProfileDef SUPERTYPE OF(ONEOF(IfcArbitraryClosedProfileDef, IfcArbitraryOpenProfileDef, IfcCompositeProfileDef,
	//IfcDerivedProfileDef, IfcParameterizedProfileDef));
	shared_ptr<IfcArbitraryClosedProfileDef> arbitrary_closed = dynamic_pointer_cast<IfcArbitraryClosedProfileDef>(profile_def);
	if( arbitrary_closed )
	{
		convertIfcArbitraryClosedProfileDef( arbitrary_closed, m_paths );
		removeDuplicates( m_paths );
		return;
	}

	shared_ptr<IfcArbitraryOpenProfileDef> arbitrary_open = dynamic_pointer_cast<IfcArbitraryOpenProfileDef>(profile_def);
	if( arbitrary_open )
	{
		convertIfcArbitraryOpenProfileDef( arbitrary_open, m_paths );
		removeDuplicates( m_paths );
		return;
	}

	shared_ptr<IfcCompositeProfileDef> composite = dynamic_pointer_cast<IfcCompositeProfileDef>(profile_def);
	if( composite )
	{
		convertIfcCompositeProfileDef( composite, m_paths );
		removeDuplicates( m_paths );
		return;
	}

	shared_ptr<IfcDerivedProfileDef> derived = dynamic_pointer_cast<IfcDerivedProfileDef>(profile_def);
	if( derived )
	{
		convertIfcDerivedProfileDef( derived, m_paths );
		removeDuplicates( m_paths );
		return;
	}

	shared_ptr<IfcParameterizedProfileDef> parameterized = dynamic_pointer_cast<IfcParameterizedProfileDef>(profile_def);
	if( parameterized )
	{
		convertIfcParameterizedProfileDefWithPosition( parameterized, m_paths );
		removeDuplicates( m_paths );
		return;
	}

	//shared_ptr<IfcNurbsProfile> nurbs = dynamic_pointer_cast<IfcNurbsProfile>(profile_def);
	//if( nurbs )
	//{
	//	convertIfcNurbsProfile( nurbs, m_paths );
	//	return;
	//}
	
	std::stringstream sstr;
	sstr << "ProfileDef not supported: " << profile_def->classname();
	throw IfcPPException( sstr.str(), __func__ );
}

void ProfileConverter::addAvoidingDuplicates( const std::vector<carve::geom::vector<2> >& polygon, std::vector<std::vector<carve::geom::vector<2> > >& paths )
{
	if( polygon.size() < 1 )
	{
		return;
	}

	std::vector<carve::geom::vector<2> > polygon_add;
	polygon_add.push_back( polygon[0] );
	for( int i=1; i<polygon.size(); ++i )
	{
		const carve::geom::vector<2> & point = polygon[i];
		const carve::geom::vector<2> & point_previous = polygon[i-1];
		
		// omit duplicate points
		if( std::abs(point.x - point_previous.x) > 0.00001 )
		{
			polygon_add.push_back( point );
			continue;
		}
		
		if( std::abs(point.y - point_previous.y) > 0.00001 )
		{
			polygon_add.push_back( point );
			continue;
		}

		//if( std::abs(point.z - point_previous.z) > 0.00001 )
		//{
		//	polygon_add.push_back( point );
		//	continue;
		//}
	}
	paths.push_back(polygon_add);
}

void ProfileConverter::convertIfcArbitraryClosedProfileDef( const shared_ptr<IfcArbitraryClosedProfileDef>& profile,	std::vector<std::vector<carve::geom::vector<2> > >& paths ) const
{
	shared_ptr<IfcCurve> outer_curve = profile->m_OuterCurve;
	std::vector<carve::geom::vector<2> > curve_polygon;
	std::vector<carve::geom::vector<2> > segment_start_points;

	CurveConverter c_conv( m_geom_settings, m_unit_converter );
	c_conv.convertIfcCurve2D( outer_curve, curve_polygon, segment_start_points );

	deleteLastPointIfEqualToFirst( curve_polygon );
	addAvoidingDuplicates( curve_polygon, paths );

	// IfcArbitraryProfileDefWithVoids
	shared_ptr<IfcArbitraryProfileDefWithVoids> profile_with_voids = dynamic_pointer_cast<IfcArbitraryProfileDefWithVoids>(profile);
	if( profile_with_voids )
	{
		std::vector<shared_ptr<IfcCurve> > inner_curves = profile_with_voids->m_InnerCurves;
		for( int i = 0; i < inner_curves.size(); ++i ) 
		{
			shared_ptr<IfcCurve> inner_ifc_curve = inner_curves[i];
			std::vector<carve::geom::vector<2> > inner_curve_polygon;
			std::vector<carve::geom::vector<2> > segment_start_points;

			c_conv.convertIfcCurve2D( inner_ifc_curve, inner_curve_polygon, segment_start_points );
			deleteLastPointIfEqualToFirst( inner_curve_polygon );
			addAvoidingDuplicates( inner_curve_polygon, paths );
		}
	}
}
	
void ProfileConverter::convertIfcArbitraryOpenProfileDef( const shared_ptr<IfcArbitraryOpenProfileDef>& profile,	std::vector<std::vector<carve::geom::vector<2> > >& paths ) const
{
	// ENTITY IfcArbitraryOpenProfileDef
	//	SUPERTYPE OF(IfcCenterLineProfileDef)
	//	SUBTYPE OF IfcProfileDef;
	//	Curve	 :	IfcBoundedCurve;

	shared_ptr<IfcCurve> ifc_curve = profile->m_Curve;
	CurveConverter c_converter( m_geom_settings, m_unit_converter );

	//TODO IfcCenterLineProfileDef
	shared_ptr<IfcCenterLineProfileDef> center_line_profile_def = dynamic_pointer_cast<IfcCenterLineProfileDef>(profile);
	if( center_line_profile_def )
	{
		if( center_line_profile_def->m_Thickness )
		{
			const double thickness = center_line_profile_def->m_Thickness->m_value * m_unit_converter->getLengthInMeterFactor();
			std::vector<carve::geom::vector<3> > segment_start_points;
			std::vector<carve::geom::vector<3> > basis_curve_points;
			c_converter.convertIfcCurve( ifc_curve, basis_curve_points, segment_start_points );

			int num_base_points = basis_curve_points.size();
			if( num_base_points < 2 )
			{
				std::cout << "IfcCenterLineProfileDef: num curve points < 2";
				return;
			}

			carve::math::Matrix matrix_sweep;
			carve::geom::vector<3> local_z( carve::geom::VECTOR( 0, 0, 1 ) );
			std::vector<carve::geom::vector<3> > left_points;
			std::vector<carve::geom::vector<3> > right_points;
			carve::geom::vector<3> point_left( carve::geom::VECTOR(0.0,-thickness*0.5, 0.0 ) );
			carve::geom::vector<3> point_right( carve::geom::VECTOR(0.0, thickness*0.5, 0.0 ) );

			for( int ii=0; ii<num_base_points; ++ii )
			{
				carve::geom::vector<3> vertex_current = basis_curve_points[ii];
				carve::geom::vector<3> vertex_next;
				carve::geom::vector<3> vertex_before;
				if( ii == 0 )
				{
					// first point
					vertex_next	= basis_curve_points[ii+1];
					carve::geom::vector<3> delta_element = vertex_next - vertex_current;
					vertex_before = vertex_current - (delta_element);
				}
				else if( ii == num_base_points-1 )
				{
					// last point
					vertex_before	= basis_curve_points[ii-1];
					carve::geom::vector<3> delta_element = vertex_current - vertex_before;
					vertex_next = vertex_before + (delta_element);
				}
				else
				{
					// inner point
					vertex_next		= basis_curve_points[ii+1];
					vertex_before	= basis_curve_points[ii-1];
				}

				carve::geom::vector<3> bisecting_normal;
				GeomUtils::bisectingPlane( vertex_before, vertex_current, vertex_next, bisecting_normal );

				if( ii == num_base_points -1 )
				{
					bisecting_normal *= -1.0;
				}

				local_z.x = 0;
				local_z.y = 0;
				local_z.z = -1;
				GeomUtils::convertPlane2Matrix( bisecting_normal, vertex_current, local_z, matrix_sweep );

				left_points.push_back(  matrix_sweep*point_left );
				right_points.push_back(  matrix_sweep*point_right );
			}

			std::reverse( right_points.begin(), right_points.end() );
			std::vector<carve::geom::vector<2> > polygon;
			for( int i2=0; i2<left_points.size(); ++i2 )
			{
				carve::geom::vector<3>& point3d = left_points[i2];
				polygon.push_back( carve::geom::VECTOR( point3d.x, point3d.y ) );
			}
			for( int i2=0; i2<right_points.size(); ++i2 )
			{
				carve::geom::vector<3>& point3d = right_points[i2];
				polygon.push_back( carve::geom::VECTOR( point3d.x, point3d.y ) );
			}
			addAvoidingDuplicates( polygon, paths );
		}
	}
	else
	{
		std::vector<carve::geom::vector<2> > polygon;
		std::vector<carve::geom::vector<2> > segment_start_points;
		c_converter.convertIfcCurve2D( ifc_curve, polygon, segment_start_points );
		addAvoidingDuplicates( polygon, paths );
	}
}
	
void ProfileConverter::convertIfcCompositeProfileDef( const shared_ptr<IfcCompositeProfileDef>& composite_profile, std::vector<std::vector<carve::geom::vector<2> > >& paths ) const
{
	std::vector<int> temploop_counts;
	std::vector<int> tempcontour_counts;
	
	std::vector<shared_ptr<IfcProfileDef> >& profiles = composite_profile->m_Profiles;
	std::vector<shared_ptr<IfcProfileDef> >::iterator it;
	
	for( it=profiles.begin(); it!=profiles.end(); ++it )
	{
		shared_ptr<IfcProfileDef> profile_def = (*it);

		shared_ptr<IfcParameterizedProfileDef> parameterized = dynamic_pointer_cast<IfcParameterizedProfileDef>(profile_def);
		if( parameterized )
		{
			convertIfcParameterizedProfileDefWithPosition( parameterized, paths );
			continue;
		}

		shared_ptr<IfcArbitraryOpenProfileDef> open = dynamic_pointer_cast<IfcArbitraryOpenProfileDef>(profile_def);
		if( open )
		{
			convertIfcArbitraryOpenProfileDef( open, paths );
			continue;
		}

		shared_ptr<IfcArbitraryClosedProfileDef> closed = dynamic_pointer_cast<IfcArbitraryClosedProfileDef>(profile_def);
		if( closed )
		{
			convertIfcArbitraryClosedProfileDef( closed, paths );
			continue;
		}

		shared_ptr<IfcCompositeProfileDef> composite = dynamic_pointer_cast<IfcCompositeProfileDef>(profile_def);
		if( composite )
		{
			convertIfcCompositeProfileDef( composite, paths );
			continue;
		}

		shared_ptr<IfcDerivedProfileDef> derived = dynamic_pointer_cast<IfcDerivedProfileDef>(profile_def);
		if( derived )
		{
			convertIfcDerivedProfileDef( derived, paths );
			continue;
		}

		std::stringstream sstr;
		sstr << "ProfileDef not supported: " << profile_def->classname();
		throw IfcPPException( sstr.str(), __func__ );
	}
}
	
void ProfileConverter::convertIfcDerivedProfileDef( const shared_ptr<IfcDerivedProfileDef>& derived_profile, std::vector<std::vector<carve::geom::vector<2> > >& paths ) const
{
	ProfileConverter temp_profiler( m_geom_settings, m_unit_converter );
	temp_profiler.computeProfile( derived_profile->m_ParentProfile );
	const std::vector<std::vector<carve::geom::vector<2> > >& parent_paths = temp_profiler.getCoordinates();

	shared_ptr<IfcCartesianTransformationOperator2D> transf_op_2D = derived_profile->m_Operator;
	
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	carve::math::Matrix transform( carve::math::Matrix::IDENT() );
	PlacementConverter::convertTransformationOperator( transf_op_2D, transform, length_factor );
	for( int i=0; i<parent_paths.size(); ++i ) 
	{
		const std::vector<carve::geom::vector<2> >& loop_parent = parent_paths[i];
		std::vector<carve::geom::vector<2> > loop;

		for( int j=0; j<loop_parent.size(); ++j ) 
		{
			const carve::geom::vector<2>& pt = loop_parent[j];
			carve::geom::vector<3> pt3d( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
			pt3d = transform*pt3d;
			loop.push_back( carve::geom::VECTOR( pt3d.x, pt3d.y ) );
		}
		paths.push_back(loop);
	}
}

void ProfileConverter::convertIfcParameterizedProfileDefWithPosition( const shared_ptr<IfcParameterizedProfileDef>& parameterized,  std::vector<std::vector<carve::geom::vector<2> > >& paths ) const
{
	std::vector<std::vector<carve::geom::vector<2> > > temp_paths;
	convertIfcParameterizedProfileDef( parameterized, temp_paths );

	// local coordinate system
	if( parameterized->m_Position )
	{
		shared_ptr<IfcAxis2Placement2D> axis2Placement2D = parameterized->m_Position;
		double length_factor = m_unit_converter->getLengthInMeterFactor();
		carve::math::Matrix transform( carve::math::Matrix::IDENT() );
		PlacementConverter::convertIfcPlacement( axis2Placement2D, transform, length_factor );

		for(int i = 0; i < temp_paths.size(); ++i )
		{
			std::vector<carve::geom::vector<2> >& path_loop = temp_paths[i];
			for(int j = 0; j < path_loop.size(); ++j )
			{
				carve::geom::vector<2>& pt = path_loop[j];
				carve::geom::vector<3> pt_3d( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
				pt_3d = transform*pt_3d;
				pt.x = pt_3d.x;
				pt.y = pt_3d.y;
			}
			paths.push_back( path_loop );
		}
	}
	else
	{
		for(int i = 0; i < temp_paths.size(); ++i )
		{
			std::vector<carve::geom::vector<2> >& path_loop = temp_paths[i];
			paths.push_back( path_loop );
		}
	}
}

void ProfileConverter::convertIfcParameterizedProfileDef( const shared_ptr<IfcParameterizedProfileDef>& profile, std::vector<std::vector<carve::geom::vector<2> > >& paths ) const
{
	//IfcParameterizedProfileDef ABSTRACT SUPERTYPE OF (ONEOF
	//	(IfcCShapeProfileDef, IfcCircleProfileDef, IfcEllipseProfileDef, IfcIShapeProfileDef, IfcLShapeProfileDef,
	//	IfcRectangleProfileDef, IfcTShapeProfileDef, IfcTrapeziumProfileDef, IfcUShapeProfileDef, IfcZShapeProfileDef))
	
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	double angle_factor = m_unit_converter->getAngleInRadianFactor();
	std::vector<carve::geom::vector<2> > outer_loop;

	shared_ptr<IfcRectangleProfileDef> rectangle_profile = dynamic_pointer_cast<IfcRectangleProfileDef>(profile);
	if( rectangle_profile )
	{
		if( rectangle_profile->m_XDim && rectangle_profile->m_YDim )
		{
			double x = rectangle_profile->m_XDim->m_value*length_factor;
			double y = rectangle_profile->m_YDim->m_value*length_factor;
					
			shared_ptr<IfcRectangleHollowProfileDef> hollow = dynamic_pointer_cast<IfcRectangleHollowProfileDef>(rectangle_profile);
			if( hollow )
			{
				if( hollow->m_WallThickness )
				{
					double t = hollow->m_WallThickness->m_value*length_factor;
					double r1 = 0;
					if(hollow->m_OuterFilletRadius)
					{
						r1 = hollow->m_InnerFilletRadius->m_value*length_factor;
					}

					double r2 = 0;
					if(hollow->m_InnerFilletRadius)
					{
						r2 = hollow->m_InnerFilletRadius->m_value*length_factor;	
					}
					
					// Outer
					if(r1 != 0)
					{
						addArc( outer_loop, r1, 0, 			M_PI_2,  x*0.5-r1,  y*0.5-r1);
						addArc( outer_loop, r1, M_PI_2, 	M_PI_2, -x*0.5+r1,  y*0.5-r1);
						addArc( outer_loop, r1, M_PI,		M_PI_2, -x*0.5+r1, -y*0.5+r1);
						addArc( outer_loop, r1, 3*M_PI_2,	M_PI_2,  x*0.5-r1, -y*0.5+r1);
					}
					else
					{
						 outer_loop.push_back( carve::geom::VECTOR(-x*0.5,-y*0.5 ) );
						 outer_loop.push_back( carve::geom::VECTOR( x*0.5,-y*0.5 ) );
						 outer_loop.push_back( carve::geom::VECTOR( x*0.5, y*0.5 ) );
						 outer_loop.push_back( carve::geom::VECTOR(-x*0.5, y*0.5 ) );
					}

					// Inner
					std::vector<carve::geom::vector<2> > inner_loop;
					x -= 2*t;
					y -= 2*t;
					if(r2 != 0)
					{
						addArc( inner_loop, r2, 0, 			M_PI_2,  x*0.5-r2,  y*0.5-r2);
						addArc( inner_loop, r2, M_PI_2, 	M_PI_2, -x*0.5+r2,  y*0.5-r2);
						addArc( inner_loop, r2, M_PI, 		M_PI_2, -x*0.5+r2, -y*0.5+r2);
						addArc( inner_loop, r2, 3*M_PI_2,	M_PI_2,  x*0.5-r2, -y*0.5+r2);
					}
					else
					{
						inner_loop.push_back( carve::geom::VECTOR(-x*0.5,-y*0.5 ));
						inner_loop.push_back( carve::geom::VECTOR( x*0.5,-y*0.5 ));
						inner_loop.push_back( carve::geom::VECTOR( x*0.5, y*0.5 ));
						inner_loop.push_back( carve::geom::VECTOR(-x*0.5, y*0.5 ));
					}
					paths.push_back(outer_loop);
					paths.push_back(inner_loop);
				}
				return;
			}
		
			// RoundedRectangle
			shared_ptr<IfcRoundedRectangleProfileDef> rounded_rectangle = dynamic_pointer_cast<IfcRoundedRectangleProfileDef>(rectangle_profile);
			if( rounded_rectangle )
			{
				
				if( rounded_rectangle->m_RoundingRadius )
				{
					double rr = rounded_rectangle->m_RoundingRadius->m_value*length_factor;
					addArc( outer_loop, rr, 0, 			M_PI_2,  x*0.5-rr,  y*0.5-rr);
					addArc( outer_loop, rr, M_PI_2, 	M_PI_2, -x*0.5+rr,  y*0.5-rr);
					addArc( outer_loop, rr, M_PI, 		M_PI_2, -x*0.5+rr, -y*0.5+rr);
					addArc( outer_loop, rr, 3*M_PI_2,	M_PI_2,  x*0.5-rr, -y*0.5+rr);
					paths.push_back(outer_loop);
				}
				return;
			}


			// else it's a standard rectangle
			outer_loop.push_back( carve::geom::VECTOR(-x*0.5,-y*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR( x*0.5,-y*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR( x*0.5, y*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR(-x*0.5, y*0.5 ));
			paths.push_back(outer_loop);
			return;
		}
	}
		
	// Trapezium
	shared_ptr<IfcTrapeziumProfileDef> trapezium = dynamic_pointer_cast<IfcTrapeziumProfileDef>(profile);
	if( trapezium )
	{
		if( trapezium->m_BottomXDim && trapezium->m_TopXDim && trapezium->m_TopXOffset && trapezium->m_YDim )
		{
			double xBottom = trapezium->m_BottomXDim->m_value*length_factor;
			double xTop = trapezium->m_TopXDim->m_value*length_factor;
			double xOffset = trapezium->m_TopXOffset->m_value*length_factor;
			double y = trapezium->m_YDim->m_value*length_factor;
			outer_loop.push_back( carve::geom::VECTOR(-xBottom*0.5,					-y*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR( xBottom*0.5,					-y*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR(-xBottom*0.5+xOffset+xTop,	y*0.5 ));	
			outer_loop.push_back( carve::geom::VECTOR(-xBottom*0.5+xOffset,			y*0.5 ));
			paths.push_back(outer_loop);
		}
		return;
	}
		
	// Circle
	shared_ptr<IfcCircleProfileDef> circle_profile_def = dynamic_pointer_cast<IfcCircleProfileDef>(profile);
	if( circle_profile_def )
	{
		double radius = circle_profile_def->m_Radius->m_value*length_factor;
		if( radius < 0.000001 )
		{
			return;
		}
		int num_segments = m_geom_settings->m_num_vertices_per_circle; // TODO: adapt to model size and complexity
		double angle = 0;
		for(int i=0; i<num_segments; ++i)
		{
			outer_loop.push_back( carve::geom::VECTOR( (radius * cos(angle)),	 (radius * sin(angle)) ));
			angle += 2.0*M_PI / double(num_segments);
		}
		paths.push_back(outer_loop);
			
		// CircleHollow
		std::vector<carve::geom::vector<2> > inner_loop;
		shared_ptr<IfcCircleHollowProfileDef> hollow = dynamic_pointer_cast<IfcCircleHollowProfileDef>(profile);
		if( hollow )
		{
			angle = 0;
			radius -= hollow->m_WallThickness->m_value*length_factor;
			
			int num_segments2 = m_geom_settings->m_num_vertices_per_circle; // TODO: adapt to model size and complexity
			for(int i=0; i<num_segments2; ++i)
			{
				inner_loop.push_back( carve::geom::VECTOR( (radius * cos(angle)),  (radius * sin(angle)) ));
				angle += 2.0*M_PI / double(num_segments2);
			}
			paths.push_back(inner_loop);
		}
		return;
	}
		
	// Ellipse
	shared_ptr<IfcEllipseProfileDef> ellipse_profile_def = dynamic_pointer_cast<IfcEllipseProfileDef>(profile);
	if( ellipse_profile_def )
	{
		if( ellipse_profile_def->m_SemiAxis1 )
		{
			if( ellipse_profile_def->m_SemiAxis2 )
			{
				double xRadius = ellipse_profile_def->m_SemiAxis1->m_value*length_factor;
				double yRadius = ellipse_profile_def->m_SemiAxis2->m_value*length_factor;
				double radiusMax = std::max(xRadius, yRadius);
				int num_segments = m_geom_settings->m_num_vertices_per_circle; // TODO: adapt to model size and complexity
				double angle=0;
				for(int i = 0; i < num_segments; ++i) 
				{
					outer_loop.push_back( carve::geom::VECTOR( (xRadius * cos(angle)),  (yRadius * sin(angle)) ));
					angle += 2.0*M_PI / double(num_segments);
				}
				paths.push_back(outer_loop);
			}
		}
		return;
	}
		
	// I
	shared_ptr<IfcIShapeProfileDef> i_shape = dynamic_pointer_cast<IfcIShapeProfileDef>(profile);
	if( i_shape )
	{
		if( i_shape->m_OverallDepth && i_shape->m_OverallWidth && i_shape->m_WebThickness && i_shape->m_FlangeThickness )
		{
			double h = i_shape->m_OverallDepth->m_value*length_factor;
			double b = i_shape->m_OverallWidth->m_value*length_factor;
			double tw = i_shape->m_WebThickness->m_value*length_factor;
			double tf = i_shape->m_FlangeThickness->m_value*length_factor;
			double r = 0;
			if(i_shape->m_FilletRadius)
			{
				r = i_shape->m_FilletRadius->m_value*length_factor;
			}

			outer_loop.push_back( carve::geom::VECTOR( b*0.5,  -h*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR( b*0.5,  (-h*0.5+tf) ));

			if(r != 0)
			{
				addArc( outer_loop, r, 3*M_PI_2, -M_PI_2, tw*0.5+r, -h*0.5+tf+r);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( tw*0.5,  (-h*0.5+tf) ) );
			}
			
			shared_ptr<IfcAsymmetricIShapeProfileDef> asym_I_profile = dynamic_pointer_cast<IfcAsymmetricIShapeProfileDef>(i_shape);
			if( asym_I_profile )
			{
				if( asym_I_profile->m_TopFlangeWidth )
				{
					double bTop = asym_I_profile->m_TopFlangeWidth->m_value*length_factor;
					double tfTop = tf;

					if(asym_I_profile->m_TopFlangeThickness )
					{
						tfTop = asym_I_profile->m_TopFlangeThickness->m_value*length_factor;
					}
					double rTop = r;
					if(asym_I_profile->m_TopFlangeFilletRadius)
					{
						rTop = asym_I_profile->m_TopFlangeFilletRadius->m_value*length_factor;
					}

					if( rTop != 0 )
					{
						addArc( outer_loop, rTop, M_PI, -M_PI_2, tw*0.5+rTop, h*0.5-tfTop-rTop);
					}
					else
					{
						outer_loop.push_back( carve::geom::VECTOR( tw*0.5,  (h*0.5-tfTop) ));
					}
					outer_loop.push_back( carve::geom::VECTOR( bTop*0.5,  (h*0.5-tfTop) ));
					outer_loop.push_back( carve::geom::VECTOR( bTop*0.5,  h*0.5 ));
				}
			}
			else
			{
				// symmetric: mirror horizontally along x-Axis
				mirrorCopyPathReverse( outer_loop, false, true );
			}

			// mirror vertically along y-axis
			mirrorCopyPathReverse( outer_loop, true, false );
			paths.push_back(outer_loop);
		}
		return;
	}
		
	// L
	shared_ptr<IfcLShapeProfileDef> l_shape = dynamic_pointer_cast<IfcLShapeProfileDef>(profile);
	if( l_shape )
	{
		if( l_shape->m_Depth && l_shape->m_Thickness )
		{
			double h = l_shape->m_Depth->m_value*length_factor;
			double b = h;
		
			if(l_shape->m_Width)
			{
				b = l_shape->m_Width->m_value*length_factor;
			}
		
			double t = l_shape->m_Thickness->m_value*length_factor;
		
			double r1 = 0;
			if(l_shape->m_FilletRadius)
			{
				r1 = l_shape->m_FilletRadius->m_value*length_factor;
			}
		
			double r2 = 0;
			if(l_shape->m_EdgeRadius)
			{
				r2 = l_shape->m_EdgeRadius->m_value*length_factor;
			}
		
			double ls = 0;
			if(l_shape->m_LegSlope)
			{
				ls = l_shape->m_LegSlope->m_value*angle_factor;
			}
			
			outer_loop.push_back( carve::geom::VECTOR( -b*0.5,  -h*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR( b*0.5,  -h*0.5 ));

			if(r2 != 0)
			{
				addArc( outer_loop, r2, 0, M_PI_2-ls, b*0.5-r2, -h*0.5+t-r2);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( b*0.5,  (-h*0.5+t) ));
			}

			double s = sin(ls);
			double c = cos(ls);
			double z1 = (-s*((c-s)*(r1+r2+t)-c*b+s*h))/(2*c*c-1);
			double z2 = (-s*((c-s)*(r1+r2+t)-c*h+s*b))/(2*c*c-1);
			if(r1 != 0)
			{
				addArc( outer_loop, r1, 3*M_PI_2-ls, -M_PI_2+2*ls, -b*0.5+t+z2+r1, -h*0.5+t+z1+r1);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( (-b*0.5+t+z2),  (-h*0.5+t+z1) ));
			}

			if(r2 != 0)
			{
				addArc( outer_loop, r2, ls, M_PI_2-ls, -b*0.5+t-r2, h*0.5-r2);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( (-b*0.5+t),  h*0.5 ));
			}

			outer_loop.push_back( carve::geom::VECTOR( -b*0.5,  h*0.5 ));
			paths.push_back( outer_loop );
		}
		return;
	}

	// U
	shared_ptr<IfcUShapeProfileDef> u_shape = dynamic_pointer_cast<IfcUShapeProfileDef>(profile);
	if( u_shape )
	{
		if( u_shape->m_Depth && u_shape->m_FlangeWidth && u_shape->m_WebThickness && u_shape->m_FlangeThickness )
		{
			double h = u_shape->m_Depth->m_value*length_factor;
			double b = u_shape->m_FlangeWidth->m_value*length_factor;
			double tw = u_shape->m_WebThickness->m_value*length_factor;
			double tf = u_shape->m_FlangeThickness->m_value*length_factor;
			double r1 = 0;
			if(u_shape->m_FilletRadius)
			{
				r1 = u_shape->m_FilletRadius->m_value*length_factor;
			}
			double r2 = 0;
			if(u_shape->m_EdgeRadius)
			{
				r2 = u_shape->m_EdgeRadius->m_value*length_factor;
			}
			double fs = 0;
			if(u_shape->m_FlangeSlope)
			{
				fs = u_shape->m_FlangeSlope->m_value*angle_factor;
			}
			
			outer_loop.push_back( carve::geom::VECTOR( -b*0.5,  -h*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR( b*0.5,  -h*0.5 ));

			double z = tan(fs)*(b*0.5-r2);
			if( r2 != 0 )
			{
				addArc( outer_loop, r2, 0, M_PI_2-fs, b*0.5-r2, -h*0.5+tf-z-r2);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( b*0.5,  (-h*0.5+tf-z) ));
			}

			z = tan(fs)*(b*0.5-tw-r1);
			if( r1 != 0 )
			{
				addArc( outer_loop, r1, 3*M_PI_2-fs, -M_PI_2+fs, -b*0.5+tw+r1, -h*0.5+tf+z+r1);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( (-b*0.5+tw),  (-h*0.5+tf+z) ));
			}

			// mirror horizontally along x-Axis
			mirrorCopyPathReverse( outer_loop, false, true );
			paths.push_back(outer_loop);
		}
		return;
	}
		
	// C
	shared_ptr<IfcCShapeProfileDef> c_shape = dynamic_pointer_cast<IfcCShapeProfileDef>(profile);
	if( c_shape )
	{
		if( c_shape->m_Depth && c_shape->m_Width && c_shape->m_Girth && c_shape->m_WallThickness )
		{
			double h = c_shape->m_Depth->m_value*length_factor;
			double b = c_shape->m_Width->m_value*length_factor;
			double g = c_shape->m_Girth->m_value*length_factor;
			double t = c_shape->m_WallThickness->m_value*length_factor;
			double r1 = 0;
			if(c_shape->m_InternalFilletRadius)
			{
				r1 = c_shape->m_InternalFilletRadius->m_value*length_factor;
			}
			
			if(r1 != 0)
			{
				addArc( outer_loop, r1+t, M_PI, M_PI_2, -b*0.5+t+r1, -h*0.5+t+r1);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( -b*0.5,  -h*0.5 ));
			}
			
			if(r1 != 0)
			{
				addArc( outer_loop, r1+t, 3*M_PI_2, M_PI_2, b*0.5-t-r1, -h*0.5+t+r1);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( b*0.5,  -h*0.5 ));
			}
			
			outer_loop.push_back( carve::geom::VECTOR( b*0.5,  (-h*0.5+g) ));
			outer_loop.push_back( carve::geom::VECTOR( (b*0.5-t),  (-h*0.5+g) ));
			
			if(r1 != 0)
			{
				addArc( outer_loop, r1, 0, -M_PI_2, b*0.5-t-r1, -h*0.5+t+r1);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( (b*0.5-t),  (-h*0.5+t) ));
			}
			
			if(r1 != 0)
			{
				addArc( outer_loop, r1, 3*M_PI_2, -M_PI_2, -b*0.5+t+r1, -h*0.5+t+r1);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( (-b*0.5+t),  (-h*0.5+t) ));
			}
			// mirror horizontally along x-Axis
			mirrorCopyPathReverse( outer_loop, false, true );
			paths.push_back(outer_loop);
		}
		return;
	}

		
	//Z-Shape-Profile
	shared_ptr<IfcZShapeProfileDef> z_shape = dynamic_pointer_cast<IfcZShapeProfileDef>(profile);
	if( z_shape )
	{
		if( z_shape->m_Depth && z_shape->m_FlangeWidth && z_shape->m_WebThickness && z_shape->m_FlangeThickness )
		{
			double h = z_shape->m_Depth->m_value*length_factor;
			double b = z_shape->m_FlangeWidth->m_value*length_factor;
			double tw = z_shape->m_WebThickness->m_value*length_factor;
			double tf = z_shape->m_FlangeThickness->m_value*length_factor;
			double r1 = 0;
			if(z_shape->m_FilletRadius)
			{
				r1 = z_shape->m_FilletRadius->m_value*length_factor;
			}

			double r2 = 0;
			if(z_shape->m_EdgeRadius)
			{
				r2 = z_shape->m_EdgeRadius->m_value*length_factor;
			}

			outer_loop.push_back( carve::geom::VECTOR( (-tw*0.5),  -h*0.5 ));
			outer_loop.push_back( carve::geom::VECTOR( (b-tw*0.5),  -h*0.5 ));
			
			if(r2 != 0)
			{
				addArc( outer_loop, r2, 0, M_PI_2, b-tw*0.5-r2, -h*0.5+tf-r2);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( (b-tw*0.5),  (-h*0.5+tf) ));
			}

			if(r1 != 0)
			{
				addArc( outer_loop, r1, 3*M_PI_2, -M_PI_2, tw*0.5+r1, -h*0.5+tf+r1);
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( (tw*0.5),  (-h*0.5+tf) ));
			}

			// mirror horizontally and vertically
			mirrorCopyPath( outer_loop, true, true );
			paths.push_back(outer_loop);
		}
		return;
	}
		
	//T-Shape-Profile
	shared_ptr<IfcTShapeProfileDef> t_shape = dynamic_pointer_cast<IfcTShapeProfileDef>(profile);
	if( t_shape )
	{
		const double h = t_shape->m_Depth->m_value*length_factor;
		const double b = t_shape->m_FlangeWidth->m_value*length_factor;
		const double tw = t_shape->m_WebThickness->m_value*length_factor*0.5;
		const double tf = t_shape->m_FlangeThickness->m_value*length_factor;
		
		double r1 = 0;
		if(t_shape->m_FilletRadius)
		{
			r1 = t_shape->m_FilletRadius->m_value*length_factor;
		}

		double r2 = 0;
		if(t_shape->m_FlangeEdgeRadius)
		{
			r2 = t_shape->m_FlangeEdgeRadius->m_value*length_factor;
		}
		
		double r3 = 0;
		if(t_shape->m_WebEdgeRadius)
		{
			r3 = t_shape->m_WebEdgeRadius->m_value*length_factor;
		}
		double fs = 0;
		
		if(t_shape->m_FlangeSlope)
		{
			fs = t_shape->m_FlangeSlope->m_value*angle_factor;
		}

		double ws = 0;
		if(t_shape->m_WebSlope)
		{
			ws = t_shape->m_WebSlope->m_value*angle_factor;
		}
				
		outer_loop.push_back( carve::geom::VECTOR( -b*0.5,  h*0.5 ));
		
		double zf = tan(fs)*(b*0.25-r2);
		double zw = tan(ws)*(h*0.5-r3);
		if(r2 != 0)
		{
			addArc( outer_loop, r2, M_PI, M_PI_2-fs, -b*0.5+r2, h*0.5-tf+zf+r2);
		}
		else
		{
			outer_loop.push_back( carve::geom::VECTOR( -b*0.5,  (h*0.5-tf+zf) ));
		}
		
		double cf = cos(fs);
		double sf = sin(fs);
		double cw = cos(ws);
		double sw = sin(ws);
		double z1 = (sf*((b-2*(r1+r2+tw-zw))*cw-2*(h-r3-r1-tf+zf)*sw)) / (2*(cf*cw-sf*sw));
		double z2 = tan(ws)*(h-r3-r1-z1-tf+zf);
		if(r1 != 0)
		{
			addArc( outer_loop, r1, M_PI_2-fs, -M_PI_2+fs+ws, -tw+zw-z2-r1, h*0.5-tf+zf-z1-r1);
		}
		else
		{
			outer_loop.push_back( carve::geom::VECTOR( (-tw+zw-z2),  (h*0.5-tf+zf-z1) ));
		}

		if(r3 != 0)
		{
			addArc( outer_loop, r3, M_PI+ws, M_PI_2-ws, -tw+zw+r3, -h*0.5+r3);
		}
		else
		{
			outer_loop.push_back( carve::geom::VECTOR( (-tw+zw), -h*0.5 ));
		}
		
		// mirror vertically along y-Axis
		mirrorCopyPathReverse( outer_loop, true, false );
		paths.push_back(outer_loop);
		return;
	}
		
	//not supported ProfileDef
	std::stringstream strs;
	strs << "IfcProfileDef not supported: " << profile->classname();
	throw IfcPPException( strs.str(), __func__ );
}

/*
void ProfileConverter::convertIfcNurbsProfile( const shared_ptr<IfcNurbsProfile>& nurbs_profile, std::vector<std::vector<carve::geom::vector<3> > >& paths )
{
	std::stringstream err;
	shared_ptr<IfcRationalBSplineSurfaceWithKnots> surface = dynamic_pointer_cast<IfcRationalBSplineSurfaceWithKnots>(nurbs_profile->m_Surface);
	if( !surface )
	{
		return;
	}
	std::vector<carve::geom::vector<3> > loop;
	double length_factor = m_unit_converter->getLengthInMeterFactor();
	std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& vec_control_points = surface->m_ControlPointsList;
	std::vector<std::vector<double> >& vec_weights = surface->m_WeightsData;

	// o------------<------------o
	// |     --> xi              |
	// |    |                    |
	// v    v                    ^
	// |    eta                  |
	// |                         |
	// o------------>------------o
	// insert control points at xi=0/1 and eta=0/1
	
	RepresentationConverter converter( m_unit_converter );

	// xi = 0
	std::vector<shared_ptr<IfcCartesianPoint> >& vec_control_points_eta0 = vec_control_points[0];
	std::vector<shared_ptr<IfcCartesianPoint> >::iterator it_xi0 = vec_control_points_eta0.begin();
	for( ; it_xi0 != vec_control_points_eta0.end(); ++it_xi0 )
	{
		shared_ptr<IfcCartesianPoint>& ifc_point = (*it_xi0);
		carve::geom::vector<3>  point;
		converter.convertIfcCartesianPoint( ifc_point, point );
		loop.push_back(point);
	}

	// eta = 1
	std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >::iterator it_eta1 = vec_control_points.begin();
	std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >::iterator it_eta1_last = vec_control_points.end();
	++it_eta1;
	--it_eta1_last;
	for( ; it_eta1 != it_eta1_last; ++it_eta1 )
	{
		std::vector<shared_ptr<IfcCartesianPoint> >& vec_eta = *it_eta1;
		shared_ptr<IfcCartesianPoint>& ifc_point = vec_eta.back();
		carve::geom::vector<3>  point;
		converter.convertIfcCartesianPoint( ifc_point, point );
		loop.push_back(point);
	}

	// xi = 1
	std::vector<shared_ptr<IfcCartesianPoint> >& vec_control_points_eta1 = vec_control_points[vec_control_points.size()-1];
	std::vector<shared_ptr<IfcCartesianPoint> >::reverse_iterator it_control_points_reverse = vec_control_points_eta1.rbegin();
	for( ; it_control_points_reverse != vec_control_points_eta1.rend(); ++it_control_points_reverse )
	{
		shared_ptr<IfcCartesianPoint>& ifc_point = (*it_control_points_reverse);
		carve::geom::vector<3>  point;
		converter.convertIfcCartesianPoint( ifc_point, point );
		loop.push_back(point);
	}

	// eta = 0
	std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >::reverse_iterator it_eta0 = vec_control_points.rbegin();
	std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >::reverse_iterator it_eta0_last = vec_control_points.rend();
	++it_eta0;
	--it_eta0_last;
	for( ; it_eta0 != it_eta0_last; ++it_eta0 )
	{
		std::vector<shared_ptr<IfcCartesianPoint> >& vec_eta = *it_eta1;
		shared_ptr<IfcCartesianPoint>& ifc_point = vec_eta.back();
		carve::geom::vector<3>  point;
		converter.convertIfcCartesianPoint( ifc_point, point );
		loop.push_back(point);
	}
	paths.push_back(loop);
}
*/
	
void ProfileConverter::deleteLastPointIfEqualToFirst( std::vector<carve::geom::vector<2> >& coords )
{
	while( coords.size() > 2 )
	{
		carve::geom::vector<2> & first = coords.front();
		carve::geom::vector<2> & last = coords.back();

		if( std::abs(first.x-last.x) < 0.00000001 )
		{
			if( std::abs(first.y-last.y) < 0.00000001 )
			{
				//if( std::abs(first.z-last.z) < 0.00000001 )
				{
					coords.pop_back();
					continue;
				}
			}
		}
		break;
	}
}

void ProfileConverter::simplifyPaths()
{
	simplifyPaths( m_paths );
}

void ProfileConverter::simplifyPaths( std::vector<std::vector<carve::geom::vector<2> > >& paths )
{
	for( std::vector<std::vector<carve::geom::vector<2> > >::iterator it_paths = paths.begin(); it_paths != paths.end(); ++it_paths )
	{
		std::vector<carve::geom::vector<2> >& path = (*it_paths);
		if( path.size() < 3 )
		{
			continue;
		}
		simplifyPath( path );
	}
}

void ProfileConverter::simplifyPath( std::vector<carve::geom::vector<2> >& path )
{
	if( path.size() < 3 )
	{
		return;
	}

	for( int i=1; i<path.size()-1; )
	{
		carve::geom::vector<2>& previous = path[i-1];
		carve::geom::vector<2>& current = path[i];
		carve::geom::vector<2>& next = path[i+1];

		carve::geom::vector<2> segment1 = current - previous;
		segment1.normalize();
		carve::geom::vector<2> segment2 = next - current;
		segment2.normalize();
		double angle = std::abs( segment1.x*segment2.x + segment1.y*segment2.y);
		if( std::abs(angle-1) < 0.00001 )
		{
			// points are colinear, current point can be removed
			path.erase( path.begin() + i );
			continue;
		}
		++i;
	}

	// 1-----0 5-----4      0-----------3         1---0 4---3      0-----------2
	// |             |  ->  |           |         |   _ ---    ->  |   _ ---
	// 2-------------3      1-----------2         2--              1---

	if( path.size() > 4 )
	{
		carve::geom::vector<2>& first = path.front();
		carve::geom::vector<2>& last = path.back();

		if( (last - first).length2() < 0.000001 )
		{
			carve::geom::vector<2> first_segment = path[1] - first;
			first_segment.normalize();
			carve::geom::vector<2> last_segment = last - path[path.size()-2];
			last_segment.normalize();
			double angle = std::abs( first_segment.x*last_segment.x + first_segment.y*last_segment.y);
			if( std::abs(angle-1) < 0.00001 )
			{
				// remove first and last point
				path.erase( path.begin() );
				path.pop_back();
			}
		}
	}
}


void ProfileConverter::addArc( std::vector<carve::geom::vector<2> >& coords, double radius, double start_angle, double opening_angle, double xM, double yM, int num_segments ) const
{
	if( num_segments < 0 )
	{
		num_segments = (int) (std::abs(opening_angle)/(2.0*M_PI)*m_geom_settings->m_num_vertices_per_circle); // TODO: adapt to model size and complexity
	}

	if( num_segments < m_geom_settings->m_min_num_vertices_per_arc )
	{
		num_segments = m_geom_settings->m_min_num_vertices_per_arc;
	}

	if( num_segments > 100 )
	{
		num_segments = 100;
	}

	double angle = start_angle;
	double angle_delta = opening_angle / (double)num_segments;
	for( int i=0; i<num_segments; ++i )
	{	
		coords.push_back( carve::geom::VECTOR( (radius*cos(angle)+xM),  (radius*sin(angle)+yM) ) );
		angle += angle_delta;
	}
}

void ProfileConverter::addArcWithEndPoint( std::vector<carve::geom::vector<2> >& coords, double radius, double start_angle, double opening_angle, double xM, double yM ) const
{
	int num_segments = (int) (std::abs(opening_angle)/(2.0*M_PI)*m_geom_settings->m_num_vertices_per_circle); // TODO: adapt to model size and complexity

	if( num_segments < m_geom_settings->m_min_num_vertices_per_arc )
	{
		num_segments = m_geom_settings->m_min_num_vertices_per_arc;
	}

	if( num_segments > 100 )
	{
		num_segments = 100;
	}

	double angle = start_angle;
	double angle_delta = opening_angle / (double)(num_segments-1);
	for( int i=0; i<num_segments; ++i )
	{	
		coords.push_back( carve::geom::VECTOR( radius*cos(angle) + xM,  radius*sin(angle) + yM ) );
		angle += angle_delta;
	}
}

void ProfileConverter::addArcWithEndPoint( std::vector<carve::geom::vector<2> >& coords, double radius, double start_angle, double opening_angle, double xM, double yM, int num_segments )
{
	if( num_segments < 3 )
	{
		num_segments = 3;
	}

	if( num_segments > 100 )
	{
		num_segments = 100;
	}

	double angle = start_angle;
	double angle_delta = opening_angle / (double)(num_segments-1);
	for( int i=0; i<num_segments; ++i )
	{	
		coords.push_back( carve::geom::VECTOR( radius*cos(angle) + xM,  radius*sin(angle) + yM  ) );
		angle += angle_delta;
	}
}

void ProfileConverter::mirrorCopyPath( std::vector<carve::geom::vector<2> >& coords, bool mirror_on_y_axis, bool mirror_on_x_axis )
{
	int points_count = coords.size();
	double x, y;
	for( int i = 0; i < points_count; ++i )
	{
		carve::geom::vector<2> & p = coords[i];
		if( mirror_on_y_axis )
		{
			x = -p.x;
		}
		else
		{
			x = p.x;
		}
		if( mirror_on_x_axis )
		{
			y = -p.y;
		}
		else
		{
			y = p.y;
		}
		coords.push_back( carve::geom::VECTOR( x, y ) );
	}
}

void ProfileConverter::mirrorCopyPathReverse( std::vector<carve::geom::vector<2> >& coords, bool mirror_on_y_axis, bool mirror_on_x_axis )
{
	int points_count = coords.size();
	double x, y;
	for( int i = points_count-1; i >= 0; --i )
	{
		carve::geom::vector<2> & p = coords[i];
		if( mirror_on_y_axis )
		{
			x = -p.x;
		}
		else
		{
			x = p.x;
		}
		if( mirror_on_x_axis )
		{
			y = -p.y;
		}
		else
		{
			y = p.y;
		}

		coords.push_back( carve::geom::VECTOR( x, y ) );
	}
}

