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
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByTemplate.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"

// ENTITY IfcPropertySetDefinition 
IfcPropertySetDefinition::IfcPropertySetDefinition() {}
IfcPropertySetDefinition::IfcPropertySetDefinition( int id ) { m_id = id; }
IfcPropertySetDefinition::~IfcPropertySetDefinition() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertySetDefinition::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertySetDefinition> other = dynamic_pointer_cast<IfcPropertySetDefinition>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
}
void IfcPropertySetDefinition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYSETDEFINITION" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertySetDefinition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertySetDefinition::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertySetDefinition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcPropertySetDefinition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
}
void IfcPropertySetDefinition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyDefinition::getAttributes( vec_attributes );
}
void IfcPropertySetDefinition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> DefinesType_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_DefinesType_inverse.size(); ++i ) { DefinesType_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcTypeObject>( m_DefinesType_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "DefinesType_inverse", DefinesType_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsDefinedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsDefinedBy_inverse.size(); ++i ) { IsDefinedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByTemplate>( m_IsDefinedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsDefinedBy_inverse", IsDefinedBy_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> DefinesOccurrence_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_DefinesOccurrence_inverse.size(); ++i ) { DefinesOccurrence_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByProperties>( m_DefinesOccurrence_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "DefinesOccurrence_inverse", DefinesOccurrence_inverse_vec_obj ) );
}
void IfcPropertySetDefinition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertySetDefinition::unlinkSelf()
{
	IfcPropertyDefinition::unlinkSelf();
}
