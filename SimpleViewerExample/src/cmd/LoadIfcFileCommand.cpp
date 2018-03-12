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

#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

#include <ifcpp/geometry/Carve/IncludeCarveHeaders.h>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingObject.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/BuildingGuid.h>
#include <ifcpp/reader/ReaderSTEP.h>

#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcBuilding.h>
#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcProductDefinitionShape.h>
#include <ifcpp/IFC4/include/IfcShapeRepresentation.h>
#include <ifcpp/IFC4/include/IfcSite.h>
#include <ifcpp/IFC4/include/IfcText.h>
#include <ifcpp/IFC4/include/IfcWall.h>

#include "IncludeGeometryHeaders.h"
#include "Command.h"
#include "IfcPlusPlusSystem.h"
#include "LoadIfcFileCommand.h"

LoadIfcFileCommand::LoadIfcFileCommand( IfcPlusPlusSystem* system ) : Command( system )
{
}

LoadIfcFileCommand::~LoadIfcFileCommand() {}

void LoadIfcFileCommand::setFilePath( std::wstring& path_in )
{
	m_file_path = path_in;
}

bool LoadIfcFileCommand::doCmd()
{
	if( m_file_path.length() == 0 )
	{
		return false;
	}

	// first remove previously loaded geometry from scenegraph
	osg::ref_ptr<osg::Switch> model_switch = m_system->getModelNode();
	SceneGraphUtils::clearAllChildNodes( model_switch );
	m_system->clearSelection();

	// reset the IFC model
	shared_ptr<GeometryConverter> geometry_converter = m_system->getGeometryConverter();
	geometry_converter->resetModel();
	std::stringstream err;

	try
	{
		// load file to IFC model
		geometry_converter->clearMessagesCallback();
		geometry_converter->resetModel();
		m_system->getModelReader()->loadModelFromFile( m_file_path, geometry_converter->getBuildingModel() );
		geometry_converter->convertGeometry();

		// convert Carve geometry to OSG
		shared_ptr<ConverterOSG> converter_osg( new ConverterOSG( geometry_converter->getGeomSettings() ) );
		converter_osg->setMessageTarget( geometry_converter.get() );
		converter_osg->convertToOSG( geometry_converter->getShapeInputData(), model_switch );

		// in case there are IFC entities that are not in the spatial structure
		const std::map<int, shared_ptr<BuildingObject> >& objects_outside_spatial_structure = geometry_converter->getObjectsOutsideSpatialStructure();
		if( objects_outside_spatial_structure.size() > 0 )
		{
			osg::ref_ptr<osg::Switch> sw_objects_outside_spatial_structure = new osg::Switch();
			sw_objects_outside_spatial_structure->setName( "IfcProduct objects outside spatial structure" );

			converter_osg->addNodes( objects_outside_spatial_structure, sw_objects_outside_spatial_structure );
			if( sw_objects_outside_spatial_structure->getNumChildren() > 0 )
			{
				model_switch->addChild( sw_objects_outside_spatial_structure );
			}
		}
	}
	catch( OutOfMemoryException& e)
	{
		throw e;
	}
	catch( BuildingException& e )
	{
		err << e.what();
	}
	catch( std::exception& e )
	{
		err << e.what();
	}
	catch( ... )
	{
		err << "loadModelFromFile, createGeometryOSG failed" << std::endl;
	}

	try
	{
		if( model_switch )
		{
			bool optimize = true;
			if( optimize )
			{
				osgUtil::Optimizer opt;
				opt.optimize(model_switch);
			}

			// if model bounding sphere is far from origin, move to origin
			const osg::BoundingSphere& bsphere = model_switch->getBound();
			if( bsphere.center().length() > 10000 )
			{
				if( bsphere.center().length()/bsphere.radius() > 100 )
				{
					std::unordered_set<osg::Geode*> set_applied;
					SceneGraphUtils::translateGroup( model_switch, -bsphere.center(), set_applied );
				}
			}
		}
	}
	catch(std::exception& e)
	{
		err << e.what();
	}

	geometry_converter->clearInputCache();
	
	if( err.tellp() > 0 )
	{
		throw BuildingException( err.str().c_str() );
	}

	return true;
}

bool LoadIfcFileCommand::undo()
{
	return true;
}

bool LoadIfcFileCommand::redo()
{
	return true;
}

