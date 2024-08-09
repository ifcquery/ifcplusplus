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

#pragma warning( disable: 4996 )
#include <iostream>
#include <ctime>
#include <memory>

#include "IfcApplication.h"
#include "IfcAxis2Placement.h"
#include "IfcAxis2Placement3D.h"
#include "IfcBuilding.h"
#include "IfcCartesianPoint.h"
#include "IfcChangeActionEnum.h"
#include "IfcConversionBasedUnit.h"
#include "IfcDimensionCount.h"
#include "IfcDimensionalExponents.h"
#include "IfcDirection.h"
#include "IfcElementAssembly.h"
#include "IfcGeometricRepresentationContext.h"
#include "IfcGloballyUniqueId.h"
#include "IfcIdentifier.h"
#include "IfcLabel.h"
#include "IfcLengthMeasure.h"
#include "IfcMeasureWithUnit.h"
#include "IfcOrganization.h"
#include "IfcOwnerHistory.h"
#include "IfcPerson.h"
#include "IfcPersonAndOrganization.h"
#include "IfcPlaneAngleMeasure.h"
#include "IfcProduct.h"
#include "IfcProject.h"
#include "IfcReal.h"
#include "IfcRelationship.h"
#include "IfcRelAggregates.h"
#include "IfcRelContainedInSpatialStructure.h"
#include "IfcSite.h"
#include "IfcSIUnit.h"
#include "IfcSIUnitName.h"
#include "IfcStyledItem.h"
#include "IfcUnit.h"
#include "IfcUnitAssignment.h"
#include "IfcUnitEnum.h"
#include "IfcValue.h"
#include "ifcpp/IFC4X3/EntityFactory.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"

#include "AttributeObject.h"
#include "BuildingGuid.h"
#include "BuildingException.h"
#include "BuildingModel.h"
#include "UnitConverter.h"

using namespace IFC4X3;

BuildingModel::BuildingModel()
{
	m_unit_converter = std::make_shared<UnitConverter>( );
	m_unit_converter->setMessageTarget( this );
	initFileHeader( "IfcPlusPlus-export.ifc", "IfcPlusPlus" );
}

BuildingModel::~BuildingModel()= default;

std::string getIfcSchemaVersionString(BuildingModel::SchemaVersionEnum version)
{
	switch (version)
	{
	case BuildingModel::IFC2X: return "IFC2X";
	case BuildingModel::IFC2X2: return "IFC2X2";
	case BuildingModel::IFC2X3: return "IFC2X3";
	case BuildingModel::IFC2X4: return "IFC2X4";
	case BuildingModel::IFC4: return "IFC4";
	case BuildingModel::IFC4X1: return "IFC4X1";
	case BuildingModel::IFC4X3: return "IFC4X3";
	}
	return "IFC_VERSION_UNDEFINED";
};

std::string BuildingModel::getIfcSchemaVersionOfLoadedFile()
{
	return getIfcSchemaVersionString(m_ifc_schema_version_loaded_file);
}

std::string BuildingModel::getIfcSchemaVersionCurrent()
{
	return getIfcSchemaVersionString(m_ifc_schema_version_current);
}

