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
#include "include/IfcActorSelect.h"
#include "include/IfcConstraint.h"
#include "include/IfcConstraintEnum.h"
#include "include/IfcDateTime.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcLogicalOperatorEnum.h"
#include "include/IfcObjective.h"
#include "include/IfcObjectiveEnum.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcText.h"

// ENTITY IfcObjective 
IfcObjective::IfcObjective() {}
IfcObjective::IfcObjective( int id ) { m_id = id; }
IfcObjective::~IfcObjective() {}

// method setEntity takes over all attributes from another instance of the class
void IfcObjective::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcObjective> other = dynamic_pointer_cast<IfcObjective>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ConstraintGrade = other->m_ConstraintGrade;
	m_ConstraintSource = other->m_ConstraintSource;
	m_CreatingActor = other->m_CreatingActor;
	m_CreationTime = other->m_CreationTime;
	m_UserDefinedGrade = other->m_UserDefinedGrade;
	m_BenchmarkValues = other->m_BenchmarkValues;
	m_LogicalAggregator = other->m_LogicalAggregator;
	m_ObjectiveQualifier = other->m_ObjectiveQualifier;
	m_UserDefinedQualifier = other->m_UserDefinedQualifier;
}
void IfcObjective::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCOBJECTIVE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ConstraintGrade ) { m_ConstraintGrade->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ConstraintSource ) { m_ConstraintSource->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CreatingActor ) { m_CreatingActor->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_CreationTime ) { m_CreationTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedGrade ) { m_UserDefinedGrade->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_BenchmarkValues );
	stream << ",";
	if( m_LogicalAggregator ) { m_LogicalAggregator->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectiveQualifier ) { m_ObjectiveQualifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedQualifier ) { m_UserDefinedQualifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcObjective::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcObjective::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<11 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcObjective, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>11 ){ std::cout << "Wrong parameter count for entity IfcObjective, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_ConstraintGrade = IfcConstraintEnum::createObjectFromStepData( args[2] );
	m_ConstraintSource = IfcLabel::createObjectFromStepData( args[3] );
	m_CreatingActor = IfcActorSelect::createObjectFromStepData( args[4], map );
	m_CreationTime = IfcDateTime::createObjectFromStepData( args[5] );
	m_UserDefinedGrade = IfcLabel::createObjectFromStepData( args[6] );
	readEntityReferenceList( args[7], m_BenchmarkValues, map );
	m_LogicalAggregator = IfcLogicalOperatorEnum::createObjectFromStepData( args[8] );
	m_ObjectiveQualifier = IfcObjectiveEnum::createObjectFromStepData( args[9] );
	m_UserDefinedQualifier = IfcLabel::createObjectFromStepData( args[10] );
}
void IfcObjective::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConstraint::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LogicalAggregator", m_LogicalAggregator ) );
	vec_attributes.push_back( std::make_pair( "ObjectiveQualifier", m_ObjectiveQualifier ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedQualifier", m_UserDefinedQualifier ) );
}
void IfcObjective::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcObjective::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConstraint::setInverseCounterparts( ptr_self_entity );
}
void IfcObjective::unlinkSelf()
{
	IfcConstraint::unlinkSelf();
}
