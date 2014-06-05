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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
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
IfcStairFlight::IfcStairFlight() {}
IfcStairFlight::IfcStairFlight( int id ) { m_id = id; }
IfcStairFlight::~IfcStairFlight() {}

// method setEntity takes over all attributes from another instance of the class
void IfcStairFlight::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcStairFlight> other = dynamic_pointer_cast<IfcStairFlight>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_Tag = other->m_Tag;
	m_NumberOfRiser = other->m_NumberOfRiser;
	m_NumberOfTreads = other->m_NumberOfTreads;
	m_RiserHeight = other->m_RiserHeight;
	m_TreadLength = other->m_TreadLength;
	m_PredefinedType = other->m_PredefinedType;
}
void IfcStairFlight::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSTAIRFLIGHT" << "(";
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
	if( m_ObjectPlacement ) { stream << "#" << m_ObjectPlacement->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Representation ) { stream << "#" << m_Representation->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_NumberOfRiser == m_NumberOfRiser ){ stream << m_NumberOfRiser; }
	else { stream << "$"; }
	stream << ",";
	if( m_NumberOfTreads == m_NumberOfTreads ){ stream << m_NumberOfTreads; }
	else { stream << "$"; }
	stream << ",";
	if( m_RiserHeight ) { m_RiserHeight->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TreadLength ) { m_TreadLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStairFlight::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStairFlight::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<13 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStairFlight, expecting 13, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>13 ){ std::cout << "Wrong parameter count for entity IfcStairFlight, expecting 13, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_Tag = IfcIdentifier::createObjectFromStepData( args[7] );
	readIntValue( args[8], m_NumberOfRiser );
	readIntValue( args[9], m_NumberOfTreads );
	m_RiserHeight = IfcPositiveLengthMeasure::createObjectFromStepData( args[10] );
	m_TreadLength = IfcPositiveLengthMeasure::createObjectFromStepData( args[11] );
	m_PredefinedType = IfcStairFlightTypeEnum::createObjectFromStepData( args[12] );
}
void IfcStairFlight::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBuildingElement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "NumberOfRiser", shared_ptr<IfcPPInt>( new IfcPPInt( m_NumberOfRiser ) ) ) );
	vec_attributes.push_back( std::make_pair( "NumberOfTreads", shared_ptr<IfcPPInt>( new IfcPPInt( m_NumberOfTreads ) ) ) );
	vec_attributes.push_back( std::make_pair( "RiserHeight", m_RiserHeight ) );
	vec_attributes.push_back( std::make_pair( "TreadLength", m_TreadLength ) );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcStairFlight::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcStairFlight::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBuildingElement::setInverseCounterparts( ptr_self_entity );
}
void IfcStairFlight::unlinkSelf()
{
	IfcBuildingElement::unlinkSelf();
}
