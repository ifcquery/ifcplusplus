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

#pragma warning( disable: 4996 )
#include <iostream>
#define BOOST_DATE_TIME_NO_LIB
#include <boost/algorithm/string/replace.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcIdentifier.h"
#include "ifcpp/IFC4/include/IfcUnitEnum.h"
#include "ifcpp/IFC4/include/IfcSIUnitName.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcChangeActionEnum.h"
#include "ifcpp/IFC4/include/IfcValue.h"
#include "ifcpp/IFC4/include/IfcPlaneAngleMeasure.h"
#include "ifcpp/IFC4/include/IfcGloballyUniqueId.h"
#include "ifcpp/IFC4/include/IfcDimensionCount.h"
#include "ifcpp/IFC4/include/IfcPerson.h"
#include "ifcpp/IFC4/include/IfcOrganization.h"
#include "ifcpp/IFC4/include/IfcPersonAndOrganization.h"
#include "ifcpp/IFC4/include/IfcApplication.h"
#include "ifcpp/IFC4/include/IfcSIUnit.h"
#include "ifcpp/IFC4/include/IfcCartesianPoint.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement3D.h"
#include "ifcpp/IFC4/include/IfcOwnerHistory.h"
#include "ifcpp/IFC4/include/IfcDimensionalExponents.h"
#include "ifcpp/IFC4/include/IfcMeasureWithUnit.h"
#include "ifcpp/IFC4/include/IfcConversionBasedUnit.h"
#include "ifcpp/IFC4/include/IfcUnitAssignment.h"
#include "ifcpp/IFC4/include/IfcProject.h"
#include "ifcpp/IFC4/include/IfcGeometricRepresentationContext.h"
#include "ifcpp/IFC4/include/IfcProduct.h"
#include "ifcpp/IFC4/include/IfcDirection.h"
#include "ifcpp/IFC4/include/IfcRelationship.h"
#include "ifcpp/IFC4/include/IfcRelAggregates.h"
#include "ifcpp/IFC4/include/IfcSite.h"
#include "ifcpp/IFC4/include/IfcBuilding.h"
#include "ifcpp/IFC4/include/IfcElementAssembly.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"
#include "ifcpp/IFC4/include/IfcUnit.h"

#include "IfcPPGuid.h"
#include "IfcPPAttributeObject.h"
#include "UnitConverter.h"
#include "IfcPPException.h"
#include "IfcPPModel.h"

IfcPPModel::IfcPPModel()
{
	m_unit_converter = shared_ptr<UnitConverter>( new UnitConverter() );
	addCallbackChild( m_unit_converter.get() );
	initFileHeader( "IfcPlusPlus-export.ifc" );
}

IfcPPModel::~IfcPPModel()
{
}

