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

// method setEntity takes over all attributes from another instance of the class
void IfcPerson::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPerson> other = dynamic_pointer_cast<IfcPerson>(other_entity);
	if( !other) { return; }
	m_Identification = other->m_Identification;
	m_FamilyName = other->m_FamilyName;
	m_GivenName = other->m_GivenName;
	m_MiddleNames = other->m_MiddleNames;
	m_PrefixTitles = other->m_PrefixTitles;
	m_SuffixTitles = other->m_SuffixTitles;
	m_Roles = other->m_Roles;
	m_Addresses = other->m_Addresses;
}
void IfcPerson::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPERSON" << "(";
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
void IfcPerson::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPerson, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcPerson, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Identification = IfcIdentifier::createObjectFromStepData( args[0] );
	m_FamilyName = IfcLabel::createObjectFromStepData( args[1] );
	m_GivenName = IfcLabel::createObjectFromStepData( args[2] );
	readTypeList( args[3], m_MiddleNames );
	readTypeList( args[4], m_PrefixTitles );
	readTypeList( args[5], m_SuffixTitles );
	readEntityReferenceList( args[6], m_Roles, map );
	readEntityReferenceList( args[7], m_Addresses, map );
}
void IfcPerson::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Identification", m_Identification ) );
	vec_attributes.push_back( std::make_pair( "FamilyName", m_FamilyName ) );
	vec_attributes.push_back( std::make_pair( "GivenName", m_GivenName ) );
	shared_ptr<IfcPPAttributeObjectVector> MiddleNames_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_MiddleNames.begin(), m_MiddleNames.end(), std::back_inserter( MiddleNames_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "MiddleNames", MiddleNames_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> PrefixTitles_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_PrefixTitles.begin(), m_PrefixTitles.end(), std::back_inserter( PrefixTitles_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "PrefixTitles", PrefixTitles_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> SuffixTitles_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_SuffixTitles.begin(), m_SuffixTitles.end(), std::back_inserter( SuffixTitles_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "SuffixTitles", SuffixTitles_vec_object ) );
}
void IfcPerson::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> EngagedIn_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_EngagedIn_inverse.size(); ++i ) { EngagedIn_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPersonAndOrganization>( m_EngagedIn_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "EngagedIn_inverse", EngagedIn_inverse_vec_obj ) );
}
void IfcPerson::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcPerson> ptr_self = dynamic_pointer_cast<IfcPerson>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcPerson::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_Addresses.size(); ++i )
	{
		if( m_Addresses[i] )
		{
			m_Addresses[i]->m_OfPerson_inverse.push_back( ptr_self );
		}
	}
}
void IfcPerson::unlinkSelf()
{
	for( int i=0; i<m_Addresses.size(); ++i )
	{
		if( m_Addresses[i] )
		{
			std::vector<weak_ptr<IfcPerson> >& OfPerson_inverse = m_Addresses[i]->m_OfPerson_inverse;
			std::vector<weak_ptr<IfcPerson> >::iterator it_OfPerson_inverse;
			for( it_OfPerson_inverse = OfPerson_inverse.begin(); it_OfPerson_inverse != OfPerson_inverse.end(); ++it_OfPerson_inverse)
			{
				shared_ptr<IfcPerson> self_candidate( *it_OfPerson_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					OfPerson_inverse.erase( it_OfPerson_inverse );
					break;
				}
			}
		}
	}
}
