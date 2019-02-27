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
#include <ifcpp/IFC4/include/IfcArbitraryClosedProfileDef.h>
#include <ifcpp/IFC4/include/IfcArbitraryOpenProfileDef.h>
#include <ifcpp/IFC4/include/IfcArbitraryProfileDefWithVoids.h>
#include <ifcpp/IFC4/include/IfcAsymmetricIShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcBoundedCurve.h>
#include <ifcpp/IFC4/include/IfcCartesianTransformationOperator2D.h>
#include <ifcpp/IFC4/include/IfcCenterLineProfileDef.h>
#include <ifcpp/IFC4/include/IfcCircleHollowProfileDef.h>
#include <ifcpp/IFC4/include/IfcCompositeProfileDef.h>
#include <ifcpp/IFC4/include/IfcCShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcDerivedProfileDef.h>
#include <ifcpp/IFC4/include/IfcEllipseProfileDef.h>
#include <ifcpp/IFC4/include/IfcIShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcLShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcNonNegativeLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcParameterizedProfileDef.h>
#include <ifcpp/IFC4/include/IfcPlaneAngleMeasure.h>
#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcProfileDef.h>
#include <ifcpp/IFC4/include/IfcRectangleProfileDef.h>
#include <ifcpp/IFC4/include/IfcRectangleHollowProfileDef.h>
#include <ifcpp/IFC4/include/IfcRoundedRectangleProfileDef.h>
#include <ifcpp/IFC4/include/IfcTrapeziumProfileDef.h>
#include <ifcpp/IFC4/include/IfcTShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcUShapeProfileDef.h>
#include <ifcpp/IFC4/include/IfcZShapeProfileDef.h>

#include "IncludeCarveHeaders.h"
#include "PointConverter.h"
#include "CurveConverter.h"
#include "PlacementConverter.h"
#include "SplineConverter.h"
#include "GeomUtils.h"

class ProfileConverter : public StatusCallback
{
public:
	
	const std::vector<std::vector<vec2> >& getCoordinates() { return m_paths; }
	void clearProfileConverter() { m_paths.clear(); }

