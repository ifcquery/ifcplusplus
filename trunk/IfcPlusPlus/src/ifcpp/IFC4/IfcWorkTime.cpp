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
#include "include/IfcDate.h"
#include "include/IfcLabel.h"
#include "include/IfcRecurrencePattern.h"
#include "include/IfcWorkTime.h"

// ENTITY IfcWorkTime 
IfcWorkTime::IfcWorkTime() {}
IfcWorkTime::IfcWorkTime( int id ) { m_id = id; }
IfcWorkTime::~IfcWorkTime() {}
shared_ptr<IfcPPObject> IfcWorkTime::getDeepCopy()
{
	shared_ptr<IfcWorkTime> copy_self( new IfcWorkTime() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_DataOrigin ) { copy_self->m_DataOrigin = dynamic_pointer_cast<IfcDataOriginEnum>( m_DataOrigin->getDeepCopy() ); }
	if( m_UserDefinedDataOrigin ) { copy_self->m_UserDefinedDataOrigin = dynamic_pointer_cast<IfcLabel>( m_UserDefinedDataOrigin->getDeepCopy() ); }
	if( m_RecurrencePattern ) { copy_self->m_RecurrencePattern = dynamic_pointer_cast<IfcRecurrencePattern>( m_RecurrencePattern->getDeepCopy() ); }
	if( m_Start ) { copy_self->m_Start = dynamic_pointer_cast<IfcDate>( m_Start->getDeepCopy() ); }
	if( m_Finish ) { copy_self->m_Finish = dynamic_pointer_cast<IfcDate>( m_Finish->getDeepCopy() ); }
	return copy_self;
}
void IfcWorkTime::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCWORKTIME" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RecurrencePattern ) { stream << "#" << m_RecurrencePattern->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Start ) { m_Start->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Finish ) { m_Finish->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcWorkTime::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcWorkTime::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcWorkTime, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcWorkTime, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromStepData( args[1] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromStepData( args[2] );
	readEntityReference( args[3], m_RecurrencePattern, map );
	m_Start = IfcDate::createObjectFromStepData( args[4] );
	m_Finish = IfcDate::createObjectFromStepData( args[5] );
}
void IfcWorkTime::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSchedulingTime::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RecurrencePattern", m_RecurrencePattern ) );
	vec_attributes.push_back( std::make_pair( "Start", m_Start ) );
	vec_attributes.push_back( std::make_pair( "Finish", m_Finish ) );
}
void IfcWorkTime::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSchedulingTime::getAttributesInverse( vec_attributes_inverse );
}
void IfcWorkTime::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSchedulingTime::setInverseCounterparts( ptr_self_entity );
}
void IfcWorkTime::unlinkSelf()
{
	IfcSchedulingTime::unlinkSelf();
}
