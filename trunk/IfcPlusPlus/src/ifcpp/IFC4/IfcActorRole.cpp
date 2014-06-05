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
#include "include/IfcActorRole.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcRoleEnum.h"
#include "include/IfcText.h"

// ENTITY IfcActorRole 
IfcActorRole::IfcActorRole() {}
IfcActorRole::IfcActorRole( int id ) { m_id = id; }
IfcActorRole::~IfcActorRole() {}

// method setEntity takes over all attributes from another instance of the class
void IfcActorRole::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcActorRole> other = dynamic_pointer_cast<IfcActorRole>(other_entity);
	if( !other) { return; }
	m_Role = other->m_Role;
	m_UserDefinedRole = other->m_UserDefinedRole;
	m_Description = other->m_Description;
}
void IfcActorRole::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCACTORROLE" << "(";
	if( m_Role ) { m_Role->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedRole ) { m_UserDefinedRole->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcActorRole::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcActorRole::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcActorRole, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcActorRole, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Role = IfcRoleEnum::createObjectFromStepData( args[0] );
	m_UserDefinedRole = IfcLabel::createObjectFromStepData( args[1] );
	m_Description = IfcText::createObjectFromStepData( args[2] );
}
void IfcActorRole::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Role", m_Role ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedRole", m_UserDefinedRole ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcActorRole::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasExternalReference_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasExternalReference_inverse.size(); ++i ) { HasExternalReference_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReference_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasExternalReference_inverse", HasExternalReference_inverse_vec_obj ) );
}
void IfcActorRole::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcActorRole::unlinkSelf()
{
}
