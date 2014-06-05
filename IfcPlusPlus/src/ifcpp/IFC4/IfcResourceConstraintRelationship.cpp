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
#include "include/IfcConstraint.h"
#include "include/IfcLabel.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcResourceObjectSelect.h"
#include "include/IfcText.h"

// ENTITY IfcResourceConstraintRelationship 
IfcResourceConstraintRelationship::IfcResourceConstraintRelationship() {}
IfcResourceConstraintRelationship::IfcResourceConstraintRelationship( int id ) { m_id = id; }
IfcResourceConstraintRelationship::~IfcResourceConstraintRelationship() {}

// method setEntity takes over all attributes from another instance of the class
void IfcResourceConstraintRelationship::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcResourceConstraintRelationship> other = dynamic_pointer_cast<IfcResourceConstraintRelationship>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingConstraint = other->m_RelatingConstraint;
	m_RelatedResourceObjects = other->m_RelatedResourceObjects;
}
void IfcResourceConstraintRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRESOURCECONSTRAINTRELATIONSHIP" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingConstraint ) { stream << "#" << m_RelatingConstraint->getId(); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_RelatedResourceObjects, true );
	stream << ");";
}
void IfcResourceConstraintRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcResourceConstraintRelationship::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcResourceConstraintRelationship, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcResourceConstraintRelationship, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_RelatingConstraint, map );
	readSelectList( args[3], m_RelatedResourceObjects, map );
}
void IfcResourceConstraintRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcResourceLevelRelationship::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingConstraint", m_RelatingConstraint ) );
	shared_ptr<IfcPPAttributeObjectVector> RelatedResourceObjects_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_RelatedResourceObjects.begin(), m_RelatedResourceObjects.end(), std::back_inserter( RelatedResourceObjects_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "RelatedResourceObjects", RelatedResourceObjects_vec_object ) );
}
void IfcResourceConstraintRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcResourceConstraintRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcResourceLevelRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcResourceConstraintRelationship> ptr_self = dynamic_pointer_cast<IfcResourceConstraintRelationship>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcResourceConstraintRelationship::setInverseCounterparts: type mismatch" ); }
	if( m_RelatingConstraint )
	{
		m_RelatingConstraint->m_PropertiesForConstraint_inverse.push_back( ptr_self );
	}
}
void IfcResourceConstraintRelationship::unlinkSelf()
{
	IfcResourceLevelRelationship::unlinkSelf();
	if( m_RelatingConstraint )
	{
		std::vector<weak_ptr<IfcResourceConstraintRelationship> >& PropertiesForConstraint_inverse = m_RelatingConstraint->m_PropertiesForConstraint_inverse;
		std::vector<weak_ptr<IfcResourceConstraintRelationship> >::iterator it_PropertiesForConstraint_inverse;
		for( it_PropertiesForConstraint_inverse = PropertiesForConstraint_inverse.begin(); it_PropertiesForConstraint_inverse != PropertiesForConstraint_inverse.end(); ++it_PropertiesForConstraint_inverse)
		{
			shared_ptr<IfcResourceConstraintRelationship> self_candidate( *it_PropertiesForConstraint_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				PropertiesForConstraint_inverse.erase( it_PropertiesForConstraint_inverse );
				break;
			}
		}
	}
}