void BuildingModel::initIfcModel()
{
	clearIfcModel();

	shared_ptr<IfcProject> project( new IfcProject( 1 ) );
	insertEntity(project);

	shared_ptr<IfcPerson> person( new IfcPerson() );
	person->m_FamilyName = std::make_shared<IfcLabel>( "FamilyName" );
	person->m_GivenName = std::make_shared<IfcLabel>( "GivenName" );
	insertEntity(person);
	
	shared_ptr<IfcOrganization> org( new IfcOrganization() );
	org->m_Name = std::make_shared<IfcLabel>( "OrganizationName" );
	insertEntity(org);

	shared_ptr<IfcPersonAndOrganization> person_org( new IfcPersonAndOrganization() );
	person_org->m_ThePerson = person;
	person_org->m_TheOrganization = org;
	insertEntity(person_org);

	shared_ptr<IfcApplication> app( new IfcApplication() );
	app->m_ApplicationDeveloper = org;
	app->m_Version = std::make_shared<IfcLabel>( "1.0" );
	app->m_ApplicationFullName = std::make_shared<IfcLabel>( "IfcPlusPlus" );
	app->m_ApplicationIdentifier = std::make_shared<IfcIdentifier>( "IfcPlusPlus" );
	insertEntity(app);

	shared_ptr<IfcCartesianPoint> point( new IfcCartesianPoint() );
	point->m_Coordinates[0] = 0.0;
	point->m_Coordinates[1] = 0.0;
	point->m_Coordinates[2] = 0.0;
	//point->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>( 0.0 ) );
	//point->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>( 0.0 ) );
	insertEntity(point);

	shared_ptr<IfcAxis2Placement3D> axis_placement( new IfcAxis2Placement3D() );
	axis_placement->m_Location = point;
	insertEntity(axis_placement);

	shared_ptr<IfcOwnerHistory> owner_history ( new IfcOwnerHistory() );
	owner_history->m_OwningUser = person_org;
	owner_history->m_OwningApplication = app;
	owner_history->m_ChangeAction = std::make_shared<IfcChangeActionEnum>( IfcChangeActionEnum::ENUM_ADDED );
	insertEntity(owner_history);

	shared_ptr<IfcDimensionalExponents> dim_exp( new IfcDimensionalExponents() );
	dim_exp->m_LengthExponent = 0;
	dim_exp->m_MassExponent = 0;
	dim_exp->m_TimeExponent = 0;
	dim_exp->m_ElectricCurrentExponent = 0;
	dim_exp->m_ThermodynamicTemperatureExponent = 0;
	dim_exp->m_AmountOfSubstanceExponent = 0;
	dim_exp->m_LuminousIntensityExponent = 0;
	insertEntity(dim_exp);

	// length unit [m]
	shared_ptr<IfcSIUnit> si_unit( new IfcSIUnit() );
	si_unit->m_UnitType = std::make_shared<IfcUnitEnum>( IfcUnitEnum::ENUM_LENGTHUNIT );
	si_unit->m_Name = std::make_shared<IfcSIUnitName>( IfcSIUnitName::ENUM_METRE );
	insertEntity(si_unit);

	// plane unit [rad]
	shared_ptr<IfcSIUnit> plane_angle_unit( new IfcSIUnit() );
	plane_angle_unit->m_UnitType = std::make_shared<IfcUnitEnum>( IfcUnitEnum::ENUM_PLANEANGLEUNIT );
	plane_angle_unit->m_Name = std::make_shared<IfcSIUnitName>( IfcSIUnitName::ENUM_RADIAN );
	insertEntity(plane_angle_unit);

	// assign units
	shared_ptr<IfcUnitAssignment> unit_assignment( new IfcUnitAssignment() );
	unit_assignment->m_Units.push_back( si_unit );
	unit_assignment->m_Units.push_back( plane_angle_unit );
	insertEntity(unit_assignment);

	project->m_GlobalId = std::make_shared<IfcGloballyUniqueId>( createBase64Uuid() );
	project->m_OwnerHistory = owner_history;
	project->m_Name = std::make_shared<IfcLabel>( "IfcPlusPlus project" );
	project->m_UnitsInContext = unit_assignment;

	// create default IfcSite
	shared_ptr<IfcSite> site( new IfcSite() );
	site->m_GlobalId = std::make_shared<IfcGloballyUniqueId>( createBase64Uuid() );
	site->m_OwnerHistory = owner_history;
	site->m_Name = std::make_shared<IfcLabel>( "Site" );
	insertEntity(site);

	shared_ptr<IfcRelAggregates> rel_aggregates_site( new IfcRelAggregates() );
	rel_aggregates_site->m_RelatingObject = project;
	insertEntity(rel_aggregates_site);

	// create default Building
	shared_ptr<IfcBuilding> building( new IfcBuilding() );
	building->m_GlobalId = std::make_shared<IfcGloballyUniqueId>( createBase64Uuid() );
	building->m_OwnerHistory = owner_history;
	building->m_Name = std::make_shared<IfcLabel>( "Building" );
	insertEntity( building );
	
	// set up world coordinate system
	shared_ptr<IfcDirection> axis( new IfcDirection() );
	insertEntity(axis);
	axis->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 1.0 ) );
	axis->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );
	axis->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );

	shared_ptr<IfcDirection> ref_direction( new IfcDirection() );
	insertEntity(ref_direction);
	ref_direction->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );
	ref_direction->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 1.0 ) );
	ref_direction->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );

	shared_ptr<IfcCartesianPoint> location( new IfcCartesianPoint() );
	insertEntity(location);
	location->m_Coordinates[0] = 0.0;
	location->m_Coordinates[1] = 0.0;
	location->m_Coordinates[2] = 0.0;
	//location->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>(0.0 ) );
	//location->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>(0.0 ) );

	shared_ptr<IfcAxis2Placement3D> world_coordinate_system( new IfcAxis2Placement3D() );
	insertEntity(world_coordinate_system);
	world_coordinate_system->m_Location = location;
	world_coordinate_system->m_Axis = axis;
	world_coordinate_system->m_RefDirection = ref_direction;

	// 3d representation context
	shared_ptr<IfcGeometricRepresentationContext> geom_context( new IfcGeometricRepresentationContext() );
	insertEntity(geom_context);
	geom_context->m_CoordinateSpaceDimension = std::make_shared<IfcDimensionCount>( 3 );
	geom_context->m_WorldCoordinateSystem = world_coordinate_system;

	updateCache();
}

