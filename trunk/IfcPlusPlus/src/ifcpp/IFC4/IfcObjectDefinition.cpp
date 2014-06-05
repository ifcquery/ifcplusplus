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
#include "include/IfcObjectDefinition.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcObjectDefinition 
IfcObjectDefinition::IfcObjectDefinition() {}
IfcObjectDefinition::IfcObjectDefinition( int id ) { m_id = id; }
IfcObjectDefinition::~IfcObjectDefinition() {}

// method setEntity takes over all attributes from another instance of the class
void IfcObjectDefinition::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcObjectDefinition> other = dynamic_pointer_cast<IfcObjectDefinition>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
}
void IfcObjectDefinition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCOBJECTDEFINITION" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcObjectDefinition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcObjectDefinition::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcObjectDefinition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcObjectDefinition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
}
void IfcObjectDefinition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRoot::getAttributes( vec_attributes );
}
void IfcObjectDefinition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasAssignments_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasAssignments_inverse.size(); ++i ) { HasAssignments_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssigns>( m_HasAssignments_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasAssignments_inverse", HasAssignments_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> Nests_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_Nests_inverse.size(); ++i ) { Nests_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelNests>( m_Nests_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "Nests_inverse", Nests_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsNestedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsNestedBy_inverse.size(); ++i ) { IsNestedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelNests>( m_IsNestedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsNestedBy_inverse", IsNestedBy_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasContext_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasContext_inverse.size(); ++i ) { HasContext_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDeclares>( m_HasContext_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasContext_inverse", HasContext_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsDecomposedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsDecomposedBy_inverse.size(); ++i ) { IsDecomposedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAggregates>( m_IsDecomposedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsDecomposedBy_inverse", IsDecomposedBy_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> Decomposes_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_Decomposes_inverse.size(); ++i ) { Decomposes_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAggregates>( m_Decomposes_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "Decomposes_inverse", Decomposes_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasAssociations_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasAssociations_inverse.size(); ++i ) { HasAssociations_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociates>( m_HasAssociations_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasAssociations_inverse", HasAssociations_inverse_vec_obj ) );
}
void IfcObjectDefinition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRoot::setInverseCounterparts( ptr_self_entity );
}
void IfcObjectDefinition::unlinkSelf()
{
	IfcRoot::unlinkSelf();
}
