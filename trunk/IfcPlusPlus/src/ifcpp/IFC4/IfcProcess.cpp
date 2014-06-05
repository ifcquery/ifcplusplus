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
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProcess.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProcess.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelSequence.h"
#include "include/IfcText.h"

// ENTITY IfcProcess 
IfcProcess::IfcProcess() {}
IfcProcess::IfcProcess( int id ) { m_id = id; }
IfcProcess::~IfcProcess() {}

// method setEntity takes over all attributes from another instance of the class
void IfcProcess::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcProcess> other = dynamic_pointer_cast<IfcProcess>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_Identification = other->m_Identification;
	m_LongDescription = other->m_LongDescription;
}
void IfcProcess::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROCESS" << "(";
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
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongDescription ) { m_LongDescription->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcProcess::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProcess::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcProcess, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcProcess, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_Identification = IfcIdentifier::createObjectFromStepData( args[5] );
	m_LongDescription = IfcText::createObjectFromStepData( args[6] );
}
void IfcProcess::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObject::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Identification", m_Identification ) );
	vec_attributes.push_back( std::make_pair( "LongDescription", m_LongDescription ) );
}
void IfcProcess::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> IsPredecessorTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsPredecessorTo_inverse.size(); ++i ) { IsPredecessorTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelSequence>( m_IsPredecessorTo_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsPredecessorTo_inverse", IsPredecessorTo_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsSuccessorFrom_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsSuccessorFrom_inverse.size(); ++i ) { IsSuccessorFrom_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelSequence>( m_IsSuccessorFrom_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsSuccessorFrom_inverse", IsSuccessorFrom_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> OperatesOn_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_OperatesOn_inverse.size(); ++i ) { OperatesOn_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssignsToProcess>( m_OperatesOn_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "OperatesOn_inverse", OperatesOn_inverse_vec_obj ) );
}
void IfcProcess::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObject::setInverseCounterparts( ptr_self_entity );
}
void IfcProcess::unlinkSelf()
{
	IfcObject::unlinkSelf();
}
