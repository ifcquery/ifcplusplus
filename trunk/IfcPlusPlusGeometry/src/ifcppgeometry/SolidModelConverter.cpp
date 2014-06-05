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

#include <ifcpp/IFC4/include/IfcPositiveLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcPlaneAngleMeasure.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcAxis1Placement.h>
#include <ifcpp/IFC4/include/IfcBoundingBox.h>
#include <ifcpp/IFC4/include/IfcBoundedCurve.h>

#include <ifcpp/IFC4/include/IfcExtrudedAreaSolid.h>
#include <ifcpp/IFC4/include/IfcFixedReferenceSweptAreaSolid.h>
#include <ifcpp/IFC4/include/IfcSurfaceCurveSweptAreaSolid.h>
#include <ifcpp/IFC4/include/IfcRevolvedAreaSolid.h>
#include <ifcpp/IFC4/include/IfcSweptDiskSolid.h>
#include <ifcpp/IFC4/include/IfcHalfSpaceSolid.h>
#include <ifcpp/IFC4/include/IfcBoxedHalfSpace.h>
#include <ifcpp/IFC4/include/IfcPolygonalBoundedHalfSpace.h>
#include <ifcpp/IFC4/include/IfcPlane.h>

#include <ifcpp/IFC4/include/IfcManifoldSolidBrep.h>
#include <ifcpp/IFC4/include/IfcClosedShell.h>
#include <ifcpp/IFC4/include/IfcFacetedBrep.h>
#include <ifcpp/IFC4/include/IfcAdvancedBrep.h>
#include <ifcpp/IFC4/include/IfcAdvancedBrepWithVoids.h>

#include <ifcpp/IFC4/include/IfcCsgSolid.h>
#include <ifcpp/IFC4/include/IfcBlock.h>
#include <ifcpp/IFC4/include/IfcSphere.h>
#include <ifcpp/IFC4/include/IfcRectangularPyramid.h>
#include <ifcpp/IFC4/include/IfcRightCircularCone.h>
#include <ifcpp/IFC4/include/IfcRightCircularCylinder.h>
#include <ifcpp/IFC4/include/IfcBooleanResult.h>
#include <ifcpp/IFC4/include/IfcBooleanOperator.h>
#include <ifcpp/IFC4/include/IfcBooleanOperand.h>
#include <ifcpp/IFC4/include/IfcBooleanClippingResult.h>

#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/IfcPPException.h>
#include "GeometrySettings.h"
#include "ProfileConverter.h"
#include "ProfileCache.h"
#include "PlacementConverter.h"
#include "CurveConverter.h"
#include "FaceConverter.h"
#include "ConverterOSG.h"
#include "GeomUtils.h"
#include "CSG_Adapter.h"
#include "DebugViewerCallback.h"
#include "GeometryException.h"
#include "RepresentationConverter.h"
#include "SolidModelConverter.h"

SolidModelConverter::SolidModelConverter( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> uc, shared_ptr<CurveConverter>	cc, shared_ptr<FaceConverter> fc, shared_ptr<ProfileCache>	pc ) 
	: m_geom_settings(geom_settings), m_unit_converter(uc), m_curve_converter(cc), m_face_converter(fc), m_profile_cache( pc )
{
}

SolidModelConverter::~SolidModelConverter()
{
}

