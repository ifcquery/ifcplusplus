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
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPort.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelConnectsPortToElement.h"
#include "include/IfcRelConnectsPorts.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcPort 
IfcPort::IfcPort() {}
IfcPort::IfcPort( int id ) { m_id = id; }
IfcPort::~IfcPort() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPort::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPort> other = dynamic_pointer_cast<IfcPort>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
}
void IfcPort::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPORT" << "(";
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
	stream << ");";
}
void IfcPort::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPort::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPort, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcPort, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
}
void IfcPort::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProduct::getAttributes( vec_attributes );
}
void IfcPort::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> ContainedIn_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ContainedIn_inverse.size(); ++i ) { ContainedIn_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelConnectsPortToElement>( m_ContainedIn_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ContainedIn_inverse", ContainedIn_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ConnectedFrom_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ConnectedFrom_inverse.size(); ++i ) { ConnectedFrom_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelConnectsPorts>( m_ConnectedFrom_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ConnectedFrom_inverse", ConnectedFrom_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ConnectedTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ConnectedTo_inverse.size(); ++i ) { ConnectedTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelConnectsPorts>( m_ConnectedTo_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ConnectedTo_inverse", ConnectedTo_inverse_vec_obj ) );
}
void IfcPort::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProduct::setInverseCounterparts( ptr_self_entity );
}
void IfcPort::unlinkSelf()
{
	IfcProduct::unlinkSelf();
}
