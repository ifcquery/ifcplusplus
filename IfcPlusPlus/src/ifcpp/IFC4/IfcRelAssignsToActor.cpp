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
#include "include/IfcActor.h"
#include "include/IfcActorRole.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcObjectTypeEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssignsToActor.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssignsToActor 
IfcRelAssignsToActor::IfcRelAssignsToActor() {}
IfcRelAssignsToActor::IfcRelAssignsToActor( int id ) { m_id = id; }
IfcRelAssignsToActor::~IfcRelAssignsToActor() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssignsToActor::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssignsToActor> other = dynamic_pointer_cast<IfcRelAssignsToActor>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatedObjectsType = other->m_RelatedObjectsType;
	m_RelatingActor = other->m_RelatingActor;
	m_ActingRole = other->m_ActingRole;
}
void IfcRelAssignsToActor::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSIGNSTOACTOR" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedObjects );
	stream << ",";
	if( m_RelatedObjectsType ) { m_RelatedObjectsType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingActor ) { stream << "#" << m_RelatingActor->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_ActingRole ) { stream << "#" << m_ActingRole->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssignsToActor::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssignsToActor::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssignsToActor, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcRelAssignsToActor, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatedObjectsType = IfcObjectTypeEnum::createObjectFromStepData( args[5] );
	readEntityReference( args[6], m_RelatingActor, map );
	readEntityReference( args[7], m_ActingRole, map );
}
void IfcRelAssignsToActor::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssigns::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingActor", m_RelatingActor ) );
	vec_attributes.push_back( std::make_pair( "ActingRole", m_ActingRole ) );
}
void IfcRelAssignsToActor::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelAssignsToActor::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssigns::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssignsToActor> ptr_self = dynamic_pointer_cast<IfcRelAssignsToActor>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssignsToActor::setInverseCounterparts: type mismatch" ); }
	if( m_RelatingActor )
	{
		m_RelatingActor->m_IsActingUpon_inverse.push_back( ptr_self );
	}
}
void IfcRelAssignsToActor::unlinkSelf()
{
	IfcRelAssigns::unlinkSelf();
	if( m_RelatingActor )
	{
		std::vector<weak_ptr<IfcRelAssignsToActor> >& IsActingUpon_inverse = m_RelatingActor->m_IsActingUpon_inverse;
		std::vector<weak_ptr<IfcRelAssignsToActor> >::iterator it_IsActingUpon_inverse;
		for( it_IsActingUpon_inverse = IsActingUpon_inverse.begin(); it_IsActingUpon_inverse != IsActingUpon_inverse.end(); ++it_IsActingUpon_inverse)
		{
			shared_ptr<IfcRelAssignsToActor> self_candidate( *it_IsActingUpon_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				IsActingUpon_inverse.erase( it_IsActingUpon_inverse );
				break;
			}
		}
	}
}