void IfcPPModel::initIfcModel()
{
	clearIfcModel();

	shared_ptr<IfcProject> project( new IfcProject( 1 ) );
	insertEntity(project);

	shared_ptr<IfcPerson> person( new IfcPerson() );
	person->m_FamilyName = shared_ptr<IfcLabel>( new IfcLabel( L"FamilyName" ) );
	person->m_GivenName = shared_ptr<IfcLabel>( new IfcLabel( L"GivenName" ) );
	insertEntity(person);
	
	shared_ptr<IfcOrganization> org( new IfcOrganization() );
	org->m_Name = shared_ptr<IfcLabel>(new IfcLabel( L"OrganizationName" ));
	insertEntity(org);

	shared_ptr<IfcPersonAndOrganization> person_org( new IfcPersonAndOrganization() );
	person_org->m_ThePerson = person;
	person_org->m_TheOrganization = org;
	insertEntity(person_org);

	shared_ptr<IfcApplication> app( new IfcApplication() );
	app->m_ApplicationDeveloper = org;
	app->m_Version = shared_ptr<IfcLabel>( new IfcLabel( L"1.0" ) );
	app->m_ApplicationFullName = shared_ptr<IfcLabel>( new IfcLabel( L"IfcPlusPlus" ) );
	app->m_ApplicationIdentifier = shared_ptr<IfcIdentifier>( new IfcIdentifier( L"IfcPlusPlus" ) );
	insertEntity(app);


	shared_ptr<IfcCartesianPoint> point( new IfcCartesianPoint() );
	point->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0.0 ) ) );
	point->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0.0 ) ) );
	point->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>( new IfcLengthMeasure( 0.0 ) ) );
	insertEntity(point);

	shared_ptr<IfcAxis2Placement3D> axis_placement( new IfcAxis2Placement3D() );
	axis_placement->m_Location = point;
	insertEntity(axis_placement);

	shared_ptr<IfcOwnerHistory> owner_history ( new IfcOwnerHistory() );
	owner_history->m_OwningUser = person_org;
	owner_history->m_OwningApplication = app;
	owner_history->m_ChangeAction = shared_ptr<IfcChangeActionEnum>( new IfcChangeActionEnum( IfcChangeActionEnum::ENUM_ADDED ) );
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
	si_unit->m_UnitType = shared_ptr<IfcUnitEnum>( new IfcUnitEnum( IfcUnitEnum::ENUM_LENGTHUNIT ) );
	si_unit->m_Name = shared_ptr<IfcSIUnitName>( new IfcSIUnitName( IfcSIUnitName::ENUM_METRE ) );
	insertEntity(si_unit);

	// plane unit [rad]
	shared_ptr<IfcSIUnit> plane_angle_unit( new IfcSIUnit() );
	plane_angle_unit->m_UnitType = shared_ptr<IfcUnitEnum>( new IfcUnitEnum( IfcUnitEnum::ENUM_PLANEANGLEUNIT ) );
	plane_angle_unit->m_Name = shared_ptr<IfcSIUnitName>( new IfcSIUnitName( IfcSIUnitName::ENUM_RADIAN ) );
	insertEntity(plane_angle_unit);

	// assign units
	shared_ptr<IfcUnitAssignment> unit_assignment( new IfcUnitAssignment() );
	//shared_ptr<IfcUnit> si_unit_select( new IfcUnit() );
	//si_unit_select->setObject( si_unit );
	unit_assignment->m_Units.push_back( si_unit );
	//shared_ptr<IfcUnit> plane_angle_unit_select( new IfcUnit() );
	//plane_angle_unit_select->setObject( plane_angle_unit );
	unit_assignment->m_Units.push_back( plane_angle_unit );
	insertEntity(unit_assignment);

	
	project->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( CreateCompressedGuidString22() ) );
	project->m_OwnerHistory = owner_history;
	project->m_Name = shared_ptr<IfcLabel>(new IfcLabel( L"IfcPlusPlus project" ) );
	project->m_UnitsInContext = unit_assignment;

	// create default IfcSite
	shared_ptr<IfcSite> site( new IfcSite() );
	site->m_GlobalId = shared_ptr<IfcGloballyUniqueId>( new IfcGloballyUniqueId( CreateCompressedGuidString22() ) );
	site->m_OwnerHistory = owner_history;
	site->m_Name = shared_ptr<IfcLabel>( new IfcLabel( L"Site" ) );
	insertEntity(site);

	shared_ptr<IfcRelAggregates> rel_aggregates_site( new IfcRelAggregates() );
	rel_aggregates_site->m_RelatingObject = project;
	insertEntity(rel_aggregates_site);

	// create default Building
	shared_ptr<IfcBuilding> building( new IfcBuilding() );
	building->m_GlobalId = shared_ptr<IfcGloballyUniqueId>( new IfcGloballyUniqueId( CreateCompressedGuidString22() ) );
	building->m_OwnerHistory = owner_history;
	building->m_Name = shared_ptr<IfcLabel>( new IfcLabel( L"Building" ) );
	insertEntity( building );

	
	// set up world coordinate system
	shared_ptr<IfcDirection> axis( new IfcDirection() );
	insertEntity(axis);
	axis->m_DirectionRatios.push_back( 0.0 );
	axis->m_DirectionRatios.push_back( 0.0 );
	axis->m_DirectionRatios.push_back( 1.0 );

	shared_ptr<IfcDirection> ref_direction( new IfcDirection() );
	insertEntity(ref_direction);
	ref_direction->m_DirectionRatios.push_back( 0.0 );
	ref_direction->m_DirectionRatios.push_back( 0.0 );
	ref_direction->m_DirectionRatios.push_back( 1.0 );

	shared_ptr<IfcCartesianPoint> location( new IfcCartesianPoint() );
	insertEntity(location);
	location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(0.0) ) );
	location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(0.0) ) );
	location->m_Coordinates.push_back( shared_ptr<IfcLengthMeasure>(new IfcLengthMeasure(0.0) ) );

	shared_ptr<IfcAxis2Placement3D> world_coordinate_system( new IfcAxis2Placement3D() );
	insertEntity(world_coordinate_system);
	world_coordinate_system->m_Location = location;
	world_coordinate_system->m_Axis = axis;
	world_coordinate_system->m_RefDirection = ref_direction;

	// 3d representation context
	shared_ptr<IfcGeometricRepresentationContext> geom_context( new IfcGeometricRepresentationContext() );
	insertEntity(geom_context);
	geom_context->m_CoordinateSpaceDimension = shared_ptr<IfcDimensionCount>(new IfcDimensionCount( 3 ) );
	geom_context->m_WorldCoordinateSystem = world_coordinate_system;

	updateCache();
}