// ENTITY IfcSolidModel ABSTRACT SUPERTYPE OF(ONEOF(IfcCsgSolid, IfcManifoldSolidBrep, IfcSweptAreaSolid, IfcSweptDiskSolid))
void SolidModelConverter::convertIfcSolidModel( const shared_ptr<IfcSolidModel>& solid_model, shared_ptr<ItemData> item_data, std::stringstream& strs_err )
{
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
			strs_err << __FUNC__ << ": SweptArea not valid" << std::endl;
			return;
		}
		shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter( swept_area );
		const std::vector<std::vector<carve::geom::vector<2> > >& profile_paths = profile_converter->getCoordinates();

		// check if local coordinate system is specified for extrusion
		carve::math::Matrix swept_area_pos;
		if( swept_area_solid->m_Position )
		{
			double length_factor = m_unit_converter->getLengthInMeterFactor();
			shared_ptr<IfcAxis2Placement3D> swept_area_position = swept_area_solid->m_Position;
			PlacementConverter::convertIfcAxis2Placement3D( swept_area_position, swept_area_pos, length_factor );
		}

		shared_ptr<IfcExtrudedAreaSolid> extruded_area = dynamic_pointer_cast<IfcExtrudedAreaSolid>(swept_area_solid);
		if( extruded_area )
		{
			shared_ptr<ItemData> item_data_extruded( new ItemData() );
			convertIfcExtrudedAreaSolid( extruded_area, item_data_extruded, strs_err );
			item_data_extruded->applyPosition( swept_area_pos );
			item_data->addItemData( item_data_extruded );
			return;
		}

		shared_ptr<IfcFixedReferenceSweptAreaSolid> fixed_reference_swept_area_solid = dynamic_pointer_cast<IfcFixedReferenceSweptAreaSolid>(swept_area_solid);
		if( fixed_reference_swept_area_solid )
		{
			//Directrix	 : OPTIONAL IfcCurve;
			//StartParam	 : OPTIONAL IfcParameterValue;
			//EndParam	 : OPTIONAL IfcParameterValue;
			//FixedReference	 : IfcDirection;

			shared_ptr<IfcCurve>& directrix_curve = fixed_reference_swept_area_solid->m_Directrix;

			shared_ptr<IfcParameterValue>& start_param = fixed_reference_swept_area_solid->m_StartParam;				//optional
			shared_ptr<IfcParameterValue>& end_param = fixed_reference_swept_area_solid->m_EndParam;					//optional
			shared_ptr<IfcDirection>& fixed_reference = fixed_reference_swept_area_solid->m_FixedReference;

			const int nvc = m_geom_settings->m_num_vertices_per_circle;
			double length_in_meter = m_unit_converter->getLengthInMeterFactor();

			std::vector<carve::geom::vector<3> > segment_start_points;
			std::vector<carve::geom::vector<3> > basis_curve_points;
			m_curve_converter->convertIfcCurve( directrix_curve, basis_curve_points, segment_start_points );

			shared_ptr<ItemData> item_data_solid( new ItemData() );
			GeomUtils::sweepArea( basis_curve_points, profile_paths, item_data_solid );
			item_data_solid->applyPosition( swept_area_pos );
			item_data->addItemData( item_data_solid );
			return;
		}

		shared_ptr<IfcRevolvedAreaSolid> revolved_area_solid = dynamic_pointer_cast<IfcRevolvedAreaSolid>(swept_area_solid);
		if( revolved_area_solid )
		{
			shared_ptr<ItemData> item_data_solid( new ItemData() );
			convertIfcRevolvedAreaSolid( revolved_area_solid, item_data_solid, strs_err );
			item_data_solid->applyPosition( swept_area_pos );
			item_data->addItemData( item_data_solid );
			return;
		}

		shared_ptr<IfcSurfaceCurveSweptAreaSolid> surface_curve_swept_area_solid = dynamic_pointer_cast<IfcSurfaceCurveSweptAreaSolid>(swept_area_solid);
		if( surface_curve_swept_area_solid )
		{
			std::cout << "IfcSurfaceCurveSweptAreaSolid not implemented" << std::endl;
			// IfcSweptAreaSolid -----------------------------------------------------------
			// attributes:
			//  shared_ptr<IfcProfileDef>					m_SweptArea;
			//  shared_ptr<IfcAxis2Placement3D>				m_Position;					//optional

			shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData );
			shared_ptr<IfcCurve>& directrix_curve = surface_curve_swept_area_solid->m_Directrix;
			const int nvc = m_geom_settings->m_num_vertices_per_circle;
			double length_in_meter = m_unit_converter->getLengthInMeterFactor();

			std::vector<carve::geom::vector<3> > segment_start_points;
			std::vector<carve::geom::vector<3> > basis_curve_points;
			m_curve_converter->convertIfcCurve( directrix_curve, basis_curve_points, segment_start_points );

			shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
			m_face_converter->convertIfcSurface( surface_curve_swept_area_solid->m_ReferenceSurface, polyline_data );

			// apply swept area position
			std::vector<carve::geom3d::Vector>& polyline_points = polyline_data->points;
			for( int ii=0; ii<polyline_points.size(); ++ii )
			{
				carve::geom3d::Vector& pt = polyline_points[ii];
				pt = swept_area_pos*pt;
			}

			//shared_ptr<IfcParameterValue>				m_StartParam;				//optional
			//shared_ptr<IfcParameterValue>				m_EndParam;					//optional

			return;
		}

		strs_err << "Unhandled IFC Representation: #" << solid_model->getId() << "=" << solid_model->classname() << std::endl;
		return;
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
			shared_ptr<ItemData> input_data_outer_shell( new ItemData() );
			m_face_converter->convertIfcFaceList( vec_faces_outer_shell, input_data_outer_shell, strs_err );
			std::copy( input_data_outer_shell->open_or_closed_polyhedrons.begin(), input_data_outer_shell->open_or_closed_polyhedrons.end(), std::back_inserter(item_data->closed_polyhedrons) );
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
				shared_ptr<IfcAdvancedBrepWithVoids> advanced_brep_with_voids = dynamic_pointer_cast<IfcAdvancedBrepWithVoids>(solid_model);
				std::vector<shared_ptr<IfcClosedShell> >& vec_voids = advanced_brep_with_voids->m_Voids;

				// TODO: subtract voids from outer shell
				std::cout << "IfcAdvancedBrep not implemented" << std::endl;
			}
			return;
		}

		strs_err << "Unhandled IFC Representation: #" << solid_model->getId() << "=" << solid_model->classname() << std::endl;
		return;
	}

	shared_ptr<IfcCsgSolid> csg_solid = dynamic_pointer_cast<IfcCsgSolid>(solid_model);
	if( csg_solid )
	{
		shared_ptr<IfcCsgSelect> csg_select = csg_solid->m_TreeRootExpression;

		if( dynamic_pointer_cast<IfcBooleanResult>(csg_select) )
		{
			shared_ptr<IfcBooleanResult> csg_select_boolean_result = dynamic_pointer_cast<IfcBooleanResult>(csg_select);
			convertIfcBooleanResult( csg_select_boolean_result, item_data, strs_err );
		}
		else if( dynamic_pointer_cast<IfcCsgPrimitive3D>(csg_select) )
		{
			shared_ptr<IfcCsgPrimitive3D> csg_select_primitive_3d = dynamic_pointer_cast<IfcCsgPrimitive3D>(csg_select);
			convertIfcCsgPrimitive3D( csg_select_primitive_3d, item_data, strs_err );
		}
		return;
	}

	//shared_ptr<IfcReferencedSectionedSpine> spine = dynamic_pointer_cast<IfcReferencedSectionedSpine>(solid_model);
	//if( spine )
	//{
	//	convertIfcReferencedSectionedSpine( spine, pos, item_data );
	//	return;
	//}

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
		const int nvc = m_geom_settings->m_num_vertices_per_circle;
		double length_in_meter = m_unit_converter->getLengthInMeterFactor();
		double radius = 0.0;
		if( swept_disp_solid->m_Radius )
		{
			radius = swept_disp_solid->m_Radius->m_value*length_in_meter;
		}

		double radius_inner = -1.0;
		if( swept_disp_solid->m_InnerRadius )
		{
			radius_inner = swept_disp_solid->m_InnerRadius->m_value*length_in_meter;
		}

		// TODO: handle inner radius, start param, end param

		std::vector<carve::geom::vector<3> > segment_start_points;
		std::vector<carve::geom::vector<3> > basis_curve_points;
		m_curve_converter->convertIfcCurve( directrix_curve, basis_curve_points, segment_start_points );

		//shared_ptr<carve::input::PolyhedronData> pipe_data( new carve::input::PolyhedronData() );
		//item_data->closed_polyhedrons.push_back(pipe_data);
		//GeomUtils::sweepDisk( basis_curve_points, pipe_data, nvc, radius, radius_inner );

		shared_ptr<ItemData> item_data_solid( new ItemData() );
		GeomUtils::sweepDisk( basis_curve_points, item_data_solid, nvc, radius, radius_inner );
		//item_data_solid->applyPosition( swept_area_pos );
		item_data->addItemData( item_data_solid );

		return;
	}

	strs_err << "Unhandled IFC Representation: #" << solid_model->getId() << "=" << solid_model->classname() << std::endl;
}

void SolidModelConverter::convertIfcExtrudedAreaSolid( const shared_ptr<IfcExtrudedAreaSolid>& extruded_area, shared_ptr<ItemData> item_data, std::stringstream& strs_err )
{
	const int entity_id = extruded_area->getId();
	if( !extruded_area->m_ExtrudedDirection )
	{
		strs_err << "Invalid ExtrudedDirection " << __func__ << std::endl;
		return;
	}

	if( !extruded_area->m_Depth )
	{
		strs_err << "Invalid Depth " << __func__ << std::endl;
		return;
	}
	double length_factor = m_unit_converter->getLengthInMeterFactor();

	// direction and length of extrusion
	const double depth = extruded_area->m_Depth->m_value*length_factor;
	carve::geom::vector<3>  extrusion_vector;
	std::vector<double>& vec_direction = extruded_area->m_ExtrudedDirection->m_DirectionRatios;
	if( vec_direction.size() > 2 )
	{
		extrusion_vector = carve::geom::VECTOR( vec_direction[0]*depth, vec_direction[1]*depth, vec_direction[2]*depth );
	}
	else if( vec_direction.size() > 1 )
	{
		extrusion_vector = carve::geom::VECTOR( vec_direction[0]*depth, vec_direction[1]*depth, 0 );
	}

	// swept area
	shared_ptr<IfcProfileDef>	swept_area = extruded_area->m_SweptArea;
	if( !swept_area )
	{
		strs_err << __FUNC__ << ": SweptArea not valid" << std::endl;
		return;
	}
	shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter(swept_area);
	profile_converter->simplifyPaths();
	const std::vector<std::vector<carve::geom::vector<2> > >& paths = profile_converter->getCoordinates();

	if( paths.size() == 0 )
	{
		return;
	}
	shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData );
	GeomUtils::extrude( paths, extrusion_vector, poly_data, strs_err );

#ifdef _DEBUG
	if( false )// entity_id == 149044 || entity_id == 149241 || entity_id == 149053 )
	{
		shared_ptr<carve::input::PolyhedronData> poly_data_copy( new carve::input::PolyhedronData( *(poly_data.get()) ) );

		std::vector<carve::geom::vector<3> >& points_copy = poly_data_copy->points;
		for( std::vector<carve::geom::vector<3> >::iterator it_points = points_copy.begin(); it_points != points_copy.end(); ++it_points )
		{
			carve::geom::vector<3>& vertex = (*it_points);
			vertex = carve::math::Matrix::TRANS( 0.1, 0.1, 0)*carve::math::Matrix::SCALE(4,4,4)*vertex;
		}

		//shared_ptr<carve::mesh::MeshSet<3> > mesh_set_copy( poly_data_copy->createMesh(carve::input::opts()) );

		//ConverterOSG::dumpToVTK( paths );
		renderPathsInDebugViewer( paths );
		renderPolyhedronInDebugViewer( poly_data_copy.get(), osg::Vec4(1.0f, 0.0f, 1.0f, 1.0f), false );
	}
