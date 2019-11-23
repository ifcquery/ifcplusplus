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
#define BOOST_DATE_TIME_NO_LIB
#include <boost/algorithm/string/replace.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <memory>
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
#include "ifcpp/IFC4/include/IfcReal.h"
#include "ifcpp/IFC4/include/IfcRelationship.h"
#include "ifcpp/IFC4/include/IfcRelAggregates.h"
#include "ifcpp/IFC4/include/IfcSite.h"
#include "ifcpp/IFC4/include/IfcBuilding.h"
#include "ifcpp/IFC4/include/IfcElementAssembly.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"
#include "ifcpp/IFC4/include/IfcUnit.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"

#include "AttributeObject.h"
#include "BuildingGuid.h"
#include "BuildingException.h"
#include "BuildingModel.h"
#include "UnitConverter.h"

BuildingModel::BuildingModel()
{
	m_unit_converter = std::make_shared<UnitConverter>( );
	m_unit_converter->setMessageTarget( this );
	initFileHeader( L"IfcPlusPlus-export.ifc" );
}

BuildingModel::~BuildingModel()= default;

void BuildingModel::initIfcModel()
{
	clearIfcModel();

	shared_ptr<IfcProject> project( new IfcProject( 1 ) );
	insertEntity(project);

	shared_ptr<IfcPerson> person( new IfcPerson() );
	person->m_FamilyName = std::make_shared<IfcLabel>( L"FamilyName" );
	person->m_GivenName = std::make_shared<IfcLabel>( L"GivenName" );
	insertEntity(person);
	
	shared_ptr<IfcOrganization> org( new IfcOrganization() );
	org->m_Name = std::make_shared<IfcLabel>( L"OrganizationName" );
	insertEntity(org);

	shared_ptr<IfcPersonAndOrganization> person_org( new IfcPersonAndOrganization() );
	person_org->m_ThePerson = person;
	person_org->m_TheOrganization = org;
	insertEntity(person_org);

	shared_ptr<IfcApplication> app( new IfcApplication() );
	app->m_ApplicationDeveloper = org;
	app->m_Version = std::make_shared<IfcLabel>( L"1.0" );
	app->m_ApplicationFullName = std::make_shared<IfcLabel>( L"IfcPlusPlus" );
	app->m_ApplicationIdentifier = std::make_shared<IfcIdentifier>( L"IfcPlusPlus" );
	insertEntity(app);

	shared_ptr<IfcCartesianPoint> point( new IfcCartesianPoint() );
	point->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>( 0.0 ) );
	point->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>( 0.0 ) );
	point->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>( 0.0 ) );
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

	project->m_GlobalId = std::make_shared<IfcGloballyUniqueId>( createBase64Uuid_wstr() );
	project->m_OwnerHistory = owner_history;
	project->m_Name = std::make_shared<IfcLabel>( L"IfcPlusPlus project" );
	project->m_UnitsInContext = unit_assignment;

	// create default IfcSite
	shared_ptr<IfcSite> site( new IfcSite() );
	site->m_GlobalId = std::make_shared<IfcGloballyUniqueId>( createBase64Uuid_wstr() );
	site->m_OwnerHistory = owner_history;
	site->m_Name = std::make_shared<IfcLabel>( L"Site" );
	insertEntity(site);

	shared_ptr<IfcRelAggregates> rel_aggregates_site( new IfcRelAggregates() );
	rel_aggregates_site->m_RelatingObject = project;
	insertEntity(rel_aggregates_site);

	// create default Building
	shared_ptr<IfcBuilding> building( new IfcBuilding() );
	building->m_GlobalId = std::make_shared<IfcGloballyUniqueId>( createBase64Uuid_wstr() );
	building->m_OwnerHistory = owner_history;
	building->m_Name = std::make_shared<IfcLabel>( L"Building" );
	insertEntity( building );
	
	// set up world coordinate system
	shared_ptr<IfcDirection> axis( new IfcDirection() );
	insertEntity(axis);
	axis->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );
	axis->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );
	axis->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );

	shared_ptr<IfcDirection> ref_direction( new IfcDirection() );
	insertEntity(ref_direction);
	ref_direction->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );
	ref_direction->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );
	ref_direction->m_DirectionRatios.push_back( std::make_shared<IfcReal>( 0.0 ) );

	shared_ptr<IfcCartesianPoint> location( new IfcCartesianPoint() );
	insertEntity(location);
	location->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>(0.0 ) );
	location->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>(0.0 ) );
	location->m_Coordinates.push_back( std::make_shared<IfcLengthMeasure>(0.0 ) );

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
	std::map<BuildingEntity*, shared_ptr<BuildingEntity> > map_entities_copy;
	map_entities_copy[project.get()] = project;
	
	shared_ptr<BuildingEntity> project_as_entity( project );
	collectDependentEntities( project_as_entity, map_entities_copy );

	for( auto it = map_entities_copy.begin(); it != map_entities_copy.end(); ++it )
	{
		shared_ptr<BuildingEntity> entity = it->second;
		insertEntity( entity );
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

void BuildingModel::setMapIfcEntities( const std::map<int, shared_ptr<BuildingEntity> >& map )
{
	clearIfcModel();
	m_map_entities.clear();
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
	int entity_id = e->m_entity_id;
	if( entity_id < 0 )
	{
		int next_unused_id = getMaxUsedEntityId() + 1;
		e->m_entity_id = next_unused_id;
		entity_id = next_unused_id;
	}

	auto it_find = m_map_entities.find( entity_id );
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
		m_map_entities.insert( it_find, std::map<int, shared_ptr<BuildingEntity> >::value_type( entity_id, e ) );
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

void BuildingModel::removeEntity( shared_ptr<BuildingEntity> e )
{
	if( !e )
	{
		messageCallback( "Entity not valid", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__, e.get() );
		return;
	}
	int remove_id = e->m_entity_id;
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
	}
	m_map_entities.erase( it_find );
}

void BuildingModel::removeEntity( int entity_id )
{
	auto it_find = m_map_entities.find(entity_id);
	if( it_find != m_map_entities.end() )
	{
		shared_ptr<BuildingEntity> entity_found = it_find->second;
		removeEntity( entity_found );
	}
}

int BuildingModel::getMaxUsedEntityId()
{
	if( m_map_entities.empty() )
	{
		return 0;
	}
	// since it is an ordered map, we can just take the key of the last entry
	int max_id = m_map_entities.rbegin()->first;
	return max_id;
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
				int entity_id = entity->m_entity_id;
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

void BuildingModel::initFileHeader( std::wstring file_name )
{
	m_file_name = file_name;
	std::string filename_escaped = encodeStepString( file_name );
	std::wstringstream strs;
	strs << "HEADER;" << std::endl;
	strs << "FILE_DESCRIPTION(('" << m_ifc_schema_version.m_IFC_FILE_SCHEMA << "'),'2;1');" << std::endl;
	strs << "FILE_NAME('" << filename_escaped.c_str() << "','";

	//2011-04-21T14:25:12
	std::locale loc( std::wcout.getloc(), new boost::posix_time::wtime_facet( L"%Y-%m-%dT%H:%M:%S" ) );
	std::basic_stringstream<wchar_t> wss;
	wss.imbue( loc );
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	wss << now;
	std::wstring ts = wss.str();

	strs << ts;
	strs << "',(''),('',''),'','IfcPlusPlus','');" << std::endl;
	strs << "FILE_SCHEMA(('" << m_ifc_schema_version.m_IFC_FILE_SCHEMA << "'));" << std::endl;
	strs << "ENDSEC;" << std::endl;

	m_file_header = strs.str();
}

void BuildingModel::setFileHeader( const std::wstring& header )
{
	m_file_header = header;
}

void BuildingModel::setFileDescription( const std::wstring& description )
{
	m_IFC_FILE_DESCRIPTION = description;
}

void BuildingModel::setFileName( const std::wstring& name )
{
	m_IFC_FILE_NAME = name;
}

void BuildingModel::clearIfcModel()
{
	m_map_entities.clear();
	m_ifc_project.reset();
	m_geom_context_3d.reset();
	m_ifc_schema_version.m_ifc_file_schema_enum = IFC4;
	m_ifc_schema_version.m_IFC_FILE_SCHEMA = L"IFC4";
	m_IFC_FILE_NAME = L"";
	m_IFC_FILE_DESCRIPTION = L"";
	m_file_header = L"";
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
		shared_ptr<BuildingEntity> obj = it->second;
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

void BuildingModel::collectDependentEntities( shared_ptr<BuildingEntity> entity, std::map<BuildingEntity*, shared_ptr<BuildingEntity> >& target_map, bool resolveInverseAttributes )
{
	if( !entity )
	{
		return;
	}
	if( target_map.find( entity.get() ) != target_map.end() )
	{
		//return;
	}
	target_map[entity.get()] = entity;

	shared_ptr<IfcElementAssembly> ele_assembly = dynamic_pointer_cast<IfcElementAssembly>( entity );
	if( ele_assembly )
	{
		int assembly_id = ele_assembly->m_entity_id;
		std::vector<weak_ptr<IfcRelAggregates> >& vec_is_decomposed_by = ele_assembly->m_IsDecomposedBy_inverse;
		for(const auto & is_decomposed_weak_ptr : vec_is_decomposed_by)
		{
			shared_ptr<IfcRelDecomposes> rel_dec;
			if( is_decomposed_weak_ptr.expired() )
			{
				continue;
			}
			shared_ptr<IfcRelAggregates> is_decomposed_ptr( is_decomposed_weak_ptr );
			if( is_decomposed_ptr )
			{
				shared_ptr<BuildingEntity> as_ifcpp_entity = is_decomposed_ptr;
				collectDependentEntities( as_ifcpp_entity, target_map, resolveInverseAttributes );
			}
		}
	}

	std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes;
	entity->getAttributes( vec_attributes );

	if( resolveInverseAttributes )
	{
		entity->getAttributesInverse( vec_attributes );
	}

	for(auto & vec_attribute : vec_attributes)
	{
		shared_ptr<BuildingObject>& attribute = vec_attribute.second;
		if( !attribute )
		{
			// empty attribute
			continue;
		}
		shared_ptr<BuildingEntity> attribute_entity = dynamic_pointer_cast<BuildingEntity>( attribute );
		if( attribute_entity )
		{
			if( target_map.find( attribute_entity.get() ) == target_map.end() )
			{
				target_map[attribute_entity.get()] = attribute_entity;
				collectDependentEntities( attribute_entity, target_map, resolveInverseAttributes );
			}
			continue;
		}

		shared_ptr<AttributeObjectVector> attribute_object_vector = dynamic_pointer_cast<AttributeObjectVector>( attribute );
		if( attribute_object_vector )
		{
			std::vector<shared_ptr<BuildingObject> >& vec_of_attributes = attribute_object_vector->m_vec;

			for(auto & attribute_object : vec_of_attributes)
			{
				shared_ptr<BuildingEntity> attribute_entity = dynamic_pointer_cast<BuildingEntity>( attribute_object );
				if( attribute_entity )
				{
					if( target_map.find( attribute_entity.get() ) == target_map.end() )
					{
						target_map[attribute_entity.get()] = attribute_entity;
						collectDependentEntities( attribute_entity, target_map, resolveInverseAttributes );
					}
				}
			}
		}
	}
}