void BuildingModel::initCopyIfcModel( const shared_ptr<BuildingModel>& other )
{
	clearIfcModel();

	shared_ptr<IfcProject> project = other->getIfcProject();
	shared_ptr<IfcRoot> projectAsRoot(project);

	std::unordered_map<BuildingObject*, shared_ptr<BuildingObject> > map_entities_copy;
	map_entities_copy[projectAsRoot.get()] = projectAsRoot;
	
	collectDependentEntities( projectAsRoot, map_entities_copy, false);

	for( auto it = map_entities_copy.begin(); it != map_entities_copy.end(); ++it )
	{
		shared_ptr<BuildingObject> bo = it->second;
		shared_ptr<BuildingEntity> entity = dynamic_pointer_cast<BuildingEntity>(bo);
		if( entity )
		{
			insertEntity(entity);
		}
	}

	updateCache();
}

shared_ptr<IfcProject> BuildingModel::getIfcProject()
{
	return m_ifc_project;
}
shared_ptr<IfcGeometricRepresentationContext> BuildingModel::getIfcGeometricRepresentationContext3D()
{
	return m_geom_context_3d;
}

void BuildingModel::setIfcProject( shared_ptr<IfcProject> project )
{
	m_ifc_project = project;
}

void BuildingModel::setIfcGeometricRepresentationContext3D( shared_ptr<IfcGeometricRepresentationContext>& context )
{
	m_geom_context_3d = context;
}

void BuildingModel::setUnitConverter( shared_ptr<UnitConverter>& uc )
{
	m_unit_converter = uc;
}

void BuildingModel::setMapIfcEntities( const std::unordered_map<int, shared_ptr<BuildingEntity> >& map )
{
	clearIfcModel();
	m_map_entities.clear();
	m_max_entity_id = -1;
	m_map_entities = map;
	updateCache();
	// todo: check model consistency
}

void BuildingModel::insertEntity( shared_ptr<BuildingEntity> e, bool overwrite_existing, bool warn_on_existing_entities )
{
	if( !e )
	{
		return;
	}
	int tag = e->m_tag;
	if( tag <= 0 )
	{
		int next_unused_id = getNextUnusedEntityTagFast();
		e->m_tag = next_unused_id;
		tag = next_unused_id;
	}

	if (tag > m_max_entity_id)
	{
		m_max_entity_id = tag;
	}

	auto it_find = m_map_entities.find( tag );
	if( it_find != m_map_entities.end() )
	{
		// key already exists
		if( overwrite_existing )
		{
			it_find->second = e;
		}
		else
		{
			if( warn_on_existing_entities )
			{
				messageCallback( "Entity already in model", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, e.get() );
			}
		}
	}
	else
	{
		// the key does not exist in the map
		m_map_entities.insert( it_find, std::map<int, shared_ptr<BuildingEntity> >::value_type( tag, e ) );
	}
}