void IfcPPModel::initCopyIfcModel( const shared_ptr<IfcPPModel>& other )
{
	clearIfcModel();

	shared_ptr<IfcProject> project = other->getIfcProject();
	std::map<IfcPPEntity*, shared_ptr<IfcPPEntity> > map_entities_copy;
	map_entities_copy[project.get()] = project;
	
	shared_ptr<IfcPPEntity> project_as_entity( project );
	collectDependentEntities( project_as_entity, map_entities_copy );

	for( auto it = map_entities_copy.begin(); it != map_entities_copy.end(); ++it )
	{
		shared_ptr<IfcPPEntity> entity = it->second;
		insertEntity( entity );
	}

	updateCache();
}

shared_ptr<IfcProject> IfcPPModel::getIfcProject()
{
	return m_ifc_project;
}
shared_ptr<IfcGeometricRepresentationContext> IfcPPModel::getIfcGeometricRepresentationContext3D()
{
	return m_geom_context_3d;
}

void IfcPPModel::setIfcProject( shared_ptr<IfcProject> project )
{
	m_ifc_project = project;
}

void IfcPPModel::setIfcGeometricRepresentationContext3D( shared_ptr<IfcGeometricRepresentationContext>& context )
{
	m_geom_context_3d = context;
}

void IfcPPModel::setUnitConverter( shared_ptr<UnitConverter>& uc )
{
	m_unit_converter = uc;
}

void IfcPPModel::setMapIfcEntities( const boost::unordered_map<int, shared_ptr<IfcPPEntity> >& map )
{
	clearIfcModel();
	m_map_entities.clear();
	m_map_entities = map;
	updateCache();
	// todo: check model consistency
}

void IfcPPModel::insertEntity( shared_ptr<IfcPPEntity> e, bool overwrite_existing, bool warn_on_existing_entities )
{
	if( !e )
	{
		return;
	}
	int entity_id = e->m_id;
	if( entity_id < 0 )
	{
		int next_unused_id = getMaxUsedEntityId() + 1;
		e->m_id = next_unused_id;
		entity_id = next_unused_id;
	}

	boost::unordered_map<int,shared_ptr<IfcPPEntity> >::iterator it_find = m_map_entities.find( entity_id );
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
		m_map_entities.insert( it_find, boost::unordered_map<int,shared_ptr<IfcPPEntity> >::value_type( entity_id, e ) );
	}
