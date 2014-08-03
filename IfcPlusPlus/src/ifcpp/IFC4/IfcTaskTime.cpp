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
#include "include/IfcDataOriginEnum.h"
#include "include/IfcDateTime.h"
#include "include/IfcDuration.h"
#include "include/IfcLabel.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcTaskDurationEnum.h"
#include "include/IfcTaskTime.h"

// ENTITY IfcTaskTime 
IfcTaskTime::IfcTaskTime() {}
IfcTaskTime::IfcTaskTime( int id ) { m_id = id; }
IfcTaskTime::~IfcTaskTime() {}
shared_ptr<IfcPPObject> IfcTaskTime::getDeepCopy()
{
	shared_ptr<IfcTaskTime> copy_self( new IfcTaskTime() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_DataOrigin ) { copy_self->m_DataOrigin = dynamic_pointer_cast<IfcDataOriginEnum>( m_DataOrigin->getDeepCopy() ); }
	if( m_UserDefinedDataOrigin ) { copy_self->m_UserDefinedDataOrigin = dynamic_pointer_cast<IfcLabel>( m_UserDefinedDataOrigin->getDeepCopy() ); }
	if( m_DurationType ) { copy_self->m_DurationType = dynamic_pointer_cast<IfcTaskDurationEnum>( m_DurationType->getDeepCopy() ); }
	if( m_ScheduleDuration ) { copy_self->m_ScheduleDuration = dynamic_pointer_cast<IfcDuration>( m_ScheduleDuration->getDeepCopy() ); }
	if( m_ScheduleStart ) { copy_self->m_ScheduleStart = dynamic_pointer_cast<IfcDateTime>( m_ScheduleStart->getDeepCopy() ); }
	if( m_ScheduleFinish ) { copy_self->m_ScheduleFinish = dynamic_pointer_cast<IfcDateTime>( m_ScheduleFinish->getDeepCopy() ); }
	if( m_EarlyStart ) { copy_self->m_EarlyStart = dynamic_pointer_cast<IfcDateTime>( m_EarlyStart->getDeepCopy() ); }
	if( m_EarlyFinish ) { copy_self->m_EarlyFinish = dynamic_pointer_cast<IfcDateTime>( m_EarlyFinish->getDeepCopy() ); }
	if( m_LateStart ) { copy_self->m_LateStart = dynamic_pointer_cast<IfcDateTime>( m_LateStart->getDeepCopy() ); }
	if( m_LateFinish ) { copy_self->m_LateFinish = dynamic_pointer_cast<IfcDateTime>( m_LateFinish->getDeepCopy() ); }
	if( m_FreeFloat ) { copy_self->m_FreeFloat = dynamic_pointer_cast<IfcDuration>( m_FreeFloat->getDeepCopy() ); }
	if( m_TotalFloat ) { copy_self->m_TotalFloat = dynamic_pointer_cast<IfcDuration>( m_TotalFloat->getDeepCopy() ); }
	if( m_IsCritical ) { copy_self->m_IsCritical = m_IsCritical; }
	if( m_StatusTime ) { copy_self->m_StatusTime = dynamic_pointer_cast<IfcDateTime>( m_StatusTime->getDeepCopy() ); }
	if( m_ActualDuration ) { copy_self->m_ActualDuration = dynamic_pointer_cast<IfcDuration>( m_ActualDuration->getDeepCopy() ); }
	if( m_ActualStart ) { copy_self->m_ActualStart = dynamic_pointer_cast<IfcDateTime>( m_ActualStart->getDeepCopy() ); }
	if( m_ActualFinish ) { copy_self->m_ActualFinish = dynamic_pointer_cast<IfcDateTime>( m_ActualFinish->getDeepCopy() ); }
	if( m_RemainingTime ) { copy_self->m_RemainingTime = dynamic_pointer_cast<IfcDuration>( m_RemainingTime->getDeepCopy() ); }
	if( m_Completion ) { copy_self->m_Completion = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_Completion->getDeepCopy() ); }
	return copy_self;
}
void IfcTaskTime::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTASKTIME" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DurationType ) { m_DurationType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ScheduleDuration ) { m_ScheduleDuration->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ScheduleStart ) { m_ScheduleStart->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ScheduleFinish ) { m_ScheduleFinish->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EarlyStart ) { m_EarlyStart->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EarlyFinish ) { m_EarlyFinish->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LateStart ) { m_LateStart->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LateFinish ) { m_LateFinish->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FreeFloat ) { m_FreeFloat->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TotalFloat ) { m_TotalFloat->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_IsCritical == false ) { stream << ".F."; }
	else if( m_IsCritical == true ) { stream << ".T."; }
	stream << ",";
	if( m_StatusTime ) { m_StatusTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActualDuration ) { m_ActualDuration->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActualStart ) { m_ActualStart->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActualFinish ) { m_ActualFinish->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RemainingTime ) { m_RemainingTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Completion ) { m_Completion->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTaskTime::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTaskTime::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<20 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTaskTime, expecting 20, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>20 ){ std::cout << "Wrong parameter count for entity IfcTaskTime, expecting 20, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromStepData( args[1] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromStepData( args[2] );
	m_DurationType = IfcTaskDurationEnum::createObjectFromStepData( args[3] );
	m_ScheduleDuration = IfcDuration::createObjectFromStepData( args[4] );
	m_ScheduleStart = IfcDateTime::createObjectFromStepData( args[5] );
	m_ScheduleFinish = IfcDateTime::createObjectFromStepData( args[6] );
	m_EarlyStart = IfcDateTime::createObjectFromStepData( args[7] );
	m_EarlyFinish = IfcDateTime::createObjectFromStepData( args[8] );
	m_LateStart = IfcDateTime::createObjectFromStepData( args[9] );
	m_LateFinish = IfcDateTime::createObjectFromStepData( args[10] );
	m_FreeFloat = IfcDuration::createObjectFromStepData( args[11] );
	m_TotalFloat = IfcDuration::createObjectFromStepData( args[12] );
	if( boost::iequals( args[13], L".F." ) ) { m_IsCritical = false; }
	else if( boost::iequals( args[13], L".T." ) ) { m_IsCritical = true; }
	m_StatusTime = IfcDateTime::createObjectFromStepData( args[14] );
	m_ActualDuration = IfcDuration::createObjectFromStepData( args[15] );
	m_ActualStart = IfcDateTime::createObjectFromStepData( args[16] );
	m_ActualFinish = IfcDateTime::createObjectFromStepData( args[17] );
	m_RemainingTime = IfcDuration::createObjectFromStepData( args[18] );
	m_Completion = IfcPositiveRatioMeasure::createObjectFromStepData( args[19] );
}
void IfcTaskTime::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSchedulingTime::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "DurationType", m_DurationType ) );
	vec_attributes.push_back( std::make_pair( "ScheduleDuration", m_ScheduleDuration ) );
	vec_attributes.push_back( std::make_pair( "ScheduleStart", m_ScheduleStart ) );
	vec_attributes.push_back( std::make_pair( "ScheduleFinish", m_ScheduleFinish ) );
	vec_attributes.push_back( std::make_pair( "EarlyStart", m_EarlyStart ) );
	vec_attributes.push_back( std::make_pair( "EarlyFinish", m_EarlyFinish ) );
	vec_attributes.push_back( std::make_pair( "LateStart", m_LateStart ) );
	vec_attributes.push_back( std::make_pair( "LateFinish", m_LateFinish ) );
	vec_attributes.push_back( std::make_pair( "FreeFloat", m_FreeFloat ) );
	vec_attributes.push_back( std::make_pair( "TotalFloat", m_TotalFloat ) );
	vec_attributes.push_back( std::make_pair( "IsCritical", shared_ptr<IfcPPBool>( new IfcPPBool( m_IsCritical ) ) ) );
	vec_attributes.push_back( std::make_pair( "StatusTime", m_StatusTime ) );
	vec_attributes.push_back( std::make_pair( "ActualDuration", m_ActualDuration ) );
	vec_attributes.push_back( std::make_pair( "ActualStart", m_ActualStart ) );
	vec_attributes.push_back( std::make_pair( "ActualFinish", m_ActualFinish ) );
	vec_attributes.push_back( std::make_pair( "RemainingTime", m_RemainingTime ) );
	vec_attributes.push_back( std::make_pair( "Completion", m_Completion ) );
}
void IfcTaskTime::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSchedulingTime::getAttributesInverse( vec_attributes_inverse );
}
void IfcTaskTime::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSchedulingTime::setInverseCounterparts( ptr_self_entity );
}
void IfcTaskTime::unlinkSelf()
{
	IfcSchedulingTime::unlinkSelf();
}
