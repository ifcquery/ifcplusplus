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
#include "include/IfcBoolean.h"
#include "include/IfcDoorType.h"
#include "include/IfcDoorTypeEnum.h"
#include "include/IfcDoorTypeOperationEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcText.h"

// ENTITY IfcDoorType 
IfcDoorType::IfcDoorType() { m_entity_enum = IFCDOORTYPE; }
IfcDoorType::IfcDoorType( int id ) { m_id = id; m_entity_enum = IFCDOORTYPE; }
IfcDoorType::~IfcDoorType() {}
shared_ptr<IfcPPObject> IfcDoorType::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDoorType> copy_self( new IfcDoorType() );
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
	if( m_ApplicableOccurrence ) { copy_self->m_ApplicableOccurrence = dynamic_pointer_cast<IfcIdentifier>( m_ApplicableOccurrence->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_HasPropertySets.size(); ++ii )
	{
		auto item_ii = m_HasPropertySets[ii];
		if( item_ii )
		{
			copy_self->m_HasPropertySets.push_back( dynamic_pointer_cast<IfcPropertySetDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_RepresentationMaps.size(); ++ii )
	{
		auto item_ii = m_RepresentationMaps[ii];
		if( item_ii )
		{
			copy_self->m_RepresentationMaps.push_back( dynamic_pointer_cast<IfcRepresentationMap>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Tag ) { copy_self->m_Tag = dynamic_pointer_cast<IfcLabel>( m_Tag->getDeepCopy(options) ); }
	if( m_ElementType ) { copy_self->m_ElementType = dynamic_pointer_cast<IfcLabel>( m_ElementType->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcDoorTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	if( m_OperationType ) { copy_self->m_OperationType = dynamic_pointer_cast<IfcDoorTypeOperationEnum>( m_OperationType->getDeepCopy(options) ); }
	if( m_ParameterTakesPrecedence ) { copy_self->m_ParameterTakesPrecedence = dynamic_pointer_cast<IfcBoolean>( m_ParameterTakesPrecedence->getDeepCopy(options) ); }
	if( m_UserDefinedOperationType ) { copy_self->m_UserDefinedOperationType = dynamic_pointer_cast<IfcLabel>( m_UserDefinedOperationType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcDoorType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDOORTYPE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ApplicableOccurrence ) { m_ApplicableOccurrence->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertySets );
	stream << ",";
	writeEntityList( stream, m_RepresentationMaps );
	stream << ",";
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ElementType ) { m_ElementType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OperationType ) { m_OperationType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ParameterTakesPrecedence ) { m_ParameterTakesPrecedence->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedOperationType ) { m_UserDefinedOperationType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDoorType::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDoorType::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 13 ){ std::stringstream err; err << "Wrong parameter count for entity IfcDoorType, expecting 13, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4] );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	readEntityReferenceList( args[6], m_RepresentationMaps, map );
	m_Tag = IfcLabel::createObjectFromSTEP( args[7] );
	m_ElementType = IfcLabel::createObjectFromSTEP( args[8] );
	m_PredefinedType = IfcDoorTypeEnum::createObjectFromSTEP( args[9] );
	m_OperationType = IfcDoorTypeOperationEnum::createObjectFromSTEP( args[10] );
	m_ParameterTakesPrecedence = IfcBoolean::createObjectFromSTEP( args[11] );
	m_UserDefinedOperationType = IfcLabel::createObjectFromSTEP( args[12] );
}
void IfcDoorType::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBuildingElementType::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "OperationType", m_OperationType ) );
	vec_attributes.push_back( std::make_pair( "ParameterTakesPrecedence", m_ParameterTakesPrecedence ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedOperationType", m_UserDefinedOperationType ) );
}
void IfcDoorType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBuildingElementType::getAttributesInverse( vec_attributes_inverse );
}
void IfcDoorType::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBuildingElementType::setInverseCounterparts( ptr_self_entity );
}
void IfcDoorType::unlinkFromInverseCounterparts()
{
	IfcBuildingElementType::unlinkFromInverseCounterparts();
}
