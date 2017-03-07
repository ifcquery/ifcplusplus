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
#include "include/IfcBoolean.h"
#include "include/IfcDataOriginEnum.h"
#include "include/IfcDateTime.h"
#include "include/IfcDuration.h"
#include "include/IfcLabel.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcRecurrencePattern.h"
#include "include/IfcTaskDurationEnum.h"
#include "include/IfcTaskTimeRecurring.h"

// ENTITY IfcTaskTimeRecurring 
IfcTaskTimeRecurring::IfcTaskTimeRecurring() { m_entity_enum = IFCTASKTIMERECURRING; }
IfcTaskTimeRecurring::IfcTaskTimeRecurring( int id ) { m_id = id; m_entity_enum = IFCTASKTIMERECURRING; }
IfcTaskTimeRecurring::~IfcTaskTimeRecurring() {}
shared_ptr<IfcPPObject> IfcTaskTimeRecurring::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTaskTimeRecurring> copy_self( new IfcTaskTimeRecurring() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_DataOrigin ) { copy_self->m_DataOrigin = dynamic_pointer_cast<IfcDataOriginEnum>( m_DataOrigin->getDeepCopy(options) ); }
	if( m_UserDefinedDataOrigin ) { copy_self->m_UserDefinedDataOrigin = dynamic_pointer_cast<IfcLabel>( m_UserDefinedDataOrigin->getDeepCopy(options) ); }
	if( m_DurationType ) { copy_self->m_DurationType = dynamic_pointer_cast<IfcTaskDurationEnum>( m_DurationType->getDeepCopy(options) ); }
	if( m_ScheduleDuration ) { copy_self->m_ScheduleDuration = dynamic_pointer_cast<IfcDuration>( m_ScheduleDuration->getDeepCopy(options) ); }
	if( m_ScheduleStart ) { copy_self->m_ScheduleStart = dynamic_pointer_cast<IfcDateTime>( m_ScheduleStart->getDeepCopy(options) ); }
	if( m_ScheduleFinish ) { copy_self->m_ScheduleFinish = dynamic_pointer_cast<IfcDateTime>( m_ScheduleFinish->getDeepCopy(options) ); }
	if( m_EarlyStart ) { copy_self->m_EarlyStart = dynamic_pointer_cast<IfcDateTime>( m_EarlyStart->getDeepCopy(options) ); }
	if( m_EarlyFinish ) { copy_self->m_EarlyFinish = dynamic_pointer_cast<IfcDateTime>( m_EarlyFinish->getDeepCopy(options) ); }
	if( m_LateStart ) { copy_self->m_LateStart = dynamic_pointer_cast<IfcDateTime>( m_LateStart->getDeepCopy(options) ); }
	if( m_LateFinish ) { copy_self->m_LateFinish = dynamic_pointer_cast<IfcDateTime>( m_LateFinish->getDeepCopy(options) ); }
	if( m_FreeFloat ) { copy_self->m_FreeFloat = dynamic_pointer_cast<IfcDuration>( m_FreeFloat->getDeepCopy(options) ); }
	if( m_TotalFloat ) { copy_self->m_TotalFloat = dynamic_pointer_cast<IfcDuration>( m_TotalFloat->getDeepCopy(options) ); }
	if( m_IsCritical ) { copy_self->m_IsCritical = dynamic_pointer_cast<IfcBoolean>( m_IsCritical->getDeepCopy(options) ); }
	if( m_StatusTime ) { copy_self->m_StatusTime = dynamic_pointer_cast<IfcDateTime>( m_StatusTime->getDeepCopy(options) ); }
	if( m_ActualDuration ) { copy_self->m_ActualDuration = dynamic_pointer_cast<IfcDuration>( m_ActualDuration->getDeepCopy(options) ); }
	if( m_ActualStart ) { copy_self->m_ActualStart = dynamic_pointer_cast<IfcDateTime>( m_ActualStart->getDeepCopy(options) ); }
	if( m_ActualFinish ) { copy_self->m_ActualFinish = dynamic_pointer_cast<IfcDateTime>( m_ActualFinish->getDeepCopy(options) ); }
	if( m_RemainingTime ) { copy_self->m_RemainingTime = dynamic_pointer_cast<IfcDuration>( m_RemainingTime->getDeepCopy(options) ); }
	if( m_Completion ) { copy_self->m_Completion = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_Completion->getDeepCopy(options) ); }
	if( m_Recurrence ) { copy_self->m_Recurrence = dynamic_pointer_cast<IfcRecurrencePattern>( m_Recurrence->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTaskTimeRecurring::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTASKTIMERECURRING" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DurationType ) { m_DurationType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ScheduleDuration ) { m_ScheduleDuration->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ScheduleStart ) { m_ScheduleStart->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ScheduleFinish ) { m_ScheduleFinish->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_EarlyStart ) { m_EarlyStart->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_EarlyFinish ) { m_EarlyFinish->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LateStart ) { m_LateStart->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LateFinish ) { m_LateFinish->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_FreeFloat ) { m_FreeFloat->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TotalFloat ) { m_TotalFloat->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_IsCritical ) { m_IsCritical->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_StatusTime ) { m_StatusTime->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ActualDuration ) { m_ActualDuration->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ActualStart ) { m_ActualStart->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ActualFinish ) { m_ActualFinish->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RemainingTime ) { m_RemainingTime->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Completion ) { m_Completion->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Recurrence ) { stream << "#" << m_Recurrence->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcTaskTimeRecurring::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTaskTimeRecurring::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 21 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTaskTimeRecurring, expecting 21, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromSTEP( args[1] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromSTEP( args[2] );
	m_DurationType = IfcTaskDurationEnum::createObjectFromSTEP( args[3] );
	m_ScheduleDuration = IfcDuration::createObjectFromSTEP( args[4] );
	m_ScheduleStart = IfcDateTime::createObjectFromSTEP( args[5] );
	m_ScheduleFinish = IfcDateTime::createObjectFromSTEP( args[6] );
	m_EarlyStart = IfcDateTime::createObjectFromSTEP( args[7] );
	m_EarlyFinish = IfcDateTime::createObjectFromSTEP( args[8] );
	m_LateStart = IfcDateTime::createObjectFromSTEP( args[9] );
	m_LateFinish = IfcDateTime::createObjectFromSTEP( args[10] );
	m_FreeFloat = IfcDuration::createObjectFromSTEP( args[11] );
	m_TotalFloat = IfcDuration::createObjectFromSTEP( args[12] );
	m_IsCritical = IfcBoolean::createObjectFromSTEP( args[13] );
	m_StatusTime = IfcDateTime::createObjectFromSTEP( args[14] );
	m_ActualDuration = IfcDuration::createObjectFromSTEP( args[15] );
	m_ActualStart = IfcDateTime::createObjectFromSTEP( args[16] );
	m_ActualFinish = IfcDateTime::createObjectFromSTEP( args[17] );
	m_RemainingTime = IfcDuration::createObjectFromSTEP( args[18] );
	m_Completion = IfcPositiveRatioMeasure::createObjectFromSTEP( args[19] );
	readEntityReference( args[20], m_Recurrence, map );
}
void IfcTaskTimeRecurring::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTaskTime::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Recurrence", m_Recurrence ) );
}
void IfcTaskTimeRecurring::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTaskTime::getAttributesInverse( vec_attributes_inverse );
}
void IfcTaskTimeRecurring::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTaskTime::setInverseCounterparts( ptr_self_entity );
}
void IfcTaskTimeRecurring::unlinkFromInverseCounterparts()
{
	IfcTaskTime::unlinkFromInverseCounterparts();
}
