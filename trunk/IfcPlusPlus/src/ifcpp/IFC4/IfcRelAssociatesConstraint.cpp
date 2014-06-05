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
#include "include/IfcConstraint.h"
#include "include/IfcDefinitionSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssociatesConstraint.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssociatesConstraint 
IfcRelAssociatesConstraint::IfcRelAssociatesConstraint() {}
IfcRelAssociatesConstraint::IfcRelAssociatesConstraint( int id ) { m_id = id; }
IfcRelAssociatesConstraint::~IfcRelAssociatesConstraint() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssociatesConstraint::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssociatesConstraint> other = dynamic_pointer_cast<IfcRelAssociatesConstraint>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_Intent = other->m_Intent;
	m_RelatingConstraint = other->m_RelatingConstraint;
}
void IfcRelAssociatesConstraint::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSOCIATESCONSTRAINT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_RelatedObjects, true );
	stream << ",";
	if( m_Intent ) { m_Intent->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingConstraint ) { stream << "#" << m_RelatingConstraint->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssociatesConstraint::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociatesConstraint::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssociatesConstraint, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcRelAssociatesConstraint, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readSelectList( args[4], m_RelatedObjects, map );
	m_Intent = IfcLabel::createObjectFromStepData( args[5] );
	readEntityReference( args[6], m_RelatingConstraint, map );
}
void IfcRelAssociatesConstraint::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssociates::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Intent", m_Intent ) );
	vec_attributes.push_back( std::make_pair( "RelatingConstraint", m_RelatingConstraint ) );
}
void IfcRelAssociatesConstraint::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelAssociatesConstraint::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssociates::setInverseCounterparts( ptr_self_entity );
}
void IfcRelAssociatesConstraint::unlinkSelf()
{
	IfcRelAssociates::unlinkSelf();
}
