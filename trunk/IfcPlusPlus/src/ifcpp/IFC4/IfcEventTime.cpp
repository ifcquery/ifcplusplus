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
#include "include/IfcEventTime.h"
#include "include/IfcLabel.h"

// ENTITY IfcEventTime 
IfcEventTime::IfcEventTime() {}
IfcEventTime::IfcEventTime( int id ) { m_id = id; }
IfcEventTime::~IfcEventTime() {}

// method setEntity takes over all attributes from another instance of the class
void IfcEventTime::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcEventTime> other = dynamic_pointer_cast<IfcEventTime>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_DataOrigin = other->m_DataOrigin;
	m_UserDefinedDataOrigin = other->m_UserDefinedDataOrigin;
	m_ActualDate = other->m_ActualDate;
	m_EarlyDate = other->m_EarlyDate;
	m_LateDate = other->m_LateDate;
	m_ScheduleDate = other->m_ScheduleDate;
}
void IfcEventTime::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCEVENTTIME" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActualDate ) { m_ActualDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EarlyDate ) { m_EarlyDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LateDate ) { m_LateDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ScheduleDate ) { m_ScheduleDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcEventTime::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcEventTime::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcEventTime, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcEventTime, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromStepData( args[1] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromStepData( args[2] );
	m_ActualDate = IfcDateTime::createObjectFromStepData( args[3] );
	m_EarlyDate = IfcDateTime::createObjectFromStepData( args[4] );
	m_LateDate = IfcDateTime::createObjectFromStepData( args[5] );
	m_ScheduleDate = IfcDateTime::createObjectFromStepData( args[6] );
}
void IfcEventTime::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSchedulingTime::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ActualDate", m_ActualDate ) );
	vec_attributes.push_back( std::make_pair( "EarlyDate", m_EarlyDate ) );
	vec_attributes.push_back( std::make_pair( "LateDate", m_LateDate ) );
	vec_attributes.push_back( std::make_pair( "ScheduleDate", m_ScheduleDate ) );
}
void IfcEventTime::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcEventTime::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSchedulingTime::setInverseCounterparts( ptr_self_entity );
}
void IfcEventTime::unlinkSelf()
{
	IfcSchedulingTime::unlinkSelf();
}
