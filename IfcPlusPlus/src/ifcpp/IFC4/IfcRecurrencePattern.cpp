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
#include "include/IfcDayInMonthNumber.h"
#include "include/IfcDayInWeekNumber.h"
#include "include/IfcInteger.h"
#include "include/IfcMonthInYearNumber.h"
#include "include/IfcRecurrencePattern.h"
#include "include/IfcRecurrenceTypeEnum.h"
#include "include/IfcTimePeriod.h"

// ENTITY IfcRecurrencePattern 
IfcRecurrencePattern::IfcRecurrencePattern() {}
IfcRecurrencePattern::IfcRecurrencePattern( int id ) { m_id = id; }
IfcRecurrencePattern::~IfcRecurrencePattern() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRecurrencePattern::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRecurrencePattern> other = dynamic_pointer_cast<IfcRecurrencePattern>(other_entity);
	if( !other) { return; }
	m_RecurrenceType = other->m_RecurrenceType;
	m_DayComponent = other->m_DayComponent;
	m_WeekdayComponent = other->m_WeekdayComponent;
	m_MonthComponent = other->m_MonthComponent;
	m_Position = other->m_Position;
	m_Interval = other->m_Interval;
	m_Occurrences = other->m_Occurrences;
	m_TimePeriods = other->m_TimePeriods;
}
void IfcRecurrencePattern::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRECURRENCEPATTERN" << "(";
	if( m_RecurrenceType ) { m_RecurrenceType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeOfIntList( stream, m_DayComponent );
	stream << ",";
	writeTypeOfIntList( stream, m_WeekdayComponent );
	stream << ",";
	writeTypeOfIntList( stream, m_MonthComponent );
	stream << ",";
	if( m_Position ) { m_Position->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Interval ) { m_Interval->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Occurrences ) { m_Occurrences->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_TimePeriods );
	stream << ");";
}
void IfcRecurrencePattern::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRecurrencePattern::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRecurrencePattern, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcRecurrencePattern, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_RecurrenceType = IfcRecurrenceTypeEnum::createObjectFromStepData( args[0] );
	readTypeOfIntList( args[1], m_DayComponent );
	readTypeOfIntList( args[2], m_WeekdayComponent );
	readTypeOfIntList( args[3], m_MonthComponent );
	m_Position = IfcInteger::createObjectFromStepData( args[4] );
	m_Interval = IfcInteger::createObjectFromStepData( args[5] );
	m_Occurrences = IfcInteger::createObjectFromStepData( args[6] );
	readEntityReferenceList( args[7], m_TimePeriods, map );
}
void IfcRecurrencePattern::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "RecurrenceType", m_RecurrenceType ) );
	shared_ptr<IfcPPAttributeObjectVector> DayComponent_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_DayComponent.begin(), m_DayComponent.end(), std::back_inserter( DayComponent_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "DayComponent", DayComponent_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> WeekdayComponent_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_WeekdayComponent.begin(), m_WeekdayComponent.end(), std::back_inserter( WeekdayComponent_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "WeekdayComponent", WeekdayComponent_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> MonthComponent_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_MonthComponent.begin(), m_MonthComponent.end(), std::back_inserter( MonthComponent_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "MonthComponent", MonthComponent_vec_object ) );
	vec_attributes.push_back( std::make_pair( "Position", m_Position ) );
	vec_attributes.push_back( std::make_pair( "Interval", m_Interval ) );
	vec_attributes.push_back( std::make_pair( "Occurrences", m_Occurrences ) );
}
void IfcRecurrencePattern::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRecurrencePattern::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcRecurrencePattern::unlinkSelf()
{
}