#endif

	// apply object coordinate system
	//std::vector<carve::geom::vector<3> >& points = poly_data->points;
	//for( std::vector<carve::geom::vector<3> >::iterator it_points = points.begin(); it_points != points.end(); ++it_points )
	//{
	//	carve::geom::vector<3>& vertex = (*it_points);
	//	vertex = pos*vertex;
	//}

	item_data->closed_polyhedrons.push_back( poly_data );

#ifdef _DEBUG
	shared_ptr<carve::mesh::MeshSet<3> > mesh_set( poly_data->createMesh(carve::input::opts()) );
	std::stringstream strs_err_meshset;
	bool poly_ok = CSG_Adapter::checkMeshSetValidAndClosed( mesh_set.get(), strs_err_meshset, -1 );

	if( !poly_ok )
	{
		std::cout << strs_err_meshset.str().c_str() << std::endl;

		renderMeshsetInDebugViewer( mesh_set.get(), osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f), true );
		dumpMeshsets( mesh_set.get(), nullptr, nullptr, extruded_area->getId(), 0 );

		shared_ptr<carve::input::PolylineSetData> polyline_data( new carve::input::PolylineSetData() );
		polyline_data->beginPolyline();
		if( paths.size() > 0 )
		{
			const std::vector<carve::geom::vector<2> >& loop = paths[0]; 
			for( int i=0; i<loop.size(); ++i )
			{
				const carve::geom::vector<2>& point = loop[i];
				carve::geom::vector<3> point3d( carve::geom::VECTOR( point.x, point.y, 0 ) );
				polyline_data->addVertex( point3d );
				polyline_data->addPolylineIndex(i);
			}

			osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			ConverterOSG::drawPolyline( polyline_data.get(), geode );

			renderPolylineInDebugViewer( polyline_data.get(), osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );
		}

	}
#endif
}

