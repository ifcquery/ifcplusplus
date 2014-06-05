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
#include "include/IfcElement.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelConnectsElements.h"
#include "include/IfcRelConnectsWithRealizingElements.h"
#include "include/IfcRelContainedInSpatialStructure.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelFillsElement.h"
#include "include/IfcRelInterferesElements.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelProjectsElement.h"
#include "include/IfcRelReferencedInSpatialStructure.h"
#include "include/IfcRelSpaceBoundary.h"
#include "include/IfcRelVoidsElement.h"
#include "include/IfcText.h"

// ENTITY IfcElement 
IfcElement::IfcElement() {}
IfcElement::IfcElement( int id ) { m_id = id; }
IfcElement::~IfcElement() {}

// method setEntity takes over all attributes from another instance of the class
void IfcElement::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcElement> other = dynamic_pointer_cast<IfcElement>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_Tag = other->m_Tag;
}
void IfcElement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCELEMENT" << "(";
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
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcElement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcElement::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcElement, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcElement, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_Tag = IfcIdentifier::createObjectFromStepData( args[7] );
}
void IfcElement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProduct::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Tag", m_Tag ) );
}
void IfcElement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> FillsVoids_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_FillsVoids_inverse.size(); ++i ) { FillsVoids_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelFillsElement>( m_FillsVoids_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "FillsVoids_inverse", FillsVoids_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ConnectedTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ConnectedTo_inverse.size(); ++i ) { ConnectedTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelConnectsElements>( m_ConnectedTo_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ConnectedTo_inverse", ConnectedTo_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsInterferedByElements_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsInterferedByElements_inverse.size(); ++i ) { IsInterferedByElements_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelInterferesElements>( m_IsInterferedByElements_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsInterferedByElements_inverse", IsInterferedByElements_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> InterferesElements_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_InterferesElements_inverse.size(); ++i ) { InterferesElements_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelInterferesElements>( m_InterferesElements_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "InterferesElements_inverse", InterferesElements_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasProjections_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasProjections_inverse.size(); ++i ) { HasProjections_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelProjectsElement>( m_HasProjections_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasProjections_inverse", HasProjections_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ReferencedInStructures_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ReferencedInStructures_inverse.size(); ++i ) { ReferencedInStructures_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelReferencedInSpatialStructure>( m_ReferencedInStructures_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ReferencedInStructures_inverse", ReferencedInStructures_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> HasOpenings_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasOpenings_inverse.size(); ++i ) { HasOpenings_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelVoidsElement>( m_HasOpenings_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasOpenings_inverse", HasOpenings_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsConnectionRealization_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsConnectionRealization_inverse.size(); ++i ) { IsConnectionRealization_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelConnectsWithRealizingElements>( m_IsConnectionRealization_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsConnectionRealization_inverse", IsConnectionRealization_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ProvidesBoundaries_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ProvidesBoundaries_inverse.size(); ++i ) { ProvidesBoundaries_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelSpaceBoundary>( m_ProvidesBoundaries_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ProvidesBoundaries_inverse", ProvidesBoundaries_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ConnectedFrom_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ConnectedFrom_inverse.size(); ++i ) { ConnectedFrom_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelConnectsElements>( m_ConnectedFrom_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ConnectedFrom_inverse", ConnectedFrom_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ContainedInStructure_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ContainedInStructure_inverse.size(); ++i ) { ContainedInStructure_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelContainedInSpatialStructure>( m_ContainedInStructure_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ContainedInStructure_inverse", ContainedInStructure_inverse_vec_obj ) );
}
void IfcElement::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProduct::setInverseCounterparts( ptr_self_entity );
}
void IfcElement::unlinkSelf()
{
	IfcProduct::unlinkSelf();
}
