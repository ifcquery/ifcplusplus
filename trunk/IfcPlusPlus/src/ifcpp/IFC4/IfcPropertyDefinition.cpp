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
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertyDefinition.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcText.h"

// ENTITY IfcPropertyDefinition 
IfcPropertyDefinition::IfcPropertyDefinition() {}
IfcPropertyDefinition::IfcPropertyDefinition( int id ) { m_id = id; }
IfcPropertyDefinition::~IfcPropertyDefinition() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertyDefinition::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertyDefinition> other = dynamic_pointer_cast<IfcPropertyDefinition>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
}
void IfcPropertyDefinition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYDEFINITION" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertyDefinition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyDefinition::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertyDefinition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcPropertyDefinition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
}
void IfcPropertyDefinition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRoot::getAttributes( vec_attributes );
}
void IfcPropertyDefinition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasContext_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasContext_inverse.size(); ++i ) { HasContext_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDeclares>( m_HasContext_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasContext_inverse", HasContext_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasAssociations_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasAssociations_inverse.size(); ++i ) { HasAssociations_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociates>( m_HasAssociations_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasAssociations_inverse", HasAssociations_inverse_vec_obj ) );
}
void IfcPropertyDefinition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRoot::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyDefinition::unlinkSelf()
{
	IfcRoot::unlinkSelf();
}