void SolidModelConverter::convertIfcRevolvedAreaSolid( const shared_ptr<IfcRevolvedAreaSolid>& revolved_area, shared_ptr<ItemData> item_data, std::stringstream& strs_err )
{
	if( !revolved_area->m_SweptArea )
	{
		return;
	}
	double length_factor = m_unit_converter->getLengthInMeterFactor();

	// angle and axis
	double angle_factor = m_unit_converter->getAngleInRadianFactor();
	shared_ptr<IfcProfileDef> swept_area_profile = revolved_area->m_SweptArea;
	if( !swept_area_profile )
	{
		strs_err << __FUNC__ << ": SweptArea not valid" << std::endl;
		return;
	}
	double revolution_angle = revolved_area->m_Angle->m_value*angle_factor;

	carve::geom::vector<3>  axis_location;
	carve::geom::vector<3>  axis_direction;
	if(revolved_area->m_Axis)
	{
		shared_ptr<IfcAxis1Placement> axis_placement = revolved_area->m_Axis;

		if( axis_placement->m_Location )
		{
			shared_ptr<IfcCartesianPoint> location_point = axis_placement->m_Location;
			m_curve_converter->convertIfcCartesianPoint( location_point, axis_location );
		}

		if( axis_placement->m_Axis )
		{
			shared_ptr<IfcDirection> axis = axis_placement->m_Axis;
			axis_direction = carve::geom::VECTOR( axis->m_DirectionRatios[0], axis->m_DirectionRatios[1], axis->m_DirectionRatios[2] );
		}
	}

	// rotation base point is the one with the smallest distance on the rotation axis
	carve::geom::vector<3>  origin;
	carve::geom::vector<3>  base_point;
	GeomUtils::closestPointOnLine( origin, axis_location, axis_direction, base_point );
	base_point *= -1;

	// swept area
	shared_ptr<ProfileConverter> profile_converter = m_profile_cache->getProfileConverter(swept_area_profile);
	const std::vector<std::vector<carve::geom::vector<2> > >& profile_coords_unchecked = profile_converter->getCoordinates();

	std::vector<std::vector<carve::geom::vector<2> > > profile_coords;
	for( int ii=0; ii<profile_coords_unchecked.size(); ++ii )
	{
		const std::vector<carve::geom::vector<2> >& profile_loop_unchecked = profile_coords_unchecked[ii];
		carve::geom::vector<3> normal_2d = GeomUtils::computePolygon2DNormal( profile_loop_unchecked );
		bool reverse_loop = false;
		if( ii == 0 )
		{
			if( normal_2d.z < 0 )
			{
				reverse_loop = true;
			}
		}
		else
		{
			if( normal_2d.z > 0 )
			{
				reverse_loop = true;
			}
		}

		profile_coords.push_back( std::vector<carve::geom::vector<2> >() );
		std::vector<carve::geom::vector<2> >& profile_loop = profile_coords.back();

		if( reverse_loop )
		{
			std::copy( profile_loop_unchecked.rbegin(), profile_loop_unchecked.rend(), std::back_inserter( profile_loop ) );
		}
		else
		{
			std::copy( profile_loop_unchecked.begin(), profile_loop_unchecked.end(), std::back_inserter( profile_loop ) );
		}
	}

	// triangulate
	std::vector<carve::geom::vector<2> > merged;
	std::vector<carve::triangulate::tri_idx> triangulated;
	try
	{
		std::vector<std::pair<size_t, size_t> > result = carve::triangulate::incorporateHolesIntoPolygon(profile_coords);	// first is loop index, second is vertex index in loop
		merged.reserve(result.size());
		for( size_t i = 0; i < result.size(); ++i )
		{
			int loop_number = result[i].first;
			int index_in_loop = result[i].second;

			if( loop_number >= profile_coords.size() )
			{
				std::cout << __FUNC__ << ": loop_number >= profile_coords.size()" << std::endl;
				continue;
			}

			const std::vector<carve::geom2d::P2>& loop_2d = profile_coords[loop_number];

			const carve::geom2d::P2& point_2d = loop_2d[index_in_loop];
			merged.push_back( point_2d );
		}
		carve::triangulate::triangulate(merged, triangulated);
		carve::triangulate::improve(merged, triangulated);
	}
	catch(...)
	{
		strs_err << "carve::triangulate::incorporateHolesIntoPolygon failed " << std::endl;
		return;
	}

	if( profile_coords.size() == 0 )
	{
		strs_err << "#" << revolved_area->getId() << " = IfcRevolvedAreaSolid: convertIfcRevolvedAreaSolid: num_loops == 0";
		return;
	}
	if( profile_coords[0].size() < 3 )
	{
		strs_err << "#" << revolved_area->getId() << " = IfcRevolvedAreaSolid: convertIfcRevolvedAreaSolid: num_polygon_points < 3";
		return;
	}

	if( revolution_angle > M_PI*2 ) revolution_angle = M_PI*2;
	if( revolution_angle < -M_PI*2 ) revolution_angle = M_PI*2;

	// TODO: calculate num segments according to length/width/height ratio and overall size of the object
	int num_segments = m_geom_settings->m_num_vertices_per_circle*(std::abs(revolution_angle)/(2.0*M_PI));
	if( num_segments < 6 )
	{
		num_segments = 6;
	}
	double angle = 0.0;
	double d_angle = revolution_angle/num_segments;

	// check if we have to change the direction
	carve::geom::vector<3>  polygon_normal = GeomUtils::computePolygon2DNormal( profile_coords[0] );
	const carve::geom::vector<2>&  pt0_2d = profile_coords[0][0];
	carve::geom::vector<3>  pt0_3d( carve::geom::VECTOR( pt0_2d.x, pt0_2d.y, 0 ) );
	carve::geom::vector<3>  pt0 = carve::math::Matrix::ROT(d_angle, axis_direction )*(pt0_3d + base_point);
	if( polygon_normal.z*pt0.z > 0 )
	{
		angle = revolution_angle;
		d_angle = -d_angle;
	}

	shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );
	item_data->closed_polyhedrons.push_back(polyhedron_data);

	// create vertices
	carve::math::Matrix m;
	for( int i = 0; i <= num_segments; ++i )
	{
		m = carve::math::Matrix::ROT( angle, -axis_direction );
		for( int j=0; j<profile_coords.size(); ++j )
		{
			const std::vector<carve::geom::vector<2> >& loop = profile_coords[j];

			for( int k=0; k<loop.size(); ++k )
			{
				const carve::geom::vector<2>& point = loop[k];

				carve::geom::vector<3>  vertex= m*( carve::geom::VECTOR( point.x, point.y, 0 ) + base_point) - base_point;
				polyhedron_data->addVertex( vertex );
			}
		}
		angle += d_angle;
	}

	int num_vertices_per_section = 0;
	for( int j=0; j<profile_coords.size(); ++j )
	{
		const std::vector<carve::geom::vector<2> >& loop = profile_coords[j];
		num_vertices_per_section += loop.size();
	}

	// compute normal of front cap
	const carve::geom::vector<3>& vertex0_section0 = polyhedron_data->getVertex(0);
	const carve::geom::vector<3>& vertex0_section1 = polyhedron_data->getVertex(num_vertices_per_section);
	carve::geom::vector<3> normal_fron_cap = (vertex0_section0 - vertex0_section1).normalize();
	
	// front cap
	int back_cap_offset = num_vertices_per_section*(num_segments);
	bool flip_faces = false;
	for( size_t i = 0; i != triangulated.size(); ++i )
	{
		carve::triangulate::tri_idx triangle = triangulated[i];
		const int vertex_id_a	= triangle.a;
		const int vertex_id_b	= triangle.b;
		const int vertex_id_c	= triangle.c;

		if( vertex_id_a == vertex_id_b || vertex_id_a == vertex_id_c || vertex_id_b == vertex_id_c )
		{
			continue;
		}

		int vertex_id_a_top = vertex_id_a + back_cap_offset;
		int vertex_id_b_top = vertex_id_b + back_cap_offset;
		int vertex_id_c_top = vertex_id_c + back_cap_offset;

		if( i == 0 )
		{
			const carve::poly::Vertex<3>& v_a = polyhedron_data->getVertex(vertex_id_a);
			const carve::poly::Vertex<3>& v_b = polyhedron_data->getVertex(vertex_id_b);
			const carve::poly::Vertex<3>& v_c = polyhedron_data->getVertex(vertex_id_c);
			std::vector<carve::geom::vector<3> > vec_triangle;
			vec_triangle.push_back( polyhedron_data->getVertex(vertex_id_a) );
			vec_triangle.push_back( polyhedron_data->getVertex(vertex_id_b) );
			vec_triangle.push_back( polyhedron_data->getVertex(vertex_id_c) );
			carve::geom::vector<3> normal_first_triangle = GeomUtils::computePolygonNormal( vec_triangle );

			if( dot( normal_first_triangle, normal_fron_cap ) < 0 )
			{
				flip_faces = true;
			}
		}

#ifdef _DEBUG
		const carve::poly::Vertex<3>& v_a = polyhedron_data->getVertex(vertex_id_a);
		const carve::poly::Vertex<3>& v_b = polyhedron_data->getVertex(vertex_id_b);
		const carve::poly::Vertex<3>& v_c = polyhedron_data->getVertex(vertex_id_c);

		carve::geom::vector<3> pa( carve::geom::VECTOR( v_a.v[0],	v_a.v[1],	v_a.v[2] ) );
		carve::geom::vector<3> pb( carve::geom::VECTOR( v_b.v[0],	v_b.v[1],	v_b.v[2] ) );
		carve::geom::vector<3> pc( carve::geom::VECTOR( v_c.v[0],	v_c.v[1],	v_c.v[2] ) );

		double A = 0.5*(cross( pa-pb, pa-pc ).length());
		if( std::abs(A) < 0.000000001 )
		{
			std::cout << "area < 0.000000001\n" << std::endl;
		}
#endif

		if( flip_faces )
		{
			polyhedron_data->addFace( vertex_id_a,		vertex_id_c,		vertex_id_b );		// bottom cap
			polyhedron_data->addFace( vertex_id_a_top,	vertex_id_b_top,	vertex_id_c_top );	// top cap, flipped outward
		}
		else
		{
			polyhedron_data->addFace( vertex_id_a,		vertex_id_b,		vertex_id_c );		// bottom cap
			polyhedron_data->addFace( vertex_id_a_top,	vertex_id_c_top,	vertex_id_b_top );	// top cap, flipped outward
		}
	}

	// faces of revolved shape
	for( int i = 0; i < num_vertices_per_section-1; ++i )
	{
		int i_offset_next = i + num_vertices_per_section;
		for( int j = 0; j < num_segments; ++j )
		{
			int j_offset = j*num_vertices_per_section;
			//polyhedron_data->addFace( j_offset+i, j_offset+i+1, j_offset+1+i_offset_next, j_offset+i_offset_next );
			polyhedron_data->addFace( j_offset+i, j_offset+i+1, j_offset+1+i_offset_next );
			polyhedron_data->addFace( j_offset+1+i_offset_next, j_offset+i_offset_next, j_offset+i );
		}
	}

	for( int j = 0; j < num_segments; ++j )
	{
		int j_offset = j*num_vertices_per_section;
		//polyhedron_data->addFace( j_offset+num_polygon_points-1, j_offset, j_offset+num_polygon_points, j_offset+num_polygon_points+num_polygon_points-1 );
		polyhedron_data->addFace( j_offset+num_vertices_per_section-1, j_offset, j_offset+num_vertices_per_section );
		polyhedron_data->addFace( j_offset+num_vertices_per_section, j_offset+num_vertices_per_section*2-1, j_offset+num_vertices_per_section-1 );
	}

#ifdef _DEBUG
	std::stringstream strs_err;
	shared_ptr<carve::mesh::MeshSet<3> > mesh_set( polyhedron_data->createMesh(carve::input::opts()) );
	if( mesh_set->meshes.size() != 1 )
	{
		std::cout << __FUNC__ << ": mesh_set->meshes.size() != 1" << std::endl;
	}
	bool meshset_ok = CSG_Adapter::checkMeshSetValidAndClosed( mesh_set.get(), strs_err, -1 );

	if( !meshset_ok )
	{
		std::cout << strs_err.str().c_str() << std::endl;
	}
#endif
}


