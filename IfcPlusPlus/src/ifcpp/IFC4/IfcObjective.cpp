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
shared_ptr<IfcPPObject> IfcObjective::getDeepCopy()
{
	shared_ptr<IfcObjective> copy_self( new IfcObjective() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_ConstraintGrade ) { copy_self->m_ConstraintGrade = dynamic_pointer_cast<IfcConstraintEnum>( m_ConstraintGrade->getDeepCopy() ); }
	if( m_ConstraintSource ) { copy_self->m_ConstraintSource = dynamic_pointer_cast<IfcLabel>( m_ConstraintSource->getDeepCopy() ); }
	if( m_CreatingActor ) { copy_self->m_CreatingActor = dynamic_pointer_cast<IfcActorSelect>( m_CreatingActor->getDeepCopy() ); }
	if( m_CreationTime ) { copy_self->m_CreationTime = dynamic_pointer_cast<IfcDateTime>( m_CreationTime->getDeepCopy() ); }
	if( m_UserDefinedGrade ) { copy_self->m_UserDefinedGrade = dynamic_pointer_cast<IfcLabel>( m_UserDefinedGrade->getDeepCopy() ); }
	for( size_t ii=0; ii<m_BenchmarkValues.size(); ++ii )
	{
		auto item_ii = m_BenchmarkValues[ii];
		if( item_ii )
		{
			copy_self->m_BenchmarkValues.push_back( dynamic_pointer_cast<IfcConstraint>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_LogicalAggregator ) { copy_self->m_LogicalAggregator = dynamic_pointer_cast<IfcLogicalOperatorEnum>( m_LogicalAggregator->getDeepCopy() ); }
	if( m_ObjectiveQualifier ) { copy_self->m_ObjectiveQualifier = dynamic_pointer_cast<IfcObjectiveEnum>( m_ObjectiveQualifier->getDeepCopy() ); }
	if( m_UserDefinedQualifier ) { copy_self->m_UserDefinedQualifier = dynamic_pointer_cast<IfcLabel>( m_UserDefinedQualifier->getDeepCopy() ); }
	return copy_self;
}
void IfcObjective::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCOBJECTIVE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ConstraintGrade ) { m_ConstraintGrade->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ConstraintSource ) { m_ConstraintSource->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_CreatingActor ) { m_CreatingActor->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_CreationTime ) { m_CreationTime->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedGrade ) { m_UserDefinedGrade->getStepParameter( stream ); } else { stream << "*"; }
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
void IfcObjective::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( m_BenchmarkValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> BenchmarkValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_BenchmarkValues.begin(), m_BenchmarkValues.end(), std::back_inserter( BenchmarkValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "BenchmarkValues", BenchmarkValues_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "LogicalAggregator", m_LogicalAggregator ) );
	vec_attributes.push_back( std::make_pair( "ObjectiveQualifier", m_ObjectiveQualifier ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedQualifier", m_UserDefinedQualifier ) );
}
void IfcObjective::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConstraint::getAttributesInverse( vec_attributes_inverse );
}
void IfcObjective::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConstraint::setInverseCounterparts( ptr_self_entity );
}
void IfcObjective::unlinkSelf()
{
	IfcConstraint::unlinkSelf();
}