void BuildingModel::removeEntity( shared_ptr<BuildingEntity> e )
{
	if( !e )
	{
		messageCallback( "Entity not valid", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, e.get() );
		return;
	}
	int remove_id = e->m_tag;
	auto it_find = m_map_entities.find(remove_id);
	if( it_find == m_map_entities.end() )
	{
		messageCallback( "Entity not found in model", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, e.get() );
		return;
	}
	shared_ptr<BuildingEntity> entity_found = it_find->second;
	entity_found->unlinkFromInverseCounterparts();
	BuildingEntity* entity_remove_ptr = e.get();
		
	if( entity_found.use_count() > 1 )
	{
		// find references to this entity and remove them
		std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes_inverse;
		entity_found->getAttributesInverse( vec_attributes_inverse );
		for(auto & ii : vec_attributes_inverse)
		{
			shared_ptr<BuildingObject>& attribute_inverse = ii.second;

			shared_ptr<BuildingEntity> attribute_inverse_entity = dynamic_pointer_cast<BuildingEntity>( attribute_inverse );
			if( attribute_inverse_entity )
			{
				std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes;
				attribute_inverse_entity->getAttributes( vec_attributes );
				for( size_t jj = 0; jj < vec_attributes.size(); ++jj )
				{
					shared_ptr<BuildingObject>& attribute = vec_attributes[jj].second;
					if( attribute.get() == entity_remove_ptr )
					{
						vec_attributes.erase( vec_attributes.begin() + jj );
						if( jj > 0 )
						{
							--jj;
						}
					}
				}
			}
		}

		if (entity_found->m_tag == m_max_entity_id) {
			if (m_max_entity_id > 0) {
				--m_max_entity_id;
			}
		}
	}
	m_map_entities.erase( it_find );
}

void BuildingModel::removeEntity( int tag )
{
	auto it_find = m_map_entities.find(tag);
	if( it_find != m_map_entities.end() )
	{
		shared_ptr<BuildingEntity> entity_found = it_find->second;
		removeEntity( entity_found );
	}
}

int BuildingModel::getNextUnusedEntityTagFast()
{
	if( m_map_entities.empty() )
	{
		m_max_entity_id = 1;
		return 1;
	}

	int next_unused_id = m_max_entity_id + 1;
#if defined(_DEBUG) || defined(_DEBUG_RELEASE)
	if (m_map_entities.find(next_unused_id) != m_map_entities.end())
	{
		// find next unused id
		messageCallback("m_max_entity_id not set correctly", StatusCallback::MESSAGE_TYPE_ERROR, __FUNCTION__, nullptr);
		++next_unused_id;
		m_max_entity_id = -1;
		for (auto it = m_map_entities.begin(); it != m_map_entities.end(); ++it)
		{
			int tag = it->first;
			if (tag > m_max_entity_id)
			{
				m_max_entity_id = tag;
			}
		}

		next_unused_id = m_max_entity_id + 1;
		if (m_map_entities.find(next_unused_id) != m_map_entities.end())
		{
			messageCallback("next_unused_id not unique", StatusCallback::MESSAGE_TYPE_ERROR, __FUNCTION__, nullptr);
		}
	}
#endif

	return next_unused_id;
}

int BuildingModel::getLowestUnusedEntityTagSlow()
{
	if (m_map_entities.empty())
	{
		return 1;
	}

	m_max_entity_id = 1;
	for (auto it = m_map_entities.begin(); it != m_map_entities.end(); ++it)
	{
		int tag = it->first;
		if (tag > m_max_entity_id)
		{
			m_max_entity_id = tag;
		}
	}

	++m_max_entity_id;
	return m_max_entity_id;
}

void BuildingModel::removeUnreferencedEntities()
{
	for( auto it_entities = m_map_entities.begin(); it_entities != m_map_entities.end();  )
	{
		shared_ptr<BuildingEntity> entity = it_entities->second;
		if( entity.use_count() < 2 )
		{
			// if use_count is only 1, the entity is only referenced in m_map_entities, so no other entity or other object holds a shared_ptr
			bool erase_entity = true;
			shared_ptr<IfcRelationship> ifc_relationship = dynamic_pointer_cast<IfcRelationship>( entity );
			if( ifc_relationship )
			{
				// relationship objects may only reference some entities while not being referenced itself
				std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes;
				ifc_relationship->getAttributes( vec_attributes );
				if( vec_attributes.size() > 4 )
				{
					// check if the object has relevant pointers
					size_t num_relevant_attributes = 0;
					// first 4 attributes are GlobalId, OwnerHistory, Name, Description
					for( size_t ii = 4; ii < vec_attributes.size(); ++ii )
					{
						shared_ptr<BuildingObject> attribute = vec_attributes[ii].second;
						if( attribute )
						{
							shared_ptr<AttributeObjectVector> attribute_vec = dynamic_pointer_cast<AttributeObjectVector>( attribute );
							if( attribute_vec )
							{
								if( !attribute_vec->m_vec.empty() )
								{
									++num_relevant_attributes;
								}
							}
							else
							{
								shared_ptr<BuildingEntity> attribute_entity = dynamic_pointer_cast<BuildingEntity>( attribute );
								if( attribute_entity )
								{
									++num_relevant_attributes;
								}
							}
						}
					}
					if( num_relevant_attributes > 0 )
					{
						erase_entity = false;
					}
				}
			}
			else
			{
				shared_ptr<IfcStyledItem> ifc_styled_item = dynamic_pointer_cast<IfcStyledItem>( entity );
				if( ifc_styled_item )
				{
					if( ifc_styled_item->m_Item )
					{
						erase_entity = false;
					}
				}
				
			}
			if( erase_entity )
			{
				entity->unlinkFromInverseCounterparts();
				auto erase_it = it_entities;
#ifdef _DEBUG
				int tag = entity->m_tag;
				const char* entity_className = IFC4X3::EntityFactory::getStringForClassID(entity->classID());
#endif
				++it_entities;
				// TODO: check here if it really has been removed
				removeEntity( entity );
			}
			else
			{
				++it_entities;
			}
		}
		else
		{
			++it_entities;
		}
	}
}

