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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcInteger.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelConnectsElements.h"
#include "include/IfcRelConnectsWithRealizingElements.h"
#include "include/IfcRelContainedInSpatialStructure.h"
#include "include/IfcRelCoversBldgElements.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelFillsElement.h"
#include "include/IfcRelInterferesElements.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelProjectsElement.h"
#include "include/IfcRelReferencedInSpatialStructure.h"
#include "include/IfcRelSpaceBoundary.h"
#include "include/IfcRelVoidsElement.h"
#include "include/IfcStairFlight.h"
#include "include/IfcStairFlightTypeEnum.h"
#include "include/IfcText.h"

// ENTITY IfcStairFlight 
IfcStairFlight::IfcStairFlight() { m_entity_enum = IFCSTAIRFLIGHT; }
IfcStairFlight::IfcStairFlight( int id ) { m_id = id; m_entity_enum = IFCSTAIRFLIGHT; }
IfcStairFlight::~IfcStairFlight() {}
shared_ptr<IfcPPObject> IfcStairFlight::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStairFlight> copy_self( new IfcStairFlight() );
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
	if( m_ObjectPlacement ) { copy_self->m_ObjectPlacement = dynamic_pointer_cast<IfcObjectPlacement>( m_ObjectPlacement->getDeepCopy(options) ); }
	if( m_Representation ) { copy_self->m_Representation = dynamic_pointer_cast<IfcProductRepresentation>( m_Representation->getDeepCopy(options) ); }
	if( m_Tag ) { copy_self->m_Tag = dynamic_pointer_cast<IfcIdentifier>( m_Tag->getDeepCopy(options) ); }
	if( m_NumberOfRisers ) { copy_self->m_NumberOfRisers = dynamic_pointer_cast<IfcInteger>( m_NumberOfRisers->getDeepCopy(options) ); }
	if( m_NumberOfTreads ) { copy_self->m_NumberOfTreads = dynamic_pointer_cast<IfcInteger>( m_NumberOfTreads->getDeepCopy(options) ); }
	if( m_RiserHeight ) { copy_self->m_RiserHeight = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_RiserHeight->getDeepCopy(options) ); }
	if( m_TreadLength ) { copy_self->m_TreadLength = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_TreadLength->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcStairFlightTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStairFlight::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTAIRFLIGHT" << "(";
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
	if( m_ObjectPlacement ) { stream << "#" << m_ObjectPlacement->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Representation ) { stream << "#" << m_Representation->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_NumberOfRisers ) { m_NumberOfRisers->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_NumberOfTreads ) { m_NumberOfTreads->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RiserHeight ) { m_RiserHeight->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TreadLength ) { m_TreadLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStairFlight::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStairFlight::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 13 ){ std::stringstream err; err << "Wrong parameter count for entity IfcStairFlight, expecting 13, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_Tag = IfcIdentifier::createObjectFromSTEP( args[7] );
	m_NumberOfRisers = IfcInteger::createObjectFromSTEP( args[8] );
	m_NumberOfTreads = IfcInteger::createObjectFromSTEP( args[9] );
	m_RiserHeight = IfcPositiveLengthMeasure::createObjectFromSTEP( args[10] );
	m_TreadLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[11] );
	m_PredefinedType = IfcStairFlightTypeEnum::createObjectFromSTEP( args[12] );
}
void IfcStairFlight::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBuildingElement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "NumberOfRisers", m_NumberOfRisers ) );
	vec_attributes.push_back( std::make_pair( "NumberOfTreads", m_NumberOfTreads ) );
	vec_attributes.push_back( std::make_pair( "RiserHeight", m_RiserHeight ) );
	vec_attributes.push_back( std::make_pair( "TreadLength", m_TreadLength ) );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcStairFlight::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBuildingElement::getAttributesInverse( vec_attributes_inverse );
}
void IfcStairFlight::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBuildingElement::setInverseCounterparts( ptr_self_entity );
}
void IfcStairFlight::unlinkFromInverseCounterparts()
{
	IfcBuildingElement::unlinkFromInverseCounterparts();
}
