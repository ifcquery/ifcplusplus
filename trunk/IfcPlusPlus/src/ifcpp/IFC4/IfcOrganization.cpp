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
#include "include/IfcOrganization.h"
#include "include/IfcOrganizationRelationship.h"
#include "include/IfcPersonAndOrganization.h"
#include "include/IfcText.h"

// ENTITY IfcOrganization 
IfcOrganization::IfcOrganization() {}
IfcOrganization::IfcOrganization( int id ) { m_id = id; }
IfcOrganization::~IfcOrganization() {}
shared_ptr<IfcPPObject> IfcOrganization::getDeepCopy()
{
	shared_ptr<IfcOrganization> copy_self( new IfcOrganization() );
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	for( size_t ii=0; ii<m_Roles.size(); ++ii )
	{
		auto item_ii = m_Roles[ii];
		if( item_ii )
		{
			copy_self->m_Roles.push_back( dynamic_pointer_cast<IfcActorRole>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_Addresses.size(); ++ii )
	{
		auto item_ii = m_Addresses[ii];
		if( item_ii )
		{
			copy_self->m_Addresses.push_back( dynamic_pointer_cast<IfcAddress>(item_ii->getDeepCopy() ) );
		}
	}
	return copy_self;
}
void IfcOrganization::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCORGANIZATION" << "(";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Roles );
	stream << ",";
	writeEntityList( stream, m_Addresses );
	stream << ");";
}
void IfcOrganization::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOrganization::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcOrganization, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcOrganization, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Identification = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Name = IfcLabel::createObjectFromStepData( args[1] );
	m_Description = IfcText::createObjectFromStepData( args[2] );
	readEntityReferenceList( args[3], m_Roles, map );
	readEntityReferenceList( args[4], m_Addresses, map );
}
void IfcOrganization::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Identification", m_Identification ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
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
void IfcOrganization::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_IsRelatedBy_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> IsRelatedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_IsRelatedBy_inverse.size(); ++i )
		{
			if( !m_IsRelatedBy_inverse[i].expired() )
			{
				IsRelatedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcOrganizationRelationship>( m_IsRelatedBy_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "IsRelatedBy_inverse", IsRelatedBy_inverse_vec_obj ) );
	}
	if( m_Relates_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Relates_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_Relates_inverse.size(); ++i )
		{
			if( !m_Relates_inverse[i].expired() )
			{
				Relates_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcOrganizationRelationship>( m_Relates_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "Relates_inverse", Relates_inverse_vec_obj ) );
	}
	if( m_Engages_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Engages_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_Engages_inverse.size(); ++i )
		{
			if( !m_Engages_inverse[i].expired() )
			{
				Engages_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPersonAndOrganization>( m_Engages_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "Engages_inverse", Engages_inverse_vec_obj ) );
	}
}
void IfcOrganization::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcOrganization> ptr_self = dynamic_pointer_cast<IfcOrganization>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcOrganization::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_Addresses.size(); ++i )
	{
		if( m_Addresses[i] )
		{
			m_Addresses[i]->m_OfOrganization_inverse.push_back( ptr_self );
		}
	}
}
void IfcOrganization::unlinkSelf()
{
	for( int i=0; i<m_Addresses.size(); ++i )
	{
		if( m_Addresses[i] )
		{
			std::vector<weak_ptr<IfcOrganization> >& OfOrganization_inverse = m_Addresses[i]->m_OfOrganization_inverse;
			std::vector<weak_ptr<IfcOrganization> >::iterator it_OfOrganization_inverse;
			for( it_OfOrganization_inverse = OfOrganization_inverse.begin(); it_OfOrganization_inverse != OfOrganization_inverse.end(); ++it_OfOrganization_inverse)
			{
				shared_ptr<IfcOrganization> self_candidate( *it_OfOrganization_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					OfOrganization_inverse.erase( it_OfOrganization_inverse );
					break;
				}
			}
		}
	}
}
