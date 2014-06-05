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
#include "include/IfcObject.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcObject 
IfcObject::IfcObject() {}
IfcObject::IfcObject( int id ) { m_id = id; }
IfcObject::~IfcObject() {}

// method setEntity takes over all attributes from another instance of the class
void IfcObject::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcObject> other = dynamic_pointer_cast<IfcObject>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
}
void IfcObject::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCOBJECT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcObject::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcObject::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcObject, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcObject, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
}
void IfcObject::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObjectDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ObjectType", m_ObjectType ) );
}
void IfcObject::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> IsDeclaredBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsDeclaredBy_inverse.size(); ++i ) { IsDeclaredBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByObject>( m_IsDeclaredBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsDeclaredBy_inverse", IsDeclaredBy_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> Declares_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_Declares_inverse.size(); ++i ) { Declares_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByObject>( m_Declares_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "Declares_inverse", Declares_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsTypedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsTypedBy_inverse.size(); ++i ) { IsTypedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByType>( m_IsTypedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsTypedBy_inverse", IsTypedBy_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsDefinedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsDefinedBy_inverse.size(); ++i ) { IsDefinedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByProperties>( m_IsDefinedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsDefinedBy_inverse", IsDefinedBy_inverse_vec_obj ) );
}
void IfcObject::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObjectDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcObject::unlinkSelf()
{
	IfcObjectDefinition::unlinkSelf();
}
