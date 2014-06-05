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
#include "include/IfcBoundaryCondition.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelConnectsStructuralActivity.h"
#include "include/IfcRelConnectsStructuralMember.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcStructuralConnection.h"
#include "include/IfcText.h"

// ENTITY IfcStructuralConnection 
IfcStructuralConnection::IfcStructuralConnection() {}
IfcStructuralConnection::IfcStructuralConnection( int id ) { m_id = id; }
IfcStructuralConnection::~IfcStructuralConnection() {}

// method setEntity takes over all attributes from another instance of the class
void IfcStructuralConnection::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcStructuralConnection> other = dynamic_pointer_cast<IfcStructuralConnection>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_AppliedCondition = other->m_AppliedCondition;
}
void IfcStructuralConnection::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSTRUCTURALCONNECTION" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectPlacement ) { stream << "#" << m_ObjectPlacement->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Representation ) { stream << "#" << m_Representation->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AppliedCondition ) { stream << "#" << m_AppliedCondition->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralConnection::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralConnection::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralConnection, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcStructuralConnection, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	readEntityReference( args[7], m_AppliedCondition, map );
}
void IfcStructuralConnection::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "AppliedCondition", m_AppliedCondition ) );
}
void IfcStructuralConnection::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> ConnectsStructuralMembers_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ConnectsStructuralMembers_inverse.size(); ++i ) { ConnectsStructuralMembers_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelConnectsStructuralMember>( m_ConnectsStructuralMembers_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ConnectsStructuralMembers_inverse", ConnectsStructuralMembers_inverse_vec_obj ) );
}
void IfcStructuralConnection::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralItem::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralConnection::unlinkSelf()
{
	IfcStructuralItem::unlinkSelf();
}
