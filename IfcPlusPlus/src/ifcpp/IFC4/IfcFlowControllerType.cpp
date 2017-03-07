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
#include "include/IfcFlowControllerType.h"
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

// ENTITY IfcFlowControllerType 
IfcFlowControllerType::IfcFlowControllerType() { m_entity_enum = IFCFLOWCONTROLLERTYPE; }
IfcFlowControllerType::IfcFlowControllerType( int id ) { m_id = id; m_entity_enum = IFCFLOWCONTROLLERTYPE; }
IfcFlowControllerType::~IfcFlowControllerType() {}
shared_ptr<IfcPPObject> IfcFlowControllerType::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFlowControllerType> copy_self( new IfcFlowControllerType() );
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
	return copy_self;
}
void IfcFlowControllerType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCFLOWCONTROLLERTYPE" << "(";
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
	stream << ");";
}
void IfcFlowControllerType::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFlowControllerType::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcFlowControllerType, expecting 9, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4] );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	readEntityReferenceList( args[6], m_RepresentationMaps, map );
	m_Tag = IfcLabel::createObjectFromSTEP( args[7] );
	m_ElementType = IfcLabel::createObjectFromSTEP( args[8] );
}
void IfcFlowControllerType::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcDistributionFlowElementType::getAttributes( vec_attributes );
}
void IfcFlowControllerType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcDistributionFlowElementType::getAttributesInverse( vec_attributes_inverse );
}
void IfcFlowControllerType::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcDistributionFlowElementType::setInverseCounterparts( ptr_self_entity );
}
void IfcFlowControllerType::unlinkFromInverseCounterparts()
{
	IfcDistributionFlowElementType::unlinkFromInverseCounterparts();
}
