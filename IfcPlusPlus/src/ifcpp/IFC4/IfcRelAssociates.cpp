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
#include "include/IfcDefinitionSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertyDefinition.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssociates 
IfcRelAssociates::IfcRelAssociates() { m_entity_enum = IFCRELASSOCIATES; }
IfcRelAssociates::IfcRelAssociates( int id ) { m_id = id; m_entity_enum = IFCRELASSOCIATES; }
IfcRelAssociates::~IfcRelAssociates() {}
shared_ptr<IfcPPObject> IfcRelAssociates::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelAssociates> copy_self( new IfcRelAssociates() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedObjects.push_back( dynamic_pointer_cast<IfcDefinitionSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcRelAssociates::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELASSOCIATES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_RelatedObjects.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcDefinitionSelect>& type_object = m_RelatedObjects[ii];
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
void IfcRelAssociates::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociates::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRelAssociates, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readSelectList( args[4], m_RelatedObjects, map );
}
void IfcRelAssociates::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelationship::getAttributes( vec_attributes );
	if( m_RelatedObjects.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedObjects_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedObjects.begin(), m_RelatedObjects.end(), std::back_inserter( RelatedObjects_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedObjects", RelatedObjects_vec_object ) );
	}
}
void IfcRelAssociates::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelationship::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelAssociates::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssociates> ptr_self = dynamic_pointer_cast<IfcRelAssociates>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssociates::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcObjectDefinition>  RelatedObjects_IfcObjectDefinition = dynamic_pointer_cast<IfcObjectDefinition>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObjectDefinition )
		{
			RelatedObjects_IfcObjectDefinition->m_HasAssociations_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcPropertyDefinition>  RelatedObjects_IfcPropertyDefinition = dynamic_pointer_cast<IfcPropertyDefinition>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcPropertyDefinition )
		{
			RelatedObjects_IfcPropertyDefinition->m_HasAssociations_inverse.push_back( ptr_self );
		}
	}
}
void IfcRelAssociates::unlinkFromInverseCounterparts()
{
	IfcRelationship::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_RelatedObjects.size(); ++i )
	{
		shared_ptr<IfcObjectDefinition>  RelatedObjects_IfcObjectDefinition = dynamic_pointer_cast<IfcObjectDefinition>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcObjectDefinition )
		{
			std::vector<weak_ptr<IfcRelAssociates> >& HasAssociations_inverse = RelatedObjects_IfcObjectDefinition->m_HasAssociations_inverse;
			for( auto it_HasAssociations_inverse = HasAssociations_inverse.begin(); it_HasAssociations_inverse != HasAssociations_inverse.end(); )
			{
				shared_ptr<IfcRelAssociates> self_candidate( *it_HasAssociations_inverse );
				if( self_candidate.get() == this )
				{
					it_HasAssociations_inverse= HasAssociations_inverse.erase( it_HasAssociations_inverse );
				}
				else
				{
					++it_HasAssociations_inverse;
				}
			}
		}
		shared_ptr<IfcPropertyDefinition>  RelatedObjects_IfcPropertyDefinition = dynamic_pointer_cast<IfcPropertyDefinition>( m_RelatedObjects[i] );
		if( RelatedObjects_IfcPropertyDefinition )
		{
			std::vector<weak_ptr<IfcRelAssociates> >& HasAssociations_inverse = RelatedObjects_IfcPropertyDefinition->m_HasAssociations_inverse;
			for( auto it_HasAssociations_inverse = HasAssociations_inverse.begin(); it_HasAssociations_inverse != HasAssociations_inverse.end(); )
			{
				shared_ptr<IfcRelAssociates> self_candidate( *it_HasAssociations_inverse );
				if( self_candidate.get() == this )
				{
					it_HasAssociations_inverse= HasAssociations_inverse.erase( it_HasAssociations_inverse );
				}
				else
				{
					++it_HasAssociations_inverse;
				}
			}
		}
	}
}
