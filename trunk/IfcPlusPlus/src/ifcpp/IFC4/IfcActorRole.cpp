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
shared_ptr<IfcPPObject> IfcActorRole::getDeepCopy()
{
	shared_ptr<IfcActorRole> copy_self( new IfcActorRole() );
	if( m_Role ) { copy_self->m_Role = dynamic_pointer_cast<IfcRoleEnum>( m_Role->getDeepCopy() ); }
	if( m_UserDefinedRole ) { copy_self->m_UserDefinedRole = dynamic_pointer_cast<IfcLabel>( m_UserDefinedRole->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	return copy_self;
}
void IfcActorRole::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCACTORROLE" << "(";
	if( m_Role ) { m_Role->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedRole ) { m_UserDefinedRole->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcActorRole::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcActorRole::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( m_HasExternalReference_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasExternalReference_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasExternalReference_inverse.size(); ++i )
		{
			if( !m_HasExternalReference_inverse[i].expired() )
			{
				HasExternalReference_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReference_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasExternalReference_inverse", HasExternalReference_inverse_vec_obj ) );
	}
}
void IfcActorRole::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcActorRole::unlinkSelf()
{
}
