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
#include "include/IfcDistributionPort.h"
#include "include/IfcDistributionPortTypeEnum.h"
#include "include/IfcDistributionSystemEnum.h"
#include "include/IfcFlowDirectionEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelConnectsPortToElement.h"
#include "include/IfcRelConnectsPorts.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcDistributionPort 
IfcDistributionPort::IfcDistributionPort() {}
IfcDistributionPort::IfcDistributionPort( int id ) { m_id = id; }
IfcDistributionPort::~IfcDistributionPort() {}

// method setEntity takes over all attributes from another instance of the class
void IfcDistributionPort::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcDistributionPort> other = dynamic_pointer_cast<IfcDistributionPort>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_FlowDirection = other->m_FlowDirection;
	m_PredefinedType = other->m_PredefinedType;
	m_SystemType = other->m_SystemType;
}
void IfcDistributionPort::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCDISTRIBUTIONPORT" << "(";
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
	if( m_FlowDirection ) { m_FlowDirection->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SystemType ) { m_SystemType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDistributionPort::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDistributionPort::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDistributionPort, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcDistributionPort, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_FlowDirection = IfcFlowDirectionEnum::createObjectFromStepData( args[7] );
	m_PredefinedType = IfcDistributionPortTypeEnum::createObjectFromStepData( args[8] );
	m_SystemType = IfcDistributionSystemEnum::createObjectFromStepData( args[9] );
}
void IfcDistributionPort::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPort::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "FlowDirection", m_FlowDirection ) );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "SystemType", m_SystemType ) );
}
void IfcDistributionPort::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcDistributionPort::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPort::setInverseCounterparts( ptr_self_entity );
}
void IfcDistributionPort::unlinkSelf()
{
	IfcPort::unlinkSelf();
}
