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
#include "include/IfcAppliedValue.h"
#include "include/IfcCrewResourceType.h"
#include "include/IfcCrewResourceTypeEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPhysicalQuantity.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToResource.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcCrewResourceType 
IfcCrewResourceType::IfcCrewResourceType() { m_entity_enum = IFCCREWRESOURCETYPE; }
IfcCrewResourceType::IfcCrewResourceType( int id ) { m_id = id; m_entity_enum = IFCCREWRESOURCETYPE; }
IfcCrewResourceType::~IfcCrewResourceType() {}
shared_ptr<IfcPPObject> IfcCrewResourceType::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCrewResourceType> copy_self( new IfcCrewResourceType() );
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
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy(options) ); }
	if( m_LongDescription ) { copy_self->m_LongDescription = dynamic_pointer_cast<IfcText>( m_LongDescription->getDeepCopy(options) ); }
	if( m_ResourceType ) { copy_self->m_ResourceType = dynamic_pointer_cast<IfcLabel>( m_ResourceType->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_BaseCosts.size(); ++ii )
	{
		auto item_ii = m_BaseCosts[ii];
		if( item_ii )
		{
			copy_self->m_BaseCosts.push_back( dynamic_pointer_cast<IfcAppliedValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_BaseQuantity ) { copy_self->m_BaseQuantity = dynamic_pointer_cast<IfcPhysicalQuantity>( m_BaseQuantity->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcCrewResourceTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCrewResourceType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCREWRESOURCETYPE" << "(";
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
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LongDescription ) { m_LongDescription->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ResourceType ) { m_ResourceType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_BaseCosts );
	stream << ",";
	if( m_BaseQuantity ) { stream << "#" << m_BaseQuantity->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCrewResourceType::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCrewResourceType::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 12 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCrewResourceType, expecting 12, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4] );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	m_Identification = IfcIdentifier::createObjectFromSTEP( args[6] );
	m_LongDescription = IfcText::createObjectFromSTEP( args[7] );
	m_ResourceType = IfcLabel::createObjectFromSTEP( args[8] );
	readEntityReferenceList( args[9], m_BaseCosts, map );
	readEntityReference( args[10], m_BaseQuantity, map );
	m_PredefinedType = IfcCrewResourceTypeEnum::createObjectFromSTEP( args[11] );
}
void IfcCrewResourceType::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConstructionResourceType::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcCrewResourceType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConstructionResourceType::getAttributesInverse( vec_attributes_inverse );
}
void IfcCrewResourceType::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConstructionResourceType::setInverseCounterparts( ptr_self_entity );
}
void IfcCrewResourceType::unlinkFromInverseCounterparts()
{
	IfcConstructionResourceType::unlinkFromInverseCounterparts();
}
