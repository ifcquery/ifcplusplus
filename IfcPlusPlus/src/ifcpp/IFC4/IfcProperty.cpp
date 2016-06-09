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
#include "include/IfcComplexProperty.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcProperty.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcResourceApprovalRelationship.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcText.h"

// ENTITY IfcProperty 
IfcProperty::IfcProperty() { m_entity_enum = IFCPROPERTY; }
IfcProperty::IfcProperty( int id ) { m_id = id; m_entity_enum = IFCPROPERTY; }
IfcProperty::~IfcProperty() {}
shared_ptr<IfcPPObject> IfcProperty::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcProperty> copy_self( new IfcProperty() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	return copy_self;
}
void IfcProperty::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcProperty::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProperty::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcProperty, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
}
void IfcProperty::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyAbstraction::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcProperty::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPropertyAbstraction::getAttributesInverse( vec_attributes_inverse );
	if( m_PartOfPset_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PartOfPset_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PartOfPset_inverse.size(); ++i )
		{
			if( !m_PartOfPset_inverse[i].expired() )
			{
				PartOfPset_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPropertySet>( m_PartOfPset_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PartOfPset_inverse", PartOfPset_inverse_vec_obj ) );
	}
	if( m_PropertyForDependance_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PropertyForDependance_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PropertyForDependance_inverse.size(); ++i )
		{
			if( !m_PropertyForDependance_inverse[i].expired() )
			{
				PropertyForDependance_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPropertyDependencyRelationship>( m_PropertyForDependance_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PropertyForDependance_inverse", PropertyForDependance_inverse_vec_obj ) );
	}
	if( m_PropertyDependsOn_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PropertyDependsOn_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PropertyDependsOn_inverse.size(); ++i )
		{
			if( !m_PropertyDependsOn_inverse[i].expired() )
			{
				PropertyDependsOn_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPropertyDependencyRelationship>( m_PropertyDependsOn_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PropertyDependsOn_inverse", PropertyDependsOn_inverse_vec_obj ) );
	}
	if( m_PartOfComplex_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PartOfComplex_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_PartOfComplex_inverse.size(); ++i )
		{
			if( !m_PartOfComplex_inverse[i].expired() )
			{
				PartOfComplex_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcComplexProperty>( m_PartOfComplex_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "PartOfComplex_inverse", PartOfComplex_inverse_vec_obj ) );
	}
	if( m_HasConstraints_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasConstraints_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasConstraints_inverse.size(); ++i )
		{
			if( !m_HasConstraints_inverse[i].expired() )
			{
				HasConstraints_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcResourceConstraintRelationship>( m_HasConstraints_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasConstraints_inverse", HasConstraints_inverse_vec_obj ) );
	}
	if( m_HasApprovals_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasApprovals_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasApprovals_inverse.size(); ++i )
		{
			if( !m_HasApprovals_inverse[i].expired() )
			{
				HasApprovals_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcResourceApprovalRelationship>( m_HasApprovals_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasApprovals_inverse", HasApprovals_inverse_vec_obj ) );
	}
}
void IfcProperty::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyAbstraction::setInverseCounterparts( ptr_self_entity );
}
void IfcProperty::unlinkFromInverseCounterparts()
{
	IfcPropertyAbstraction::unlinkFromInverseCounterparts();
}
