/* -*-c++-*- IFC++ www.ifcquery.com
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

#include <BRepBndLib.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <BRepAlgo_NormalProjection.hxx>
#include <BRepGProp.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <GProp_GProps.hxx>
#include <ProjLib_ProjectedCurve.hxx>
#include <ShapeAnalysis.hxx>
#include <TopoDS.hxx>

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/IfcPPBasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/UnitConverter.h>

#include <ifcpp/IFC4/include/IfcAdvancedBrepWithVoids.h>
#include <ifcpp/IFC4/include/IfcBlock.h>
#include <ifcpp/IFC4/include/IfcBooleanResult.h>
#include <ifcpp/IFC4/include/IfcBooleanOperand.h>
#include <ifcpp/IFC4/include/IfcBooleanOperator.h>
#include <ifcpp/IFC4/include/IfcBooleanClippingResult.h>
#include <ifcpp/IFC4/include/IfcBoxedHalfSpace.h>
#include <ifcpp/IFC4/include/IfcCsgPrimitive3D.h>
#include <ifcpp/IFC4/include/IfcCsgSolid.h>
#include <ifcpp/IFC4/include/IfcExtrudedAreaSolid.h>
#include <ifcpp/IFC4/include/IfcFacetedBrep.h>
#include <ifcpp/IFC4/include/IfcFixedReferenceSweptAreaSolid.h>
#include <ifcpp/IFC4/include/IfcHalfSpaceSolid.h>
#include <ifcpp/IFC4/include/IfcManifoldSolidBrep.h>
#include <ifcpp/IFC4/include/IfcPolygonalBoundedHalfSpace.h>
#include <ifcpp/IFC4/include/IfcRectangularPyramid.h>
#include <ifcpp/IFC4/include/IfcRevolvedAreaSolid.h>
#include <ifcpp/IFC4/include/IfcRightCircularCone.h>
#include <ifcpp/IFC4/include/IfcRightCircularCylinder.h>
#include <ifcpp/IFC4/include/IfcSectionedSpine.h>
#include <ifcpp/IFC4/include/IfcSolidModel.h>
#include <ifcpp/IFC4/include/IfcSphere.h>
#include <ifcpp/IFC4/include/IfcSurfaceCurveSweptAreaSolid.h>
#include <ifcpp/IFC4/include/IfcSweptDiskSolid.h>

#include "GeometryInputData.h"
#include "PointConverter.h"
#include "ProfileCache.h"
#include "FaceConverter.h"
#include "CurveConverter.h"
#include "CSG_Adapter.h"

class IFCPP_EXPORT SolidModelConverter : public StatusCallback
{
public:
	shared_ptr<GeometrySettings>		m_geom_settings;
	shared_ptr<UnitConverter>			m_unit_converter;
	shared_ptr<CurveConverter>			m_curve_converter;
	shared_ptr<FaceConverter>			m_face_converter;
	shared_ptr<ProfileCache>			m_profile_cache;

	SolidModelConverter( shared_ptr<GeometrySettings>& gs, shared_ptr<UnitConverter>& uc, shared_ptr<CurveConverter>& cc, shared_ptr<FaceConverter>& fc, shared_ptr<ProfileCache>& pcache )
		: m_geom_settings( gs ), m_unit_converter( uc ), m_curve_converter( cc ), m_face_converter( fc ), m_profile_cache( pcache )
	{
	}

	virtual ~SolidModelConverter(){}

	// ENTITY IfcSolidModel ABSTRACT SUPERTYPE OF(ONEOF(IfcCsgSolid, IfcManifoldSolidBrep, IfcSweptAreaSolid, IfcSweptDiskSolid))
	void convertIfcSolidModel( const shared_ptr<IfcSolidModel>& solid_model, shared_ptr<ItemShapeData>& item_data )
	{
		const int nvc = m_geom_settings->getNumVerticesPerCircle();
		const double length_in_meter = m_unit_converter->getLengthInMeterFactor();

		shared_ptr<IfcSweptAreaSolid> swept_area_solid = dynamic_pointer_cast<IfcSweptAreaSolid>(solid_model);
		if( swept_area_solid )
		{
			//ENTITY IfcSweptAreaSolid
			//	ABSTRACT SUPERTYPE OF(ONEOF(IfcExtrudedAreaSolid, IfcFixedReferenceSweptAreaSolid, IfcRevolvedAreaSolid, IfcSurfaceCurveSweptAreaSolid))
			//	SUBTYPE OF IfcSolidModel;
			//	SweptArea	 :	IfcProfileDef;
			//	Position	 :	OPTIONAL IfcAxis2Placement3D;
			//	WHERE
			//	SweptAreaType	 :	SweptArea.ProfileType = IfcProfileTypeEnum.Area;
			//END_ENTITY;

			shared_ptr<IfcProfileDef>& swept_area = swept_area_solid->m_SweptArea;
			if( !swept_area )
			{
				messageCallback( "SweptArea not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, swept_area_solid.get() );
				return;
			}

			shared_ptr<ItemShapeData> item_data_solid( new ItemShapeData() );  // TODO create only before actually necessary
			if( !item_data_solid )
			{
				throw IfcPPOutOfMemoryException( __FUNC__ );
			}

			// check if local coordinate system is specified for extrusion
			gp_Trsf swept_area_pos;
			if( swept_area_solid->m_Position )
			{
				double length_factor = m_unit_converter->getLengthInMeterFactor();
				shared_ptr<IfcAxis2Placement3D> swept_area_position = swept_area_solid->m_Position;
				PlacementConverter::convertIfcAxis2Placement3D( swept_area_position, length_factor, swept_area_pos );
			}

			shared_ptr<IfcExtrudedAreaSolid> extruded_area = dynamic_pointer_cast<IfcExtrudedAreaSolid>(swept_area_solid);
			if( extruded_area )
			{
				convertIfcExtrudedAreaSolid( extruded_area, item_data_solid );
				item_data_solid->applyTransformToItem( swept_area_pos );
				item_data->addItemData( item_data_solid );
				return;
			}

			shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter( swept_area );
			TopoDS_Face swept_area_face = profile_converter->getProfileFace();

			shared_ptr<IfcFixedReferenceSweptAreaSolid> fixed_reference_swept_area_solid = dynamic_pointer_cast<IfcFixedReferenceSweptAreaSolid>(swept_area_solid);
			if( fixed_reference_swept_area_solid )
			{
				//Directrix	 : OPTIONAL IfcCurve;
				//StartParam	 : OPTIONAL IfcParameterValue;
				//EndParam	 : OPTIONAL IfcParameterValue;
				//FixedReference	 : IfcDirection;

				shared_ptr<IfcCurve>& ifc_directrix_curve = fixed_reference_swept_area_solid->m_Directrix;
				//shared_ptr<IfcParameterValue>& ifc_start_param = fixed_reference_swept_area_solid->m_StartParam;				//optional
				//shared_ptr<IfcParameterValue>& ifc_end_param = fixed_reference_swept_area_solid->m_EndParam;					//optional
				//shared_ptr<IfcDirection>& ifc_fixed_reference = fixed_reference_swept_area_solid->m_FixedReference;				// TODO: apply fixed reference
				messageCallback( "IfcFixedReferenceSweptAreaSolid: Fixed reference not implemented", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, fixed_reference_swept_area_solid.get() );

				TopoDS_Wire directrix_wire;
				m_curve_converter->convertIfcCurve( ifc_directrix_curve, directrix_wire );

#ifdef IFCPP_GEOM_DEBUG
				GeomDebugDump::dumpShape( directrix_wire, vec4( 0.5, 0.5, 0.3, 1.0 ), true, true );
#endif

				gp_Trsf directrix, position;
				gp_Pnt directrix_origin;
				gp_Vec directrix_tangent;

				TopExp_Explorer exp( directrix_wire, TopAbs_EDGE );
				TopoDS_Edge edge = TopoDS::Edge( exp.Current() );
				double u0, u1;
				Handle_Geom_Curve geom_curve = BRep_Tool::Curve( edge, u0, u1 );
				geom_curve->D1( u0, directrix_origin, directrix_tangent );

				if( gp_Dir( 0, 0, 1 ).IsNormal( directrix_tangent, Precision::Approximation() ) )
				{
					directrix.SetTransformation( gp_Ax3( directrix_origin, directrix_tangent, gp_Dir( 0, 0, 1 ) ), gp::XOY() );
				}
				else
				{
					directrix.SetTransformation( gp_Ax3( directrix_origin, directrix_tangent ), gp::XOY() );
				}
				swept_area_face = TopoDS::Face( BRepBuilderAPI_Transform( swept_area_face, directrix ) );

				BRepOffsetAPI_MakePipeShell mk_sweep( directrix_wire );
				mk_sweep.Add( swept_area_face );
				mk_sweep.SetTransitionMode( BRepBuilderAPI_RightCorner );
				//if( directrix_on_plane )
				{
					mk_sweep.SetMode( gp_Dir( 0, 0, 1 ) );
				}
				mk_sweep.Build();
				mk_sweep.MakeSolid();
				TopoDS_Shape shape = mk_sweep.Shape();
				GeomUtils::applyMatrixToShape( shape, swept_area_pos );
				item_data_solid->addShape( shape );
				item_data->addItemData( item_data_solid );
				return;
			}

			shared_ptr<IfcRevolvedAreaSolid> revolved_area_solid = dynamic_pointer_cast<IfcRevolvedAreaSolid>(swept_area_solid);
			if( revolved_area_solid )
			{
				convertIfcRevolvedAreaSolid( revolved_area_solid, item_data_solid );
				item_data_solid->applyTransformToItem( swept_area_pos );
				item_data->addItemData( item_data_solid );
				return;
			}

			shared_ptr<IfcSurfaceCurveSweptAreaSolid> surface_curve_swept_area_solid = dynamic_pointer_cast<IfcSurfaceCurveSweptAreaSolid>(swept_area_solid);
			if( surface_curve_swept_area_solid )
			{
				shared_ptr<IfcCurve>& ifc_directrix_curve = surface_curve_swept_area_solid->m_Directrix;
				//shared_ptr<IfcParameterValue>& ifc_start_param = surface_curve_swept_area_solid->m_StartParam;				//optional
				//shared_ptr<IfcParameterValue>& ifc_end_param = surface_curve_swept_area_solid->m_EndParam;					//optional
				shared_ptr<IfcSurface>& ifc_reference_surface = surface_curve_swept_area_solid->m_ReferenceSurface;			// TODO: apply start_param, end_param

				TopoDS_Wire directrix_wire;
				m_curve_converter->convertIfcCurve( ifc_directrix_curve, directrix_wire );

				// reference surface
				Handle_Geom_Surface geom_surface;
				TopoDS_Face ref_face;
				m_face_converter->convertIfcSurface( ifc_reference_surface, geom_surface, ref_face );

				TopExp_Explorer exp( directrix_wire, TopAbs_EDGE );
				TopoDS_Edge edge = TopoDS::Edge( exp.Current() );
				if( !edge.IsNull() )
				{
					// find position and direction of beginning of directrix curve
					double u0, u1;
					gp_Pnt directrix_origin;
					gp_Vec directrix_tangent;
					Handle_Geom_Curve directrix_geom_curve = BRep_Tool::Curve( edge, u0, u1 );
					directrix_geom_curve->D1( u0, directrix_origin, directrix_tangent );

					gp_Trsf directrix_transform;
					if( gp_Dir( 0, 0, 1 ).IsNormal( directrix_tangent, Precision::Approximation() ) )
					{
						directrix_transform.SetTransformation( gp_Ax3( directrix_origin, directrix_tangent, gp_Dir( 0, 0, 1 ) ), gp::XOY() );
					}
					else
					{
						directrix_transform.SetTransformation( gp_Ax3( directrix_origin, directrix_tangent ), gp::XOY() );
					}
					// rotate swept area face into directrix tangent
					swept_area_face = TopoDS::Face( BRepBuilderAPI_Transform( swept_area_face, directrix_transform ) );
				}

				try
				{
					BRepOffsetAPI_MakePipeShell builder( directrix_wire );
					const TopoDS_Wire& outer_wire = ShapeAnalysis::OuterWire( swept_area_face );
					builder.Add( outer_wire );
					builder.SetTransitionMode( BRepBuilderAPI_RightCorner );
					builder.Build();
					builder.MakeSolid();
					TopoDS_Shape shape = builder.Shape();

					GeomUtils::applyMatrixToShape( shape, swept_area_pos );

					item_data_solid->addShape( shape );
					item_data->addItemData( item_data_solid );
				}
				catch( Standard_Failure& sf )
				{
					messageCallback( sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, solid_model.get() );

#ifdef IFCPP_GEOM_DEBUG
					std::cout << sf.GetMessageString() << std::endl;
					GeomDebugDump::dumpShape( directrix_wire, vec4( 0.5, 0.5, 0.3, 1.0 ), true, false );
					GeomDebugDump::dumpShape( swept_area_face, vec4( 0.5, 0.5, 0.3, 1.0 ), true, false );
#endif
				}
				return;
			}

			messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, solid_model.get() );
		}

		shared_ptr<IfcManifoldSolidBrep> manifold_solid_brep = dynamic_pointer_cast<IfcManifoldSolidBrep>(solid_model);
		if( manifold_solid_brep )
		{
			//ENTITY IfcManifoldSolidBrep 
			//	ABSTRACT SUPERTYPE OF(ONEOF(IfcAdvancedBrep, IfcFacetedBrep))
			//	SUBTYPE OF IfcSolidModel;
			//		Outer	 :	IfcClosedShell;
			//END_ENTITY;

			shared_ptr<IfcClosedShell>& outer_shell = manifold_solid_brep->m_Outer;
			if( outer_shell )
			{
				// first convert outer shell
				std::vector<shared_ptr<IfcFace> >& vec_faces_outer_shell = outer_shell->m_CfsFaces;
				shared_ptr<ItemShapeData> item_data_shell( new ItemShapeData() );
				m_face_converter->convertIfcFaceList( vec_faces_outer_shell, item_data_shell, FaceConverter::CLOSED_SHELL );
				item_data->addItemData( item_data_shell );
			}

			shared_ptr<IfcFacetedBrep> faceted_brep = dynamic_pointer_cast<IfcFacetedBrep>(manifold_solid_brep);
			if( faceted_brep )
			{
				// no additional attributes
				return;
			}

			shared_ptr<IfcAdvancedBrep> advanced_brep = dynamic_pointer_cast<IfcAdvancedBrep>(manifold_solid_brep);
			if( advanced_brep )
			{
				// ENTITY IfcAdvancedBrep	SUPERTYPE OF(IfcAdvancedBrepWithVoids)
				if( dynamic_pointer_cast<IfcAdvancedBrepWithVoids>(advanced_brep) )
				{
					//shared_ptr<IfcAdvancedBrepWithVoids> advanced_brep_with_voids = dynamic_pointer_cast<IfcAdvancedBrepWithVoids>( solid_model );
					//std::vector<shared_ptr<IfcClosedShell> >& vec_voids = advanced_brep_with_voids->m_Voids;

					// TODO: subtract voids from outer shell
#ifdef _DEBUG
					std::cout << "IfcAdvancedBrep not implemented" << std::endl;
#endif
				}
				return;
			}

			messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, solid_model.get() );
		}

		shared_ptr<IfcCsgSolid> csg_solid = dynamic_pointer_cast<IfcCsgSolid>(solid_model);
		if( csg_solid )
		{
			shared_ptr<IfcCsgSelect> csg_select = csg_solid->m_TreeRootExpression;

			shared_ptr<IfcBooleanResult> csg_select_boolean_result = dynamic_pointer_cast<IfcBooleanResult>(csg_select);
			if( csg_select_boolean_result )
			{
				shared_ptr<ItemShapeData> item_data_boolean_result( new ItemShapeData() );
				convertIfcBooleanResult( csg_select_boolean_result, item_data_boolean_result );
				item_data->addItemData( item_data_boolean_result );
			}
			else
			{
				shared_ptr<IfcCsgPrimitive3D> csg_select_primitive_3d = dynamic_pointer_cast<IfcCsgPrimitive3D>(csg_select);
				if( csg_select_primitive_3d )
				{
					shared_ptr<ItemShapeData> item_data_primitive( new ItemShapeData() );
					convertIfcCsgPrimitive3D( csg_select_primitive_3d, item_data_primitive );
					item_data->addItemData( item_data_primitive );
				}
			}
			return;
		}

		shared_ptr<IfcSweptDiskSolid> swept_disp_solid = dynamic_pointer_cast<IfcSweptDiskSolid>(solid_model);
		if( swept_disp_solid )
		{
			//ENTITY IfcSweptDiskSolid;
			//	ENTITY IfcRepresentationItem;
			//	INVERSE
			//		LayerAssignments	 : 	SET OF IfcPresentationLayerAssignment FOR AssignedItems;
			//		StyledByItem	 : 	SET [0:1] OF IfcStyledItem FOR Item;
			//	ENTITY IfcGeometricRepresentationItem;
			//	ENTITY IfcSolidModel;
			//		DERIVE
			//		Dim	 : 	IfcDimensionCount :=  3;
			//	ENTITY IfcSweptDiskSolid;
			//		Directrix	 : 	IfcCurve;
			//		Radius	 : 	IfcPositiveLengthMeasure;
			//		InnerRadius	 : 	OPTIONAL IfcPositiveLengthMeasure;
			//		StartParam	 : 	OPTIONAL IfcParameterValue;
			//		EndParam	 : 	OPTIONAL IfcParameterValue;
			//END_ENTITY;	

			shared_ptr<IfcCurve>& directrix_curve = swept_disp_solid->m_Directrix;
			TopoDS_Wire spine_wire;
			m_curve_converter->convertIfcCurve( directrix_curve, spine_wire );

			double radius = 0.0;
			if( swept_disp_solid->m_Radius )
			{
				radius = swept_disp_solid->m_Radius->m_value*length_in_meter;
			}
			try
			{
				gp_Dir up( 0, 0, 1 );
				gp_Circ circle( gp_Ax2( gp_Pnt( 0, 0, 0 ), up ), radius );
				TopoDS_Edge circle_edge = BRepBuilderAPI_MakeEdge( circle );

				gp_Trsf spine_begin_transform, position;
				gp_Pnt directrix_origin;
				gp_Vec spine_begin_tangent;

				TopExp_Explorer exp( spine_wire, TopAbs_EDGE );
				TopoDS_Edge first_spine_edge = TopoDS::Edge( exp.Current() );
				double u0, u1;
				Handle_Geom_Curve spine_begin_curve = BRep_Tool::Curve( first_spine_edge, u0, u1 );
				spine_begin_curve->D1( u0, directrix_origin, spine_begin_tangent );

				if( up.IsNormal( spine_begin_tangent, Precision::Approximation() ) )
				{
					spine_begin_transform.SetTransformation( gp_Ax3( directrix_origin, spine_begin_tangent, up ), gp::XOY() );
				}
				else
				{
					spine_begin_transform.SetTransformation( gp_Ax3( directrix_origin, spine_begin_tangent ), gp::XOY() );
				}
				circle_edge = TopoDS::Edge( BRepBuilderAPI_Transform( circle_edge, spine_begin_transform ) );

				BRepOffsetAPI_MakePipeShell mk_sweep( spine_wire );
				mk_sweep.SetTransitionMode( BRepBuilderAPI_RoundCorner );
				mk_sweep.Add( BRepBuilderAPI_MakeWire( circle_edge ) );

				double radius_inner = -1.0;
				if( swept_disp_solid->m_InnerRadius )
				{
					radius_inner = swept_disp_solid->m_InnerRadius->m_value*length_in_meter;

					gp_Circ inner_circle( gp_Ax2( gp_Pnt( 0, 0, 0 ), up ), radius_inner );
					BRepBuilderAPI_MakeEdge mk_edge_inner( inner_circle );
					TopoDS_Edge edge_inner = mk_edge_inner.Edge();
					mk_sweep.Add( edge_inner );
				}

				// TODO: handle start param, end param
				mk_sweep.Build();
				mk_sweep.MakeSolid();
				TopoDS_Shape swept_shape = mk_sweep.Shape();
				item_data->addShape( swept_shape );
			}
			catch( Standard_Failure sf )
			{
				messageCallback( sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, solid_model.get() );
#ifdef _DEBUG
				std::cout << sf.GetMessageString() << std::endl;
#endif
			}
			return;
		}

		messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, solid_model.get() );
	}

	void convertIfcExtrudedAreaSolid( const shared_ptr<IfcExtrudedAreaSolid>& extruded_area, shared_ptr<ItemShapeData> item_data )
	{
		if( !extruded_area->m_ExtrudedDirection )
		{
			messageCallback( "Invalid ExtrudedDirection", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, extruded_area.get() );
			return;
		}

		if( !extruded_area->m_Depth )
		{
			messageCallback( "Invalid Depth", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, extruded_area.get() );
			return;
		}
		double length_factor = m_unit_converter->getLengthInMeterFactor();

		// direction and length of extrusion
		const double depth = extruded_area->m_Depth->m_value*length_factor;
		vec3  extrusion_vector;
		std::vector<shared_ptr<IfcReal> >& vec_direction = extruded_area->m_ExtrudedDirection->m_DirectionRatios;
		if( GeomUtils::allPointersValid( vec_direction ) )
		{
			if( vec_direction.size() > 2 )
			{
				extrusion_vector = vec3( vec_direction[0]->m_value * depth, vec_direction[1]->m_value * depth, vec_direction[2]->m_value * depth );
			}
			else if( vec_direction.size() > 1 )
			{
				extrusion_vector = vec3( vec_direction[0]->m_value * depth, vec_direction[1]->m_value * depth, 0 );
			}
		}

		// swept area
		shared_ptr<IfcProfileDef>	swept_area = extruded_area->m_SweptArea;
		if( !swept_area )
		{
			messageCallback( "Invalid SweptArea", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, extruded_area.get() );
			return;
		}
		shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter( swept_area );
		const TopoDS_Face& extrusion_face = profile_converter->getProfileFace();

		if( !extrusion_face.IsNull() )
		{
			try
			{
				TopoDS_Solid extruded_solid = TopoDS::Solid( BRepPrimAPI_MakePrism( extrusion_face, extrusion_vector ) );
				if( !extruded_solid.IsNull() )
				{
					item_data->addShape( extruded_solid );
				}
			}
			catch( Standard_Failure sf )
			{
				messageCallback( sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, extruded_area.get() );
			}
			catch( ... )
			{
				messageCallback( "BRepPrimAPI_MakePrism failed", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, extruded_area.get() );
			}
		}
	}

	void convertRevolvedAreaSolid( const TopoDS_Face& revolving_face, const vec3& axis_location, const vec3& axis_direction, double revolution_angle, shared_ptr<ItemShapeData> item_data, IfcPPEntity* entity_of_origin = nullptr )
	{
		gp_Pnt revolution_axis_location( axis_location.X(), axis_location.Y(), axis_location.Z() );
		gp_Ax1 revolution_axis( revolution_axis_location, gp_Dir( axis_direction.X(), axis_direction.Y(), axis_direction.Z() ) );

		if( revolution_angle > M_PI * 2.0 ) revolution_angle = M_PI * 2.0;
		if( revolution_angle < -M_PI * 2.0 ) revolution_angle = M_PI * 2.0;

		try
		{
			TopoDS_Shape revolved_shape;
			if( revolution_angle >= 2.0*M_PI - GEOM_EPSILON_LENGTH )
			{
				revolved_shape = BRepPrimAPI_MakeRevol( revolving_face, revolution_axis );
			}
			else
			{
				revolved_shape = BRepPrimAPI_MakeRevol( revolving_face, revolution_axis, revolution_angle );
			}
			if( !revolved_shape.IsNull() )
			{
				item_data->addShape( revolved_shape );
			}
		}
		catch( Standard_Failure sf )
		{
			messageCallback( sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
		}
		catch( ... )
		{
			messageCallback( "BRepPrimAPI_MakeRevol failed", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, entity_of_origin );
		}
	}

	void convertIfcRevolvedAreaSolid( const shared_ptr<IfcRevolvedAreaSolid>& revolved_area, shared_ptr<ItemShapeData> item_data )
	{
		// TODO: use Sweeper::sweepArea
		if( !revolved_area )
		{
			messageCallback( "Invalid IfcRevolvedAreaSolid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, revolved_area.get() );
			return;
		}
		if( !revolved_area->m_Angle )
		{
			messageCallback( "Invalid SweptArea", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, revolved_area.get() );
			return;
		}
		shared_ptr<IfcProfileDef> swept_area_profile = revolved_area->m_SweptArea;
		if( !swept_area_profile )
		{
			messageCallback( "Invalid SweptArea", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, revolved_area.get() );
			return;
		}

		// revolution angle
		double angle_factor = m_unit_converter->getAngleInRadiantFactor();
		if( m_unit_converter->getAngularUnit() == UnitConverter::UNDEFINED )
		{
			// angular unit definition not found in model, default to radian
			angle_factor = 1.0;

			if( revolved_area->m_Angle->m_value > M_PI )
			{
				// assume degree
				angle_factor = M_PI / 180.0;
			}
		}
		double revolution_angle = revolved_area->m_Angle->m_value*angle_factor;
		const double length_factor = m_unit_converter->getLengthInMeterFactor();

		// revolution axis
		vec3  axis_location;
		vec3  axis_direction( 1, 0, 0 );
		if( revolved_area->m_Axis )
		{
			const shared_ptr<IfcAxis1Placement>& axis_placement = revolved_area->m_Axis;

			if( axis_placement->m_Location )
			{
				const shared_ptr<IfcCartesianPoint>& location_point = axis_placement->m_Location;
				PointConverter::convertIfcCartesianPoint( location_point, axis_location, length_factor );
			}

			if( axis_placement->m_Axis )
			{
				const shared_ptr<IfcDirection>& axis = axis_placement->m_Axis;
				if( !PointConverter::convertIfcDirection( axis, axis_direction ) )
					axis_direction = vec3( axis->m_DirectionRatios[0]->m_value, axis->m_DirectionRatios[1]->m_value, axis->m_DirectionRatios[2]->m_value );
			}
		}

		// swept area
		shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter( swept_area_profile );
		const TopoDS_Face& extrusion_face = profile_converter->getProfileFace();
		convertRevolvedAreaSolid( extrusion_face, axis_location, axis_direction, revolution_angle, item_data );
	}

	void convertIfcBooleanResult( const shared_ptr<IfcBooleanResult>& bool_result, shared_ptr<ItemShapeData> item_data )
	{
		const shared_ptr<IfcBooleanOperator>& ifc_boolean_operator = bool_result->m_Operator;
		const shared_ptr<IfcBooleanOperand>& ifc_first_operand = bool_result->m_FirstOperand;
		const shared_ptr<IfcBooleanOperand>& ifc_second_operand = bool_result->m_SecondOperand;
		if( !ifc_boolean_operator || !ifc_first_operand || !ifc_second_operand )
		{
			messageCallback( "Invalid IfcBooleanOperator or IfcBooleanOperand", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, bool_result.get() );
			return;
		}
		CSG_Operation csg_op = CSG_A_MINUS_B;

		if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_UNION )
		{
			csg_op = CSG_UNION;
		}
		else if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_INTERSECTION )
		{
			csg_op = CSG_INTERSECTION;
		}
		else if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_DIFFERENCE )
		{
			csg_op = CSG_A_MINUS_B;
		}
		else
		{
			messageCallback( "Invalid IfcBooleanOperator", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, bool_result.get() );
			return;
		}

		// convert the first operand
		shared_ptr<ItemShapeData> first_operand_data( new ItemShapeData() );
		if( !first_operand_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		shared_ptr<ItemShapeData> empty_operand;
		convertIfcBooleanOperand( ifc_first_operand, first_operand_data, empty_operand );

		// convert the second operand
		shared_ptr<ItemShapeData> second_operand_data( new ItemShapeData() );
		if( !second_operand_data )
		{
			throw IfcPPOutOfMemoryException( __FUNC__ );
		}
		convertIfcBooleanOperand( ifc_second_operand, second_operand_data, first_operand_data );

		// for every first operand polyhedrons, apply all second operand polyhedrons
		std::vector<TopoDS_Shape>& vec_first_operand_shapes = first_operand_data->m_shapes;
		for( size_t i_shape_first = 0; i_shape_first < vec_first_operand_shapes.size(); ++i_shape_first )
		{
			TopoDS_Shape& first_operand_shape = vec_first_operand_shapes[i_shape_first];
			if( first_operand_shape.IsNull() )
			{
				messageCallback( "bad first operand", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, bool_result.get() );
				continue;
			}
			const std::vector<TopoDS_Shape>& vec_second_operand_shape = second_operand_data->getShapes();
			for( size_t i_shape_second = 0; i_shape_second < vec_second_operand_shape.size(); ++i_shape_second )
			{
				const TopoDS_Shape& second_operand_shape = vec_second_operand_shape[i_shape_second];
				if( second_operand_shape.IsNull() )
				{
					messageCallback( "bad second operand", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, bool_result.get() );
					continue;
				}

				TopoDS_Shape result;
#ifdef IFCPP_GEOM_DEBUG
				GeomDebugDump::dumpEdgesOfShape( first_operand_shape, vec4( 0.5, 0.5, 0.5, 1.0 ), true, true );
				GeomDebugDump::dumpShape( second_operand_shape, vec4( 0.4, 0.5, 0.6, 1.0 ), true, false );
#endif
				double fuzzy_value = 1.e-7*m_unit_converter->getLengthInMeterFactor();
				CSG_Adapter::computeCSG( first_operand_shape, second_operand_shape, result, csg_op, fuzzy_value, this, bool_result.get() );
				first_operand_shape = result;
			}
		}

		// now copy processed first operands to result input data
		std::copy( first_operand_data->m_shapes.begin(), first_operand_data->m_shapes.end(), std::back_inserter( item_data->m_shapes ) );

		shared_ptr<IfcBooleanClippingResult> boolean_clipping_result = dynamic_pointer_cast<IfcBooleanClippingResult>(bool_result);
		if( boolean_clipping_result )
		{
			// no additional attributes, just the type of operands and operator is restricted:
			// WHERE
			// FirstOperand is IFCSWEPTAREASOLID or IFCSWEPTDISCSOLID or IFCBOOLEANCLIPPINGRESULT
			// SecondOperand is IFCHALFSPACESOLID
			// OperatorType	 :	Operator = DIFFERENCE;
		}
	}

	void convertIfcCsgPrimitive3D( const shared_ptr<IfcCsgPrimitive3D>& csg_primitive, shared_ptr<ItemShapeData> item_data )
	{
		const double length_factor = m_unit_converter->getLengthInMeterFactor();

		// ENTITY IfcCsgPrimitive3D  ABSTRACT SUPERTYPE OF(ONEOF(IfcBlock, IfcRectangularPyramid, IfcRightCircularCone, IfcRightCircularCylinder, IfcSphere
		gp_Trsf primitive_placement_matrix;
		shared_ptr<IfcAxis2Placement3D>& primitive_placement = csg_primitive->m_Position;
		if( primitive_placement )
		{
			PlacementConverter::convertIfcAxis2Placement3D( primitive_placement, length_factor, primitive_placement_matrix );
		}

		if( !item_data )
		{
			messageCallback( "Invalid item_data", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
			return;
		}

		shared_ptr<IfcBlock> block = dynamic_pointer_cast<IfcBlock>(csg_primitive);
		if( block )
		{
			if( !block->m_XLength )
			{
				messageCallback( "Invalid XLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}
			if( !block->m_YLength )
			{
				messageCallback( "Invalid YLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}
			if( !block->m_ZLength )
			{
				messageCallback( "Invalid ZLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}

			const double x_length = block->m_XLength->m_value*0.5*length_factor;
			const double y_length = block->m_YLength->m_value*0.5*length_factor;
			const double z_length = block->m_ZLength->m_value*0.5*length_factor;

			TopoDS_Solid box_solid = BRepPrimAPI_MakeBox( x_length, y_length, z_length );
			GeomUtils::applyMatrixToShape( box_solid, primitive_placement_matrix );
			item_data->addShape( box_solid );
			return;
		}

		shared_ptr<IfcRectangularPyramid> rectangular_pyramid = dynamic_pointer_cast<IfcRectangularPyramid>(csg_primitive);
		if( rectangular_pyramid )
		{
			if( !rectangular_pyramid->m_XLength )
			{
				messageCallback( "Invalid XLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}
			if( !rectangular_pyramid->m_YLength )
			{
				messageCallback( "Invalid YLength", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}
			if( !rectangular_pyramid->m_Height )
			{
				messageCallback( "Invalid Height", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}

			const double dx = rectangular_pyramid->m_XLength->m_value*0.5*length_factor;
			const double dy = rectangular_pyramid->m_YLength->m_value*0.5*length_factor;
			const double dz = rectangular_pyramid->m_Height->m_value*0.5*length_factor;

			TopoDS_Shape pyramid_shape = BRepPrimAPI_MakeWedge( dx, dy, dz, dx*0.5, dz*0.5, dx*0.5, dz*0.5 );
			if( !pyramid_shape.IsNull() )
			{
				GeomUtils::applyMatrixToShape( pyramid_shape, primitive_placement_matrix );
				item_data->addShape( pyramid_shape );
			}
			return;
		}

		shared_ptr<IfcRightCircularCone> right_circular_cone = dynamic_pointer_cast<IfcRightCircularCone>(csg_primitive);
		if( right_circular_cone )
		{
			if( !right_circular_cone->m_Height )
			{
				messageCallback( "Invalid Height", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}
			if( !right_circular_cone->m_BottomRadius )
			{
				messageCallback( "Invalid BottomRadius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}

			const double height = right_circular_cone->m_Height->m_value*length_factor;
			const double radius = right_circular_cone->m_BottomRadius->m_value*length_factor;

			TopoDS_Solid cone_shape = BRepPrimAPI_MakeCone( radius, 0.0, height );
			GeomUtils::applyMatrixToShape( cone_shape, primitive_placement_matrix );
			item_data->addShape( cone_shape );
			return;
		}

		shared_ptr<IfcRightCircularCylinder> right_circular_cylinder = dynamic_pointer_cast<IfcRightCircularCylinder>(csg_primitive);
		if( right_circular_cylinder )
		{
			if( !right_circular_cylinder->m_Height )
			{
				messageCallback( "Invalid Height", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}

			if( !right_circular_cylinder->m_Radius )
			{
				messageCallback( "Invalid Radius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}

			double height = right_circular_cylinder->m_Height->m_value*length_factor;
			double radius = right_circular_cylinder->m_Radius->m_value*length_factor;

			TopoDS_Shape cylinder_shape = BRepPrimAPI_MakeCylinder( radius, height, M_PI*2.0 );
			GeomUtils::applyMatrixToShape( cylinder_shape, primitive_placement_matrix );
			item_data->addShape( cylinder_shape );
			return;
		}

		shared_ptr<IfcSphere> sphere = dynamic_pointer_cast<IfcSphere>(csg_primitive);
		if( sphere )
		{
			if( !sphere->m_Radius )
			{
				messageCallback( "Invalid Radius", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
				return;
			}

			double radius = sphere->m_Radius->m_value;
			TopoDS_Solid sphere_shape = BRepPrimAPI_MakeSphere( radius );
			GeomUtils::applyMatrixToShape( sphere_shape, primitive_placement_matrix );
			item_data->addShape( sphere_shape );
			return;
		}
		messageCallback( "Unhandled IFC Representation", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, csg_primitive.get() );
	}

	void convertIfcHalfSpaceSolid( const shared_ptr<IfcHalfSpaceSolid>& half_space_solid, shared_ptr<ItemShapeData> item_data, const shared_ptr<ItemShapeData>& other_operand )
	{
		//ENTITY IfcHalfSpaceSolid SUPERTYPE OF(ONEOF(IfcBoxedHalfSpace, IfcPolygonalBoundedHalfSpace))
		double length_factor = m_unit_converter->getLengthInMeterFactor();
		shared_ptr<IfcSurface> ifc_base_surface = half_space_solid->m_BaseSurface;

		// base surface
		shared_ptr<IfcElementarySurface> elem_base_surface = dynamic_pointer_cast<IfcElementarySurface>(ifc_base_surface);
		if( !elem_base_surface )
		{
			messageCallback( "The base surface shall be an unbounded surface (subtype of IfcElementarySurface)", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, half_space_solid.get() );
			return;
		}
		shared_ptr<IfcAxis2Placement3D>& base_surface_pos = elem_base_surface->m_Position;
		gp_Pln base_surface_plane;
		gp_Trsf base_surface_transform;
		if( base_surface_pos )
		{
			PlacementConverter::convertIfcAxis2Placement3D( base_surface_pos, length_factor, base_surface_plane );
			PlacementConverter::convertIfcAxis2Placement3D( base_surface_pos, length_factor, base_surface_transform );
		}

		shared_ptr<IfcBoxedHalfSpace> boxed_half_space = dynamic_pointer_cast<IfcBoxedHalfSpace>(half_space_solid);
		if( boxed_half_space )
		{
			shared_ptr<IfcBoundingBox> bbox = boxed_half_space->m_Enclosure;
			if( !bbox )
			{
				messageCallback( "Enclosure not given", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, half_space_solid.get() );
				return;
			}

			if( !bbox->m_Corner || !bbox->m_XDim || !bbox->m_YDim || !bbox->m_ZDim )
			{
				messageCallback( "Enclosure not valid", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, half_space_solid.get() );
				return;
			}
			shared_ptr<IfcCartesianPoint>&			bbox_corner = bbox->m_Corner;
			shared_ptr<IfcPositiveLengthMeasure>&	bbox_x_dim = bbox->m_XDim;
			shared_ptr<IfcPositiveLengthMeasure>&	bbox_y_dim = bbox->m_YDim;
			shared_ptr<IfcPositiveLengthMeasure>&	bbox_z_dim = bbox->m_ZDim;

			vec3 corner;
			PointConverter::convertIfcCartesianPoint( bbox_corner, corner, length_factor );
			gp_Trsf box_position_matrix;
			box_position_matrix.SetTranslation( corner );
			box_position_matrix.Multiply( base_surface_transform );
			vec3 bbox_extent( bbox_x_dim->m_value, bbox_y_dim->m_value, bbox_z_dim->m_value );

			// unbounded half space solid, create simple box
			TopoDS_Solid box_solid = BRepPrimAPI_MakeBox( bbox_x_dim->m_value*2.0, bbox_y_dim->m_value*2.0, bbox_z_dim->m_value*2.0 );
			GeomUtils::applyTranslationToShape( box_solid, -bbox_extent );
			GeomUtils::applyMatrixToShape( box_solid, box_position_matrix );
			item_data->addShape( box_solid );
			return;
		}

		// check dimenstions of other operand
		double extrusion_depth = HALF_SPACE_BOX_SIZE;
#ifdef _DEBUG
		extrusion_depth *= 0.1;
#endif
		if( other_operand )
		{
			Bnd_Box bbox;
			for( size_t ii = 0; ii < other_operand->m_shapes.size(); ++ii )
			{
				TopoDS_Shape& shape = other_operand->m_shapes[ii];
				BRepBndLib::Add( shape, bbox );
			}

			double xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
			if( !bbox.IsVoid() )
			{
				bbox.Get( xmin, ymin, zmin, xmax, ymax, zmax );
				double max_extent = std::max( (xmax - xmin), std::max( (ymax - ymin), (zmax - zmin) ) );
				extrusion_depth = std::max( extrusion_depth, max_extent*5.0 );
			}
		}

		shared_ptr<IfcPolygonalBoundedHalfSpace> polygonal_half_space = dynamic_pointer_cast<IfcPolygonalBoundedHalfSpace>(half_space_solid);
		if( polygonal_half_space )
		{
			// ENTITY IfcPolygonalBoundedHalfSpace 
			//	SUBTYPE OF IfcHalfSpaceSolid;
			//	Position	 :	IfcAxis2Placement3D;
			//	PolygonalBoundary	 :	IfcBoundedCurve;

			gp_Trsf boundary_position_transform;
			vec3 boundary_normal( 0, 0, 1 );
			if( polygonal_half_space->m_Position )
			{
				PlacementConverter::convertIfcAxis2Placement3D( polygonal_half_space->m_Position, length_factor, boundary_position_transform );
				boundary_normal.Transform( boundary_position_transform );
			}

			// PolygonalBoundary is given in 2D
			TopoDS_Wire polygonal_boundary;
			shared_ptr<IfcBoundedCurve> bounded_curve = polygonal_half_space->m_PolygonalBoundary;
			m_curve_converter->convertIfcCurve( bounded_curve, polygonal_boundary );
			GeomUtils::applyMatrixToShape( polygonal_boundary, boundary_position_transform );

			vec3 clipping_solid_extrusion_direction = boundary_normal.Multiplied( extrusion_depth*2.0 );
			gp_Dir base_surface_normal = base_surface_plane.Axis().Direction();

			// If the agreement flag is TRUE, then the subset is the one the normal points away from
			bool agreement = half_space_solid->m_AgreementFlag->m_value;
			double dot_product = base_surface_normal.Dot( boundary_normal.Normalized() );
			if( dot_product < 0 )
			{
				// vectors are opposite
				clipping_solid_extrusion_direction = -clipping_solid_extrusion_direction;
			}

			if( !agreement )
			{
				clipping_solid_extrusion_direction = -clipping_solid_extrusion_direction;
			}

			TopoDS_Shape half_space_shape;
			vec3 half_space_extrusion_vector = boundary_normal.Multiplied( extrusion_depth );

			try
			{
				TopoDS_Face polygonal_boundary_face = BRepBuilderAPI_MakeFace( polygonal_boundary );
				if( polygonal_boundary_face.IsNull() )
				{
					messageCallback( "BRepBuilderAPI_MakeFace failed", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
					return;
				}

				// move half space boundary down, in case that the clipping plane is below
				GeomUtils::applyTranslationToShape( polygonal_boundary_face, -half_space_extrusion_vector );
				half_space_shape = BRepPrimAPI_MakePrism( polygonal_boundary_face, half_space_extrusion_vector.Multiplied( 2.0 ) );

				// cut off part of solid below base surface
				TopoDS_Face base_surface_face = BRepBuilderAPI_MakeFace( base_surface_plane, -extrusion_depth, extrusion_depth, -extrusion_depth, extrusion_depth );
				TopoDS_Solid clipping_solid = TopoDS::Solid( BRepPrimAPI_MakePrism( base_surface_face, clipping_solid_extrusion_direction ) );

#ifdef IFCPP_GEOM_DEBUG
				GeomDebugDump::dumpEdgesOfShape( half_space_shape, vec4( 0.5, 0.6, 0.7, 1.0 ), true, false );
				GeomDebugDump::dumpEdgesOfShape( clipping_solid, vec4( 0.7, 0.6, 0.7, 1.0 ), true, false );
				GeomDebugDump::dumpShape( base_surface_face, vec4( 0.7, 0.6, 0.7, 0.5 ), true, false );
#endif

				TopoDS_Solid result;
				double fuzzy_value = 1.e-7*m_unit_converter->getLengthInMeterFactor();
				CSG_Adapter::computeCSG( half_space_shape, clipping_solid, result, CSG_A_MINUS_B, fuzzy_value, this, polygonal_half_space.get() );

				if( !result.IsNull() )
				{
#ifdef IFCPP_GEOM_DEBUG
					GeomDebugDump::dumpShape( result, vec4( 0.5, 0.6, 0.7, 1.0 ), true, false );
#endif
					half_space_shape = result;
				}
			}
			catch( Standard_Failure sf )
			{
				messageCallback( sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
				return;
			}
			catch( ... )
			{
				messageCallback( "BRepPrimAPI_MakePrism failed", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
				return;
			}

			if( half_space_shape.IsNull() )
			{
				messageCallback( "BRepPrimAPI_MakePrism failed", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
				return;
			}

			item_data->addShape( half_space_shape );
		}
		else
		{
			// unbounded half space solid, create simple box
			Handle_Geom_Surface geom_surface;
			TopoDS_Face base_face;
			m_face_converter->convertIfcSurface( ifc_base_surface, geom_surface, base_face );
			if( base_face.IsNull() )
			{
				messageCallback( "Could not convert BaseSurface", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
				return;
			}

			try
			{
				gp_Vec face_normal = GeomUtils::computeFaceNormal( base_face );
				bool agreement = half_space_solid->m_AgreementFlag->m_value;
				if( !agreement )
				{
					face_normal = -face_normal;
				}

				gp_Vec  half_space_extrusion_direction = -face_normal;
				gp_Vec  half_space_extrusion_vector = half_space_extrusion_direction*HALF_SPACE_BOX_SIZE;

				TopoDS_Solid extruded_half_space = TopoDS::Solid( BRepPrimAPI_MakePrism( base_face, half_space_extrusion_vector ) );
				if( !extruded_half_space.IsNull() )
				{
					item_data->addShape( extruded_half_space );
#ifdef IFCPP_GEOM_DEBUG
					GeomDebugDump::dumpShape( extruded_half_space, vec4( 0.5, 0.57, 0.6, 1.0 ), true, true );
#endif
				}
				else
				{
#ifdef IFCPP_GEOM_DEBUG
					GeomDebugDump::dumpShape( base_face, vec4( 0.5, 0.57, 0.6, 1.0 ), true, true );
					for( TopExp_Explorer exp( base_face, TopAbs_VERTEX ); exp.More(); exp.Next() )
					{
						TopoDS_Vertex vert = TopoDS::Vertex( exp.Current() );
						gp_Pnt vertex_point = BRep_Tool::Pnt( vert );
						vec3 normal = GeomUtils::computeFaceNormal( base_face );
						gp_Pnt extruded_point( vertex_point.X() + half_space_extrusion_vector.X(), vertex_point.Y() + half_space_extrusion_vector.Y(), vertex_point.Z() + half_space_extrusion_vector.X() );

						TopoDS_Edge edge = BRepBuilderAPI_MakeEdge( vert, BRepBuilderAPI_MakeVertex( extruded_point ) );
						GeomDebugDump::dumpShape( edge, vec4( 0.5, 0.57, 0.6, 1.0 ), true, true );
					}
#endif
				}
			}
			catch( Standard_Failure sf )
			{
				messageCallback( sf.GetMessageString(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
				return;
			}
			catch( ... )
			{
				messageCallback( "BRepPrimAPI_MakePrism failed", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, polygonal_half_space.get() );
				return;
			}
			
			return;
		}
	}


	void convertIfcBooleanOperand( const shared_ptr<IfcBooleanOperand>& operand_select, shared_ptr<ItemShapeData> item_data, const shared_ptr<ItemShapeData>& other_operand )
	{
		// TYPE IfcBooleanOperand = SELECT	(IfcBooleanResult	,IfcCsgPrimitive3D	,IfcHalfSpaceSolid	,IfcSolidModel);
		shared_ptr<IfcSolidModel> solid_model = dynamic_pointer_cast<IfcSolidModel>(operand_select);
		if( solid_model )
		{
			convertIfcSolidModel( solid_model, item_data );
			return;
		}

		shared_ptr<IfcHalfSpaceSolid> half_space_solid = dynamic_pointer_cast<IfcHalfSpaceSolid>(operand_select);
		if( half_space_solid )
		{
			convertIfcHalfSpaceSolid( half_space_solid, item_data, other_operand );
			return;
		}

		shared_ptr<IfcBooleanResult> boolean_result = dynamic_pointer_cast<IfcBooleanResult>(operand_select);
		if( boolean_result )
		{
			convertIfcBooleanResult( boolean_result, item_data );
			return;
		}

		shared_ptr<IfcCsgPrimitive3D> csg_primitive3D = dynamic_pointer_cast<IfcCsgPrimitive3D>(operand_select);
		if( csg_primitive3D )
		{
			convertIfcCsgPrimitive3D( csg_primitive3D, item_data );
			return;
		}

		std::stringstream strs_err;
		strs_err << "Unhandled IFC Representation: " << operand_select->className();
		throw IfcPPException( strs_err.str().c_str(), __FUNC__ );
	}

	void convertIfcSectionedSpine( const shared_ptr<IfcSectionedSpine>& spine, shared_ptr<ItemShapeData> item_data )
	{
		const shared_ptr<IfcCompositeCurve> spine_curve = spine->m_SpineCurve;
		if( !spine_curve )
		{
			messageCallback( "invalid IfcHalfSpaceSolid.BaseSurface", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, spine.get() );
			return;
		}
		const std::vector<shared_ptr<IfcProfileDef> >& vec_cross_sections = spine->m_CrossSections;
		const std::vector<shared_ptr<IfcAxis2Placement3D> >& vec_cross_section_positions = spine->m_CrossSectionPositions;

		std::vector<shared_ptr<IfcProfileDef> >::iterator it_cross_sections;

		size_t num_cross_sections = vec_cross_sections.size();
		if( vec_cross_section_positions.size() < num_cross_sections )
		{
			num_cross_sections = vec_cross_section_positions.size();
		}

		std::vector<shared_ptr<IfcCompositeCurveSegment> > segements = spine_curve->m_Segments;
		size_t num_segments = segements.size();
		if( vec_cross_section_positions.size() < num_segments + 1 )
		{
			num_segments = vec_cross_section_positions.size() - 1;
		}

		TopoDS_Wire cirve_wire;
		m_curve_converter->convertIfcCurve( spine_curve, cirve_wire );

#ifdef _DEBUG
		std::cout << "IfcSectionedSpine not implemented." << std::endl;
#endif
	}
};
