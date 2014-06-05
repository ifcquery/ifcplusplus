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
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProcess.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelSequence.h"
#include "include/IfcTask.h"
#include "include/IfcTaskTime.h"
#include "include/IfcTaskTypeEnum.h"
#include "include/IfcText.h"

// ENTITY IfcTask 
IfcTask::IfcTask() {}
IfcTask::IfcTask( int id ) { m_id = id; }
IfcTask::~IfcTask() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTask::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTask> other = dynamic_pointer_cast<IfcTask>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_Identification = other->m_Identification;
	m_LongDescription = other->m_LongDescription;
	m_Status = other->m_Status;
	m_WorkMethod = other->m_WorkMethod;
	m_IsMilestone = other->m_IsMilestone;
	m_Priority = other->m_Priority;
	m_TaskTime = other->m_TaskTime;
	m_PredefinedType = other->m_PredefinedType;
}
void IfcTask::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTASK" << "(";
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
	if( m_LongDescription ) { m_LongDescription->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Status ) { m_Status->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_WorkMethod ) { m_WorkMethod->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_IsMilestone == false ) { stream << ".F."; }
	else if( m_IsMilestone == true ) { stream << ".T."; }
	stream << ",";
	if( m_Priority == m_Priority ){ stream << m_Priority; }
	else { stream << "$"; }
	stream << ",";
	if( m_TaskTime ) { stream << "#" << m_TaskTime->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTask::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTask::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<13 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTask, expecting 13, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>13 ){ std::cout << "Wrong parameter count for entity IfcTask, expecting 13, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_Identification = IfcIdentifier::createObjectFromStepData( args[5] );
	m_LongDescription = IfcText::createObjectFromStepData( args[6] );
	m_Status = IfcLabel::createObjectFromStepData( args[7] );
	m_WorkMethod = IfcLabel::createObjectFromStepData( args[8] );
	if( _stricmp( args[9].c_str(), ".F." ) == 0 ) { m_IsMilestone = false; }
	else if( _stricmp( args[9].c_str(), ".T." ) == 0 ) { m_IsMilestone = true; }
	readIntValue( args[10], m_Priority );
	readEntityReference( args[11], m_TaskTime, map );
	m_PredefinedType = IfcTaskTypeEnum::createObjectFromStepData( args[12] );
}
void IfcTask::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProcess::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Status", m_Status ) );
	vec_attributes.push_back( std::make_pair( "WorkMethod", m_WorkMethod ) );
	vec_attributes.push_back( std::make_pair( "IsMilestone", shared_ptr<IfcPPBool>( new IfcPPBool( m_IsMilestone ) ) ) );
	vec_attributes.push_back( std::make_pair( "Priority", shared_ptr<IfcPPInt>( new IfcPPInt( m_Priority ) ) ) );
	vec_attributes.push_back( std::make_pair( "TaskTime", m_TaskTime ) );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcTask::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTask::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProcess::setInverseCounterparts( ptr_self_entity );
}
void IfcTask::unlinkSelf()
{
	IfcProcess::unlinkSelf();
}
