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
#include "include/IfcResourceTime.h"

// ENTITY IfcResourceTime 
IfcResourceTime::IfcResourceTime() {}
IfcResourceTime::IfcResourceTime( int id ) { m_id = id; }
IfcResourceTime::~IfcResourceTime() {}
shared_ptr<IfcPPObject> IfcResourceTime::getDeepCopy()
{
	shared_ptr<IfcResourceTime> copy_self( new IfcResourceTime() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_DataOrigin ) { copy_self->m_DataOrigin = dynamic_pointer_cast<IfcDataOriginEnum>( m_DataOrigin->getDeepCopy() ); }
	if( m_UserDefinedDataOrigin ) { copy_self->m_UserDefinedDataOrigin = dynamic_pointer_cast<IfcLabel>( m_UserDefinedDataOrigin->getDeepCopy() ); }
	if( m_ScheduleWork ) { copy_self->m_ScheduleWork = dynamic_pointer_cast<IfcDuration>( m_ScheduleWork->getDeepCopy() ); }
	if( m_ScheduleUsage ) { copy_self->m_ScheduleUsage = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_ScheduleUsage->getDeepCopy() ); }
	if( m_ScheduleStart ) { copy_self->m_ScheduleStart = dynamic_pointer_cast<IfcDateTime>( m_ScheduleStart->getDeepCopy() ); }
	if( m_ScheduleFinish ) { copy_self->m_ScheduleFinish = dynamic_pointer_cast<IfcDateTime>( m_ScheduleFinish->getDeepCopy() ); }
	if( m_ScheduleContour ) { copy_self->m_ScheduleContour = dynamic_pointer_cast<IfcLabel>( m_ScheduleContour->getDeepCopy() ); }
	if( m_LevelingDelay ) { copy_self->m_LevelingDelay = dynamic_pointer_cast<IfcDuration>( m_LevelingDelay->getDeepCopy() ); }
	if( m_IsOverAllocated ) { copy_self->m_IsOverAllocated = m_IsOverAllocated; }
	if( m_StatusTime ) { copy_self->m_StatusTime = dynamic_pointer_cast<IfcDateTime>( m_StatusTime->getDeepCopy() ); }
	if( m_ActualWork ) { copy_self->m_ActualWork = dynamic_pointer_cast<IfcDuration>( m_ActualWork->getDeepCopy() ); }
	if( m_ActualUsage ) { copy_self->m_ActualUsage = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_ActualUsage->getDeepCopy() ); }
	if( m_ActualStart ) { copy_self->m_ActualStart = dynamic_pointer_cast<IfcDateTime>( m_ActualStart->getDeepCopy() ); }
	if( m_ActualFinish ) { copy_self->m_ActualFinish = dynamic_pointer_cast<IfcDateTime>( m_ActualFinish->getDeepCopy() ); }
	if( m_RemainingWork ) { copy_self->m_RemainingWork = dynamic_pointer_cast<IfcDuration>( m_RemainingWork->getDeepCopy() ); }
	if( m_RemainingUsage ) { copy_self->m_RemainingUsage = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_RemainingUsage->getDeepCopy() ); }
	if( m_Completion ) { copy_self->m_Completion = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_Completion->getDeepCopy() ); }
	return copy_self;
}
void IfcResourceTime::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRESOURCETIME" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ScheduleWork ) { m_ScheduleWork->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ScheduleUsage ) { m_ScheduleUsage->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ScheduleStart ) { m_ScheduleStart->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ScheduleFinish ) { m_ScheduleFinish->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ScheduleContour ) { m_ScheduleContour->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LevelingDelay ) { m_LevelingDelay->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_IsOverAllocated == false ) { stream << ".F."; }
	else if( m_IsOverAllocated == true ) { stream << ".T."; }
	stream << ",";
	if( m_StatusTime ) { m_StatusTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActualWork ) { m_ActualWork->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActualUsage ) { m_ActualUsage->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActualStart ) { m_ActualStart->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActualFinish ) { m_ActualFinish->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RemainingWork ) { m_RemainingWork->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RemainingUsage ) { m_RemainingUsage->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Completion ) { m_Completion->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcResourceTime::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcResourceTime::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<18 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcResourceTime, expecting 18, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>18 ){ std::cout << "Wrong parameter count for entity IfcResourceTime, expecting 18, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromStepData( args[1] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromStepData( args[2] );
	m_ScheduleWork = IfcDuration::createObjectFromStepData( args[3] );
	m_ScheduleUsage = IfcPositiveRatioMeasure::createObjectFromStepData( args[4] );
	m_ScheduleStart = IfcDateTime::createObjectFromStepData( args[5] );
	m_ScheduleFinish = IfcDateTime::createObjectFromStepData( args[6] );
	m_ScheduleContour = IfcLabel::createObjectFromStepData( args[7] );
	m_LevelingDelay = IfcDuration::createObjectFromStepData( args[8] );
	if( boost::iequals( args[9], L".F." ) ) { m_IsOverAllocated = false; }
	else if( boost::iequals( args[9], L".T." ) ) { m_IsOverAllocated = true; }
	m_StatusTime = IfcDateTime::createObjectFromStepData( args[10] );
	m_ActualWork = IfcDuration::createObjectFromStepData( args[11] );
	m_ActualUsage = IfcPositiveRatioMeasure::createObjectFromStepData( args[12] );
	m_ActualStart = IfcDateTime::createObjectFromStepData( args[13] );
	m_ActualFinish = IfcDateTime::createObjectFromStepData( args[14] );
	m_RemainingWork = IfcDuration::createObjectFromStepData( args[15] );
	m_RemainingUsage = IfcPositiveRatioMeasure::createObjectFromStepData( args[16] );
	m_Completion = IfcPositiveRatioMeasure::createObjectFromStepData( args[17] );
}
void IfcResourceTime::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSchedulingTime::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ScheduleWork", m_ScheduleWork ) );
	vec_attributes.push_back( std::make_pair( "ScheduleUsage", m_ScheduleUsage ) );
	vec_attributes.push_back( std::make_pair( "ScheduleStart", m_ScheduleStart ) );
	vec_attributes.push_back( std::make_pair( "ScheduleFinish", m_ScheduleFinish ) );
	vec_attributes.push_back( std::make_pair( "ScheduleContour", m_ScheduleContour ) );
	vec_attributes.push_back( std::make_pair( "LevelingDelay", m_LevelingDelay ) );
	vec_attributes.push_back( std::make_pair( "IsOverAllocated", shared_ptr<IfcPPBool>( new IfcPPBool( m_IsOverAllocated ) ) ) );
	vec_attributes.push_back( std::make_pair( "StatusTime", m_StatusTime ) );
	vec_attributes.push_back( std::make_pair( "ActualWork", m_ActualWork ) );
	vec_attributes.push_back( std::make_pair( "ActualUsage", m_ActualUsage ) );
	vec_attributes.push_back( std::make_pair( "ActualStart", m_ActualStart ) );
	vec_attributes.push_back( std::make_pair( "ActualFinish", m_ActualFinish ) );
	vec_attributes.push_back( std::make_pair( "RemainingWork", m_RemainingWork ) );
	vec_attributes.push_back( std::make_pair( "RemainingUsage", m_RemainingUsage ) );
	vec_attributes.push_back( std::make_pair( "Completion", m_Completion ) );
}
void IfcResourceTime::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSchedulingTime::getAttributesInverse( vec_attributes_inverse );
}
void IfcResourceTime::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSchedulingTime::setInverseCounterparts( ptr_self_entity );
}
void IfcResourceTime::unlinkSelf()
{
	IfcSchedulingTime::unlinkSelf();
}