void SolidModelConverter::convertIfcBooleanResult( const shared_ptr<IfcBooleanResult>& bool_result, shared_ptr<ItemData> item_data, std::stringstream& strs_err )
{
	const int boolean_result_id = bool_result->getId();
	shared_ptr<IfcBooleanClippingResult> boolean_clipping_result = dynamic_pointer_cast<IfcBooleanClippingResult>(bool_result);
	if( boolean_clipping_result )
	{
		shared_ptr<IfcBooleanOperator> ifc_boolean_operator = boolean_clipping_result->m_Operator;
		shared_ptr<IfcBooleanOperand> ifc_first_operand = boolean_clipping_result->m_FirstOperand;
		shared_ptr<IfcBooleanOperand> ifc_second_operand = boolean_clipping_result->m_SecondOperand;

		if( !ifc_boolean_operator || !ifc_first_operand || !ifc_second_operand )
		{
			std::cout << __FUNC__ << ": invalid IfcBooleanOperator or IfcBooleanOperand" << std::endl;
			return;
		}

		carve::csg::CSG::OP csg_operation = carve::csg::CSG::A_MINUS_B;
		if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_UNION )
		{
			csg_operation = carve::csg::CSG::UNION;
		}
		else if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_INTERSECTION )
		{
			csg_operation = carve::csg::CSG::INTERSECTION;
		}
		else if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_DIFFERENCE )
		{
			csg_operation = carve::csg::CSG::A_MINUS_B;
		}
		else
		{
			strs_err << __FUNC__ << ": invalid IfcBooleanOperator" << std::endl;
		}

		int id1 = 0;
		if( dynamic_pointer_cast<IfcPPEntity>( ifc_first_operand ) )
		{
			id1 = dynamic_pointer_cast<IfcPPEntity>( ifc_first_operand )->getId();
		}

		int id2 = 0;
		if( dynamic_pointer_cast<IfcPPEntity>( ifc_second_operand ) )
		{
			id2 = dynamic_pointer_cast<IfcPPEntity>( ifc_second_operand )->getId();
		}

		// convert the first operand
		shared_ptr<ItemData> first_operand_data( new ItemData() );
		shared_ptr<ItemData> empty_operand;
		convertIfcBooleanOperand( ifc_first_operand, first_operand_data, empty_operand, strs_err );
		first_operand_data->createMeshSetsFromClosedPolyhedrons();

		// convert the second operand
		shared_ptr<ItemData> second_operand_data( new ItemData() );
		convertIfcBooleanOperand( ifc_second_operand, second_operand_data, first_operand_data, strs_err );
		second_operand_data->createMeshSetsFromClosedPolyhedrons();
		
		// for every first operand polyhedrons, apply all second operand polyhedrons
		std::vector<shared_ptr<carve::mesh::MeshSet<3> > >::iterator it_first_operands;
		for( it_first_operands=first_operand_data->meshsets.begin(); it_first_operands!=first_operand_data->meshsets.end(); ++it_first_operands )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& first_operand_meshset = (*it_first_operands);

			std::vector<shared_ptr<carve::mesh::MeshSet<3> > >::iterator it_second_operands;
			for( it_second_operands=second_operand_data->meshsets.begin(); it_second_operands!=second_operand_data->meshsets.end(); ++it_second_operands )
			{
				shared_ptr<carve::mesh::MeshSet<3> >& second_operand_meshset = (*it_second_operands);
				shared_ptr<carve::mesh::MeshSet<3> > result;
				bool csg_op_ok = CSG_Adapter::computeCSG( first_operand_meshset, second_operand_meshset, csg_operation, id1, id2, strs_err, result );
				
				if( csg_op_ok )
				{
					first_operand_meshset = result;
				}
				item_data->m_csg_computed = true;
			}
		}

		// now copy processed first operands to result input data
		std::copy( first_operand_data->meshsets.begin(), first_operand_data->meshsets.end(), std::back_inserter(item_data->meshsets) );
	}
}


