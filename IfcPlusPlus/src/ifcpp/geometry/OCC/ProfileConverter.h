/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
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

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepFilletAPI_MakeFillet2d.hxx>
#include <BRepOffsetAPI_MakeOffset.hxx>
#include <Geom_Circle.hxx>
#include <gp_Trsf2d.hxx>
#include <IntAna2d_AnaIntersection.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopoDS_Wire.hxx>

#include <ifcpp/model/shared_ptr.h>
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
#include "GeometrySettings.h"
#include "PointConverter.h"
#include "CurveConverter.h"
#include "PlacementConverter.h"
#include "SplineConverter.h"
#include "GeomUtils.h"

class ProfileConverter : public StatusCallback
{
public:
	
	const TopoDS_Face& getProfileFace() { return m_profile_face; }
	void clearProfileConverter() { m_profile_face.Nullify(); }

	shared_ptr<CurveConverter>				m_curve_converter;
	shared_ptr<SplineConverter>				m_spline_converter;

protected:
	TopoDS_Face	m_profile_face;
	
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
		shared_ptr<IfcParameterizedProfileDef> parameterized = dynamic_pointer_cast<IfcParameterizedProfileDef>(profile_def);
		if( parameterized )
		{
			convertIfcParameterizedProfileDefWithPosition( parameterized, m_profile_face );
			return;
		}

		shared_ptr<IfcArbitraryClosedProfileDef> arbitrary_closed = dynamic_pointer_cast<IfcArbitraryClosedProfileDef>(profile_def);
		if( arbitrary_closed )
		{
			convertIfcArbitraryClosedProfileDef( arbitrary_closed, m_profile_face );
			return;
		}

		shared_ptr<IfcArbitraryOpenProfileDef> arbitrary_open = dynamic_pointer_cast<IfcArbitraryOpenProfileDef>(profile_def);
		if( arbitrary_open )
		{
			convertIfcArbitraryOpenProfileDef( arbitrary_open, m_profile_face );
			return;
		}

		shared_ptr<IfcCompositeProfileDef> composite = dynamic_pointer_cast<IfcCompositeProfileDef>(profile_def);
		if( composite )
		{
			convertIfcCompositeProfileDef( composite, m_profile_face );
			return;
		}

		shared_ptr<IfcDerivedProfileDef> derived = dynamic_pointer_cast<IfcDerivedProfileDef>(profile_def);
		if( derived )
		{
			convertIfcDerivedProfileDef( derived, m_profile_face );
			return;
		}

