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
#include "include/IfcCostSchedule.h"
#include "include/IfcCostScheduleTypeEnum.h"
#include "include/IfcDateTime.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToControl.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcCostSchedule 
IfcCostSchedule::IfcCostSchedule() {}
IfcCostSchedule::IfcCostSchedule( int id ) { m_id = id; }
IfcCostSchedule::~IfcCostSchedule() {}

// method setEntity takes over all attributes from another instance of the class
void IfcCostSchedule::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcCostSchedule> other = dynamic_pointer_cast<IfcCostSchedule>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_Identification = other->m_Identification;
	m_PredefinedType = other->m_PredefinedType;
	m_Status = other->m_Status;
	m_SubmittedOn = other->m_SubmittedOn;
	m_UpdateDate = other->m_UpdateDate;
}
void IfcCostSchedule::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCOSTSCHEDULE" << "(";
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
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Status ) { m_Status->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SubmittedOn ) { m_SubmittedOn->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UpdateDate ) { m_UpdateDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCostSchedule::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCostSchedule::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCostSchedule, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcCostSchedule, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_Identification = IfcIdentifier::createObjectFromStepData( args[5] );
	m_PredefinedType = IfcCostScheduleTypeEnum::createObjectFromStepData( args[6] );
	m_Status = IfcLabel::createObjectFromStepData( args[7] );
	m_SubmittedOn = IfcDateTime::createObjectFromStepData( args[8] );
	m_UpdateDate = IfcDateTime::createObjectFromStepData( args[9] );
}
void IfcCostSchedule::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcControl::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "Status", m_Status ) );
	vec_attributes.push_back( std::make_pair( "SubmittedOn", m_SubmittedOn ) );
	vec_attributes.push_back( std::make_pair( "UpdateDate", m_UpdateDate ) );
}
void IfcCostSchedule::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcCostSchedule::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcControl::setInverseCounterparts( ptr_self_entity );
}
void IfcCostSchedule::unlinkSelf()
{
	IfcControl::unlinkSelf();
}
