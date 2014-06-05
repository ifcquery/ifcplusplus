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
#include "include/IfcContext.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelNests.h"
#include "include/IfcRepresentationContext.h"
#include "include/IfcText.h"
#include "include/IfcUnitAssignment.h"

// ENTITY IfcContext 
IfcContext::IfcContext() {}
IfcContext::IfcContext( int id ) { m_id = id; }
IfcContext::~IfcContext() {}

// method setEntity takes over all attributes from another instance of the class
void IfcContext::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcContext> other = dynamic_pointer_cast<IfcContext>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_LongName = other->m_LongName;
	m_Phase = other->m_Phase;
	m_RepresentationContexts = other->m_RepresentationContexts;
	m_UnitsInContext = other->m_UnitsInContext;
}
void IfcContext::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCONTEXT" << "(";
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
	if( m_LongName ) { m_LongName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Phase ) { m_Phase->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RepresentationContexts );
	stream << ",";
	if( m_UnitsInContext ) { stream << "#" << m_UnitsInContext->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcContext::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcContext::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcContext, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcContext, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_LongName = IfcLabel::createObjectFromStepData( args[5] );
	m_Phase = IfcLabel::createObjectFromStepData( args[6] );
	readEntityReferenceList( args[7], m_RepresentationContexts, map );
	readEntityReference( args[8], m_UnitsInContext, map );
}
void IfcContext::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObjectDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ObjectType", m_ObjectType ) );
	vec_attributes.push_back( std::make_pair( "LongName", m_LongName ) );
	vec_attributes.push_back( std::make_pair( "Phase", m_Phase ) );
	vec_attributes.push_back( std::make_pair( "UnitsInContext", m_UnitsInContext ) );
}
void IfcContext::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> IsDefinedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsDefinedBy_inverse.size(); ++i ) { IsDefinedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByProperties>( m_IsDefinedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsDefinedBy_inverse", IsDefinedBy_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> Declares_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_Declares_inverse.size(); ++i ) { Declares_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDeclares>( m_Declares_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "Declares_inverse", Declares_inverse_vec_obj ) );
}
void IfcContext::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObjectDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcContext::unlinkSelf()
{
	IfcObjectDefinition::unlinkSelf();
}