void SolidModelConverter::convertIfcCsgPrimitive3D(	const shared_ptr<IfcCsgPrimitive3D>& csg_primitive,	shared_ptr<ItemData> item_data, std::stringstream& strs_err )
{
	shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );
	double length_factor = m_unit_converter->getLengthInMeterFactor();

	// ENTITY IfcCsgPrimitive3D  ABSTRACT SUPERTYPE OF(ONEOF(IfcBlock, IfcRectangularPyramid, IfcRightCircularCone, IfcRightCircularCylinder, IfcSphere
	shared_ptr<IfcAxis2Placement3D>& primitive_placement = csg_primitive->m_Position;

	carve::math::Matrix primitive_placement_matrix;
	if( primitive_placement )
	{
		PlacementConverter::convertIfcAxis2Placement3D( primitive_placement, primitive_placement_matrix, length_factor );
	}

	shared_ptr<IfcBlock> block = dynamic_pointer_cast<IfcBlock>(csg_primitive);
	if( block )
	{
		double x_length = length_factor;
		double y_length = length_factor;
		double z_length = length_factor;

		if( block->m_XLength )
		{
			x_length = block->m_XLength->m_value*0.5*length_factor;
		}
		if( block->m_YLength )
		{
			y_length = block->m_YLength->m_value*0.5*length_factor;
		}
		if( block->m_ZLength )
		{
			z_length = block->m_ZLength->m_value*0.5*length_factor;
		}

		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, y_length, z_length));
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(-x_length, y_length, z_length));
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(-x_length,-y_length, z_length));
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length,-y_length, z_length));
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, y_length,-z_length));
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(-x_length, y_length,-z_length));
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(-x_length,-y_length,-z_length));
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length,-y_length,-z_length));

		polyhedron_data->addFace(0,	1, 2);
		polyhedron_data->addFace(2, 3, 0);

		polyhedron_data->addFace(7, 6, 5);
		polyhedron_data->addFace(5, 4, 7);

		polyhedron_data->addFace(0,	4, 5);
		polyhedron_data->addFace(5, 1, 0);

		polyhedron_data->addFace(1, 5, 6);
		polyhedron_data->addFace(6, 2, 1);

		polyhedron_data->addFace(2, 6, 7);
		polyhedron_data->addFace(7, 3, 2);

		polyhedron_data->addFace(3, 7, 4);
		polyhedron_data->addFace(4, 0, 3);

		item_data->closed_polyhedrons.push_back( polyhedron_data );
		return;
	}

	shared_ptr<IfcRectangularPyramid> rectangular_pyramid = dynamic_pointer_cast<IfcRectangularPyramid>(csg_primitive);
	if( rectangular_pyramid )
	{
		double x_length = length_factor;
		double y_length = length_factor;
		double height = length_factor;

		if( rectangular_pyramid->m_XLength )
		{
			x_length = rectangular_pyramid->m_XLength->m_value*0.5*length_factor;
		}
		if( rectangular_pyramid->m_YLength )
		{
			y_length = rectangular_pyramid->m_YLength->m_value*0.5*length_factor;
		}
		if( rectangular_pyramid->m_Height )
		{
			height = rectangular_pyramid->m_Height->m_value*0.5*length_factor;
		}

		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(0, 0, height) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length,-y_length, 0.0) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(-x_length,-y_length, 0.0) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(-x_length, y_length, 0.0) );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR( x_length, y_length, 0.0) );

		polyhedron_data->addFace(1,	2, 3);
		polyhedron_data->addFace(3, 4, 1);
		polyhedron_data->addFace(0,	2, 1);
		polyhedron_data->addFace(0,	1, 4);
		polyhedron_data->addFace(0,	4, 3);
		polyhedron_data->addFace(0,	3, 2);

		item_data->closed_polyhedrons.push_back( polyhedron_data );
		return;
	}

	shared_ptr<IfcRightCircularCone> right_circular_cone = dynamic_pointer_cast<IfcRightCircularCone>(csg_primitive);
	if( right_circular_cone )
	{
		if( !right_circular_cone->m_Height )
		{
			std::cout << "IfcRightCircularCone: height not given" << std::endl;
			return;
		}
		if( !right_circular_cone->m_BottomRadius )
		{
			std::cout << "IfcRightCircularCone: radius not given" << std::endl;
			return;
		}

		double height = right_circular_cone->m_Height->m_value*length_factor;
		double radius = right_circular_cone->m_BottomRadius->m_value*length_factor;

		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(0.0, 0.0, height) ); // top
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(0.0, 0.0, 0.0) ); // bottom center

		double angle = 0;
		double d_angle = 2.0*M_PI/double(m_geom_settings->m_num_vertices_per_circle);	// TODO: adapt to model size and complexity
		for( int i = 0; i < m_geom_settings->m_num_vertices_per_circle; ++i )
		{
			polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(sin(angle)*radius, cos(angle)*radius, 0.0) );
			angle += d_angle;
		}

		// outer shape
		for( int i = 0; i < m_geom_settings->m_num_vertices_per_circle-1; ++i )
		{
			polyhedron_data->addFace(0, i+3, i+2);
		}
		polyhedron_data->addFace( 0, 2, m_geom_settings->m_num_vertices_per_circle+1 );

		// bottom circle
		for( int i = 0; i < m_geom_settings->m_num_vertices_per_circle-1; ++i )
		{
			polyhedron_data->addFace(1, i+2, i+3 );
		}
		polyhedron_data->addFace(1, m_geom_settings->m_num_vertices_per_circle+1, 2 );

		item_data->closed_polyhedrons.push_back( polyhedron_data );
		return;
	}

	shared_ptr<IfcRightCircularCylinder> right_circular_cylinder = dynamic_pointer_cast<IfcRightCircularCylinder>(csg_primitive);
	if( right_circular_cylinder )
	{
		if( !right_circular_cylinder->m_Height )
		{
			std::cout << "IfcRightCircularCylinder: height not given" << std::endl;
			return;
		}

		if( !right_circular_cylinder->m_Radius )
		{
			std::cout << "IfcRightCircularCylinder: radius not given" << std::endl;
			return;
		}

		int slices = m_geom_settings->m_num_vertices_per_circle;
		double rad = 0;

		//carve::mesh::MeshSet<3> * cylinder_mesh = makeCylinder( slices, rad, height, primitive_placement_matrix);
		double height = right_circular_cylinder->m_Height->m_value*length_factor;
		double radius = right_circular_cylinder->m_Radius->m_value*length_factor;

		double angle = 0;
		double d_angle = 2.0*M_PI/double(m_geom_settings->m_num_vertices_per_circle);	// TODO: adapt to model size and complexity
		for( int i = 0; i < m_geom_settings->m_num_vertices_per_circle; ++i )
		{
			polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(sin(angle)*radius, cos(angle)*radius, height) );
			polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(sin(angle)*radius, cos(angle)*radius, 0.0) );
			angle += d_angle;
		}

		for( int i = 0; i < m_geom_settings->m_num_vertices_per_circle-1; ++i )
		{
			polyhedron_data->addFace( 0, i*2+2, i*2+4 );		// top cap:		0-2-4	0-4-6		0-6-8
			polyhedron_data->addFace( 1, i*2+3, i*2+5 );		// bottom cap:	1-3-5	1-5-7		1-7-9
			//polyhedron_data->addFace( i, i+1, i+3, i+2 );		// side
			polyhedron_data->addFace( i, i+1, i+3 );		// side
			polyhedron_data->addFace( i+3, i+2, i );		// side
		}

		//polyhedron_data->addFace( 2*m_geom_settings->m_num_vertices_per_circle-2,	2*m_geom_settings->m_num_vertices_per_circle-1,		1,	0 );		// side
		polyhedron_data->addFace( 2*m_geom_settings->m_num_vertices_per_circle-2,	2*m_geom_settings->m_num_vertices_per_circle-1,		1 );		// side
		polyhedron_data->addFace( 1,	0,	2*m_geom_settings->m_num_vertices_per_circle-2 );		// side

		item_data->closed_polyhedrons.push_back( polyhedron_data );
		return;
	}

	shared_ptr<IfcSphere> sphere = dynamic_pointer_cast<IfcSphere>(csg_primitive);
	if( sphere )
	{
		if( !sphere->m_Radius )
		{
			std::cout << "IfcSphere: radius not given" << std::endl;
			return;
		}

		double radius = sphere->m_Radius->m_value;

		//        \   |   /
		//         2- 1 -nvc
		//        / \ | / \
		//    ---3--- 0 ---7---
		//       \  / | \ /
		//         4- 5 -6
		//        /   |   \

		shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(0.0, 0.0, radius) ); // top

		const int nvc = m_geom_settings->m_num_vertices_per_circle;
		const int num_vertical_edges = nvc*0.5;
		double d_vertical_angle = M_PI/double(num_vertical_edges-1);	// TODO: adapt to model size and complexity
		double vertical_angle = d_vertical_angle;

		for( int vertical = 1; vertical < num_vertical_edges-1; ++vertical )
		{
			// for each vertical angle, add one horizontal circle
			double vertical_level = cos( vertical_angle )*radius;
			double radius_at_level = sin( vertical_angle )*radius;
			double horizontal_angle = 0;
			double d_horizontal_angle = 2.0*M_PI/double(nvc);
			for( int i = 0; i < nvc; ++i )
			{
				polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(sin(horizontal_angle)*radius_at_level, cos(horizontal_angle)*radius_at_level, vertical_level) );
				horizontal_angle += d_horizontal_angle;
			}
			vertical_angle += d_vertical_angle;
		}
		polyhedron_data->addVertex( primitive_placement_matrix*carve::geom::VECTOR(0.0, 0.0, -radius) ); // bottom

		// uppper triangle fan
		for( int i = 0; i < nvc-1; ++i )
		{
			polyhedron_data->addFace(0, i+2, i+1);
		}
		polyhedron_data->addFace(0, 1, nvc);

		for( int vertical = 1; vertical < num_vertical_edges-2; ++vertical )
		{
			int offset_inner = nvc*(vertical-1) + 1;
			int offset_outer = nvc*vertical + 1;
			for( int i = 0; i < nvc-1; ++i )
			{
				//polyhedron_data->addFace( offset_inner+i, offset_inner+1+i, offset_outer+1+i,  offset_outer+i );
				polyhedron_data->addFace( offset_inner+i,		offset_inner+1+i,	offset_outer+1+i );
				polyhedron_data->addFace( offset_outer+1+i,		offset_outer+i,		offset_inner+i );
			}
			//polyhedron_data->addFace( offset_inner+nvc-1, offset_inner, offset_outer,  offset_outer+nvc-1 );
			polyhedron_data->addFace( offset_inner+nvc-1,	offset_inner,			offset_outer );
			polyhedron_data->addFace( offset_outer,			offset_outer+nvc-1,		offset_inner+nvc-1 );

		}

		// lower triangle fan
		int last_index = (num_vertical_edges-2)*nvc + 1;
		for( int i = 0; i < nvc-1; ++i )
		{
			polyhedron_data->addFace(last_index, last_index-(i+2), last_index-(i+1) );
		}
		polyhedron_data->addFace(last_index, last_index-1, last_index-nvc);
		item_data->closed_polyhedrons.push_back( polyhedron_data );
		return;
	}
	strs_err << "Unhandled IFC Representation: #" << csg_primitive->getId() << "=" << csg_primitive->classname() << std::endl;
}


void extrudeBox( const std::vector<carve::geom::vector<3> >& boundary_points, const carve::geom::vector<3>& extrusion_vector, shared_ptr<carve::input::PolyhedronData>& box_data )
{
	box_data->addVertex( boundary_points[0] );
	box_data->addVertex( boundary_points[1] );
	box_data->addVertex( boundary_points[2] );
	box_data->addVertex( boundary_points[3] );
	box_data->addVertex( boundary_points[0] + extrusion_vector );
	box_data->addVertex( boundary_points[1] + extrusion_vector );
	box_data->addVertex( boundary_points[2] + extrusion_vector );
	box_data->addVertex( boundary_points[3] + extrusion_vector );
	box_data->addFace( 0,1,2 );
	box_data->addFace( 2,3,0 );
	box_data->addFace( 1,5,6 );
	box_data->addFace( 6,2,1 );
	box_data->addFace( 5,4,7 );
	box_data->addFace( 7,6,5 );
	box_data->addFace( 0,3,7 );
	box_data->addFace( 7,4,0 );
	box_data->addFace( 0,4,5 );
	box_data->addFace( 5,1,0 );
	box_data->addFace( 2,6,7 );
	box_data->addFace( 7,3,2 );
}

