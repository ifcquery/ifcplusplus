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
#include "include/IfcCovering.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelCoversSpaces.h"
#include "include/IfcSpace.h"
#include "include/IfcText.h"

// ENTITY IfcRelCoversSpaces 
IfcRelCoversSpaces::IfcRelCoversSpaces() {}
IfcRelCoversSpaces::IfcRelCoversSpaces( int id ) { m_id = id; }
IfcRelCoversSpaces::~IfcRelCoversSpaces() {}
shared_ptr<IfcPPObject> IfcRelCoversSpaces::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelCoversSpaces> copy_self( new IfcRelCoversSpaces() );
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
	if( m_RelatingSpace ) { copy_self->m_RelatingSpace = dynamic_pointer_cast<IfcSpace>( m_RelatingSpace->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedCoverings.size(); ++ii )
	{
		auto item_ii = m_RelatedCoverings[ii];
		if( item_ii )
		{
			copy_self->m_RelatedCoverings.push_back( dynamic_pointer_cast<IfcCovering>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcRelCoversSpaces::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELCOVERSSPACES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingSpace ) { stream << "#" << m_RelatingSpace->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedCoverings );
	stream << ");";
}
void IfcRelCoversSpaces::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelCoversSpaces::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelCoversSpaces, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReference( args[4], m_RelatingSpace, map );
	readEntityReferenceList( args[5], m_RelatedCoverings, map );
}
void IfcRelCoversSpaces::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingSpace", m_RelatingSpace ) );
	if( m_RelatedCoverings.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedCoverings_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedCoverings.begin(), m_RelatedCoverings.end(), std::back_inserter( RelatedCoverings_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedCoverings", RelatedCoverings_vec_object ) );
	}
}
void IfcRelCoversSpaces::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelConnects::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelCoversSpaces::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelCoversSpaces> ptr_self = dynamic_pointer_cast<IfcRelCoversSpaces>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelCoversSpaces::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedCoverings.size(); ++i )
	{
		if( m_RelatedCoverings[i] )
		{
			m_RelatedCoverings[i]->m_CoversSpaces_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingSpace )
	{
		m_RelatingSpace->m_HasCoverings_inverse.push_back( ptr_self );
	}
}
void IfcRelCoversSpaces::unlinkSelf()
{
	IfcRelConnects::unlinkSelf();
	for( size_t i=0; i<m_RelatedCoverings.size(); ++i )
	{
		if( m_RelatedCoverings[i] )
		{
			std::vector<weak_ptr<IfcRelCoversSpaces> >& CoversSpaces_inverse = m_RelatedCoverings[i]->m_CoversSpaces_inverse;
			for( auto it_CoversSpaces_inverse = CoversSpaces_inverse.begin(); it_CoversSpaces_inverse != CoversSpaces_inverse.end(); ++it_CoversSpaces_inverse)
			{
				shared_ptr<IfcRelCoversSpaces> self_candidate( *it_CoversSpaces_inverse );
				if( self_candidate.get() == this )
				{
					CoversSpaces_inverse.erase( it_CoversSpaces_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingSpace )
	{
		std::vector<weak_ptr<IfcRelCoversSpaces> >& HasCoverings_inverse = m_RelatingSpace->m_HasCoverings_inverse;
		for( auto it_HasCoverings_inverse = HasCoverings_inverse.begin(); it_HasCoverings_inverse != HasCoverings_inverse.end(); ++it_HasCoverings_inverse)
		{
			shared_ptr<IfcRelCoversSpaces> self_candidate( *it_HasCoverings_inverse );
			if( self_candidate.get() == this )
			{
				HasCoverings_inverse.erase( it_HasCoverings_inverse );
				break;
			}
		}
	}
}
