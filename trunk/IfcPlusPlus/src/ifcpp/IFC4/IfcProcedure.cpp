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
#include "include/IfcProcedure.h"
#include "include/IfcProcedureTypeEnum.h"
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

// ENTITY IfcProcedure 
IfcProcedure::IfcProcedure() {}
IfcProcedure::IfcProcedure( int id ) { m_id = id; }
IfcProcedure::~IfcProcedure() {}

// method setEntity takes over all attributes from another instance of the class
void IfcProcedure::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcProcedure> other = dynamic_pointer_cast<IfcProcedure>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_Identification = other->m_Identification;
	m_LongDescription = other->m_LongDescription;
	m_PredefinedType = other->m_PredefinedType;
}
void IfcProcedure::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROCEDURE" << "(";
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
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcProcedure::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProcedure::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcProcedure, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcProcedure, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_Identification = IfcIdentifier::createObjectFromStepData( args[5] );
	m_LongDescription = IfcText::createObjectFromStepData( args[6] );
	m_PredefinedType = IfcProcedureTypeEnum::createObjectFromStepData( args[7] );
}
void IfcProcedure::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProcess::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcProcedure::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcProcedure::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProcess::setInverseCounterparts( ptr_self_entity );
}
void IfcProcedure::unlinkSelf()
{
	IfcProcess::unlinkSelf();
}