void BuildingModel::initFileHeader( const std::string& fileName, const std::string& generatingApplication )
{
	m_file_name = fileName;
	std::string filename_escaped = encodeStepString( fileName );
	std::stringstream strs;
	strs << "HEADER;" << std::endl;
	strs << "FILE_DESCRIPTION(('ViewDefinition [CoordinationView]'),'2;1');" << std::endl;
	strs << "FILE_NAME('" << filename_escaped.c_str() << "','";

	//2011-04-21T14:25:12
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", timeinfo);
	std::string str(buffer);

	strs << buffer;
	strs << "',(''),('',''),'',' " << generatingApplication << "','');" << std::endl;
	strs << "FILE_SCHEMA(('" << getIfcSchemaVersionCurrent() << "'));" << std::endl;
	strs << "ENDSEC;" << std::endl;

	m_file_header = strs.str();
}

void BuildingModel::setFileHeader( const std::string& header )
{
	m_file_header = header;
}

void BuildingModel::setFileDescription( const std::string& description )
{
	m_IFC_FILE_DESCRIPTION = description;
}

void BuildingModel::setFileName( const std::string& name )
{
	m_IFC_FILE_NAME = name;
}

void BuildingModel::clearIfcModel()
{
	m_map_entities.clear();
	m_max_entity_id = -1;
	m_ifc_project.reset();
	m_geom_context_3d.reset();
	m_ifc_schema_version_current = IFC4X1;
	m_ifc_schema_version_loaded_file = IFC4X1;
	m_IFC_FILE_NAME = "";
	m_IFC_FILE_DESCRIPTION = "";
	m_file_header = "";
	m_unit_converter->resetUnitFactors();
}

void BuildingModel::resetIfcModel()
{
	clearIfcModel();
	initIfcModel();
	updateCache();
}

void BuildingModel::updateCache()
{
	bool found_project = false;
	bool found_geom_context = false;

	shared_ptr<IfcProject> keep_project = m_ifc_project;
	m_ifc_project.reset();

	// try to find IfcProject and IfcGeometricRepresentationContext
	for( auto it=m_map_entities.begin(); it!=m_map_entities.end(); ++it )
	{
		int tag = it->first;
		shared_ptr<BuildingEntity> obj = it->second;
		if( obj->classID() == IFCPROJECT )
		{
			if( m_ifc_project )
			{
				messageCallback("More than one IfcProject in model", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, m_ifc_project.get());
			}
			m_ifc_project = dynamic_pointer_cast<IfcProject>(obj);
			
			if( m_ifc_project )
			{
				found_project = true;
				if( found_geom_context )
				{
					break;
				}
			}
			else
			{
				std::cout << "BuildingModel::updateCache: IfcProject found but dynamic_cast failed. Is RTTI enabled?" << std::endl;
			}
		}
		else if( obj->classID() == IFCGEOMETRICREPRESENTATIONCONTEXT )
		{
			shared_ptr<IfcGeometricRepresentationContext> context = dynamic_pointer_cast<IfcGeometricRepresentationContext>(obj);
			if( context )
			{
				if( context->m_CoordinateSpaceDimension )
				{
					if( context->m_CoordinateSpaceDimension->m_value == 3 )
					{
						m_geom_context_3d = context;
						found_geom_context = true;
						if( found_project )
						{
							break;
						}
					}
				}
			}
		}
	}

	if( found_project )
	{
		m_unit_converter->setIfcProject( m_ifc_project );
	}
	else
	{
		std::cout << "BuildingModel::updateCache, IfcProject not found in model with " << m_map_entities.size() << " items" << std::endl;
	}
}

