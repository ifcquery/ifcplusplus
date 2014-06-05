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
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelContainedInSpatialStructure.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelReferencedInSpatialStructure.h"
#include "include/IfcRelServicesBuildings.h"
#include "include/IfcSpatialElement.h"
#include "include/IfcText.h"

// ENTITY IfcSpatialElement 
IfcSpatialElement::IfcSpatialElement() {}
IfcSpatialElement::IfcSpatialElement( int id ) { m_id = id; }
IfcSpatialElement::~IfcSpatialElement() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSpatialElement::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSpatialElement> other = dynamic_pointer_cast<IfcSpatialElement>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_LongName = other->m_LongName;
}
void IfcSpatialElement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSPATIALELEMENT" << "(";
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
	if( m_LongName ) { m_LongName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSpatialElement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSpatialElement::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSpatialElement, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcSpatialElement, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_LongName = IfcLabel::createObjectFromStepData( args[7] );
}
void IfcSpatialElement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProduct::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LongName", m_LongName ) );
}
void IfcSpatialElement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> ContainsElements_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ContainsElements_inverse.size(); ++i ) { ContainsElements_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelContainedInSpatialStructure>( m_ContainsElements_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ContainsElements_inverse", ContainsElements_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ServicedBySystems_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ServicedBySystems_inverse.size(); ++i ) { ServicedBySystems_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelServicesBuildings>( m_ServicedBySystems_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ServicedBySystems_inverse", ServicedBySystems_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ReferencesElements_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ReferencesElements_inverse.size(); ++i ) { ReferencesElements_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelReferencedInSpatialStructure>( m_ReferencesElements_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ReferencesElements_inverse", ReferencesElements_inverse_vec_obj ) );
}
void IfcSpatialElement::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProduct::setInverseCounterparts( ptr_self_entity );
}
void IfcSpatialElement::unlinkSelf()
{
	IfcProduct::unlinkSelf();
}
