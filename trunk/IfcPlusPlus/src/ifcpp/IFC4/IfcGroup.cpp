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
#include "include/IfcGroup.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToGroup.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcGroup 
IfcGroup::IfcGroup() {}
IfcGroup::IfcGroup( int id ) { m_id = id; }
IfcGroup::~IfcGroup() {}

// method setEntity takes over all attributes from another instance of the class
void IfcGroup::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcGroup> other = dynamic_pointer_cast<IfcGroup>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
}
void IfcGroup::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCGROUP" << "(";
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
void IfcGroup::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGroup::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcGroup, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcGroup, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
}
void IfcGroup::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObject::getAttributes( vec_attributes );
}
void IfcGroup::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> IsGroupedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsGroupedBy_inverse.size(); ++i ) { IsGroupedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssignsToGroup>( m_IsGroupedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsGroupedBy_inverse", IsGroupedBy_inverse_vec_obj ) );
}
void IfcGroup::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObject::setInverseCounterparts( ptr_self_entity );
}
void IfcGroup::unlinkSelf()
{
	IfcObject::unlinkSelf();
}