void BuildingModel::clearCache()
{
	m_ifc_project.reset();
	m_geom_context_3d.reset();
}

void BuildingModel::resolveInverseAttributes()
{
	for( auto it = m_map_entities.begin(); it != m_map_entities.end(); ++it )
	{
		shared_ptr<BuildingEntity> entity = it->second;
		if( !entity )
		{
			continue;
		}

		entity->setInverseCounterparts( entity );
	}
}

void BuildingModel::unsetInverseAttributes()
{
	for( auto it = m_map_entities.begin(); it != m_map_entities.end(); ++it )
	{
		shared_ptr<BuildingEntity> entity = it->second;
		if( !entity )
		{
			continue;
		}
		entity->unlinkFromInverseCounterparts();
	}
}

void BuildingModel::collectDependentEntities( shared_ptr<BuildingObject> obj, std::unordered_map<BuildingObject*, shared_ptr<BuildingObject> >& target_map, bool resolveInverseAttributes)
{
	if( !obj )
	{
		return;
	}
	if( target_map.find( obj.get() ) != target_map.end() )
	{
		//return;
	}

	target_map[obj.get()] = obj;

	shared_ptr<BuildingEntity> entity = dynamic_pointer_cast<BuildingEntity>( obj );

	if( entity )
	{
		shared_ptr<IfcElementAssembly> ele_assembly = dynamic_pointer_cast<IfcElementAssembly>(entity);
		if( ele_assembly )
		{
			int assembly_id = ele_assembly->m_tag;
			std::vector<weak_ptr<IfcRelAggregates> >& vec_is_decomposed_by = ele_assembly->m_IsDecomposedBy_inverse;
			for( const auto& is_decomposed_weak_ptr : vec_is_decomposed_by )
			{
				shared_ptr<IfcRelDecomposes> rel_dec;
				if( is_decomposed_weak_ptr.expired() )
				{
					continue;
				}
				shared_ptr<IfcRelAggregates> is_decomposed_ptr(is_decomposed_weak_ptr);
				if( is_decomposed_ptr )
				{
					shared_ptr<BuildingEntity> as_ifcpp_entity = is_decomposed_ptr;
					collectDependentEntities(as_ifcpp_entity, target_map, resolveInverseAttributes);
				}
			}
		}

		std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes;
		entity->getAttributes(vec_attributes);

		if( resolveInverseAttributes )
		{
			entity->getAttributesInverse(vec_attributes);
		}

		for( auto& vec_attribute : vec_attributes )
		{
			shared_ptr<BuildingObject>& attribute = vec_attribute.second;
			if( !attribute )
			{
				// empty attribute
				continue;
			}
			shared_ptr<BuildingEntity> attribute_entity = dynamic_pointer_cast<BuildingEntity>(attribute);
			if( attribute_entity )
			{
				if( target_map.find(attribute_entity.get()) == target_map.end() )
				{
					target_map[attribute_entity.get()] = attribute_entity;
					collectDependentEntities(attribute_entity, target_map, resolveInverseAttributes);
				}
				continue;
			}

			shared_ptr<AttributeObjectVector> attribute_object_vector = dynamic_pointer_cast<AttributeObjectVector>(attribute);
			if( attribute_object_vector )
			{
				std::vector<shared_ptr<BuildingObject> >& vec_of_attributes = attribute_object_vector->m_vec;
				for( auto& attribute_object : vec_of_attributes )
				{
					collectDependentEntities(attribute_object, target_map, resolveInverseAttributes);
				}
				continue;
			}
		}
		return;
	}

	shared_ptr<AttributeObjectVector> attribute_object_vector = dynamic_pointer_cast<AttributeObjectVector>(obj);
	if( attribute_object_vector )
	{
		std::vector<shared_ptr<BuildingObject> >& vec_of_attributes = attribute_object_vector->m_vec;
		for( auto& attribute_object : vec_of_attributes )
		{
			collectDependentEntities(attribute_object, target_map, resolveInverseAttributes);
		}
	}
}
