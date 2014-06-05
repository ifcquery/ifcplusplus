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

// method setEntity takes over all attributes from another instance of the class
void IfcInventory::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcInventory> other = dynamic_pointer_cast<IfcInventory>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_PredefinedType = other->m_PredefinedType;
	m_Jurisdiction = other->m_Jurisdiction;
	m_ResponsiblePersons = other->m_ResponsiblePersons;
	m_LastUpdateDate = other->m_LastUpdateDate;
	m_CurrentValue = other->m_CurrentValue;
	m_OriginalValue = other->m_OriginalValue;
}
void IfcInventory::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCINVENTORY" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Jurisdiction ) { m_Jurisdiction->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_ResponsiblePersons );
	stream << ",";
	if( m_LastUpdateDate ) { m_LastUpdateDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CurrentValue ) { stream << "#" << m_CurrentValue->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_OriginalValue ) { stream << "#" << m_OriginalValue->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcInventory::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcInventory::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<11 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcInventory, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>11 ){ std::cout << "Wrong parameter count for entity IfcInventory, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_PredefinedType = IfcInventoryTypeEnum::createObjectFromStepData( args[5] );
	m_Jurisdiction = IfcActorSelect::createObjectFromStepData( args[6], map );
	readEntityReferenceList( args[7], m_ResponsiblePersons, map );
	m_LastUpdateDate = IfcDate::createObjectFromStepData( args[8] );
	readEntityReference( args[9], m_CurrentValue, map );
	readEntityReference( args[10], m_OriginalValue, map );
}
void IfcInventory::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGroup::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "Jurisdiction", m_Jurisdiction ) );
	vec_attributes.push_back( std::make_pair( "LastUpdateDate", m_LastUpdateDate ) );
	vec_attributes.push_back( std::make_pair( "CurrentValue", m_CurrentValue ) );
	vec_attributes.push_back( std::make_pair( "OriginalValue", m_OriginalValue ) );
}
void IfcInventory::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcInventory::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGroup::setInverseCounterparts( ptr_self_entity );
}
void IfcInventory::unlinkSelf()
{
	IfcGroup::unlinkSelf();
}