		messageCallback( "Profile not supported", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, profile_def.get() );
	}
	void convertIfcArbitraryClosedProfileDef( const shared_ptr<IfcArbitraryClosedProfileDef>& profile, TopoDS_Face& target_face )
	{
		shared_ptr<IfcCurve> outer_curve = profile->m_OuterCurve;
		if( outer_curve )
		{
			TopoDS_Wire curve_wire;
			m_curve_converter->convertIfcCurve( outer_curve, curve_wire );
			GeomUtils::closeWire( curve_wire );

			if( !curve_wire.IsNull() )
			{
				if( !curve_wire.Closed() )
				{
					std::cout << "!curve_wire.Closed()" << std::endl;
				}
				TopoDS_Face face = BRepBuilderAPI_MakeFace( curve_wire );
				if( !face.IsNull() )
				{
					target_face = face;
				}
			}

#ifdef _DEBUG
			Standard_Boolean closed = curve_wire.Closed();
			if( !closed )
			{
				std::cout << "wire not closed" << std::endl;

				std::vector<gp_Pnt> loop_points;
				for( TopExp_Explorer exp( target_face, TopAbs_VERTEX ); exp.More(); exp.Next() )
				{
					TopoDS_Vertex result_vert = TopoDS::Vertex( exp.Current() );
					gp_Pnt point1 = BRep_Tool::Pnt( result_vert );
					loop_points.push_back( point1 );
				}
				GeomDebugUtils::dumpPolyline( loop_points, vec4( 0.5, 0.5, 0.4, 1.0 ), true, true );

				gp_Trsf transform;
				transform.SetTranslation( vec3( 0, 0, 1 ) );
				GeomUtils::applyMatrixToShape( curve_wire, transform );
				GeomDebugUtils::dumpShape( curve_wire, vec4( 0.5, 0.5, 0.4, 1.0 ), true, true );
				GeomDebugUtils::dumpShape( target_face, vec4( 0.5, 0.5, 0.4, 1.0 ), true, true );
			}
#endif
		}

		// IfcArbitraryProfileDefWithVoids
		shared_ptr<IfcArbitraryProfileDefWithVoids> profile_with_voids = dynamic_pointer_cast<IfcArbitraryProfileDefWithVoids>(profile);
		if( profile_with_voids )
		{
			std::vector<shared_ptr<IfcCurve> > inner_curves = profile_with_voids->m_InnerCurves;
			for( size_t ii_inner_curve = 0; ii_inner_curve < inner_curves.size(); ++ii_inner_curve )
			{
				shared_ptr<IfcCurve> inner_ifc_curve = inner_curves[ii_inner_curve];
				TopoDS_Wire inner_curve_polygon;
				m_curve_converter->convertIfcCurve( inner_ifc_curve, inner_curve_polygon );

				BRepBuilderAPI_MakeFace mk_face( target_face );
				// TODO: check normal vector (edge orientation)
				mk_face.Add( TopoDS::Wire( inner_curve_polygon ) );
				target_face = mk_face.Face();
			}

#ifdef _DEBUG
			//GeomDebugUtils::dumpShape( target_face, vec4( 0.5, 0.5, 0.4, 1.0 ), true, true );
#endif
		}
	}
	void convertIfcArbitraryOpenProfileDef( const shared_ptr<IfcArbitraryOpenProfileDef>& profile, TopoDS_Face& face )
	{
		// ENTITY IfcArbitraryOpenProfileDef
		//	SUPERTYPE OF(IfcCenterLineProfileDef)
		//	SUBTYPE OF IfcProfileDef;
		//	Curve	 :	IfcBoundedCurve;

		shared_ptr<IfcCurve> ifc_curve = profile->m_Curve;
		shared_ptr<UnitConverter>& uc = m_curve_converter->m_unit_converter;

		TopoDS_Wire basis_curve_wire;
		m_curve_converter->convertIfcCurve( ifc_curve, basis_curve_wire );

		if( basis_curve_wire.IsNull() )
		{
#ifdef _DEBUG
			std::cout << "IfcCenterLineProfileDef: invalid basis curve";
#endif
			return;
		}

		// IfcCenterLineProfileDef
		shared_ptr<IfcCenterLineProfileDef> center_line_profile_def = dynamic_pointer_cast<IfcCenterLineProfileDef>(profile);
		if( center_line_profile_def )
		{
			if( center_line_profile_def->m_Thickness )
			{
				const double thickness = center_line_profile_def->m_Thickness->m_value * uc->getLengthInMeterFactor();

				BRepOffsetAPI_MakeOffset offset( BRepBuilderAPI_MakeFace( gp_Pln( gp::Origin(), gp::DZ() ) ) );
				offset.AddWire( basis_curve_wire );
				offset.Perform( thickness*0.5 );
				face = BRepBuilderAPI_MakeFace( TopoDS::Wire( offset ) );
			}
		}
		else
		{
			face = BRepBuilderAPI_MakeFace( basis_curve_wire );
		}
	}
	void convertIfcCompositeProfileDef( const shared_ptr<IfcCompositeProfileDef>& composite_profile, TopoDS_Face& face )
	{
		std::vector<int> temploop_counts;
		std::vector<int> tempcontour_counts;

		std::vector<shared_ptr<IfcProfileDef> >& vec_profiles = composite_profile->m_Profiles;
		for( size_t ii = 0; ii < vec_profiles.size(); ++ii )
		{
			const shared_ptr<IfcProfileDef>& profile_def = vec_profiles[ii];

			shared_ptr<IfcParameterizedProfileDef> parameterized = dynamic_pointer_cast<IfcParameterizedProfileDef>(profile_def);
			if( parameterized )
			{
				convertIfcParameterizedProfileDefWithPosition( parameterized, face );
				continue;
			}

			shared_ptr<IfcArbitraryOpenProfileDef> open = dynamic_pointer_cast<IfcArbitraryOpenProfileDef>(profile_def);
			if( open )
			{
				convertIfcArbitraryOpenProfileDef( open, face );
				continue;
			}

			shared_ptr<IfcArbitraryClosedProfileDef> closed = dynamic_pointer_cast<IfcArbitraryClosedProfileDef>(profile_def);
			if( closed )
			{
				convertIfcArbitraryClosedProfileDef( closed, face );
				continue;
			}

			shared_ptr<IfcCompositeProfileDef> composite = dynamic_pointer_cast<IfcCompositeProfileDef>(profile_def);
			if( composite )
			{
				convertIfcCompositeProfileDef( composite, face );
				continue;
			}

			shared_ptr<IfcDerivedProfileDef> derived = dynamic_pointer_cast<IfcDerivedProfileDef>(profile_def);
			if( derived )
			{
				convertIfcDerivedProfileDef( derived, face );
				continue;
			}

			messageCallback( "Profile not supported", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, profile_def.get() );
		}
	}
	void convertIfcDerivedProfileDef( const shared_ptr<IfcDerivedProfileDef>& derived_profile, TopoDS_Face& face )
	{
		const double length_factor = m_curve_converter->m_unit_converter->getLengthInMeterFactor();
		ProfileConverter temp_profiler( m_curve_converter, m_spline_converter );
		temp_profiler.computeProfile( derived_profile->m_ParentProfile );
		TopoDS_Face parent_profile_face = temp_profiler.getProfileFace();

		gp_Trsf transform;
		const shared_ptr<IfcCartesianTransformationOperator2D>& transf_op_2D = derived_profile->m_Operator;
		PlacementConverter::convertTransformationOperator( transf_op_2D, length_factor, transform, this );

		GeomUtils::applyMatrixToShape( parent_profile_face, transform );
		face = parent_profile_face;
	}
	void convertIfcParameterizedProfileDefWithPosition( const shared_ptr<IfcParameterizedProfileDef>& parameterized, TopoDS_Face& face )
	{
		const double length_factor = m_curve_converter->m_unit_converter->getLengthInMeterFactor();
		TopoDS_Face temp_face;
		convertIfcParameterizedProfileDef( parameterized, temp_face );

		// local coordinate system
		if( parameterized->m_Position )
		{
			shared_ptr<IfcAxis2Placement2D> axis2Placement2D = parameterized->m_Position;
			gp_Trsf transform;
			PlacementConverter::convertIfcPlacement( axis2Placement2D, length_factor, transform, this );

			GeomUtils::applyMatrixToShape( temp_face, transform );
			face = temp_face;
		}
		else
		{
			face = temp_face;
		}
	}
	void convertIfcParameterizedProfileDef( const shared_ptr<IfcParameterizedProfileDef>& profile, TopoDS_Face& face )
	{
		//IfcParameterizedProfileDef ABSTRACT SUPERTYPE OF (ONEOF
		//	(IfcCShapeProfileDef, IfcCircleProfileDef, IfcEllipseProfileDef, IfcIShapeProfileDef, IfcLShapeProfileDef,
		//	IfcRectangleProfileDef, IfcTShapeProfileDef, IfcTrapeziumProfileDef, IfcUShapeProfileDef, IfcZShapeProfileDef))

		shared_ptr<UnitConverter>& uc = m_curve_converter->m_unit_converter;
		shared_ptr<GeometrySettings>& gs = m_curve_converter->m_geom_settings;
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

		// Rectangle profile
		shared_ptr<IfcRectangleProfileDef> rectangle_profile = dynamic_pointer_cast<IfcRectangleProfileDef>(profile);
		if( rectangle_profile )
		{
			if( rectangle_profile->m_XDim && rectangle_profile->m_YDim )
			{
				double x_dim = rectangle_profile->m_XDim->m_value*length_factor;
				double y_dim = rectangle_profile->m_YDim->m_value*length_factor;

				if( x_dim < GEOM_PROFILE_SIZE_EPSILON || y_dim < GEOM_PROFILE_SIZE_EPSILON )
				{
					messageCallback( "Invalid profile size", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
					return;
				}

				std::vector<vec2> coords_outer =
				{
					vec2( -x_dim*0.5, -y_dim*0.5 ),
					vec2( x_dim*0.5, -y_dim*0.5 ),
					vec2( x_dim*0.5,  y_dim*0.5 ),
					vec2( -x_dim*0.5,  y_dim*0.5 )
				};

				shared_ptr<IfcRectangleHollowProfileDef> hollow = dynamic_pointer_cast<IfcRectangleHollowProfileDef>(rectangle_profile);
				if( hollow )
				{
					if( hollow->m_WallThickness )
					{
						const double t = hollow->m_WallThickness->m_value*length_factor;
						std::vector<vec2> coords_inner =
						{
							vec2( -x_dim*0.5 + t, -y_dim*0.5 + t ),
							vec2( x_dim*0.5 - t, -y_dim*0.5 + t ),
							vec2( x_dim*0.5 - t,  y_dim*0.5 - t ),
							vec2( -x_dim*0.5 + t,  y_dim*0.5 - t )
						};

						std::vector<TangentialPoint2D> coords_with_radius_outer;
						GeomUtils::createVector2DWithRadius( coords_outer, coords_with_radius_outer );

						std::vector<TangentialPoint2D> coords_with_radius_inner;
						GeomUtils::createVector2DWithRadius( coords_inner, coords_with_radius_inner );

						if( hollow->m_OuterFilletRadius && !m_curve_converter->m_geom_settings->isIgnoreProfileRadius() )
						{
							double outer_fillet_radius = hollow->m_OuterFilletRadius->m_value*length_factor;
							coords_with_radius_outer[0].m_radius = outer_fillet_radius;
							coords_with_radius_outer[1].m_radius = outer_fillet_radius;
							coords_with_radius_outer[2].m_radius = outer_fillet_radius;
							coords_with_radius_outer[3].m_radius = outer_fillet_radius;
						}

						if( hollow->m_InnerFilletRadius && !gs->isIgnoreProfileRadius() )
						{
							double inner_fillet_radius = hollow->m_InnerFilletRadius->m_value*length_factor;
							coords_with_radius_inner[0].m_radius = inner_fillet_radius;
							coords_with_radius_inner[1].m_radius = inner_fillet_radius;
							coords_with_radius_inner[2].m_radius = inner_fillet_radius;
							coords_with_radius_inner[3].m_radius = inner_fillet_radius;
						}

						TopoDS_Face face_outer;
						TopoDS_Face face_inner;
						PointConverter::createFaceFromPoints( coords_with_radius_outer, face_outer );
						PointConverter::createFaceFromPoints( coords_with_radius_inner, face_inner );

						if( face_outer.IsNull() || face_outer.IsNull() )
						{
							return;
						}

						const TopoDS_Wire& outer_wire = ShapeAnalysis::OuterWire( face_outer );
						const TopoDS_Wire& inner_wire = ShapeAnalysis::OuterWire( face_inner );

						BRepBuilderAPI_MakeFace mk_face( outer_wire, false );
						mk_face.Add( inner_wire );

						ShapeFix_Shape sfs( mk_face.Face() );
						sfs.Perform();
						face = TopoDS::Face( sfs.Shape() );
					}
					return;
				}

				// RoundedRectangle
				shared_ptr<IfcRoundedRectangleProfileDef> rounded_rectangle = dynamic_pointer_cast<IfcRoundedRectangleProfileDef>(rectangle_profile);
				if( rounded_rectangle && !gs->isIgnoreProfileRadius() )
				{
					if( rounded_rectangle->m_RoundingRadius )
					{
						double rounding_radius = rounded_rectangle->m_RoundingRadius->m_value*length_factor;

						std::vector<TangentialPoint2D> coords_with_radius;
						GeomUtils::createVector2DWithRadius( coords_outer, coords_with_radius );
						if( rounding_radius > 0 && !gs->isIgnoreProfileRadius() )
						{
							coords_with_radius[0].m_radius = rounding_radius;
							coords_with_radius[1].m_radius = rounding_radius;
							coords_with_radius[2].m_radius = rounding_radius;
							coords_with_radius[3].m_radius = rounding_radius;
						}

						PointConverter::createFaceFromPoints( coords_with_radius, face );
					}
					return;
				}

				// plain rectangle
				PointConverter::createFaceFromPoints( coords_outer, face );
				return;
			}
		}

		// Trapezium profile
		shared_ptr<IfcTrapeziumProfileDef> trapezium = dynamic_pointer_cast<IfcTrapeziumProfileDef>(profile);
		if( trapezium )
		{
			if( trapezium->m_BottomXDim && trapezium->m_TopXDim && trapezium->m_TopXOffset && trapezium->m_YDim )
			{
				double x_bottom = trapezium->m_BottomXDim->m_value*length_factor;
				double x_top = trapezium->m_TopXDim->m_value*length_factor;
				double x_offset = trapezium->m_TopXOffset->m_value*length_factor;
				double y_dim = trapezium->m_YDim->m_value*length_factor;
				std::vector<vec2> profile_loop;
				profile_loop.push_back( vec2( -x_bottom*0.5, -y_dim*0.5 ) );
				profile_loop.push_back( vec2( x_bottom*0.5, -y_dim*0.5 ) );
				profile_loop.push_back( vec2( -x_bottom*0.5 + x_offset + x_top, y_dim*0.5 ) );
				profile_loop.push_back( vec2( -x_bottom*0.5 + x_offset, y_dim*0.5 ) );
				PointConverter::createFaceFromPoints( profile_loop, face );
			}
			return;
		}

		// Circle profile
		shared_ptr<IfcCircleProfileDef> circle_profile_def = dynamic_pointer_cast<IfcCircleProfileDef>(profile);
		if( circle_profile_def )
		{
			shared_ptr<IfcPositiveLengthMeasure> radius_measure = circle_profile_def->m_Radius;
			if( !radius_measure )
			{
				messageCallback( "IfcCircleProfileDef: radius not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			double radius = radius_measure->m_value*length_factor;
			if( radius < GEOM_PROFILE_SIZE_EPSILON )
			{
				messageCallback( "Invalid profile size", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			// CircleHollow
			shared_ptr<IfcCircleHollowProfileDef> hollow = dynamic_pointer_cast<IfcCircleHollowProfileDef>(profile);
			if( hollow )
			{
				if( hollow->m_WallThickness )
				{
					double t = hollow->m_WallThickness->m_value*length_factor;

					gp_Dir dir( 0, 0, 1 );
					gp_Pnt point( 0, 0, 0 );
					gp_Circ circle( gp_Ax2( point, dir ), radius );
					BRepBuilderAPI_MakeWire mk_wire;
					mk_wire.Add( BRepBuilderAPI_MakeEdge( circle ) );
					TopoDS_Wire circle_wire = mk_wire.Wire();

#ifdef _DEBUG
					Bnd_Box bbox;
					BRepBndLib::Add( circle_wire, bbox );
					double xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
					bbox.Get( xmin, ymin, zmin, xmax, ymax, zmax );
					// wrong results!! bug open since 2012!!!  https://tracker.dev.opencascade.org/view.php?id=23233
#endif

					TopoDS_Wire inner_circle_wire;
					if( t > 0 )
					{
						radius -= t;
						gp_Circ inner_circle( gp_Ax2( point, dir ), radius );
						BRepBuilderAPI_MakeWire mk_inner_wire;
						mk_inner_wire.Add( BRepBuilderAPI_MakeEdge( inner_circle ) );
						inner_circle_wire = mk_inner_wire.Wire();
					}

					if( !circle_wire.IsNull() )
					{
						if( !inner_circle_wire.IsNull() )
						{
							BRepBuilderAPI_MakeFace mk_face( circle_wire, false );
							mk_face.Add( inner_circle_wire );

							ShapeFix_Shape sfs( mk_face.Face() );
							sfs.Perform();
							face = TopoDS::Face( sfs.Shape() );
						}
						else
						{
							face = BRepBuilderAPI_MakeFace( circle_wire );
						}
					}

					return;
				}
			}


			// regular circle
			BRepBuilderAPI_MakeWire mk_wire;
			Handle_Geom_Circle circle = new Geom_Circle( gp::XOY(), radius );
			TopoDS_Edge edge = BRepBuilderAPI_MakeEdge( circle );
			mk_wire.Add( edge );
			TopoDS_Wire circle_wire = mk_wire.Wire();

			if( !circle_wire.IsNull() )
			{
				face = BRepBuilderAPI_MakeFace( circle_wire );
			}

			return;
		}

		// Ellipse profile
		shared_ptr<IfcEllipseProfileDef> ellipse_profile_def = dynamic_pointer_cast<IfcEllipseProfileDef>(profile);
		if( ellipse_profile_def )
		{
			if( !ellipse_profile_def->m_SemiAxis1 )
			{
				messageCallback( "IfcEllipseProfileDef: axis not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			double x_radius = ellipse_profile_def->m_SemiAxis1->m_value*length_factor;
			double y_radius = x_radius;

			if( ellipse_profile_def->m_SemiAxis2 )
			{
				y_radius = ellipse_profile_def->m_SemiAxis2->m_value*length_factor;
			}
			gp_Dir dir( 0, 0, 1 );
			gp_Ax2 ellipse_axis( gp_Pnt( 0, 0, 0 ), dir );
			GC_MakeEllipse mk_ellipse( ellipse_axis, x_radius, y_radius );

			BRepBuilderAPI_MakeEdge mk_edge( mk_ellipse.Value() );
			TopoDS_Edge edge = mk_edge.Edge();
			face = BRepBuilderAPI_MakeFace( BRepBuilderAPI_MakeWire( edge ) );
			return;
		}

		// I-shaped profile
		shared_ptr<IfcIShapeProfileDef> i_shape = dynamic_pointer_cast<IfcIShapeProfileDef>(profile);
		if( i_shape )
		{
			if( !i_shape->m_OverallDepth || !i_shape->m_OverallWidth || !i_shape->m_WebThickness || !i_shape->m_FlangeThickness )
			{
				messageCallback( "IfcIShapeProfileDef: mandatory attributes not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			const double profile_height = i_shape->m_OverallDepth->m_value*length_factor;
			const double width = i_shape->m_OverallWidth->m_value*length_factor;
			const double t_web = i_shape->m_WebThickness->m_value*length_factor;
			const double t_flange = i_shape->m_FlangeThickness->m_value*length_factor;
			double fillet_radius = 0;
			if( i_shape->m_FilletRadius && !gs->isIgnoreProfileRadius() )
			{
				fillet_radius = i_shape->m_FilletRadius->m_value*length_factor;
			}

			double flange_edge_radius = 0;
			if( i_shape->m_FlangeEdgeRadius && !gs->isIgnoreProfileRadius() )
			{
				flange_edge_radius = i_shape->m_FlangeEdgeRadius->m_value*length_factor;
			}

			shared_ptr<IfcAsymmetricIShapeProfileDef> asym_I_profile = dynamic_pointer_cast<IfcAsymmetricIShapeProfileDef>(i_shape);
			if( asym_I_profile )
			{
				if( asym_I_profile->m_TopFlangeWidth )
				{
					const double width_top = asym_I_profile->m_TopFlangeWidth->m_value*length_factor;
					double t_top_flange = t_flange;

					if( asym_I_profile->m_TopFlangeThickness )
					{
						t_top_flange = asym_I_profile->m_TopFlangeThickness->m_value*length_factor;
					}

					double fillet_radius_top = fillet_radius;
					if( asym_I_profile->m_TopFlangeFilletRadius && !gs->isIgnoreProfileRadius() )
					{
						fillet_radius_top = asym_I_profile->m_TopFlangeFilletRadius->m_value*length_factor;
					}

					if( fillet_radius_top != 0 )
					{

					}
					else
					{

					}

					// TODO: implement

				}
			}

			std::vector<vec2> profile_coords =
			{
				vec2( -width*0.5, -profile_height*0.5 ),
				vec2( width*0.5, -profile_height*0.5 ),
				vec2( width*0.5, -profile_height*0.5 + t_flange ),
				vec2( t_web*0.5, -profile_height*0.5 + t_flange ),
				vec2( t_web*0.5, profile_height*0.5 - t_flange ),
				vec2( width*0.5, profile_height*0.5 - t_flange ),
				vec2( width*0.5, profile_height*0.5 ),
				vec2( -width*0.5, profile_height*0.5 ),
				vec2( -width*0.5, profile_height*0.5 - t_flange ),
				vec2( -t_web*0.5, profile_height*0.5 - t_flange ),
				vec2( -t_web*0.5, -profile_height*0.5 + t_flange ),
				vec2( -width*0.5, -profile_height*0.5 + t_flange )
			};

			std::vector<TangentialPoint2D> coords_with_radius;
			GeomUtils::createVector2DWithRadius( profile_coords, coords_with_radius );

			if( fillet_radius > 0 )
			{
				coords_with_radius[3].m_radius = fillet_radius;
				coords_with_radius[4].m_radius = fillet_radius;
				coords_with_radius[9].m_radius = fillet_radius;
				coords_with_radius[10].m_radius = fillet_radius;
			}

			if( flange_edge_radius > 0 )
			{
				coords_with_radius[2].m_radius = flange_edge_radius;
				coords_with_radius[5].m_radius = flange_edge_radius;
				coords_with_radius[8].m_radius = flange_edge_radius;
				coords_with_radius[11].m_radius = flange_edge_radius;
			}

			PointConverter::createFaceFromPoints( coords_with_radius, face );

			return;
		}

		// L-shaped profile
		shared_ptr<IfcLShapeProfileDef> l_shape = dynamic_pointer_cast<IfcLShapeProfileDef>(profile);
		if( l_shape )
		{
			if( !l_shape->m_Depth || !l_shape->m_Thickness )
			{
				messageCallback( "IfcIShapeProfileDef: mandatory attributes not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}
			//     t
			//p5  ____   p4                         __
			//   |    |                              ^
			//   |    |                              |
			//   |    |                              |
			//   |    |     y ^                      |
			//   |    |       |                      | 
			//   |    |      o-->x                   |
			//   |    |                        profile_height 
			//   |    |                              |
			//   |    |                              |
			//   |    |______________  p2            |
			//   |  p3               |               |
			//   |___________________|              _v_
			// p0        width         p1

			const double profile_height = l_shape->m_Depth->m_value*length_factor;
			double width = profile_height;	// assume equal-sided profile in case no width is given

			if( l_shape->m_Width )
			{
				width = l_shape->m_Width->m_value*length_factor;
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

			double leg_slope_radiant = 0;
			if( l_shape->m_LegSlope )
			{
				const double angle_factor = uc->getAngleInRadiantFactor();
				leg_slope_radiant = l_shape->m_LegSlope->m_value*angle_factor;
			}

			if( width*0.5 < GEOM_PROFILE_SIZE_EPSILON || profile_height*0.5 < GEOM_PROFILE_SIZE_EPSILON || t < GEOM_PROFILE_SIZE_EPSILON )
			{
				messageCallback( "Invalid profile size", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			gp_Pnt2d p2( width*0.5, -profile_height*0.5 + t );
			gp_Pnt2d p3( -width*0.5 + t, -profile_height*0.5 + t );
			gp_Pnt2d p4( -width*0.5 + t, profile_height*0.5 );
			if( leg_slope_radiant > 0 )
			{
				gp_Lin2d p2_p3( p2, gp_Dir2d( -1, 0 ) );
				gp_Lin2d p3_p4( p3, gp_Dir2d( 0, 1 ) );

				// slope starts at p2
				gp_Trsf2d rotate_p2_p3;
				rotate_p2_p3.SetRotation( p2, -leg_slope_radiant);
				p2_p3.Transform( rotate_p2_p3 );

				gp_Trsf2d rotate_p3_p4;
				rotate_p3_p4.SetRotation( p4, leg_slope_radiant );
				p3_p4.Transform( rotate_p3_p4 );

				IntAna2d_AnaIntersection intersection( p2_p3, p3_p4 );
				if( intersection.IsDone() && intersection.NbPoints() == 1 )
				{
					p3 = intersection.Point( 1 ).Value();
				}
			}

			std::vector<vec2> profile_coords =
			{
				vec2( -width*0.5,-profile_height*0.5 ),
				vec2( width*0.5,-profile_height*0.5 ),
				vec2( p2.X(), p2.Y() ),
				vec2( p3.X(), p3.Y() ),
				vec2( p4.X(), p4.Y() ),
				vec2( -width*0.5 + t,profile_height*0.5 ),
				vec2( -width*0.5,profile_height*0.5 )
			};

			std::vector<TangentialPoint2D> coords_with_radius;
			GeomUtils::createVector2DWithRadius( profile_coords, coords_with_radius );

			if( fillet_radius > 0 )
			{
				coords_with_radius[3].m_radius = fillet_radius;
			}

			if( edge_radius > 0 )
			{
				coords_with_radius[2].m_radius = edge_radius;
				coords_with_radius[4].m_radius = edge_radius;
			}

			PointConverter::createFaceFromPoints( coords_with_radius, face );

			return;
		}

		// U-shaped profile
		shared_ptr<IfcUShapeProfileDef> u_shape = dynamic_pointer_cast<IfcUShapeProfileDef>(profile);
		if( u_shape )
		{
			if( !u_shape->m_Depth || !u_shape->m_FlangeWidth || !u_shape->m_WebThickness || !u_shape->m_FlangeThickness )
			{
				messageCallback( "IfcUShapeProfileDef: mandatory attributes not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}
			const double height = u_shape->m_Depth->m_value*length_factor;
			const double width = u_shape->m_FlangeWidth->m_value*length_factor;
			const double t_web = u_shape->m_WebThickness->m_value*length_factor;
			const double t_flange = u_shape->m_FlangeThickness->m_value*length_factor;
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

			if( width < GEOM_PROFILE_SIZE_EPSILON || height < GEOM_PROFILE_SIZE_EPSILON || t_web < GEOM_PROFILE_SIZE_EPSILON || t_flange < GEOM_PROFILE_SIZE_EPSILON )
			{
				messageCallback( "Invalid profile size", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			//  p7_____________________ p6            __
			//   |                     |               ^
			//   |   p4________________|               |
			//   |    |      p45       p5              |
			//   |    |
			//   |    |
			//   |    |
			//   |    |       ^ y
			//   |    |       |                       height
			//   |    |       o--> x
			//   |    |
			//   |    |  t_web
			//   |    |
			//   |    |
			//   |    |_______p23______ p2             |
			//   |   p3                |  t_flange     |
			//   |_____________________|              _v_
			//  p0                      p1
			//        m_FlangeWidth

			gp_Pnt2d p0( -width*0.5, -height*0.5 );
			gp_Pnt2d p1( width*0.5, -height*0.5 );
			gp_Pnt2d p2( width*0.5, -height*0.5 + t_flange );
			gp_Pnt2d p3( -width*0.5 + t_web, -height*0.5 + t_flange );


			if( u_shape->m_FlangeSlope )
			{
				const double angle_factor = uc->getAngleInRadiantFactor();
				double slope = u_shape->m_FlangeSlope->m_value*angle_factor;

				// slope starts at p23 in lower flange
				gp_Lin2d p1_p2( p1, gp_Dir2d( 0, 1 ) );
				gp_Lin2d p2_p3( p2, gp_Dir2d( -1, 0 ) );
				gp_Pnt2d p23( (p2.X() + p3.X())*0.5, (p2.Y() + p3.Y())*0.5 );
				gp_Trsf2d rotate_p2_p3;
				rotate_p2_p3.SetRotation( p23, -slope );
				p2_p3.Transform( rotate_p2_p3 );

				IntAna2d_AnaIntersection intersection( p1_p2, p2_p3 );
				if( intersection.IsDone() && intersection.NbPoints() == 1 )
				{
					p2 = intersection.Point( 1 ).Value();
				}

				gp_Lin2d p3_p4( p3, gp_Dir2d( 0, 1 ) );
				intersection.Perform( p2_p3, p3_p4 );
				if( intersection.IsDone() && intersection.NbPoints() == 1 )
				{
					p3 = intersection.Point( 1 ).Value();
				}
			}
			gp_Pnt2d p4( p3.X(), -p3.Y() );
			gp_Pnt2d p5( p2.X(), -p2.Y() );
			gp_Pnt2d p6( p1.X(), -p1.Y() );
			gp_Pnt2d p7( p0.X(), -p0.Y() );

			std::vector<vec2> profile_coords =
			{
				vec2( p0.X(), p0.Y() ),
				vec2( p1.X(), p1.Y() ),
				vec2( p2.X(), p2.Y() ),
				vec2( p3.X(), p3.Y() ),
				vec2( p4.X(), p4.Y() ),
				vec2( p5.X(), p5.Y() ),
				vec2( p6.X(), p6.Y() ),
				vec2( p7.X(), p7.Y() )
			};

			std::vector<TangentialPoint2D> coords_with_radius;
			GeomUtils::createVector2DWithRadius( profile_coords, coords_with_radius );

			if( fillet_radius > 0 )
			{
				coords_with_radius[3].m_radius = fillet_radius;
				coords_with_radius[4].m_radius = fillet_radius;
			}

			if( edge_radius > 0 )
			{
				coords_with_radius[2].m_radius = edge_radius;
				coords_with_radius[5].m_radius = edge_radius;
			}

			PointConverter::createFaceFromPoints( coords_with_radius, face );

			return;
		}

		// C-shaped profile
		shared_ptr<IfcCShapeProfileDef> c_shape = dynamic_pointer_cast<IfcCShapeProfileDef>(profile);
		if( c_shape )
		{
			if( !c_shape->m_Depth || !c_shape->m_Width || !c_shape->m_Girth || !c_shape->m_WallThickness )
			{
				messageCallback( "IfcCShapeProfileDef: mandatory attributes not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			const double profile_height = c_shape->m_Depth->m_value*length_factor;
			const double width = c_shape->m_Width->m_value*length_factor;
			const double girth = c_shape->m_Girth->m_value*length_factor;
			const double t = c_shape->m_WallThickness->m_value*length_factor;
			double fillet_radius = 0;
			if( c_shape->m_InternalFilletRadius && !gs->isIgnoreProfileRadius() )
			{
				fillet_radius = c_shape->m_InternalFilletRadius->m_value*length_factor;
			}

			if( width*0.5 < GEOM_PROFILE_SIZE_EPSILON || profile_height*0.5 < GEOM_PROFILE_SIZE_EPSILON || t < GEOM_PROFILE_SIZE_EPSILON || girth < GEOM_PROFILE_SIZE_EPSILON )
			{
				messageCallback( "Invalid profile size", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			std::vector<vec2> profile_coords =
			{
				vec2( -width*0.5,	-profile_height*0.5 ),
				vec2( width*0.5,	-profile_height*0.5 ),
				vec2( width*0.5,	-profile_height*0.5 + girth ),
				vec2( width*0.5 - t,-profile_height*0.5 + girth ),
				vec2( width*0.5 - t,-profile_height*0.5 + t ),
				vec2( -width*0.5 + t,-profile_height*0.5 + t ),
				vec2( -width*0.5 + t,profile_height*0.5 - t ),
				vec2( width*0.5 - t,profile_height*0.5 - t ),
				vec2( width*0.5 - t,profile_height*0.5 - girth ),
				vec2( width*0.5,	profile_height*0.5 - girth ),
				vec2( width*0.5,	profile_height*0.5 ),
				vec2( -width*0.5,	profile_height*0.5 )
			};

			std::vector<TangentialPoint2D> coords_with_radius;
			GeomUtils::createVector2DWithRadius( profile_coords, coords_with_radius );
			if( fillet_radius > 0 )
			{
				double outer_fillet_radius = fillet_radius + t;
				coords_with_radius[0].m_radius = outer_fillet_radius;
				coords_with_radius[1].m_radius = outer_fillet_radius;
				coords_with_radius[4].m_radius = fillet_radius;
				coords_with_radius[5].m_radius = fillet_radius;
				coords_with_radius[6].m_radius = fillet_radius;
				coords_with_radius[7].m_radius = fillet_radius;
				coords_with_radius[10].m_radius = outer_fillet_radius;
				coords_with_radius[11].m_radius = outer_fillet_radius;
			}

			PointConverter::createFaceFromPoints( coords_with_radius, face );

			return;
		}

		// Z-shape profile
		shared_ptr<IfcZShapeProfileDef> z_shape = dynamic_pointer_cast<IfcZShapeProfileDef>(profile);
		if( z_shape )
		{
			if( !z_shape->m_Depth || !z_shape->m_FlangeWidth || !z_shape->m_WebThickness || !z_shape->m_FlangeThickness )
			{
				messageCallback( "IfcCShapeProfileDef: mandatory attributes not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}
			const double profile_height = z_shape->m_Depth->m_value*length_factor;
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

			return;
		}

		// T-shape profile
		shared_ptr<IfcTShapeProfileDef> t_shape = dynamic_pointer_cast<IfcTShapeProfileDef>(profile);
		if( t_shape )
		{
			if( !t_shape->m_Depth || !t_shape->m_FlangeWidth || !t_shape->m_WebThickness || !t_shape->m_FlangeThickness )
			{
				messageCallback( "IfcTShapeProfileDef: mandatory attributes not set", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			const double profile_height = t_shape->m_Depth->m_value*length_factor;
			const double width = t_shape->m_FlangeWidth->m_value*length_factor;
			const double t_web = t_shape->m_WebThickness->m_value*length_factor;
			const double t_flange = t_shape->m_FlangeThickness->m_value*length_factor;

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

			if( width*0.5 < GEOM_PROFILE_SIZE_EPSILON || profile_height*0.5 < GEOM_PROFILE_SIZE_EPSILON || t_web < GEOM_PROFILE_SIZE_EPSILON || t_flange < GEOM_PROFILE_SIZE_EPSILON )
			{
				messageCallback( "Invalid profile size", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, profile.get() );
				return;
			}

			//    p0_____________________________p7
			//     |                             |   t_flange
			//     |____p12___p2   p5____________|
			//    p1          |     |            p6
			//                |  ^y |
			//                |  |  |
			//                |  o--|-> x
			//              p23     |
			//                |     |
			//                |     |
			//                |     |
			//                |_____|
			//                p3     p4
			//                   t_web
			
			gp_Pnt2d p0( -width*0.5, profile_height*0.5 );
			gp_Pnt2d p1( -width*0.5, profile_height*0.5 - t_flange );
			gp_Pnt2d p2( -t_web*0.5, profile_height*0.5 - t_flange );
			gp_Pnt2d p3( -t_web*0.5, -profile_height*0.5 );

			if( flange_slope > 0 || web_slope > 0 )
			{
				// slope starts at p12 in flange
				gp_Lin2d p1_p2( p1, gp_Dir2d( 1, 0 ) );
				gp_Pnt2d p12( (p1.X() + p2.X())*0.5, (p1.Y() + p2.Y())*0.5 );
				
				if( flange_slope > 0 )
				{
					gp_Trsf2d rotate_p1_p2;
					rotate_p1_p2.SetRotation( p12, -flange_slope );
					p1_p2.Transform( rotate_p1_p2 );
				}

				gp_Lin2d p2_p3( p2, gp_Dir2d( 0, -1 ) );
				gp_Pnt2d p23( (p2.X() + p3.X())*0.5, (p2.Y() + p3.Y())*0.5 );
				if( web_slope > 0 )
				{
					gp_Trsf2d rotate_p2_p3;
					rotate_p2_p3.SetRotation( p23, web_slope );
					p2_p3.Transform( rotate_p2_p3 );
				}

				IntAna2d_AnaIntersection intersection( p1_p2, p2_p3 );
				if( intersection.IsDone() && intersection.NbPoints() == 1 )
				{
					p2 = intersection.Point( 1 ).Value();
				}

				gp_Lin2d p0_p1( p0, gp_Dir2d( 0, -1 ) );
				intersection.Perform( p0_p1, p1_p2 );
				if( intersection.IsDone() && intersection.NbPoints() == 1 )
				{
					p1 = intersection.Point( 1 ).Value();
				}

				gp_Lin2d p3_p4( p3, gp_Dir2d( 1, 0 ) );
				intersection.Perform( p2_p3, p3_p4 );
				if( intersection.IsDone() && intersection.NbPoints() == 1 )
				{
					p3 = intersection.Point( 1 ).Value();
				}
			}
			gp_Pnt2d p4( -p3.X(), p3.Y() );
			gp_Pnt2d p5( -p2.X(), p2.Y() );
			gp_Pnt2d p6( -p1.X(), p1.Y() );
			gp_Pnt2d p7( -p0.X(), p0.Y() );

			std::vector<vec2> profile_coords =
			{
				vec2( p0.X(), p0.Y() ),
				vec2( p1.X(), p1.Y() ),
				vec2( p2.X(), p2.Y() ),
				vec2( p3.X(), p3.Y() ),
				vec2( p4.X(), p4.Y() ),
				vec2( p5.X(), p5.Y() ),
				vec2( p6.X(), p6.Y() ),
				vec2( p7.X(), p7.Y() )
			};

			std::vector<TangentialPoint2D> coords_with_radius;
			GeomUtils::createVector2DWithRadius( profile_coords, coords_with_radius );

			if( fillet_radius > 0 )
			{
				coords_with_radius[2].m_radius = fillet_radius;
				coords_with_radius[5].m_radius = fillet_radius;
			}

			if( flange_edge_radius > 0 )
			{
				coords_with_radius[1].m_radius = flange_edge_radius;
				coords_with_radius[6].m_radius = flange_edge_radius;
			}

			if( web_edge_radius > 0 )
			{
				coords_with_radius[3].m_radius = web_edge_radius;
				coords_with_radius[4].m_radius = web_edge_radius;
			}

			PointConverter::createFaceFromPoints( coords_with_radius, face );
			
			return;
		}

		messageCallback( "Profile not supported", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, profile.get() );
	}
};
