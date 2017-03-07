/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcConstraint.h"
#include "include/IfcLabel.h"
#include "include/IfcProperty.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcResourceObjectSelect.h"
#include "include/IfcText.h"

// ENTITY IfcResourceConstraintRelationship 
IfcResourceConstraintRelationship::IfcResourceConstraintRelationship() { m_entity_enum = IFCRESOURCECONSTRAINTRELATIONSHIP; }
IfcResourceConstraintRelationship::IfcResourceConstraintRelationship( int id ) { m_id = id; m_entity_enum = IFCRESOURCECONSTRAINTRELATIONSHIP; }
IfcResourceConstraintRelationship::~IfcResourceConstraintRelationship() {}
shared_ptr<IfcPPObject> IfcResourceConstraintRelationship::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcResourceConstraintRelationship> copy_self( new IfcResourceConstraintRelationship() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_RelatingConstraint ) { copy_self->m_RelatingConstraint = dynamic_pointer_cast<IfcConstraint>( m_RelatingConstraint->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedResourceObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedResourceObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedResourceObjects.push_back( dynamic_pointer_cast<IfcResourceObjectSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcResourceConstraintRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRESOURCECONSTRAINTRELATIONSHIP" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingConstraint ) { stream << "#" << m_RelatingConstraint->m_id; } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_RelatedResourceObjects.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcResourceObjectSelect>& type_object = m_RelatedResourceObjects[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ");";
}
void IfcResourceConstraintRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcResourceConstraintRelationship::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcResourceConstraintRelationship, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_RelatingConstraint, map );
	readSelectList( args[3], m_RelatedResourceObjects, map );
}
void IfcResourceConstraintRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcResourceLevelRelationship::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingConstraint", m_RelatingConstraint ) );
	if( m_RelatedResourceObjects.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedResourceObjects_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedResourceObjects.begin(), m_RelatedResourceObjects.end(), std::back_inserter( RelatedResourceObjects_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedResourceObjects", RelatedResourceObjects_vec_object ) );
	}
}
void IfcResourceConstraintRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcResourceLevelRelationship::getAttributesInverse( vec_attributes_inverse );
}
void IfcResourceConstraintRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcResourceLevelRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcResourceConstraintRelationship> ptr_self = dynamic_pointer_cast<IfcResourceConstraintRelationship>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcResourceConstraintRelationship::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedResourceObjects.size(); ++i )
	{
		shared_ptr<IfcProperty>  RelatedResourceObjects_IfcProperty = dynamic_pointer_cast<IfcProperty>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcProperty )
		{
			RelatedResourceObjects_IfcProperty->m_HasConstraints_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingConstraint )
	{
		m_RelatingConstraint->m_PropertiesForConstraint_inverse.push_back( ptr_self );
	}
}
void IfcResourceConstraintRelationship::unlinkFromInverseCounterparts()
{
	IfcResourceLevelRelationship::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_RelatedResourceObjects.size(); ++i )
	{
		shared_ptr<IfcProperty>  RelatedResourceObjects_IfcProperty = dynamic_pointer_cast<IfcProperty>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcProperty )
		{
			std::vector<weak_ptr<IfcResourceConstraintRelationship> >& HasConstraints_inverse = RelatedResourceObjects_IfcProperty->m_HasConstraints_inverse;
			for( auto it_HasConstraints_inverse = HasConstraints_inverse.begin(); it_HasConstraints_inverse != HasConstraints_inverse.end(); )
			{
				shared_ptr<IfcResourceConstraintRelationship> self_candidate( *it_HasConstraints_inverse );
				if( self_candidate.get() == this )
				{
					it_HasConstraints_inverse= HasConstraints_inverse.erase( it_HasConstraints_inverse );
				}
				else
				{
					++it_HasConstraints_inverse;
				}
			}
		}
	}
	if( m_RelatingConstraint )
	{
		std::vector<weak_ptr<IfcResourceConstraintRelationship> >& PropertiesForConstraint_inverse = m_RelatingConstraint->m_PropertiesForConstraint_inverse;
		for( auto it_PropertiesForConstraint_inverse = PropertiesForConstraint_inverse.begin(); it_PropertiesForConstraint_inverse != PropertiesForConstraint_inverse.end(); )
		{
			shared_ptr<IfcResourceConstraintRelationship> self_candidate( *it_PropertiesForConstraint_inverse );
			if( self_candidate.get() == this )
			{
				it_PropertiesForConstraint_inverse= PropertiesForConstraint_inverse.erase( it_PropertiesForConstraint_inverse );
			}
			else
			{
				++it_PropertiesForConstraint_inverse;
			}
		}
	}
}
