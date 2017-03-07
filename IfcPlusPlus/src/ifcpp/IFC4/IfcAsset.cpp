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
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
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
IfcAsset::IfcAsset() { m_entity_enum = IFCASSET; }
IfcAsset::IfcAsset( int id ) { m_id = id; m_entity_enum = IFCASSET; }
IfcAsset::~IfcAsset() {}
shared_ptr<IfcPPObject> IfcAsset::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAsset> copy_self( new IfcAsset() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
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
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy(options) ); }
	if( m_OriginalValue ) { copy_self->m_OriginalValue = dynamic_pointer_cast<IfcCostValue>( m_OriginalValue->getDeepCopy(options) ); }
	if( m_CurrentValue ) { copy_self->m_CurrentValue = dynamic_pointer_cast<IfcCostValue>( m_CurrentValue->getDeepCopy(options) ); }
	if( m_TotalReplacementCost ) { copy_self->m_TotalReplacementCost = dynamic_pointer_cast<IfcCostValue>( m_TotalReplacementCost->getDeepCopy(options) ); }
	if( m_Owner ) { copy_self->m_Owner = dynamic_pointer_cast<IfcActorSelect>( m_Owner->getDeepCopy(options) ); }
	if( m_User ) { copy_self->m_User = dynamic_pointer_cast<IfcActorSelect>( m_User->getDeepCopy(options) ); }
	if( m_ResponsiblePerson ) { copy_self->m_ResponsiblePerson = dynamic_pointer_cast<IfcPerson>( m_ResponsiblePerson->getDeepCopy(options) ); }
	if( m_IncorporationDate ) { copy_self->m_IncorporationDate = dynamic_pointer_cast<IfcDate>( m_IncorporationDate->getDeepCopy(options) ); }
	if( m_DepreciatedValue ) { copy_self->m_DepreciatedValue = dynamic_pointer_cast<IfcCostValue>( m_DepreciatedValue->getDeepCopy(options) ); }
	return copy_self;
}
void IfcAsset::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCASSET" << "(";
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
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OriginalValue ) { stream << "#" << m_OriginalValue->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_CurrentValue ) { stream << "#" << m_CurrentValue->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_TotalReplacementCost ) { stream << "#" << m_TotalReplacementCost->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Owner ) { m_Owner->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_User ) { m_User->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ResponsiblePerson ) { stream << "#" << m_ResponsiblePerson->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_IncorporationDate ) { m_IncorporationDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DepreciatedValue ) { stream << "#" << m_DepreciatedValue->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcAsset::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAsset::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 14 ){ std::stringstream err; err << "Wrong parameter count for entity IfcAsset, expecting 14, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	m_Identification = IfcIdentifier::createObjectFromSTEP( args[5] );
	readEntityReference( args[6], m_OriginalValue, map );
	readEntityReference( args[7], m_CurrentValue, map );
	readEntityReference( args[8], m_TotalReplacementCost, map );
	m_Owner = IfcActorSelect::createObjectFromSTEP( args[9], map );
	m_User = IfcActorSelect::createObjectFromSTEP( args[10], map );
	readEntityReference( args[11], m_ResponsiblePerson, map );
	m_IncorporationDate = IfcDate::createObjectFromSTEP( args[12] );
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
void IfcAsset::unlinkFromInverseCounterparts()
{
	IfcGroup::unlinkFromInverseCounterparts();
}
