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
#include "include/IfcLabel.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRepresentation.h"
#include "include/IfcRepresentationContext.h"
#include "include/IfcRepresentationItem.h"
#include "include/IfcRepresentationMap.h"

// ENTITY IfcRepresentation 
IfcRepresentation::IfcRepresentation() {}
IfcRepresentation::IfcRepresentation( int id ) { m_id = id; }
IfcRepresentation::~IfcRepresentation() {}
shared_ptr<IfcPPObject> IfcRepresentation::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRepresentation> copy_self( new IfcRepresentation() );
	if( m_ContextOfItems )
	{
		if( options.shallow_copy_IfcRepresentationContext ) { copy_self->m_ContextOfItems = m_ContextOfItems; }
		else { copy_self->m_ContextOfItems = dynamic_pointer_cast<IfcRepresentationContext>( m_ContextOfItems->getDeepCopy(options) ); }
	}
	if( m_RepresentationIdentifier ) { copy_self->m_RepresentationIdentifier = dynamic_pointer_cast<IfcLabel>( m_RepresentationIdentifier->getDeepCopy(options) ); }
	if( m_RepresentationType ) { copy_self->m_RepresentationType = dynamic_pointer_cast<IfcLabel>( m_RepresentationType->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Items.size(); ++ii )
	{
		auto item_ii = m_Items[ii];
		if( item_ii )
		{
			copy_self->m_Items.push_back( dynamic_pointer_cast<IfcRepresentationItem>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREPRESENTATION" << "(";
	if( m_ContextOfItems ) { stream << "#" << m_ContextOfItems->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_RepresentationIdentifier ) { m_RepresentationIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RepresentationType ) { m_RepresentationType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Items );
	stream << ");";
}
void IfcRepresentation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRepresentation::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRepresentation, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_ContextOfItems, map );
	m_RepresentationIdentifier = IfcLabel::createObjectFromSTEP( args[1] );
	m_RepresentationType = IfcLabel::createObjectFromSTEP( args[2] );
	readEntityReferenceList( args[3], m_Items, map );
}
void IfcRepresentation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "ContextOfItems", m_ContextOfItems ) );
	vec_attributes.push_back( std::make_pair( "RepresentationIdentifier", m_RepresentationIdentifier ) );
	vec_attributes.push_back( std::make_pair( "RepresentationType", m_RepresentationType ) );
	if( m_Items.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Items_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Items.begin(), m_Items.end(), std::back_inserter( Items_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Items", Items_vec_object ) );
	}
}
void IfcRepresentation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_RepresentationMap_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RepresentationMap_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_RepresentationMap_inverse.size(); ++i )
		{
			if( !m_RepresentationMap_inverse[i].expired() )
			{
				RepresentationMap_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRepresentationMap>( m_RepresentationMap_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "RepresentationMap_inverse", RepresentationMap_inverse_vec_obj ) );
	}
	if( m_LayerAssignments_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LayerAssignments_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_LayerAssignments_inverse.size(); ++i )
		{
			if( !m_LayerAssignments_inverse[i].expired() )
			{
				LayerAssignments_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPresentationLayerAssignment>( m_LayerAssignments_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "LayerAssignments_inverse", LayerAssignments_inverse_vec_obj ) );
	}
	if( m_OfProductRepresentation_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> OfProductRepresentation_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_OfProductRepresentation_inverse.size(); ++i )
		{
			if( !m_OfProductRepresentation_inverse[i].expired() )
			{
				OfProductRepresentation_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcProductRepresentation>( m_OfProductRepresentation_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "OfProductRepresentation_inverse", OfProductRepresentation_inverse_vec_obj ) );
	}
}
void IfcRepresentation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcRepresentation> ptr_self = dynamic_pointer_cast<IfcRepresentation>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRepresentation::setInverseCounterparts: type mismatch" ); }
	if( m_ContextOfItems )
	{
		m_ContextOfItems->m_RepresentationsInContext_inverse.push_back( ptr_self );
	}
}
void IfcRepresentation::unlinkSelf()
{
	if( m_ContextOfItems )
	{
		std::vector<weak_ptr<IfcRepresentation> >& RepresentationsInContext_inverse = m_ContextOfItems->m_RepresentationsInContext_inverse;
		for( auto it_RepresentationsInContext_inverse = RepresentationsInContext_inverse.begin(); it_RepresentationsInContext_inverse != RepresentationsInContext_inverse.end(); ++it_RepresentationsInContext_inverse)
		{
			shared_ptr<IfcRepresentation> self_candidate( *it_RepresentationsInContext_inverse );
			if( self_candidate.get() == this )
			{
				RepresentationsInContext_inverse.erase( it_RepresentationsInContext_inverse );
				break;
			}
		}
	}
}
