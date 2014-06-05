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
#include "include/IfcApplication.h"
#include "include/IfcChangeActionEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPersonAndOrganization.h"
#include "include/IfcStateEnum.h"
#include "include/IfcTimeStamp.h"

// ENTITY IfcOwnerHistory 
IfcOwnerHistory::IfcOwnerHistory() {}
IfcOwnerHistory::IfcOwnerHistory( int id ) { m_id = id; }
IfcOwnerHistory::~IfcOwnerHistory() {}

// method setEntity takes over all attributes from another instance of the class
void IfcOwnerHistory::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcOwnerHistory> other = dynamic_pointer_cast<IfcOwnerHistory>(other_entity);
	if( !other) { return; }
	m_OwningUser = other->m_OwningUser;
	m_OwningApplication = other->m_OwningApplication;
	m_State = other->m_State;
	m_ChangeAction = other->m_ChangeAction;
	m_LastModifiedDate = other->m_LastModifiedDate;
	m_LastModifyingUser = other->m_LastModifyingUser;
	m_LastModifyingApplication = other->m_LastModifyingApplication;
	m_CreationDate = other->m_CreationDate;
}
void IfcOwnerHistory::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCOWNERHISTORY" << "(";
	if( m_OwningUser ) { stream << "#" << m_OwningUser->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_OwningApplication ) { stream << "#" << m_OwningApplication->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_State ) { m_State->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ChangeAction ) { m_ChangeAction->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LastModifiedDate ) { m_LastModifiedDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LastModifyingUser ) { stream << "#" << m_LastModifyingUser->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_LastModifyingApplication ) { stream << "#" << m_LastModifyingApplication->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_CreationDate ) { m_CreationDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcOwnerHistory::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOwnerHistory::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcOwnerHistory, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcOwnerHistory, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_OwningUser, map );
	readEntityReference( args[1], m_OwningApplication, map );
	m_State = IfcStateEnum::createObjectFromStepData( args[2] );
	m_ChangeAction = IfcChangeActionEnum::createObjectFromStepData( args[3] );
	m_LastModifiedDate = IfcTimeStamp::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_LastModifyingUser, map );
	readEntityReference( args[6], m_LastModifyingApplication, map );
	m_CreationDate = IfcTimeStamp::createObjectFromStepData( args[7] );
}
void IfcOwnerHistory::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "OwningUser", m_OwningUser ) );
	vec_attributes.push_back( std::make_pair( "OwningApplication", m_OwningApplication ) );
	vec_attributes.push_back( std::make_pair( "State", m_State ) );
	vec_attributes.push_back( std::make_pair( "ChangeAction", m_ChangeAction ) );
	vec_attributes.push_back( std::make_pair( "LastModifiedDate", m_LastModifiedDate ) );
	vec_attributes.push_back( std::make_pair( "LastModifyingUser", m_LastModifyingUser ) );
	vec_attributes.push_back( std::make_pair( "LastModifyingApplication", m_LastModifyingApplication ) );
	vec_attributes.push_back( std::make_pair( "CreationDate", m_CreationDate ) );
}
void IfcOwnerHistory::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcOwnerHistory::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcOwnerHistory::unlinkSelf()
{
}
