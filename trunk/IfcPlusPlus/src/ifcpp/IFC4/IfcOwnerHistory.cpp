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
#include "ifcpp/model/IfcPPGuid.h"
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
shared_ptr<IfcPPObject> IfcOwnerHistory::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcOwnerHistory> copy_self( new IfcOwnerHistory() );
	if( m_OwningUser ) { copy_self->m_OwningUser = dynamic_pointer_cast<IfcPersonAndOrganization>( m_OwningUser->getDeepCopy(options) ); }
	if( m_OwningApplication ) { copy_self->m_OwningApplication = dynamic_pointer_cast<IfcApplication>( m_OwningApplication->getDeepCopy(options) ); }
	if( m_State ) { copy_self->m_State = dynamic_pointer_cast<IfcStateEnum>( m_State->getDeepCopy(options) ); }
	if( m_ChangeAction ) { copy_self->m_ChangeAction = dynamic_pointer_cast<IfcChangeActionEnum>( m_ChangeAction->getDeepCopy(options) ); }
	if( m_LastModifiedDate ) { copy_self->m_LastModifiedDate = dynamic_pointer_cast<IfcTimeStamp>( m_LastModifiedDate->getDeepCopy(options) ); }
	if( m_LastModifyingUser ) { copy_self->m_LastModifyingUser = dynamic_pointer_cast<IfcPersonAndOrganization>( m_LastModifyingUser->getDeepCopy(options) ); }
	if( m_LastModifyingApplication ) { copy_self->m_LastModifyingApplication = dynamic_pointer_cast<IfcApplication>( m_LastModifyingApplication->getDeepCopy(options) ); }
	if( m_CreationDate ) { copy_self->m_CreationDate = dynamic_pointer_cast<IfcTimeStamp>( m_CreationDate->getDeepCopy(options) ); }
	return copy_self;
}
void IfcOwnerHistory::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCOWNERHISTORY" << "(";
	if( m_OwningUser ) { stream << "#" << m_OwningUser->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_OwningApplication ) { stream << "#" << m_OwningApplication->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_State ) { m_State->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ChangeAction ) { m_ChangeAction->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LastModifiedDate ) { m_LastModifiedDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LastModifyingUser ) { stream << "#" << m_LastModifyingUser->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_LastModifyingApplication ) { stream << "#" << m_LastModifyingApplication->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_CreationDate ) { m_CreationDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcOwnerHistory::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOwnerHistory::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcOwnerHistory, expecting 8, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_OwningUser, map );
	readEntityReference( args[1], m_OwningApplication, map );
	m_State = IfcStateEnum::createObjectFromSTEP( args[2] );
	m_ChangeAction = IfcChangeActionEnum::createObjectFromSTEP( args[3] );
	m_LastModifiedDate = IfcTimeStamp::createObjectFromSTEP( args[4] );
	readEntityReference( args[5], m_LastModifyingUser, map );
	readEntityReference( args[6], m_LastModifyingApplication, map );
	m_CreationDate = IfcTimeStamp::createObjectFromSTEP( args[7] );
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
