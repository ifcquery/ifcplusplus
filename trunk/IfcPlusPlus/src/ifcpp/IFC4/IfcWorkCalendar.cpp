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
#include "include/IfcRelAssignsToControl.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"
#include "include/IfcWorkCalendar.h"
#include "include/IfcWorkCalendarTypeEnum.h"
#include "include/IfcWorkTime.h"

// ENTITY IfcWorkCalendar 
IfcWorkCalendar::IfcWorkCalendar() {}
IfcWorkCalendar::IfcWorkCalendar( int id ) { m_id = id; }
IfcWorkCalendar::~IfcWorkCalendar() {}
shared_ptr<IfcPPObject> IfcWorkCalendar::getDeepCopy()
{
	shared_ptr<IfcWorkCalendar> copy_self( new IfcWorkCalendar() );
	if( m_GlobalId ) { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy() ); }
	if( m_OwnerHistory ) { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy() ); }
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy() ); }
	for( size_t ii=0; ii<m_WorkingTimes.size(); ++ii )
	{
		auto item_ii = m_WorkingTimes[ii];
		if( item_ii )
		{
			copy_self->m_WorkingTimes.push_back( dynamic_pointer_cast<IfcWorkTime>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_ExceptionTimes.size(); ++ii )
	{
		auto item_ii = m_ExceptionTimes[ii];
		if( item_ii )
		{
			copy_self->m_ExceptionTimes.push_back( dynamic_pointer_cast<IfcWorkTime>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcWorkCalendarTypeEnum>( m_PredefinedType->getDeepCopy() ); }
	return copy_self;
}
void IfcWorkCalendar::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCWORKCALENDAR" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_WorkingTimes );
	stream << ",";
	writeEntityList( stream, m_ExceptionTimes );
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcWorkCalendar::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcWorkCalendar::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcWorkCalendar, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcWorkCalendar, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_Identification = IfcIdentifier::createObjectFromStepData( args[5] );
	readEntityReferenceList( args[6], m_WorkingTimes, map );
	readEntityReferenceList( args[7], m_ExceptionTimes, map );
	m_PredefinedType = IfcWorkCalendarTypeEnum::createObjectFromStepData( args[8] );
}
void IfcWorkCalendar::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcControl::getAttributes( vec_attributes );
	if( m_WorkingTimes.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> WorkingTimes_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_WorkingTimes.begin(), m_WorkingTimes.end(), std::back_inserter( WorkingTimes_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "WorkingTimes", WorkingTimes_vec_object ) );
	}
	if( m_ExceptionTimes.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ExceptionTimes_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ExceptionTimes.begin(), m_ExceptionTimes.end(), std::back_inserter( ExceptionTimes_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ExceptionTimes", ExceptionTimes_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcWorkCalendar::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcControl::getAttributesInverse( vec_attributes_inverse );
}
void IfcWorkCalendar::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcControl::setInverseCounterparts( ptr_self_entity );
}
void IfcWorkCalendar::unlinkSelf()
{
	IfcControl::unlinkSelf();
}