	shared_ptr<CurveConverter>				m_curve_converter;
	shared_ptr<SplineConverter>				m_spline_converter;

protected:
	std::vector<std::vector<vec2> >	m_paths;
	
public:
	ProfileConverter( shared_ptr<CurveConverter>& cc, shared_ptr<SplineConverter>& sc )
		: m_curve_converter( cc ), m_spline_converter( sc )
	{
	}
	virtual ~ProfileConverter()
	{
	}
	void computeProfile( shared_ptr<IfcProfileDef> profile_def )
	{
		// ENTITY IfcProfileDef SUPERTYPE OF(ONEOF(IfcArbitraryClosedProfileDef, IfcArbitraryOpenProfileDef, IfcCompositeProfileDef, IfcDerivedProfileDef, IfcParameterizedProfileDef));
		shared_ptr<IfcParameterizedProfileDef> parameterized = dynamic_pointer_cast<IfcParameterizedProfileDef>( profile_def );
		if( parameterized )
		{
			convertIfcParameterizedProfileDefWithPosition( parameterized, m_paths );
			GeomUtils::removeDuplicates( m_paths );
			return;
		}

		shared_ptr<IfcArbitraryClosedProfileDef> arbitrary_closed = dynamic_pointer_cast<IfcArbitraryClosedProfileDef>( profile_def );
		if( arbitrary_closed )
		{
			convertIfcArbitraryClosedProfileDef( arbitrary_closed, m_paths );
			GeomUtils::removeDuplicates( m_paths );
			return;
		}

		shared_ptr<IfcArbitraryOpenProfileDef> arbitrary_open = dynamic_pointer_cast<IfcArbitraryOpenProfileDef>( profile_def );
		if( arbitrary_open )
		{
			convertIfcArbitraryOpenProfileDef( arbitrary_open, m_paths );
			GeomUtils::removeDuplicates( m_paths );
			return;
		}

		shared_ptr<IfcCompositeProfileDef> composite = dynamic_pointer_cast<IfcCompositeProfileDef>( profile_def );
		if( composite )
		{
			convertIfcCompositeProfileDef( composite, m_paths );
			GeomUtils::removeDuplicates( m_paths );
			return;
		}

		shared_ptr<IfcDerivedProfileDef> derived = dynamic_pointer_cast<IfcDerivedProfileDef>( profile_def );
		if( derived )
		{
			convertIfcDerivedProfileDef( derived, m_paths );
			GeomUtils::removeDuplicates( m_paths );
			return;
		}

		messageCallback( "Profile not supported", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, profile_def.get() );
	}
	void addAvoidingDuplicates( const std::vector<vec2>& polygon, std::vector<std::vector<vec2> >& paths )
	{
		if( polygon.size() < 1 )
		{
			return;
		}

		std::vector<vec2> polygon_add;
		polygon_add.push_back( polygon[0] );
		for( size_t i = 1; i < polygon.size(); ++i )
		{
			const vec2 & point = polygon[i];
			const vec2 & point_previous = polygon[i - 1];

			// omit duplicate points
			if( std::abs( point.x - point_previous.x ) > 0.00001 )
			{
				polygon_add.push_back( point );
				continue;
			}

			if( std::abs( point.y - point_previous.y ) > 0.00001 )
			{
				polygon_add.push_back( point );
				continue;
			}
		}
		paths.push_back( polygon_add );
	}
	void convertIfcArbitraryClosedProfileDef( const shared_ptr<IfcArbitraryClosedProfileDef>& profile, std::vector<std::vector<vec2> >& paths )
	{
		shared_ptr<IfcCurve> outer_curve = profile->m_OuterCurve;
		if( outer_curve )
		{
			std::vector<vec2> curve_polygon;
			std::vector<vec2> segment_start_points;
			m_curve_converter->convertIfcCurve2D( outer_curve, curve_polygon, segment_start_points );
			deleteLastPointIfEqualToFirst( curve_polygon );
			addAvoidingDuplicates( curve_polygon, paths );
		}

		// IfcArbitraryProfileDefWithVoids
		shared_ptr<IfcArbitraryProfileDefWithVoids> profile_with_voids = dynamic_pointer_cast<IfcArbitraryProfileDefWithVoids>( profile );
		if( profile_with_voids )
		{
			std::vector<shared_ptr<IfcCurve> > inner_curves = profile_with_voids->m_InnerCurves;
			for( size_t i = 0; i < inner_curves.size(); ++i )
			{
				shared_ptr<IfcCurve> inner_ifc_curve = inner_curves[i];
				std::vector<vec2> inner_curve_polygon;
				std::vector<vec2> segment_start_points;

				m_curve_converter->convertIfcCurve2D( inner_ifc_curve, inner_curve_polygon, segment_start_points );
				deleteLastPointIfEqualToFirst( inner_curve_polygon );
				addAvoidingDuplicates( inner_curve_polygon, paths );
			}
		}
	}
	void convertIfcArbitraryOpenProfileDef( const shared_ptr<IfcArbitraryOpenProfileDef>& profile, std::vector<std::vector<vec2> >& paths )
	{
		// ENTITY IfcArbitraryOpenProfileDef
		//	SUPERTYPE OF(IfcCenterLineProfileDef)
		//	SUBTYPE OF IfcProfileDef;
		//	Curve	 :	IfcBoundedCurve;

		shared_ptr<IfcCurve> ifc_curve = profile->m_Curve;
		const shared_ptr<UnitConverter>& uc = m_curve_converter->getPointConverter()->getUnitConverter();

		// IfcCenterLineProfileDef
		shared_ptr<IfcCenterLineProfileDef> center_line_profile_def = dynamic_pointer_cast<IfcCenterLineProfileDef>( profile );
		if( center_line_profile_def )
		{
			if( center_line_profile_def->m_Thickness )
			{
				const double thickness = center_line_profile_def->m_Thickness->m_value * uc->getLengthInMeterFactor();
				std::vector<vec3> segment_start_points;
				std::vector<vec3> basis_curve_points;
				m_curve_converter->convertIfcCurve( ifc_curve, basis_curve_points, segment_start_points );

				size_t num_base_points = basis_curve_points.size();
				if( num_base_points < 2 )
				{
#ifdef _DEBUG
					std::cout << "IfcCenterLineProfileDef: num curve points < 2";
#endif
					return;
				}

				carve::math::Matrix matrix_sweep;
				vec3 local_z( carve::geom::VECTOR( 0, 0, 1 ) );
				std::vector<vec3> left_points;
				std::vector<vec3> right_points;
				vec3 point_left( carve::geom::VECTOR( 0.0, -thickness*0.5, 0.0 ) );
				vec3 point_right( carve::geom::VECTOR( 0.0, thickness*0.5, 0.0 ) );

				for( size_t ii = 0; ii < num_base_points; ++ii )
				{
					vec3 vertex_current = basis_curve_points[ii];
					vec3 vertex_next;
					vec3 vertex_before;
					if( ii == 0 )
					{
						// first point
						vertex_next = basis_curve_points[ii + 1];
						vec3 delta_element = vertex_next - vertex_current;
						vertex_before = vertex_current - ( delta_element );
					}
					else if( ii == num_base_points - 1 )
					{
						// last point
						vertex_before = basis_curve_points[ii - 1];
						vec3 delta_element = vertex_current - vertex_before;
						vertex_next = vertex_before + ( delta_element );
					}
					else
					{
						// inner point
						vertex_next = basis_curve_points[ii + 1];
						vertex_before = basis_curve_points[ii - 1];
					}

					vec3 bisecting_normal;
					GeomUtils::bisectingPlane( vertex_before, vertex_current, vertex_next, bisecting_normal );

					if( ii == num_base_points - 1 )
					{
						bisecting_normal *= -1.0;
					}

					local_z.x = 0;
					local_z.y = 0;
					local_z.z = -1;
					GeomUtils::convertPlane2Matrix( bisecting_normal, vertex_current, local_z, matrix_sweep );

					left_points.push_back( matrix_sweep*point_left );
					right_points.push_back( matrix_sweep*point_right );
				}

				std::reverse( right_points.begin(), right_points.end() );
				std::vector<vec2> polygon;
				for( size_t i2 = 0; i2 < left_points.size(); ++i2 )
				{
					vec3& point3d = left_points[i2];
					polygon.push_back( carve::geom::VECTOR( point3d.x, point3d.y ) );
				}
				for( size_t i2 = 0; i2 < right_points.size(); ++i2 )
				{
					vec3& point3d = right_points[i2];
					polygon.push_back( carve::geom::VECTOR( point3d.x, point3d.y ) );
				}
				addAvoidingDuplicates( polygon, paths );
			}
		}
		else
		{
			std::vector<vec2> polygon;
			std::vector<vec2> segment_start_points;
			m_curve_converter->convertIfcCurve2D( ifc_curve, polygon, segment_start_points );
			addAvoidingDuplicates( polygon, paths );
		}
	}
	void convertIfcCompositeProfileDef( const shared_ptr<IfcCompositeProfileDef>& composite_profile, std::vector<std::vector<vec2> >& paths )
	{
		std::vector<int> temploop_counts;
		std::vector<int> tempcontour_counts;

		std::vector<shared_ptr<IfcProfileDef> >& vec_profiles = composite_profile->m_Profiles;
		for( size_t ii = 0; ii < vec_profiles.size(); ++ii )
		{
			shared_ptr<IfcProfileDef>& profile_def = vec_profiles[ii];

			shared_ptr<IfcParameterizedProfileDef> parameterized = dynamic_pointer_cast<IfcParameterizedProfileDef>( profile_def );
			if( parameterized )
			{
				convertIfcParameterizedProfileDefWithPosition( parameterized, paths );
				continue;
			}

			shared_ptr<IfcArbitraryOpenProfileDef> open = dynamic_pointer_cast<IfcArbitraryOpenProfileDef>( profile_def );
			if( open )
			{
				convertIfcArbitraryOpenProfileDef( open, paths );
				continue;
			}

			shared_ptr<IfcArbitraryClosedProfileDef> closed = dynamic_pointer_cast<IfcArbitraryClosedProfileDef>( profile_def );
			if( closed )
			{
				convertIfcArbitraryClosedProfileDef( closed, paths );
				continue;
			}

			shared_ptr<IfcCompositeProfileDef> composite = dynamic_pointer_cast<IfcCompositeProfileDef>( profile_def );
			if( composite )
			{
				convertIfcCompositeProfileDef( composite, paths );
				continue;
			}

			shared_ptr<IfcDerivedProfileDef> derived = dynamic_pointer_cast<IfcDerivedProfileDef>( profile_def );
			if( derived )
			{
				convertIfcDerivedProfileDef( derived, paths );
				continue;
			}

			messageCallback( "Profile not supported", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, profile_def.get() );
		}
	}
	void convertIfcDerivedProfileDef( const shared_ptr<IfcDerivedProfileDef>& derived_profile, std::vector<std::vector<vec2> >& paths )
	{
		ProfileConverter temp_profiler( m_curve_converter, m_spline_converter );
		temp_profiler.computeProfile( derived_profile->m_ParentProfile );
		const std::vector<std::vector<vec2> >& parent_paths = temp_profiler.getCoordinates();

		shared_ptr<IfcCartesianTransformationOperator2D> transf_op_2D = derived_profile->m_Operator;

		shared_ptr<TransformData> transform;
		m_curve_converter->getPlcamentConverter()->convertTransformationOperator( transf_op_2D, transform );
		for( size_t i = 0; i < parent_paths.size(); ++i )
		{
			const std::vector<vec2>& loop_parent = parent_paths[i];
			std::vector<vec2> loop;

			for( size_t j = 0; j < loop_parent.size(); ++j )
			{
				const vec2& pt = loop_parent[j];
				vec3 pt3d( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
				if( transform )
				{
					pt3d = transform->m_matrix*pt3d;
				}
				loop.push_back( carve::geom::VECTOR( pt3d.x, pt3d.y ) );
			}
			paths.push_back( loop );
		}
	}
	void convertIfcParameterizedProfileDefWithPosition( const shared_ptr<IfcParameterizedProfileDef>& parameterized, std::vector<std::vector<vec2> >& paths )
	{
		std::vector<std::vector<vec2> > temp_paths;
		convertIfcParameterizedProfileDef( parameterized, temp_paths );

		// local coordinate system
		if( parameterized->m_Position )
		{
			shared_ptr<IfcAxis2Placement2D> axis2Placement2D = parameterized->m_Position;
			shared_ptr<TransformData> transform;
			m_curve_converter->getPlcamentConverter()->convertIfcPlacement( axis2Placement2D, transform );

			for( size_t i = 0; i < temp_paths.size(); ++i )
			{
				std::vector<vec2>& path_loop = temp_paths[i];
				for( size_t j = 0; j < path_loop.size(); ++j )
				{
					vec2& pt = path_loop[j];
					vec3 pt_3d( carve::geom::VECTOR( pt.x, pt.y, 0 ) );
					if( transform )
					{
						pt_3d = transform->m_matrix*pt_3d;
					}
					pt.x = pt_3d.x;
					pt.y = pt_3d.y;
				}
				paths.push_back( path_loop );
			}
		}
		else
		{
			for( size_t i = 0; i < temp_paths.size(); ++i )
			{
				std::vector<vec2>& path_loop = temp_paths[i];
				paths.push_back( path_loop );
			}
		}
	}
	void convertIfcParameterizedProfileDef( const shared_ptr<IfcParameterizedProfileDef>& profile, std::vector<std::vector<vec2> >& paths )
	{
		//IfcParameterizedProfileDef ABSTRACT SUPERTYPE OF (ONEOF
		//	(IfcCShapeProfileDef, IfcCircleProfileDef, IfcEllipseProfileDef, IfcIShapeProfileDef, IfcLShapeProfileDef,
		//	IfcRectangleProfileDef, IfcTShapeProfileDef, IfcTrapeziumProfileDef, IfcUShapeProfileDef, IfcZShapeProfileDef))

		const shared_ptr<UnitConverter>& uc = m_curve_converter->getPointConverter()->getUnitConverter();
		const shared_ptr<GeometrySettings>& gs = m_curve_converter->getGeomSettings();
		if( !uc )
		{
			messageCallback( "UnitConverter not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
			return;
		}
		if( !gs )
		{
			messageCallback( "GeometrySettings not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
			return;
		}

		const double length_factor = uc->getLengthInMeterFactor();
		std::vector<vec2> outer_loop;

		// Rectangle profile
		shared_ptr<IfcRectangleProfileDef> rectangle_profile = dynamic_pointer_cast<IfcRectangleProfileDef>( profile );
		if( rectangle_profile )
		{
			if( rectangle_profile->m_XDim && rectangle_profile->m_YDim )
			{
				double x_dim = rectangle_profile->m_XDim->m_value*length_factor;
				double y_dim = rectangle_profile->m_YDim->m_value*length_factor;

				shared_ptr<IfcRectangleHollowProfileDef> hollow = dynamic_pointer_cast<IfcRectangleHollowProfileDef>( rectangle_profile );
				if( hollow )
				{
					if( hollow->m_WallThickness )
					{
						const double t = hollow->m_WallThickness->m_value*length_factor;
						double outer_fillet_radius = 0;
						if( hollow->m_OuterFilletRadius && !m_curve_converter->getGeomSettings()->isIgnoreProfileRadius() )
						{
							outer_fillet_radius = hollow->m_OuterFilletRadius->m_value*length_factor;
						}

						double inner_fillet_radius = 0;
						if( hollow->m_InnerFilletRadius && !gs->isIgnoreProfileRadius() )
						{
							inner_fillet_radius = hollow->m_InnerFilletRadius->m_value*length_factor;
						}

						// Outer
						if( outer_fillet_radius != 0 )
						{
							addArc( outer_loop, outer_fillet_radius, 0, M_PI_2, x_dim*0.5 - outer_fillet_radius, y_dim*0.5 - outer_fillet_radius );
							addArc( outer_loop, outer_fillet_radius, M_PI_2, M_PI_2, -x_dim*0.5 + outer_fillet_radius, y_dim*0.5 - outer_fillet_radius );
							addArc( outer_loop, outer_fillet_radius, M_PI, M_PI_2, -x_dim*0.5 + outer_fillet_radius, -y_dim*0.5 + outer_fillet_radius );
							addArc( outer_loop, outer_fillet_radius, 3 * M_PI_2, M_PI_2, x_dim*0.5 - outer_fillet_radius, -y_dim*0.5 + outer_fillet_radius );
						}
						else
						{
							outer_loop.push_back( carve::geom::VECTOR( -x_dim*0.5, -y_dim*0.5 ) );
							outer_loop.push_back( carve::geom::VECTOR( x_dim*0.5, -y_dim*0.5 ) );
							outer_loop.push_back( carve::geom::VECTOR( x_dim*0.5, y_dim*0.5 ) );
							outer_loop.push_back( carve::geom::VECTOR( -x_dim*0.5, y_dim*0.5 ) );
						}

						// Inner
						std::vector<vec2> inner_loop;
						x_dim -= 2 * t;
						y_dim -= 2 * t;
						if( inner_fillet_radius != 0 )
						{
							addArc( inner_loop, inner_fillet_radius, 0, M_PI_2, x_dim*0.5 - inner_fillet_radius, y_dim*0.5 - inner_fillet_radius );
							addArc( inner_loop, inner_fillet_radius, M_PI_2, M_PI_2, -x_dim*0.5 + inner_fillet_radius, y_dim*0.5 - inner_fillet_radius );
							addArc( inner_loop, inner_fillet_radius, M_PI, M_PI_2, -x_dim*0.5 + inner_fillet_radius, -y_dim*0.5 + inner_fillet_radius );
							addArc( inner_loop, inner_fillet_radius, 3 * M_PI_2, M_PI_2, x_dim*0.5 - inner_fillet_radius, -y_dim*0.5 + inner_fillet_radius );
						}
						else
						{
							inner_loop.push_back( carve::geom::VECTOR( -x_dim*0.5, -y_dim*0.5 ) );
							inner_loop.push_back( carve::geom::VECTOR( x_dim*0.5, -y_dim*0.5 ) );
							inner_loop.push_back( carve::geom::VECTOR( x_dim*0.5, y_dim*0.5 ) );
							inner_loop.push_back( carve::geom::VECTOR( -x_dim*0.5, y_dim*0.5 ) );
						}
						paths.push_back( outer_loop );
						paths.push_back( inner_loop );
					}
					return;
				}

				// RoundedRectangle
				shared_ptr<IfcRoundedRectangleProfileDef> rounded_rectangle = dynamic_pointer_cast<IfcRoundedRectangleProfileDef>( rectangle_profile );
				if( rounded_rectangle && !gs->isIgnoreProfileRadius() )
				{
					if( rounded_rectangle->m_RoundingRadius )
					{
						double rr = rounded_rectangle->m_RoundingRadius->m_value*length_factor;
						addArc( outer_loop, rr, 0, M_PI_2, x_dim*0.5 - rr, y_dim*0.5 - rr );
						addArc( outer_loop, rr, M_PI_2, M_PI_2, -x_dim*0.5 + rr, y_dim*0.5 - rr );
						addArc( outer_loop, rr, M_PI, M_PI_2, -x_dim*0.5 + rr, -y_dim*0.5 + rr );
						addArc( outer_loop, rr, 3 * M_PI_2, M_PI_2, x_dim*0.5 - rr, -y_dim*0.5 + rr );
						paths.push_back( outer_loop );
					}
					return;
				}


				// else it's a standard rectangle
				outer_loop.push_back( carve::geom::VECTOR( -x_dim*0.5, -y_dim*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( x_dim*0.5, -y_dim*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( x_dim*0.5, y_dim*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( -x_dim*0.5, y_dim*0.5 ) );
				paths.push_back( outer_loop );
				return;
			}
		}

		// Trapezium profile
		shared_ptr<IfcTrapeziumProfileDef> trapezium = dynamic_pointer_cast<IfcTrapeziumProfileDef>( profile );
		if( trapezium )
		{
			if( trapezium->m_BottomXDim && trapezium->m_TopXDim && trapezium->m_TopXOffset && trapezium->m_YDim )
			{
				double x_bottom = trapezium->m_BottomXDim->m_value*length_factor;
				double x_top = trapezium->m_TopXDim->m_value*length_factor;
				double x_offset = trapezium->m_TopXOffset->m_value*length_factor;
				double y_dim = trapezium->m_YDim->m_value*length_factor;
				outer_loop.push_back( carve::geom::VECTOR( -x_bottom*0.5, -y_dim*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( x_bottom*0.5, -y_dim*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( -x_bottom*0.5 + x_offset + x_top, y_dim*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( -x_bottom*0.5 + x_offset, y_dim*0.5 ) );
				paths.push_back( outer_loop );
			}
			return;
		}

		// Circle profile
		shared_ptr<IfcCircleProfileDef> circle_profile_def = dynamic_pointer_cast<IfcCircleProfileDef>( profile );
		if( circle_profile_def )
		{
			shared_ptr<IfcPositiveLengthMeasure> radius_measure = circle_profile_def->m_Radius;
			if( radius_measure )
			{
				double radius = radius_measure->m_value*length_factor;
				if( radius < 0.000001 )
				{
					return;
				}
				int num_segments = gs->getNumVerticesPerCircleWithRadius(radius);
				double angle = 0;
				for( int i = 0; i < num_segments; ++i )
				{
					outer_loop.push_back( carve::geom::VECTOR( ( radius * cos( angle ) ), ( radius * sin( angle ) ) ) );
					angle += 2.0*M_PI / double( num_segments );
				}
				paths.push_back( outer_loop );

				// CircleHollow
				std::vector<vec2> inner_loop;
				shared_ptr<IfcCircleHollowProfileDef> hollow = dynamic_pointer_cast<IfcCircleHollowProfileDef>( profile );
				if( hollow )
				{
					angle = 0;
					radius -= hollow->m_WallThickness->m_value*length_factor;

					int num_segments2 = gs->getNumVerticesPerCircleWithRadius(radius);
					for( int i = 0; i < num_segments2; ++i )
					{
						inner_loop.push_back( carve::geom::VECTOR( ( radius * cos( angle ) ), ( radius * sin( angle ) ) ) );
						angle += 2.0*M_PI / double( num_segments2 );
					}
					paths.push_back( inner_loop );
				}
			}
			return;
		}

		// Ellipse profile
		shared_ptr<IfcEllipseProfileDef> ellipse_profile_def = dynamic_pointer_cast<IfcEllipseProfileDef>( profile );
		if( ellipse_profile_def )
		{
			if( ellipse_profile_def->m_SemiAxis1 )
			{
				if( ellipse_profile_def->m_SemiAxis2 )
				{
					double x_radius = ellipse_profile_def->m_SemiAxis1->m_value*length_factor;
					double y_radius = ellipse_profile_def->m_SemiAxis2->m_value*length_factor;
					int num_segments = gs->getNumVerticesPerCircleWithRadius(std::max(x_radius, y_radius));
					double angle = 0;
					for( int i = 0; i < num_segments; ++i )
					{
						outer_loop.push_back( carve::geom::VECTOR( ( x_radius * cos( angle ) ), ( y_radius * sin( angle ) ) ) );
						angle += 2.0*M_PI / double( num_segments );
					}
					paths.push_back( outer_loop );
				}
			}
			return;
		}

		// I-shaped profile
		shared_ptr<IfcIShapeProfileDef> i_shape = dynamic_pointer_cast<IfcIShapeProfileDef>( profile );
		if( i_shape )
		{
			if( i_shape->m_OverallDepth && i_shape->m_OverallWidth && i_shape->m_WebThickness && i_shape->m_FlangeThickness )
			{
				const double h = i_shape->m_OverallDepth->m_value*length_factor;
				const double width = i_shape->m_OverallWidth->m_value*length_factor;
				const double tw = i_shape->m_WebThickness->m_value*length_factor;
				const double tf = i_shape->m_FlangeThickness->m_value*length_factor;
				double fillet_radius = 0;
				if( i_shape->m_FilletRadius && !gs->isIgnoreProfileRadius() )
				{
					fillet_radius = i_shape->m_FilletRadius->m_value*length_factor;
				}

				outer_loop.push_back( carve::geom::VECTOR( width*0.5, -h*0.5 ) );

				// TODO: implement flange slope

				double flange_edge_radius = 0;
				if( i_shape->m_FlangeEdgeRadius && !gs->isIgnoreProfileRadius() )
				{
					flange_edge_radius = i_shape->m_FlangeEdgeRadius->m_value*length_factor;
					addArc( outer_loop, flange_edge_radius, 0, M_PI_2, width*0.5 - flange_edge_radius, -h*0.5 + tf - flange_edge_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( width*0.5, ( -h*0.5 + tf ) ) );
				}

				if( fillet_radius != 0 )
				{
					addArc( outer_loop, fillet_radius, 3 * M_PI_2, -M_PI_2, tw*0.5 + fillet_radius, -h*0.5 + tf + fillet_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( tw*0.5, ( -h*0.5 + tf ) ) );
				}

				shared_ptr<IfcAsymmetricIShapeProfileDef> asym_I_profile = dynamic_pointer_cast<IfcAsymmetricIShapeProfileDef>( i_shape );
				if( asym_I_profile )
				{
					if( asym_I_profile->m_TopFlangeWidth )
					{
						const double width_top = asym_I_profile->m_TopFlangeWidth->m_value*length_factor;
						double tfTop = tf;

						if( asym_I_profile->m_TopFlangeThickness )
						{
							tfTop = asym_I_profile->m_TopFlangeThickness->m_value*length_factor;
						}
						double rTop = fillet_radius;
						if( asym_I_profile->m_TopFlangeFilletRadius && !gs->isIgnoreProfileRadius() )
						{
							rTop = asym_I_profile->m_TopFlangeFilletRadius->m_value*length_factor;
						}

						if( rTop != 0 )
						{
							addArc( outer_loop, rTop, M_PI, -M_PI_2, tw*0.5 + rTop, h*0.5 - tfTop - rTop );
						}
						else
						{
							outer_loop.push_back( carve::geom::VECTOR( tw*0.5, ( h*0.5 - tfTop ) ) );
						}
						outer_loop.push_back( carve::geom::VECTOR( width_top*0.5, ( h*0.5 - tfTop ) ) );
						outer_loop.push_back( carve::geom::VECTOR( width_top*0.5, h*0.5 ) );
					}
				}
				else
				{
					// symmetric: mirror horizontally along x-Axis
					mirrorCopyPathReverse( outer_loop, false, true );
				}

				// mirror vertically along y-axis
				mirrorCopyPathReverse( outer_loop, true, false );
				paths.push_back( outer_loop );
			}
			return;
		}

		// L-shaped profile
		shared_ptr<IfcLShapeProfileDef> l_shape = dynamic_pointer_cast<IfcLShapeProfileDef>( profile );
		if( l_shape )
		{
			if( l_shape->m_Depth && l_shape->m_Thickness )
			{
				const double h = l_shape->m_Depth->m_value*length_factor;
				double w = h;

				if( l_shape->m_Width )
				{
					w = l_shape->m_Width->m_value*length_factor;
				}

				double t = l_shape->m_Thickness->m_value*length_factor;

				double fillet_radius = 0;
				if( l_shape->m_FilletRadius && !gs->isIgnoreProfileRadius() )
				{
					fillet_radius = l_shape->m_FilletRadius->m_value*length_factor;
				}

				double edge_radius = 0;
				if( l_shape->m_EdgeRadius && !gs->isIgnoreProfileRadius() )
				{
					edge_radius = l_shape->m_EdgeRadius->m_value*length_factor;
				}

				double leg_slope = 0;
				if( l_shape->m_LegSlope )
				{
					const double angle_factor = uc->getAngleInRadiantFactor();
					leg_slope = l_shape->m_LegSlope->m_value*angle_factor;
				}

				outer_loop.push_back( carve::geom::VECTOR( -w*0.5, -h*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( w*0.5, -h*0.5 ) );

				if( edge_radius != 0 )
				{
					double y_edge_radius_start = -h*0.5 + t - edge_radius;
					addArc( outer_loop, edge_radius, 0, M_PI_2 - leg_slope, w*0.5 - edge_radius, y_edge_radius_start );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( w*0.5, ( -h*0.5 + t ) ) );
				}

				const double s = sin( leg_slope );
				const double c = cos( leg_slope );
				const double z1 = ( -s*( ( c - s )*( fillet_radius + edge_radius + t ) - c*w + s*h ) ) / ( 2 * c*c - 1 );
				const double z2 = ( -s*( ( c - s )*( fillet_radius + edge_radius + t ) - c*h + s*w ) ) / ( 2 * c*c - 1 );
				if( fillet_radius != 0 )
				{
					addArc( outer_loop, fillet_radius, 3 * M_PI_2 - leg_slope, -M_PI_2 + 2 * leg_slope, -w*0.5 + t + z2 + fillet_radius, -h*0.5 + t + z1 + fillet_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( ( -w*0.5 + t + z2 ), ( -h*0.5 + t + z1 ) ) );
				}

				if( edge_radius != 0 )
				{
					addArc( outer_loop, edge_radius, leg_slope, M_PI_2 - leg_slope, -w*0.5 + t - edge_radius, h*0.5 - edge_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( ( -w*0.5 + t ), h*0.5 ) );
				}

				outer_loop.push_back( carve::geom::VECTOR( -w*0.5, h*0.5 ) );
				paths.push_back( outer_loop );
			}
			return;
		}

		// U-shaped profile
		shared_ptr<IfcUShapeProfileDef> u_shape = dynamic_pointer_cast<IfcUShapeProfileDef>( profile );
		if( u_shape )
		{
			if( u_shape->m_Depth && u_shape->m_FlangeWidth && u_shape->m_WebThickness && u_shape->m_FlangeThickness )
			{
				const double height = u_shape->m_Depth->m_value*length_factor;
				const double width = u_shape->m_FlangeWidth->m_value*length_factor;
				const double tw = u_shape->m_WebThickness->m_value*length_factor;
				const double tf = u_shape->m_FlangeThickness->m_value*length_factor;
				double fillet_radius = 0;
				if( u_shape->m_FilletRadius && !gs->isIgnoreProfileRadius() )
				{
					fillet_radius = u_shape->m_FilletRadius->m_value*length_factor;
				}
				double edge_radius = 0;
				if( u_shape->m_EdgeRadius && !gs->isIgnoreProfileRadius() )
				{
					edge_radius = u_shape->m_EdgeRadius->m_value*length_factor;
				}
				double fs = 0;
				if( u_shape->m_FlangeSlope )
				{
					const double angle_factor = uc->getAngleInRadiantFactor();
					fs = u_shape->m_FlangeSlope->m_value*angle_factor;
				}

				outer_loop.push_back( carve::geom::VECTOR( -width*0.5, -height*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( width*0.5, -height*0.5 ) );

				double z = tan( fs )*( width*0.5 - edge_radius );
				if( edge_radius != 0 )
				{
					addArc( outer_loop, edge_radius, 0, M_PI_2 - fs, width*0.5 - edge_radius, -height*0.5 + tf - z - edge_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( width*0.5, ( -height*0.5 + tf - z ) ) );
				}

				z = tan( fs )*( width*0.5 - tw - fillet_radius );
				if( fillet_radius != 0 )
				{
					addArc( outer_loop, fillet_radius, 3 * M_PI_2 - fs, -M_PI_2 + fs, -width*0.5 + tw + fillet_radius, -height*0.5 + tf + z + fillet_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( ( -width*0.5 + tw ), ( -height*0.5 + tf + z ) ) );
				}

				// mirror horizontally along x-Axis
				mirrorCopyPathReverse( outer_loop, false, true );
				paths.push_back( outer_loop );
			}
			return;
		}

		// C-shaped profile
		shared_ptr<IfcCShapeProfileDef> c_shape = dynamic_pointer_cast<IfcCShapeProfileDef>( profile );
		if( c_shape )
		{
			if( c_shape->m_Depth && c_shape->m_Width && c_shape->m_Girth && c_shape->m_WallThickness )
			{
				const double h = c_shape->m_Depth->m_value*length_factor;
				const double width = c_shape->m_Width->m_value*length_factor;
				const double g = c_shape->m_Girth->m_value*length_factor;
				const double t = c_shape->m_WallThickness->m_value*length_factor;
				double fillet_radius = 0;
				if( c_shape->m_InternalFilletRadius && !gs->isIgnoreProfileRadius() )
				{
					fillet_radius = c_shape->m_InternalFilletRadius->m_value*length_factor;
				}

				if( fillet_radius != 0 )
				{
					addArc( outer_loop, fillet_radius + t, M_PI, M_PI_2, -width*0.5 + t + fillet_radius, -h*0.5 + t + fillet_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( -width*0.5, -h*0.5 ) );
				}

				if( fillet_radius != 0 )
				{
					addArc( outer_loop, fillet_radius + t, 3 * M_PI_2, M_PI_2, width*0.5 - t - fillet_radius, -h*0.5 + t + fillet_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( width*0.5, -h*0.5 ) );
				}

				outer_loop.push_back( carve::geom::VECTOR( width*0.5, ( -h*0.5 + g ) ) );
				outer_loop.push_back( carve::geom::VECTOR( ( width*0.5 - t ), ( -h*0.5 + g ) ) );

				if( fillet_radius != 0 )
				{
					addArc( outer_loop, fillet_radius, 0, -M_PI_2, width*0.5 - t - fillet_radius, -h*0.5 + t + fillet_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( ( width*0.5 - t ), ( -h*0.5 + t ) ) );
				}

				if( fillet_radius != 0 )
				{
					addArc( outer_loop, fillet_radius, 3 * M_PI_2, -M_PI_2, -width*0.5 + t + fillet_radius, -h*0.5 + t + fillet_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( ( -width*0.5 + t ), ( -h*0.5 + t ) ) );
				}
				// mirror horizontally along x-Axis
				mirrorCopyPathReverse( outer_loop, false, true );
				paths.push_back( outer_loop );
			}
			return;
		}

		// Z-shape profile
		shared_ptr<IfcZShapeProfileDef> z_shape = dynamic_pointer_cast<IfcZShapeProfileDef>( profile );
		if( z_shape )
		{
			if( z_shape->m_Depth && z_shape->m_FlangeWidth && z_shape->m_WebThickness && z_shape->m_FlangeThickness )
			{
				const double h = z_shape->m_Depth->m_value*length_factor;
				const double width = z_shape->m_FlangeWidth->m_value*length_factor;
				const double tw = z_shape->m_WebThickness->m_value*length_factor;
				const double tf = z_shape->m_FlangeThickness->m_value*length_factor;
				double fillet_radius = 0;
				if( z_shape->m_FilletRadius && !gs->isIgnoreProfileRadius() )
				{
					fillet_radius = z_shape->m_FilletRadius->m_value*length_factor;
				}

				double edge_radius = 0;
				if( z_shape->m_EdgeRadius && !gs->isIgnoreProfileRadius() )
				{
					edge_radius = z_shape->m_EdgeRadius->m_value*length_factor;
				}

				outer_loop.push_back( carve::geom::VECTOR( ( -tw*0.5 ), -h*0.5 ) );
				outer_loop.push_back( carve::geom::VECTOR( ( width - tw*0.5 ), -h*0.5 ) );

				if( edge_radius != 0 )
				{
					addArc( outer_loop, edge_radius, 0, M_PI_2, width - tw*0.5 - edge_radius, -h*0.5 + tf - edge_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( ( width - tw*0.5 ), ( -h*0.5 + tf ) ) );
				}

				if( fillet_radius != 0 )
				{
					addArc( outer_loop, fillet_radius, 3 * M_PI_2, -M_PI_2, tw*0.5 + fillet_radius, -h*0.5 + tf + fillet_radius );
				}
				else
				{
					outer_loop.push_back( carve::geom::VECTOR( ( tw*0.5 ), ( -h*0.5 + tf ) ) );
				}

				// mirror horizontally and vertically
				mirrorCopyPath( outer_loop, true, true );
				paths.push_back( outer_loop );
			}
			return;
		}

		// T-shape profile
		shared_ptr<IfcTShapeProfileDef> t_shape = dynamic_pointer_cast<IfcTShapeProfileDef>( profile );
		if( t_shape )
		{
			const double h = t_shape->m_Depth->m_value*length_factor;
			const double width = t_shape->m_FlangeWidth->m_value*length_factor;
			const double tw = t_shape->m_WebThickness->m_value*length_factor*0.5;
			const double tf = t_shape->m_FlangeThickness->m_value*length_factor;

			double fillet_radius = 0;
			if( t_shape->m_FilletRadius && !gs->isIgnoreProfileRadius() )
			{
				fillet_radius = t_shape->m_FilletRadius->m_value*length_factor;
			}

			double flange_edge_radius = 0;
			if( t_shape->m_FlangeEdgeRadius && !gs->isIgnoreProfileRadius() )
			{
				flange_edge_radius = t_shape->m_FlangeEdgeRadius->m_value*length_factor;
			}

			double web_edge_radius = 0;
			if( t_shape->m_WebEdgeRadius && !gs->isIgnoreProfileRadius() )
			{
				web_edge_radius = t_shape->m_WebEdgeRadius->m_value*length_factor;
			}
			double flange_slope = 0;

			if( t_shape->m_FlangeSlope )
			{
				const double angle_factor = uc->getAngleInRadiantFactor();
				flange_slope = t_shape->m_FlangeSlope->m_value*angle_factor;
			}

			double web_slope = 0;
			if( t_shape->m_WebSlope )
			{
				const double angle_factor = uc->getAngleInRadiantFactor();
				web_slope = t_shape->m_WebSlope->m_value*angle_factor;
			}

			outer_loop.push_back( carve::geom::VECTOR( -width*0.5, h*0.5 ) );

			const double zf = tan( flange_slope )*( width*0.25 - flange_edge_radius );
			const double zw = tan( web_slope )*( h*0.5 - web_edge_radius );
			if( flange_edge_radius != 0 )
			{
				addArc( outer_loop, flange_edge_radius, M_PI, M_PI_2 - flange_slope, -width*0.5 + flange_edge_radius, h*0.5 - tf + zf + flange_edge_radius );
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( -width*0.5, ( h*0.5 - tf + zf ) ) );
			}

			const double cf = cos( flange_slope );
			const double sf = sin( flange_slope );
			const double cw = cos( web_slope );
			const double sw = sin( web_slope );
			const double z1 = ( sf*( ( width - 2 * ( fillet_radius + flange_edge_radius + tw - zw ) )*cw - 2 * ( h - web_edge_radius - fillet_radius - tf + zf )*sw ) ) / ( 2 * ( cf*cw - sf*sw ) );
			const double z2 = tan( web_slope )*( h - web_edge_radius - fillet_radius - z1 - tf + zf );
			if( fillet_radius != 0 )
			{
				addArc( outer_loop, fillet_radius, M_PI_2 - flange_slope, -M_PI_2 + flange_slope + web_slope, -tw + zw - z2 - fillet_radius, h*0.5 - tf + zf - z1 - fillet_radius );
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( ( -tw + zw - z2 ), ( h*0.5 - tf + zf - z1 ) ) );
			}

			if( web_edge_radius != 0 )
			{
				double x_center = -tw + zw + web_edge_radius;
				if( x_center > 0 )
				{
					x_center = 0;
				}
				addArc( outer_loop, web_edge_radius, M_PI + web_slope, M_PI_2 - web_slope, x_center, -h*0.5 + web_edge_radius );
				while( outer_loop.size() > 0 )
				{
					if( outer_loop.back().x < 0 )
					{
						break;
					}
					outer_loop.pop_back();
				}
				outer_loop.push_back( carve::geom::VECTOR( 0, -h*0.5 ) );
			}
			else
			{
				outer_loop.push_back( carve::geom::VECTOR( ( -tw + zw ), -h*0.5 ) );
			}

			// mirror vertically along y-Axis
			mirrorCopyPathReverse( outer_loop, true, false );
			paths.push_back( outer_loop );
			return;
		}

		messageCallback( "Profile not supported", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, profile.get() );
	}
	static void deleteLastPointIfEqualToFirst( std::vector<vec2>& coords )
	{
		while( coords.size() > 2 )
		{
			vec2 & first = coords.front();
			vec2 & last = coords.back();

			if( std::abs( first.x - last.x ) < 0.00000001 )
			{
				if( std::abs( first.y - last.y ) < 0.00000001 )
				{
					coords.pop_back();
					continue;
				}
			}
			break;
		}
	}
	void simplifyPaths()
	{
		simplifyPaths( m_paths );
	}
	static void simplifyPaths( std::vector<std::vector<vec2> >& paths )
	{
		for( std::vector<std::vector<vec2> >::iterator it_paths = paths.begin(); it_paths != paths.end(); ++it_paths )
		{
			std::vector<vec2>& path = ( *it_paths );
			if( path.size() < 3 )
			{
				continue;
			}
			simplifyPath( path );
		}
	}
	static void simplifyPath( std::vector<vec2>& path )
	{
		if( path.size() < 3 )
		{
			return;
		}

		for( size_t i = 1; i < path.size(); )
		{
			vec2& previous = path[i - 1];
			vec2& current = path[i];

			vec2 segment1 = current - previous;
			if( segment1.length2() < 0.000000001 )
			{
				path.erase( path.begin() + i );
				continue;
			}
			++i;
		}

		for( size_t i = 1; i < path.size() - 1; )
		{
			vec2& previous = path[i - 1];
			vec2& current = path[i];
			vec2& next = path[i + 1];

			vec2 segment1 = current - previous;
			segment1.normalize();
			vec2 segment2 = next - current;
			segment2.normalize();
			double angle = std::abs( segment1.x*segment2.x + segment1.y*segment2.y );
			if( std::abs( angle - 1 ) < 0.00001 )
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
			vec2& first = path.front();
			vec2& last = path.back();

			if( ( last - first ).length2() < 0.000001 )
			{
				vec2 first_segment = path[1] - first;
				first_segment.normalize();
				vec2 last_segment = last - path[path.size() - 2];
				last_segment.normalize();
				double angle = std::abs( first_segment.x*last_segment.x + first_segment.y*last_segment.y );
				if( std::abs( angle - 1 ) < 0.00001 )
				{
					// remove first and last point
					path.erase( path.begin() );
					path.pop_back();
				}
			}
		}
	}
	void addArc( std::vector<vec2>& coords, double radius, double start_angle, double opening_angle, double x_center, double y_center, int num_segments = 4 ) const
	{
		const shared_ptr<GeometrySettings>& gs = m_curve_converter->getGeomSettings();
		if( !gs )
		{
			return;
		}
		//int num_segments = (int)( std::abs( opening_angle ) / ( 2.0*M_PI )*gs->getNumVerticesPerCircle() ); // TODO: adapt to model size and complexity
		if( num_segments < gs->getMinNumVerticesPerArc() )
		{
			num_segments = gs->getMinNumVerticesPerArc();
		}

		if( num_segments > 100 )
		{
			num_segments = 100;
		}

		double angle = start_angle;
		double angle_delta = opening_angle / (double)( num_segments );
		for( int i = 0; i < num_segments+1; ++i )
		{
			coords.push_back( carve::geom::VECTOR( radius*cos( angle ) + x_center, radius*sin( angle ) + y_center ) );
			angle += angle_delta;
		}
	}

	static void mirrorCopyPath( std::vector<vec2>& coords, bool mirror_on_y_axis, bool mirror_on_x_axis )
	{
		int points_count = coords.size();
		double x, y;
		for( int i = 0; i < points_count; ++i )
		{
			vec2 & p = coords[i];
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
	static void mirrorCopyPathReverse( std::vector<vec2>& coords, bool mirror_on_y_axis, bool mirror_on_x_axis )
	{
		int points_count = coords.size();
		double x, y;
		for( int i = points_count - 1; i >= 0; --i )
		{
			vec2 & p = coords[i];
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
};
