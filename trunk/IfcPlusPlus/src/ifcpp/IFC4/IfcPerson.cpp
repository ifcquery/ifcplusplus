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
#include "include/IfcActorRole.h"
#include "include/IfcAddress.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcPerson.h"
#include "include/IfcPersonAndOrganization.h"

// ENTITY IfcPerson 
IfcPerson::IfcPerson() {}
IfcPerson::IfcPerson( int id ) { m_id = id; }
IfcPerson::~IfcPerson() {}
shared_ptr<IfcPPObject> IfcPerson::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPerson> copy_self( new IfcPerson() );
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy(options) ); }
	if( m_FamilyName ) { copy_self->m_FamilyName = dynamic_pointer_cast<IfcLabel>( m_FamilyName->getDeepCopy(options) ); }
	if( m_GivenName ) { copy_self->m_GivenName = dynamic_pointer_cast<IfcLabel>( m_GivenName->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_MiddleNames.size(); ++ii )
	{
		auto item_ii = m_MiddleNames[ii];
		if( item_ii )
		{
			copy_self->m_MiddleNames.push_back( dynamic_pointer_cast<IfcLabel>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_PrefixTitles.size(); ++ii )
	{
		auto item_ii = m_PrefixTitles[ii];
		if( item_ii )
		{
			copy_self->m_PrefixTitles.push_back( dynamic_pointer_cast<IfcLabel>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_SuffixTitles.size(); ++ii )
	{
		auto item_ii = m_SuffixTitles[ii];
		if( item_ii )
		{
			copy_self->m_SuffixTitles.push_back( dynamic_pointer_cast<IfcLabel>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_Roles.size(); ++ii )
	{
		auto item_ii = m_Roles[ii];
		if( item_ii )
		{
			copy_self->m_Roles.push_back( dynamic_pointer_cast<IfcActorRole>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_Addresses.size(); ++ii )
	{
		auto item_ii = m_Addresses[ii];
		if( item_ii )
		{
			copy_self->m_Addresses.push_back( dynamic_pointer_cast<IfcAddress>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcPerson::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPERSON" << "(";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FamilyName ) { m_FamilyName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_GivenName ) { m_GivenName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_MiddleNames );
	stream << ",";
	writeTypeList( stream, m_PrefixTitles );
	stream << ",";
	writeTypeList( stream, m_SuffixTitles );
	stream << ",";
	writeEntityList( stream, m_Roles );
	stream << ",";
	writeEntityList( stream, m_Addresses );
	stream << ");";
}
void IfcPerson::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPerson::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPerson, expecting 8, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Identification = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_FamilyName = IfcLabel::createObjectFromSTEP( args[1] );
	m_GivenName = IfcLabel::createObjectFromSTEP( args[2] );
	readSelectList( args[3], m_MiddleNames, map );
	readSelectList( args[4], m_PrefixTitles, map );
	readSelectList( args[5], m_SuffixTitles, map );
	readEntityReferenceList( args[6], m_Roles, map );
	readEntityReferenceList( args[7], m_Addresses, map );
}
void IfcPerson::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Identification", m_Identification ) );
	vec_attributes.push_back( std::make_pair( "FamilyName", m_FamilyName ) );
	vec_attributes.push_back( std::make_pair( "GivenName", m_GivenName ) );
	if( m_MiddleNames.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> MiddleNames_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_MiddleNames.begin(), m_MiddleNames.end(), std::back_inserter( MiddleNames_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "MiddleNames", MiddleNames_vec_object ) );
	}
	if( m_PrefixTitles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PrefixTitles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_PrefixTitles.begin(), m_PrefixTitles.end(), std::back_inserter( PrefixTitles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "PrefixTitles", PrefixTitles_vec_object ) );
	}
	if( m_SuffixTitles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> SuffixTitles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_SuffixTitles.begin(), m_SuffixTitles.end(), std::back_inserter( SuffixTitles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "SuffixTitles", SuffixTitles_vec_object ) );
	}
	if( m_Roles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Roles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Roles.begin(), m_Roles.end(), std::back_inserter( Roles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Roles", Roles_vec_object ) );
	}
	if( m_Addresses.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Addresses_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Addresses.begin(), m_Addresses.end(), std::back_inserter( Addresses_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Addresses", Addresses_vec_object ) );
	}
}
void IfcPerson::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_EngagedIn_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> EngagedIn_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_EngagedIn_inverse.size(); ++i )
		{
			if( !m_EngagedIn_inverse[i].expired() )
			{
				EngagedIn_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPersonAndOrganization>( m_EngagedIn_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "EngagedIn_inverse", EngagedIn_inverse_vec_obj ) );
	}
}
void IfcPerson::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcPerson> ptr_self = dynamic_pointer_cast<IfcPerson>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcPerson::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_Addresses.size(); ++i )
	{
		if( m_Addresses[i] )
		{
			m_Addresses[i]->m_OfPerson_inverse.push_back( ptr_self );
		}
	}
}
void IfcPerson::unlinkSelf()
{
	for( size_t i=0; i<m_Addresses.size(); ++i )
	{
		if( m_Addresses[i] )
		{
			std::vector<weak_ptr<IfcPerson> >& OfPerson_inverse = m_Addresses[i]->m_OfPerson_inverse;
			for( auto it_OfPerson_inverse = OfPerson_inverse.begin(); it_OfPerson_inverse != OfPerson_inverse.end(); ++it_OfPerson_inverse)
			{
				shared_ptr<IfcPerson> self_candidate( *it_OfPerson_inverse );
				if( self_candidate.get() == this )
				{
					OfPerson_inverse.erase( it_OfPerson_inverse );
					break;
				}
			}
		}
	}
}