void SolidModelConverter::convertIfcBooleanOperand( const shared_ptr<IfcBooleanOperand>& operand, shared_ptr<ItemData> item_data, const shared_ptr<ItemData>& other_operand, std::stringstream& strs_err )
{
	shared_ptr<IfcSolidModel> solid_model = dynamic_pointer_cast<IfcSolidModel>(operand);
	if( solid_model )
	{
		convertIfcSolidModel( solid_model, item_data, strs_err );
		return;
	}
	double length_factor = m_unit_converter->getLengthInMeterFactor();

	shared_ptr<IfcHalfSpaceSolid> half_space_solid = dynamic_pointer_cast<IfcHalfSpaceSolid>(operand);
	if( half_space_solid )
	{
		//ENTITY IfcHalfSpaceSolid SUPERTYPE OF(ONEOF(IfcBoxedHalfSpace, IfcPolygonalBoundedHalfSpace))
		shared_ptr<IfcSurface> base_surface = half_space_solid->m_BaseSurface;

		// base surface
		shared_ptr<IfcElementarySurface> elem_base_surface = dynamic_pointer_cast<IfcElementarySurface>(base_surface);
		if( !elem_base_surface )
		{
			strs_err << __FUNC__ << ": The base surface shall be an unbounded surface (subtype of IfcElementarySurface)" << std::endl;
			return;
		}
		shared_ptr<IfcAxis2Placement3D>& base_surface_pos = elem_base_surface->m_Position;
		carve::geom::plane<3> base_surface_plane;
		carve::geom::vector<3> base_surface_position;
		carve::math::Matrix base_position_matrix( carve::math::Matrix::IDENT() );
		if( base_surface_pos )
		{
			PlacementConverter::getPlane( base_surface_pos, base_surface_plane, base_surface_position, length_factor );
			PlacementConverter::convertIfcAxis2Placement3D( base_surface_pos, base_position_matrix, length_factor );
		}

		// If the agreement flag is TRUE, then the subset is the one the normal points away from
		bool agreement = half_space_solid->m_AgreementFlag;
		if( !agreement )
		{
			base_surface_plane.negate();
		}

		shared_ptr<IfcBoxedHalfSpace> boxed_half_space = dynamic_pointer_cast<IfcBoxedHalfSpace>(half_space_solid);
		if( boxed_half_space )
		{
			shared_ptr<IfcBoundingBox> bbox = boxed_half_space->m_Enclosure;
			if( !bbox )
			{
				strs_err << __FUNC__ << ": IfcBoxedHalfSpace: Enclosure not given" << std::endl;
				return;
			}

			if( !bbox->m_Corner || !bbox->m_XDim || !bbox->m_YDim || !bbox->m_ZDim )
			{
				strs_err << __FUNC__ << ": IfcBoxedHalfSpace: Enclosure not valid" << std::endl;
				return;
			}
			shared_ptr<IfcCartesianPoint>&			bbox_corner = bbox->m_Corner;
			shared_ptr<IfcPositiveLengthMeasure>&	bbox_x_dim = bbox->m_XDim;
			shared_ptr<IfcPositiveLengthMeasure>&	bbox_y_dim = bbox->m_YDim;
			shared_ptr<IfcPositiveLengthMeasure>&	bbox_z_dim = bbox->m_ZDim;

			carve::geom::vector<3> corner;
			m_curve_converter->convertIfcCartesianPoint( bbox_corner, corner );
			carve::math::Matrix box_position_matrix = base_position_matrix*carve::math::Matrix::TRANS( corner );

			// else, its an unbounded half space solid, create simple box
			shared_ptr<carve::input::PolyhedronData> polyhedron_data( new carve::input::PolyhedronData() );
			polyhedron_data->addVertex( carve::geom::VECTOR( bbox_x_dim->m_value, bbox_y_dim->m_value, bbox_z_dim->m_value));
			polyhedron_data->addVertex( carve::geom::VECTOR(-bbox_x_dim->m_value, bbox_y_dim->m_value, bbox_z_dim->m_value));
			polyhedron_data->addVertex( carve::geom::VECTOR(-bbox_x_dim->m_value,-bbox_y_dim->m_value, bbox_z_dim->m_value));
			polyhedron_data->addVertex( carve::geom::VECTOR( bbox_x_dim->m_value,-bbox_y_dim->m_value, bbox_z_dim->m_value));
			polyhedron_data->addVertex( carve::geom::VECTOR( bbox_x_dim->m_value, bbox_y_dim->m_value,-bbox_z_dim->m_value));
			polyhedron_data->addVertex( carve::geom::VECTOR(-bbox_x_dim->m_value, bbox_y_dim->m_value,-bbox_z_dim->m_value));
			polyhedron_data->addVertex( carve::geom::VECTOR(-bbox_x_dim->m_value,-bbox_y_dim->m_value,-bbox_z_dim->m_value));
			polyhedron_data->addVertex( carve::geom::VECTOR( bbox_x_dim->m_value,-bbox_y_dim->m_value,-bbox_z_dim->m_value));

			polyhedron_data->addFace(0,	1, 2);
			polyhedron_data->addFace(2, 3, 0);
			polyhedron_data->addFace(7, 6, 5);
			polyhedron_data->addFace(5, 4, 7);
			polyhedron_data->addFace(0,	4, 5);
			polyhedron_data->addFace(5, 1, 0);
			polyhedron_data->addFace(1, 5, 6);
			polyhedron_data->addFace(6, 2, 1);
			polyhedron_data->addFace(2, 6, 7);
			polyhedron_data->addFace(7, 3, 2);
			polyhedron_data->addFace(3, 7, 4);
			polyhedron_data->addFace(4, 0, 3);

			item_data->closed_polyhedrons.push_back( polyhedron_data );

			// apply box coordinate system
			for( std::vector<carve::geom::vector<3> >::iterator it_points = polyhedron_data->points.begin(); it_points != polyhedron_data->points.end(); ++it_points )
			{
				carve::geom::vector<3> & poly_point = (*it_points);
				poly_point = box_position_matrix*poly_point;
			}

			return;
		}

		// check dimenstions of other operand
		double extrusion_depth = HALF_SPACE_BOX_SIZE;
		//carve::geom::vector<3> other_operand_pos = base_surface_position;
		if( other_operand )
		{
			carve::geom::aabb<3> aabb;
			other_operand->createMeshSetsFromClosedPolyhedrons();
			for( int ii=0; ii<other_operand->meshsets.size(); ++ii )
			{
				shared_ptr<carve::mesh::MeshSet<3> >& meshset = other_operand->meshsets[ii];
				if( ii == 0 )
				{
					aabb.pos	=	meshset->getAABB().pos;
					aabb.extent =	meshset->getAABB().extent;
				}
				else
				{
					aabb.unionAABB( meshset->getAABB() );
				}
			}

			carve::geom::vector<3>& aabb_extent = aabb.extent;
			double max_extent = std::max( aabb_extent.x, std::max( aabb_extent.y, aabb_extent.z ) );
			extrusion_depth = 2.0*max_extent;
			//other_operand_pos = aabb.pos;
		}

		shared_ptr<IfcPolygonalBoundedHalfSpace> polygonal_half_space = dynamic_pointer_cast<IfcPolygonalBoundedHalfSpace>(half_space_solid);
		if( polygonal_half_space )
		{
			// ENTITY IfcPolygonalBoundedHalfSpace 
			//	SUBTYPE OF IfcHalfSpaceSolid;
			//	Position	 :	IfcAxis2Placement3D;
			//	PolygonalBoundary	 :	IfcBoundedCurve;

			carve::math::Matrix boundary_position_matrix( carve::math::Matrix::IDENT() );
			carve::geom::vector<3> boundary_plane_normal( carve::geom::VECTOR( 0, 0, 1 ) );
			carve::geom::vector<3> boundary_position;
			if( polygonal_half_space->m_Position )
			{
				PlacementConverter::convertIfcAxis2Placement3D( polygonal_half_space->m_Position, boundary_position_matrix, length_factor );
				boundary_plane_normal = carve::geom::VECTOR( boundary_position_matrix._31, boundary_position_matrix._32, boundary_position_matrix._33 );
				boundary_position = carve::geom::VECTOR( boundary_position_matrix._41, boundary_position_matrix._42, boundary_position_matrix._43 );
			}

			// PolygonalBoundary is given in 2D
			std::vector<carve::geom::vector<2> > polygonal_boundary;
			std::vector<carve::geom::vector<2> > segment_start_points_2d;
			shared_ptr<IfcBoundedCurve> bounded_curve = polygonal_half_space->m_PolygonalBoundary;
			m_curve_converter->convertIfcCurve2D( bounded_curve, polygonal_boundary, segment_start_points_2d );
			ProfileConverter::deleteLastPointIfEqualToFirst( polygonal_boundary );
			ProfileConverter::simplifyPath( polygonal_boundary );

			//if( other_operand )
			//{
			//	extrusion_depth = extrusion_depth*2.0;
			//}
			carve::geom::vector<3> solid_extrusion_direction = boundary_plane_normal;
			double agreement_check = dot( base_surface_plane.N, boundary_plane_normal );
			if( agreement_check > 0 )
			{
				solid_extrusion_direction = -solid_extrusion_direction;
			}

			std::stringstream err;
			std::vector<std::vector<carve::geom::vector<2> > > paths;
			paths.push_back( polygonal_boundary );
			shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData );
			GeomUtils::extrude( paths, carve::geom::vector<3>( carve::geom::VECTOR( 0, 0, extrusion_depth ) ), poly_data, err );

			const int num_poly_boundary_points = polygonal_boundary.size();
			if( poly_data->points.size() != 2*num_poly_boundary_points )
			{
				strs_err << __FUNC__ << " problems in extrude: poly_data->points.size() != 2*polygonal_boundary.size()" << std::endl;
				return;
			}

			// apply position of PolygonalBoundary
			GeomUtils::applyPosition( poly_data, boundary_position_matrix );

			// project to base surface
			for( int i_base_point = 0; i_base_point < poly_data->points.size(); ++i_base_point )
			{
				carve::geom::vector<3>& poly_point = poly_data->points[i_base_point];//(*it_points);

				// points below the base surface are projected into plane
				//double distance_to_base_surface = carve::geom::distance(base_surface_plane, poly_point);
				carve::geom::vector<3> v;
				double t;
				carve::IntersectionClass intersect = carve::geom3d::rayPlaneIntersection( base_surface_plane, poly_point, poly_point + boundary_plane_normal, v, t);
				if( intersect > 0 )
				{
					if( agreement_check > 0 )
					{
						if( i_base_point < num_poly_boundary_points )
						{
							poly_point = v + solid_extrusion_direction*extrusion_depth;
						}
						else
						{
							poly_point = v;
						}
					}
					else
					{
						if( i_base_point < num_poly_boundary_points )
						{
							poly_point = v;
						}
						else
						{
							poly_point = v + solid_extrusion_direction*extrusion_depth;
						}
					}
				}
				else
				{
					std::cout << "no intersection found" << std::endl;
				}
			}

			item_data->closed_polyhedrons.push_back( poly_data );

			//shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh(carve::input::opts()) );
			//	renderMeshsetInDebugViewer( meshset.get(), osg::Vec4(1.0f, 0.5f, 0.0f, 1.0f), true );

			//	for( int ii=0; ii<other_operand->meshsets.size(); ++ii )
			//	{
			//	shared_ptr<carve::mesh::MeshSet<3> >& meshset = other_operand->meshsets[ii];
			//	renderMeshsetInDebugViewer( meshset.get(), osg::Vec4(0.8f, 0.0f, 1.0f, 1.0f), true );
			//	}

		}
		else
		{
			// else, its an unbounded half space solid, create simple box

			int var = 0;

			if( var == 0 )
			{
				shared_ptr<carve::input::PolylineSetData> surface_data ( new carve::input::PolylineSetData() );
				m_face_converter->convertIfcSurface( base_surface, surface_data );
				std::vector<carve::geom::vector<3> > base_surface_points = surface_data->points;

				if( base_surface_points.size() != 4 )
				{
					std::cout << __FUNC__ << ": invalid IfcHalfSpaceSolid.BaseSurface" << std::endl;
					return;
				}
				// If the agreement flag is TRUE, then the subset is the one the normal points away from
				bool agreement = half_space_solid->m_AgreementFlag;
				if( !agreement )
				{
					std::reverse( base_surface_points.begin(), base_surface_points.end() );
				}
				carve::geom::vector<3>  base_surface_normal = GeomUtils::computePolygonNormal( base_surface_points );

				carve::geom::vector<3>  half_space_extrusion_direction = -base_surface_normal;
				carve::geom::vector<3>  half_space_extrusion_vector = half_space_extrusion_direction*HALF_SPACE_BOX_SIZE;
				shared_ptr<carve::input::PolyhedronData> half_space_box_data( new carve::input::PolyhedronData() );
				item_data->closed_polyhedrons.push_back(half_space_box_data);
				extrudeBox( base_surface_points, half_space_extrusion_vector, half_space_box_data );
			}
			
			if( var == 1 )
			{
				std::vector<carve::geom::vector<3> > box_base_points;
				box_base_points.push_back( base_position_matrix*carve::geom::VECTOR( extrusion_depth,  extrusion_depth, 0.0 ));
				box_base_points.push_back( base_position_matrix*carve::geom::VECTOR(-extrusion_depth,  extrusion_depth, 0.0 ));
				box_base_points.push_back( base_position_matrix*carve::geom::VECTOR(-extrusion_depth, -extrusion_depth, 0.0 ));
				box_base_points.push_back( base_position_matrix*carve::geom::VECTOR( extrusion_depth, -extrusion_depth, 0.0 ));

				carve::geom::vector<3>  half_space_extrusion_direction = -base_surface_plane.N;
				carve::geom::vector<3>  half_space_extrusion_vector = half_space_extrusion_direction*extrusion_depth;

				carve::geom::vector<3>  box_base_normal = GeomUtils::computePolygonNormal( box_base_points );
				double dot_normal = dot( box_base_normal, base_surface_plane.N );
				if( dot_normal > 0 )
				{
				std::reverse( box_base_points.begin(), box_base_points.end() );
				}

				shared_ptr<carve::input::PolyhedronData> half_space_box_data( new carve::input::PolyhedronData() );
				item_data->closed_polyhedrons.push_back(half_space_box_data);
				extrudeBox( box_base_points, half_space_extrusion_vector, half_space_box_data );
			}

			return;
		}
		return;
	}

	shared_ptr<IfcBooleanResult> boolean_result = dynamic_pointer_cast<IfcBooleanResult>(operand);
	if( boolean_result )
	{
		convertIfcBooleanResult( boolean_result, item_data, strs_err );
		return;
	}

	shared_ptr<IfcCsgPrimitive3D> csg_primitive3D = dynamic_pointer_cast<IfcCsgPrimitive3D>(operand);
	if( csg_primitive3D )
	{
		convertIfcCsgPrimitive3D( csg_primitive3D, item_data, strs_err );
		return;
	}

	strs_err << "Unhandled IFC Representation: " << operand->classname() << std::endl;
}
