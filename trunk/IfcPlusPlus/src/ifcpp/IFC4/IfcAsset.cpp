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
#include "include/IfcAsset.h"
#include "include/IfcCostValue.h"
#include "include/IfcDate.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
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

// ENTITY IfcAsset 
IfcAsset::IfcAsset() {}
IfcAsset::IfcAsset( int id ) { m_id = id; }
IfcAsset::~IfcAsset() {}
shared_ptr<IfcPPObject> IfcAsset::getDeepCopy()
{
	shared_ptr<IfcAsset> copy_self( new IfcAsset() );
	if( m_GlobalId ) { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy() ); }
	if( m_OwnerHistory ) { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy() ); }
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy() ); }
	if( m_OriginalValue ) { copy_self->m_OriginalValue = dynamic_pointer_cast<IfcCostValue>( m_OriginalValue->getDeepCopy() ); }
	if( m_CurrentValue ) { copy_self->m_CurrentValue = dynamic_pointer_cast<IfcCostValue>( m_CurrentValue->getDeepCopy() ); }
	if( m_TotalReplacementCost ) { copy_self->m_TotalReplacementCost = dynamic_pointer_cast<IfcCostValue>( m_TotalReplacementCost->getDeepCopy() ); }
	if( m_Owner ) { copy_self->m_Owner = dynamic_pointer_cast<IfcActorSelect>( m_Owner->getDeepCopy() ); }
	if( m_User ) { copy_self->m_User = dynamic_pointer_cast<IfcActorSelect>( m_User->getDeepCopy() ); }
	if( m_ResponsiblePerson ) { copy_self->m_ResponsiblePerson = dynamic_pointer_cast<IfcPerson>( m_ResponsiblePerson->getDeepCopy() ); }
	if( m_IncorporationDate ) { copy_self->m_IncorporationDate = dynamic_pointer_cast<IfcDate>( m_IncorporationDate->getDeepCopy() ); }
	if( m_DepreciatedValue ) { copy_self->m_DepreciatedValue = dynamic_pointer_cast<IfcCostValue>( m_DepreciatedValue->getDeepCopy() ); }
	return copy_self;
}
void IfcAsset::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCASSET" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OriginalValue ) { stream << "#" << m_OriginalValue->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_CurrentValue ) { stream << "#" << m_CurrentValue->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TotalReplacementCost ) { stream << "#" << m_TotalReplacementCost->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Owner ) { m_Owner->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_User ) { m_User->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ResponsiblePerson ) { stream << "#" << m_ResponsiblePerson->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_IncorporationDate ) { m_IncorporationDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DepreciatedValue ) { stream << "#" << m_DepreciatedValue->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcAsset::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAsset::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<14 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcAsset, expecting 14, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>14 ){ std::cout << "Wrong parameter count for entity IfcAsset, expecting 14, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_Identification = IfcIdentifier::createObjectFromStepData( args[5] );
	readEntityReference( args[6], m_OriginalValue, map );
	readEntityReference( args[7], m_CurrentValue, map );
	readEntityReference( args[8], m_TotalReplacementCost, map );
	m_Owner = IfcActorSelect::createObjectFromStepData( args[9], map );
	m_User = IfcActorSelect::createObjectFromStepData( args[10], map );
	readEntityReference( args[11], m_ResponsiblePerson, map );
	m_IncorporationDate = IfcDate::createObjectFromStepData( args[12] );
	readEntityReference( args[13], m_DepreciatedValue, map );
}
void IfcAsset::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGroup::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Identification", m_Identification ) );
	vec_attributes.push_back( std::make_pair( "OriginalValue", m_OriginalValue ) );
	vec_attributes.push_back( std::make_pair( "CurrentValue", m_CurrentValue ) );
	vec_attributes.push_back( std::make_pair( "TotalReplacementCost", m_TotalReplacementCost ) );
	vec_attributes.push_back( std::make_pair( "Owner", m_Owner ) );
	vec_attributes.push_back( std::make_pair( "User", m_User ) );
	vec_attributes.push_back( std::make_pair( "ResponsiblePerson", m_ResponsiblePerson ) );
	vec_attributes.push_back( std::make_pair( "IncorporationDate", m_IncorporationDate ) );
	vec_attributes.push_back( std::make_pair( "DepreciatedValue", m_DepreciatedValue ) );
}
void IfcAsset::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGroup::getAttributesInverse( vec_attributes_inverse );
}
void IfcAsset::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGroup::setInverseCounterparts( ptr_self_entity );
}
void IfcAsset::unlinkSelf()
{
	IfcGroup::unlinkSelf();
}
