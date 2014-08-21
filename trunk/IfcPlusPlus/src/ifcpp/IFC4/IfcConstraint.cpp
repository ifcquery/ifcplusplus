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
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcActorSelect.h"
#include "include/IfcConstraint.h"
#include "include/IfcConstraintEnum.h"
#include "include/IfcDateTime.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcText.h"

// ENTITY IfcConstraint 
IfcConstraint::IfcConstraint() {}
IfcConstraint::IfcConstraint( int id ) { m_id = id; }
IfcConstraint::~IfcConstraint() {}
shared_ptr<IfcPPObject> IfcConstraint::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcConstraint> copy_self( new IfcConstraint() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ConstraintGrade ) { copy_self->m_ConstraintGrade = dynamic_pointer_cast<IfcConstraintEnum>( m_ConstraintGrade->getDeepCopy(options) ); }
	if( m_ConstraintSource ) { copy_self->m_ConstraintSource = dynamic_pointer_cast<IfcLabel>( m_ConstraintSource->getDeepCopy(options) ); }
	if( m_CreatingActor ) { copy_self->m_CreatingActor = dynamic_pointer_cast<IfcActorSelect>( m_CreatingActor->getDeepCopy(options) ); }
	if( m_CreationTime ) { copy_self->m_CreationTime = dynamic_pointer_cast<IfcDateTime>( m_CreationTime->getDeepCopy(options) ); }
	if( m_UserDefinedGrade ) { copy_self->m_UserDefinedGrade = dynamic_pointer_cast<IfcLabel>( m_UserDefinedGrade->getDeepCopy(options) ); }
	return copy_self;
}
void IfcConstraint::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCONSTRAINT" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ConstraintGrade ) { m_ConstraintGrade->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ConstraintSource ) { m_ConstraintSource->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CreatingActor ) { m_CreatingActor->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_CreationTime ) { m_CreationTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedGrade ) { m_UserDefinedGrade->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcConstraint::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConstraint::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcConstraint, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_ConstraintGrade = IfcConstraintEnum::createObjectFromSTEP( args[2] );
	m_ConstraintSource = IfcLabel::createObjectFromSTEP( args[3] );
	m_CreatingActor = IfcActorSelect::createObjectFromSTEP( args[4], map );
	m_CreationTime = IfcDateTime::createObjectFromSTEP( args[5] );
	m_UserDefinedGrade = IfcLabel::createObjectFromSTEP( args[6] );
}
void IfcConstraint::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "ConstraintGrade", m_ConstraintGrade ) );
	vec_attributes.push_back( std::make_pair( "ConstraintSource", m_ConstraintSource ) );
	vec_attributes.push_back( std::make_pair( "CreatingActor", m_CreatingActor ) );
	vec_attributes.push_back( std::make_pair( "CreationTime", m_CreationTime ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedGrade", m_UserDefinedGrade ) );
}
void IfcConstraint::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_HasExternalReferences_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasExternalReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasExternalReferences_inverse.size(); ++i )
		{
			if( !m_HasExternalReferences_inverse[i].expired() )
			{
				HasExternalReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReferences_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasExternalReferences_inverse", HasExternalReferences_inverse_vec_obj ) );
	}
	if( m_PropertiesForConstraint_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PropertiesForConstraint_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PropertiesForConstraint_inverse.size(); ++i )
		{
			if( !m_PropertiesForConstraint_inverse[i].expired() )
			{
				PropertiesForConstraint_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcResourceConstraintRelationship>( m_PropertiesForConstraint_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PropertiesForConstraint_inverse", PropertiesForConstraint_inverse_vec_obj ) );
	}
}
void IfcConstraint::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcConstraint::unlinkSelf()
{
}