void LoadIfcFileCommand::loadWallExample()
{
	// create new IFC model
	shared_ptr<BuildingModel> ifc_model = m_system->getIfcModel();
	if( !ifc_model )
	{
		ifc_model = shared_ptr<BuildingModel>( new BuildingModel() );
	}
	double unit_length_factor = 1.0;
	ifc_model->clearIfcModel();
	ifc_model->getUnitConverter()->setLengthInMeterFactor( unit_length_factor );
	shared_ptr<GeometryConverter> geometry_converter = m_system->getGeometryConverter();
	geometry_converter->setModel( ifc_model );

	
	std::vector<shared_ptr<BuildingEntity> > vec_new_entities;

	// create wall object
	shared_ptr<IfcWall> wall( new IfcWall() );
	vec_new_entities.push_back( wall );
	wall->m_Name = shared_ptr<IfcLabel>( new IfcLabel() );
	wall->m_Name->m_value = L"Hello Wall";
	wall->m_Description = shared_ptr<IfcText>( new IfcText() );
	wall->m_Description->m_value = L"Wall example";

	if( true )
	{
		// Example for property set
		shared_ptr<IfcPropertySet> ifc_property_set( new IfcPropertySet() );
		vec_new_entities.push_back( ifc_property_set );
		ifc_property_set->m_GlobalId = shared_ptr<IfcGloballyUniqueId>( new IfcGloballyUniqueId( createGUID32_wstr() ) );

		ifc_property_set->m_Name = shared_ptr<IfcLabel>( new IfcLabel() );
		ifc_property_set->m_Name->m_value = L"Pset_Wall";   // name of the property set

		ifc_property_set->m_Description = shared_ptr<IfcText>( new IfcText() );
		ifc_property_set->m_Description->m_value = L"Pset to define Wall";

		std::vector<shared_ptr<IfcProperty> >& vec_properties = ifc_property_set->m_HasProperties;

		{
			shared_ptr<IfcPropertySingleValue> prop1( new IfcPropertySingleValue() );
			vec_new_entities.push_back( prop1 );
			vec_properties.push_back( prop1 );
			prop1->m_Name = shared_ptr<IfcIdentifier>( new IfcIdentifier() );  // name of property 1
			prop1->m_Name->m_value = L"WallIentifier";

			shared_ptr<IfcIdentifier> prop1_value( new IfcIdentifier() );  // value property
			prop1_value->m_value = L"W1";
			prop1->m_NominalValue = prop1_value;
		}

		{
			shared_ptr<IfcPropertySingleValue> prop2( new IfcPropertySingleValue() );
			vec_new_entities.push_back( prop2 );
			vec_properties.push_back( prop2 );
			prop2->m_Name = shared_ptr<IfcIdentifier>( new IfcIdentifier() );
			prop2->m_Name->m_value = L"Footprint area";

			shared_ptr<IfcReal> prop2_value( new IfcReal() );
			prop2_value->m_value = 0.8;
			prop2->m_NominalValue = prop2_value;
		}

		shared_ptr<IfcRelDefinesByProperties> ifc_rel_defines( new IfcRelDefinesByProperties() );
		vec_new_entities.push_back( ifc_rel_defines );

		ifc_rel_defines->m_RelatedObjects.clear();
		ifc_rel_defines->m_RelatedObjects.push_back( wall );
		ifc_rel_defines->m_RelatingPropertyDefinition = ifc_property_set;
	}


	// global placement
	shared_ptr<IfcAxis2Placement3D> placement_origin( new IfcAxis2Placement3D() );
	vec_new_entities.push_back( placement_origin );

	int entity_id = -1;
	carve::math::Matrix matrix1( carve::math::Matrix::IDENT() );
	geometry_converter->getRepresentationConverter()->getPlacementConverter()->convertMatrix( matrix1, placement_origin, entity_id, vec_new_entities ); // 1.0

	// local placement of wall
	shared_ptr<IfcLocalPlacement> local_placement( new IfcLocalPlacement() );
	entity_id = -1;
	carve::math::Matrix matrix2( carve::math::Matrix::TRANS( 10.0*unit_length_factor, 5.0*unit_length_factor, 0.2*unit_length_factor ) );
	geometry_converter->getRepresentationConverter()->getPlacementConverter()->convertMatrix( matrix2, placement_origin, entity_id, vec_new_entities ); // 1.0

	vec_new_entities.push_back( local_placement );
	local_placement->m_RelativePlacement = placement_origin;
	wall->m_ObjectPlacement = local_placement;


	// geometry
	double lower_level = 0.2*unit_length_factor;
	double upper_level = 3.5*unit_length_factor;
	shared_ptr<IfcShapeRepresentation> shape_representation( new IfcShapeRepresentation() );
	vec_new_entities.push_back( shape_representation );
	shape_representation->m_ContextOfItems; // TODO: set context
	shape_representation->m_RepresentationIdentifier = shared_ptr<IfcLabel>( new IfcLabel( L"Body" ) );
	shape_representation->m_RepresentationType = shared_ptr<IfcLabel>( new IfcLabel( L"SweptSolid" ) );

	shared_ptr<IfcProductDefinitionShape> product_def_shape( new IfcProductDefinitionShape() );
	vec_new_entities.push_back( product_def_shape );

	wall->m_Representation = product_def_shape;
	product_def_shape->m_Representations.push_back( shape_representation );

	// extruded solid
	shared_ptr<IfcExtrudedAreaSolid> extruded_solid( new IfcExtrudedAreaSolid() );
	vec_new_entities.push_back( extruded_solid );

	extruded_solid->m_Position = shared_ptr<IfcAxis2Placement3D>( new IfcAxis2Placement3D() );
	vec_new_entities.push_back( extruded_solid->m_Position );
	extruded_solid->m_Position->m_Axis = shared_ptr<IfcDirection>( new IfcDirection() );  // z axis
	vec_new_entities.push_back( extruded_solid->m_Position->m_Axis );
	extruded_solid->m_Position->m_Axis->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 0 ) ) );
	extruded_solid->m_Position->m_Axis->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 0 ) ) );
	extruded_solid->m_Position->m_Axis->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 1 ) ) );

	extruded_solid->m_Position->m_RefDirection = shared_ptr<IfcDirection>( new IfcDirection() );  // x axis
	vec_new_entities.push_back( extruded_solid->m_Position->m_RefDirection );
	extruded_solid->m_Position->m_RefDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 1 ) ) );
	extruded_solid->m_Position->m_RefDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 0 ) ) );
	extruded_solid->m_Position->m_RefDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 0 ) ) );

	extruded_solid->m_Position->m_Location = shared_ptr<IfcCartesianPoint>( new IfcCartesianPoint() );
	vec_new_entities.push_back( extruded_solid->m_Position->m_Location );
	extruded_solid->m_Position->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0 ) ) );
	extruded_solid->m_Position->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0 ) ) );
	extruded_solid->m_Position->m_Location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( lower_level ) ) );

	extruded_solid->m_ExtrudedDirection = shared_ptr<IfcDirection>( new IfcDirection() );
	vec_new_entities.push_back( extruded_solid->m_ExtrudedDirection );
	extruded_solid->m_ExtrudedDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 0 ) ) );
	extruded_solid->m_ExtrudedDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 0 ) ) );
	extruded_solid->m_ExtrudedDirection->m_DirectionRatios.push_back( shared_ptr<IfcReal>( new IfcReal( 1 ) ) );
	extruded_solid->m_Depth = shared_ptr<IfcPositiveLengthMeasure>( new IfcPositiveLengthMeasure() );
	extruded_solid->m_Depth->m_value = ( upper_level - lower_level );

	shared_ptr<IfcArbitraryClosedProfileDef> swept_area( new IfcArbitraryClosedProfileDef() );
	vec_new_entities.push_back( swept_area );
	extruded_solid->m_SweptArea = swept_area;

	shared_ptr<IfcPolyline> poly_line( new IfcPolyline() );
	vec_new_entities.push_back( poly_line );
	swept_area->m_OuterCurve = poly_line;

	shared_ptr<IfcCartesianPoint> ifc_point1( new IfcCartesianPoint() );
	vec_new_entities.push_back( ifc_point1 );
	ifc_point1->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0 ) ) );
	ifc_point1->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0 ) ) );
	poly_line->m_Points.push_back( ifc_point1 );


	shared_ptr<IfcCartesianPoint> ifc_point2( new IfcCartesianPoint() );
	vec_new_entities.push_back( ifc_point2 );
	ifc_point2->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 5.0*unit_length_factor ) ) );
	ifc_point2->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0 ) ) );
	poly_line->m_Points.push_back( ifc_point2 );

	shared_ptr<IfcCartesianPoint> ifc_point3( new IfcCartesianPoint() );
	vec_new_entities.push_back( ifc_point3 );
	ifc_point3->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 5.0*unit_length_factor ) ) );
	ifc_point3->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0.25*unit_length_factor ) ) );
	poly_line->m_Points.push_back( ifc_point3 );


	shared_ptr<IfcCartesianPoint> ifc_point4( new IfcCartesianPoint() );
	vec_new_entities.push_back( ifc_point4 );
	ifc_point4->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0.0 ) ) );
	ifc_point4->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0.25*unit_length_factor ) ) );
	poly_line->m_Points.push_back( ifc_point4 );

	shape_representation->m_Items.push_back( extruded_solid );

	// building structure
	shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
	if( !ifc_project )
	{
		ifc_project = shared_ptr<IfcProject>( new IfcProject() );
		vec_new_entities.push_back( ifc_project );
		ifc_model->setIfcProject( ifc_project );
	}
	
	shared_ptr<IfcSite> ifc_site( new IfcSite() );
	vec_new_entities.push_back( ifc_site );

	shared_ptr<IfcBuilding> ifc_building( new IfcBuilding() );
	vec_new_entities.push_back( ifc_building );

	shared_ptr<IfcBuildingStorey> ifc_building_storey( new IfcBuildingStorey() );
	vec_new_entities.push_back( ifc_building_storey );

	shared_ptr<IfcRelAggregates> rel_aggregates_project_site( new IfcRelAggregates() );
	rel_aggregates_project_site->m_RelatingObject = ifc_project;
	rel_aggregates_project_site->m_RelatedObjects.push_back( ifc_site );
	vec_new_entities.push_back( rel_aggregates_project_site );

	shared_ptr<IfcRelAggregates> rel_aggregates_site_building( new IfcRelAggregates() );
	rel_aggregates_site_building->m_RelatingObject = ifc_site;
	rel_aggregates_site_building->m_RelatedObjects.push_back( ifc_building );
	vec_new_entities.push_back( rel_aggregates_site_building );

	shared_ptr<IfcRelAggregates> rel_aggregates_building_buildingstorey( new IfcRelAggregates() );
	rel_aggregates_building_buildingstorey->m_RelatingObject = ifc_building;
	rel_aggregates_building_buildingstorey->m_RelatedObjects.push_back( ifc_building_storey );
	vec_new_entities.push_back( rel_aggregates_building_buildingstorey );

	shared_ptr<IfcRelAggregates> rel_aggregates_buildingstorey_wall( new IfcRelAggregates() );
	rel_aggregates_buildingstorey_wall->m_RelatingObject = ifc_building_storey;
	rel_aggregates_buildingstorey_wall->m_RelatedObjects.push_back( wall );
	vec_new_entities.push_back( rel_aggregates_buildingstorey_wall );


	// insert entities into IFC model
	for( auto entity : vec_new_entities )
	{
		shared_ptr<IfcRoot> ifc_root_object = dynamic_pointer_cast<IfcRoot>( entity );
		if( ifc_root_object )
		{
			// each object that is derived from IfcRoot should have a GUID
			if( !ifc_root_object->m_GlobalId )
			{
				ifc_root_object->m_GlobalId = shared_ptr<IfcGloballyUniqueId>( new IfcGloballyUniqueId( createGUID32_wstr() ) );
			}
		}
		ifc_model->insertEntity( entity );
	}

	ifc_model->unsetInverseAttributes();
	ifc_model->resolveInverseAttributes();
	ifc_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"IFC4";
	m_system->setIfcModel( ifc_model );
	
	// create geometry for OSG
	osg::ref_ptr<osg::Switch> model_switch = m_system->getModelNode();
	SceneGraphUtils::clearAllChildNodes( model_switch );
	m_system->clearSelection();

	std::stringstream err;

	try
	{
		geometry_converter->clearMessagesCallback();
		geometry_converter->convertGeometry();

		// convert Carve or OCC geometry to OSG
		shared_ptr<ConverterOSG> converter_osg( new ConverterOSG( geometry_converter->getGeomSettings() ) );
		converter_osg->setMessageTarget( geometry_converter.get() );
		converter_osg->convertToOSG( geometry_converter->getShapeInputData(), model_switch );

		// in case there are IFC entities that are not in the spatial structure
		const std::map<int, shared_ptr<BuildingObject> >& objects_outside_spatial_structure = geometry_converter->getObjectsOutsideSpatialStructure();
		if( objects_outside_spatial_structure.size() > 0 )
		{
			osg::ref_ptr<osg::Switch> sw_objects_outside_spatial_structure = new osg::Switch();
			sw_objects_outside_spatial_structure->setName( "IfcProduct objects outside spatial structure" );

			converter_osg->addNodes( objects_outside_spatial_structure, sw_objects_outside_spatial_structure );
			if( sw_objects_outside_spatial_structure->getNumChildren() > 0 )
			{
				model_switch->addChild( sw_objects_outside_spatial_structure );
			}
		}
	}
	catch( OutOfMemoryException& e )
	{
		throw e;
	}
	catch( BuildingException& e )
	{
		err << e.what();
	}
	catch( std::exception& e )
	{
		err << e.what();
	}
	catch( ... )
	{
		err << "loadModelFromFile, createGeometryOSG failed" << std::endl;
	}
}
