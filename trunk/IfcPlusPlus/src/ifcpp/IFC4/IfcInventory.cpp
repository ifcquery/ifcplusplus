/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
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
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcActorSelect.h"
#include "include/IfcCostValue.h"
#include "include/IfcDate.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcInventory.h"
#include "include/IfcInventoryTypeEnum.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPerson.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToGroup.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcInventory 
IfcInventory::IfcInventory() {}
IfcInventory::IfcInventory( int id ) { m_id = id; }
IfcInventory::~IfcInventory() {}
shared_ptr<IfcPPObject> IfcInventory::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcInventory> copy_self( new IfcInventory() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( CreateCompressedGuidString22() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcInventoryTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	if( m_Jurisdiction ) { copy_self->m_Jurisdiction = dynamic_pointer_cast<IfcActorSelect>( m_Jurisdiction->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ResponsiblePersons.size(); ++ii )
	{
		auto item_ii = m_ResponsiblePersons[ii];
		if( item_ii )
		{
			copy_self->m_ResponsiblePersons.push_back( dynamic_pointer_cast<IfcPerson>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_LastUpdateDate ) { copy_self->m_LastUpdateDate = dynamic_pointer_cast<IfcDate>( m_LastUpdateDate->getDeepCopy(options) ); }
	if( m_CurrentValue ) { copy_self->m_CurrentValue = dynamic_pointer_cast<IfcCostValue>( m_CurrentValue->getDeepCopy(options) ); }
	if( m_OriginalValue ) { copy_self->m_OriginalValue = dynamic_pointer_cast<IfcCostValue>( m_OriginalValue->getDeepCopy(options) ); }
	return copy_self;
}
void IfcInventory::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCINVENTORY" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Jurisdiction ) { m_Jurisdiction->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	writeEntityList( stream, m_ResponsiblePersons );
	stream << ",";
	if( m_LastUpdateDate ) { m_LastUpdateDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CurrentValue ) { stream << "#" << m_CurrentValue->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_OriginalValue ) { stream << "#" << m_OriginalValue->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcInventory::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcInventory::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 11 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcInventory, expecting 11, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	m_PredefinedType = IfcInventoryTypeEnum::createObjectFromSTEP( args[5] );
	m_Jurisdiction = IfcActorSelect::createObjectFromSTEP( args[6], map );
	readEntityReferenceList( args[7], m_ResponsiblePersons, map );
	m_LastUpdateDate = IfcDate::createObjectFromSTEP( args[8] );
	readEntityReference( args[9], m_CurrentValue, map );
	readEntityReference( args[10], m_OriginalValue, map );
}
void IfcInventory::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGroup::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "Jurisdiction", m_Jurisdiction ) );
	if( m_ResponsiblePersons.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ResponsiblePersons_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ResponsiblePersons.begin(), m_ResponsiblePersons.end(), std::back_inserter( ResponsiblePersons_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ResponsiblePersons", ResponsiblePersons_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "LastUpdateDate", m_LastUpdateDate ) );
	vec_attributes.push_back( std::make_pair( "CurrentValue", m_CurrentValue ) );
	vec_attributes.push_back( std::make_pair( "OriginalValue", m_OriginalValue ) );
}
void IfcInventory::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGroup::getAttributesInverse( vec_attributes_inverse );
}
void IfcInventory::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGroup::setInverseCounterparts( ptr_self_entity );
}
void IfcInventory::unlinkSelf()
{
	IfcGroup::unlinkSelf();
}
