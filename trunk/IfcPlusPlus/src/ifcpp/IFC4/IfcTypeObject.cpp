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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"

// ENTITY IfcTypeObject 
IfcTypeObject::IfcTypeObject() {}
IfcTypeObject::IfcTypeObject( int id ) { m_id = id; }
IfcTypeObject::~IfcTypeObject() {}
shared_ptr<IfcPPObject> IfcTypeObject::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTypeObject> copy_self( new IfcTypeObject() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( CreateCompressedGuidString22() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ApplicableOccurrence ) { copy_self->m_ApplicableOccurrence = dynamic_pointer_cast<IfcIdentifier>( m_ApplicableOccurrence->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_HasPropertySets.size(); ++ii )
	{
		auto item_ii = m_HasPropertySets[ii];
		if( item_ii )
		{
			copy_self->m_HasPropertySets.push_back( dynamic_pointer_cast<IfcPropertySetDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTypeObject::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTYPEOBJECT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ApplicableOccurrence ) { m_ApplicableOccurrence->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertySets );
	stream << ");";
}
void IfcTypeObject::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTypeObject::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTypeObject, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4] );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
}
void IfcTypeObject::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObjectDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ApplicableOccurrence", m_ApplicableOccurrence ) );
	if( m_HasPropertySets.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasPropertySets_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_HasPropertySets.begin(), m_HasPropertySets.end(), std::back_inserter( HasPropertySets_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "HasPropertySets", HasPropertySets_vec_object ) );
	}
}
void IfcTypeObject::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcObjectDefinition::getAttributesInverse( vec_attributes_inverse );
	if( m_Types_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Types_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_Types_inverse.size(); ++i )
		{
			if( !m_Types_inverse[i].expired() )
			{
				Types_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelDefinesByType>( m_Types_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "Types_inverse", Types_inverse_vec_obj ) );
	}
}
void IfcTypeObject::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObjectDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcTypeObject> ptr_self = dynamic_pointer_cast<IfcTypeObject>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcTypeObject::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_HasPropertySets.size(); ++i )
	{
		if( m_HasPropertySets[i] )
		{
			m_HasPropertySets[i]->m_DefinesType_inverse.push_back( ptr_self );
		}
	}
}
void IfcTypeObject::unlinkSelf()
{
	IfcObjectDefinition::unlinkSelf();
	for( size_t i=0; i<m_HasPropertySets.size(); ++i )
	{
		if( m_HasPropertySets[i] )
		{
			std::vector<weak_ptr<IfcTypeObject> >& DefinesType_inverse = m_HasPropertySets[i]->m_DefinesType_inverse;
			for( auto it_DefinesType_inverse = DefinesType_inverse.begin(); it_DefinesType_inverse != DefinesType_inverse.end(); ++it_DefinesType_inverse)
			{
				shared_ptr<IfcTypeObject> self_candidate( *it_DefinesType_inverse );
				if( self_candidate.get() == this )
				{
					DefinesType_inverse.erase( it_DefinesType_inverse );
					break;
				}
			}
		}
	}
}