#ifdef _DEBUG
	shared_ptr<IfcProduct> product = dynamic_pointer_cast<IfcProduct>( e );
	if( product )
	{
		if( !product->m_GlobalId )
		{
			messageCallback( "IfcProduct->m_GlobalId not set", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, product.get() );
			return;
		}
		if( product->m_GlobalId->m_value.length() < 22 )
		{
			messageCallback( "IfcProduct->m_GlobalId.length() < 22", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, product.get() );
		}
	}
#endif

	// TODO: if type is IfcRoot (or subtype), and GlobalID not set, create one
}

void IfcPPModel::removeEntity( shared_ptr<IfcPPEntity> e )
{
	if( !e )
	{
		messageCallback( "Entity not valid", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, e.get() );
		return;
	}
	int remove_id = e->m_id;
	auto it_find = m_map_entities.find(remove_id);
	if( it_find == m_map_entities.end() )
	{
		messageCallback( "Entity not found in model", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, e.get() );
		return;
	}
	shared_ptr<IfcPPEntity> entity_found = it_find->second;
	entity_found->unlinkFromInverseCounterparts();
	IfcPPEntity* entity_remove_ptr = e.get();
		
	if( entity_found.use_count() > 1 )
	{
		// find references to this entity and remove them
		std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > > vec_attributes_inverse;
		entity_found->getAttributesInverse( vec_attributes_inverse );
		for( size_t ii = 0; ii < vec_attributes_inverse.size(); ++ii )
		{
			shared_ptr<IfcPPObject>& attribute_inverse = vec_attributes_inverse[ii].second;

			shared_ptr<IfcPPEntity> attribute_inverse_entity = dynamic_pointer_cast<IfcPPEntity>( attribute_inverse );
			if( attribute_inverse_entity )
			{
				std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > > vec_attributes;
				attribute_inverse_entity->getAttributes( vec_attributes );
				for( size_t jj = 0; jj < vec_attributes.size(); ++jj )
				{
					shared_ptr<IfcPPObject>& attribute = vec_attributes[jj].second;
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
	}
	m_map_entities.erase( it_find );
}

void IfcPPModel::removeEntity( int entity_id )
{
	auto it_find = m_map_entities.find(entity_id);
	if( it_find != m_map_entities.end() )
	{
		shared_ptr<IfcPPEntity> entity_found = it_find->second;
		removeEntity( entity_found );
	}
}

int IfcPPModel::getMaxUsedEntityId()
{
	if( m_map_entities.size() == 0 )
	{
		return 0;
	}
	int max_used_id = 1;
	for( auto it = m_map_entities.begin(); it != m_map_entities.end(); ++it )
	{
		if( it->first > max_used_id )
		{
			max_used_id = it->first + 1;
		}
	}
	//m_map_entities.rbegin()->first;
	return max_used_id;
}

void IfcPPModel::removeUnreferencedEntities()
{
	for( auto it_entities = m_map_entities.begin(); it_entities != m_map_entities.end();  )
	{
		shared_ptr<IfcPPEntity> entity = it_entities->second;
		if( entity.use_count() < 2 )
		{
			// if use_count is only 1, the entity is only referenced in m_map_entities, so no other entity or other object holds a shared_ptr
			bool erase_entity = true;
			shared_ptr<IfcRelationship> ifc_relationship = dynamic_pointer_cast<IfcRelationship>( entity );
			if( ifc_relationship )
			{
				// relationship objects may only reference some entities while not beeing referenced itself
				std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > > vec_attributes;
				ifc_relationship->getAttributes( vec_attributes );
				if( vec_attributes.size() > 4 )
				{
					// check if the object has relevant pointers
					size_t num_relevant_attributes = 0;
					// first 4 attributes are GlobalId, OwnerHistory, Name, Description
					for( size_t ii = 4; ii < vec_attributes.size(); ++ii )
					{
						shared_ptr<IfcPPObject> attribute = vec_attributes[ii].second;
						if( attribute )
						{
							shared_ptr<IfcPPAttributeObjectVector> attribute_vec = dynamic_pointer_cast<IfcPPAttributeObjectVector>( attribute );
							if( attribute_vec )
							{
								if( attribute_vec->m_vec.size() > 0 )
								{
									++num_relevant_attributes;
								}
							}
							else
							{
								shared_ptr<IfcPPEntity> attribute_entity = dynamic_pointer_cast<IfcPPEntity>( attribute );
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
				int entity_id = entity->m_id;
				const char* entity_className = entity->className();
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

void IfcPPModel::initFileHeader( std::string file_name )
{
	std::string filename_escaped = boost::replace_all_copy(file_name, "\\", "\\\\");
	std::wstringstream strs;
	strs << "ISO-10303-21;" << std::endl;
	strs << "HEADER;" << std::endl;
	strs << "FILE_DESCRIPTION(('IFC4'),'2;1');" << std::endl;
	strs << "FILE_NAME('" << filename_escaped.c_str() << "','";

	//2011-04-21T14:25:12
	std::locale loc( std::wcout.getloc(), new boost::posix_time::wtime_facet( L"%Y-%m-%dT%H:%M:%S" ) );
	std::basic_stringstream<wchar_t> wss;
	wss.imbue( loc );
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	wss << now;
	std::wstring ts = wss.str().c_str();

	strs << ts;
	strs << "',(''),('',''),'','IfcPlusPlus','');" << std::endl;
	strs << "FILE_SCHEMA(('IFC4'));" << std::endl;
	strs << "ENDSEC;" << std::endl;

	m_file_header = strs.str().c_str();
}

void IfcPPModel::setFileHeader( std::wstring header )
{
	m_file_header = header;
}

void IfcPPModel::setFileDescription( std::wstring schema )
{
	m_IFC_FILE_DESCRIPTION = schema;
}

void IfcPPModel::setFileName( std::wstring schema )
{
	m_IFC_FILE_NAME = schema;
}

void IfcPPModel::setIfcSchemaVersion( IfcPPSchemaVersion& ver )
{
	m_ifc_schema_version.m_IFC_FILE_SCHEMA = ver.m_IFC_FILE_SCHEMA.c_str();
	m_ifc_schema_version.m_ifc_file_schema_enum = ver.m_ifc_file_schema_enum;
}

void IfcPPModel::clearIfcModel()
{
	m_ifc_project.reset();
	m_geom_context_3d.reset();
	m_map_entities.clear();
	m_ifc_schema_version.m_ifc_file_schema_enum = IFC_VERSION_UNDEFINED;
	m_ifc_schema_version.m_IFC_FILE_SCHEMA = L"";
	m_IFC_FILE_NAME = L"";
	m_IFC_FILE_DESCRIPTION = L"";
	m_file_header = L"";
	m_unit_converter->resetComplete();
}

void IfcPPModel::resetIfcModel()
{
	clearIfcModel();
	initIfcModel();
	updateCache();
}

void IfcPPModel::updateCache()
{
	bool found_project = false;
	bool found_geom_context = false;

	shared_ptr<IfcProject> keep_project = m_ifc_project;
	m_ifc_project.reset();

	// try to find IfcProject and IfcGeometricRepresentationContext
	for( auto it=m_map_entities.begin(); it!=m_map_entities.end(); ++it )
	{
		shared_ptr<IfcPPEntity> obj = it->second;
		if( dynamic_pointer_cast<IfcProject>(obj) )
		{
			if( m_ifc_project )
			{
				messageCallback( "More than one IfcProject in model", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__, m_ifc_project.get() );
			}
			m_ifc_project = dynamic_pointer_cast<IfcProject>( obj );
			found_project = true;
			if( found_geom_context )
			{
				break;
			}
		}
		else if( dynamic_pointer_cast<IfcGeometricRepresentationContext>(obj) )
		{
			shared_ptr<IfcGeometricRepresentationContext> context = dynamic_pointer_cast<IfcGeometricRepresentationContext>( obj );
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

	if( found_project )
	{
		m_unit_converter->setIfcProject( m_ifc_project );
	}
}

void IfcPPModel::clearCache()
{
	m_ifc_project.reset();
	m_geom_context_3d.reset();
}

void IfcPPModel::resolveInverseAttributes()
{
	for( auto it = m_map_entities.begin(); it != m_map_entities.end(); ++it )
	{
		shared_ptr<IfcPPEntity> entity = it->second;
		if( !entity )
		{
			continue;
		}
		entity->setInverseCounterparts( entity );
	}
}

void IfcPPModel::collectDependentEntities( shared_ptr<IfcPPEntity>& entity, std::map<IfcPPEntity*, shared_ptr<IfcPPEntity> >& target_map )
{
	if( !entity )
	{
		return;
	}
	//if( entity->m_id < 0 )
	//{
	//	entity->setId( next_entity_id );
	//	++next_entity_id;
	//}

	shared_ptr<IfcElementAssembly> ele_assembly = dynamic_pointer_cast<IfcElementAssembly>( entity );
	if( ele_assembly )
	{
		int assembly_id = ele_assembly->m_id;
		std::vector<weak_ptr<IfcRelAggregates> >& vec_is_decomposed_by = ele_assembly->m_IsDecomposedBy_inverse;
		for( size_t ii = 0; ii < vec_is_decomposed_by.size(); ++ii )
		{
			shared_ptr<IfcRelDecomposes> rel_dec;
			const weak_ptr<IfcRelAggregates>& is_decomposed_weak_ptr = vec_is_decomposed_by[ii];
			if( is_decomposed_weak_ptr.expired() )
			{
				continue;
			}
			shared_ptr<IfcRelAggregates> is_decomposed_ptr( is_decomposed_weak_ptr );
			if( is_decomposed_ptr )
			{
				int rel_aggregates_id = is_decomposed_ptr->m_id;

				shared_ptr<IfcPPEntity> as_ifcpp_entity = is_decomposed_ptr;
				collectDependentEntities( as_ifcpp_entity, target_map );
			}
		}
	}

	std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > > vec_attributes;
	entity->getAttributes( vec_attributes );
	for( size_t ii = 0; ii < vec_attributes.size(); ++ii )
	{
		shared_ptr<IfcPPObject>& attribute = vec_attributes[ii].second;
		if( !attribute )
		{
			// empty attribute
			continue;
		}
		shared_ptr<IfcPPEntity> attribute_entity = dynamic_pointer_cast<IfcPPEntity>( attribute );
		if( attribute_entity )
		{
			if( target_map.find( attribute_entity.get() ) == target_map.end() )
			{
				target_map[attribute_entity.get()] = attribute_entity;
				collectDependentEntities( attribute_entity, target_map );

			}
			continue;
		}

		shared_ptr<IfcPPAttributeObjectVector> attribute_object_vector = dynamic_pointer_cast<IfcPPAttributeObjectVector>( attribute );
		if( attribute_object_vector )
		{
			std::vector<shared_ptr<IfcPPObject> >& vec_of_attributes = attribute_object_vector->m_vec;

			for( size_t jj = 0; jj < vec_of_attributes.size(); ++jj )
			{
				shared_ptr<IfcPPObject>& attribute_object = vec_of_attributes[jj];

				shared_ptr<IfcPPEntity> attribute_entity = dynamic_pointer_cast<IfcPPEntity>( attribute_object );
				if( attribute_entity )
				{
					if( target_map.find( attribute_entity.get() ) == target_map.end() )
					{
						target_map[attribute_entity.get()] = attribute_entity;
						collectDependentEntities( attribute_entity, target_map );

					}
				}
			}
		}
	}

	if( target_map.find( entity.get() ) == target_map.end() )
	{
		target_map[entity.get()] = entity;
	}
}
