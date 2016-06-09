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
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcLayeredItem.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRepresentation.h"
#include "include/IfcRepresentationItem.h"
#include "include/IfcText.h"

// ENTITY IfcPresentationLayerAssignment 
IfcPresentationLayerAssignment::IfcPresentationLayerAssignment() { m_entity_enum = IFCPRESENTATIONLAYERASSIGNMENT; }
IfcPresentationLayerAssignment::IfcPresentationLayerAssignment( int id ) { m_id = id; m_entity_enum = IFCPRESENTATIONLAYERASSIGNMENT; }
IfcPresentationLayerAssignment::~IfcPresentationLayerAssignment() {}
shared_ptr<IfcPPObject> IfcPresentationLayerAssignment::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPresentationLayerAssignment> copy_self( new IfcPresentationLayerAssignment() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_AssignedItems.size(); ++ii )
	{
		auto item_ii = m_AssignedItems[ii];
		if( item_ii )
		{
			copy_self->m_AssignedItems.push_back( dynamic_pointer_cast<IfcLayeredItem>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Identifier ) { copy_self->m_Identifier = dynamic_pointer_cast<IfcIdentifier>( m_Identifier->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPresentationLayerAssignment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPRESENTATIONLAYERASSIGNMENT" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_AssignedItems.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcLayeredItem>& type_object = m_AssignedItems[ii];
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
	stream << ",";
	if( m_Identifier ) { m_Identifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPresentationLayerAssignment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPresentationLayerAssignment::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPresentationLayerAssignment, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readSelectList( args[2], m_AssignedItems, map );
	m_Identifier = IfcIdentifier::createObjectFromSTEP( args[3] );
}
void IfcPresentationLayerAssignment::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	if( m_AssignedItems.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> AssignedItems_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_AssignedItems.begin(), m_AssignedItems.end(), std::back_inserter( AssignedItems_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "AssignedItems", AssignedItems_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "Identifier", m_Identifier ) );
}
void IfcPresentationLayerAssignment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPresentationLayerAssignment::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcPresentationLayerAssignment> ptr_self = dynamic_pointer_cast<IfcPresentationLayerAssignment>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcPresentationLayerAssignment::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_AssignedItems.size(); ++i )
	{
		shared_ptr<IfcRepresentation>  AssignedItems_IfcRepresentation = dynamic_pointer_cast<IfcRepresentation>( m_AssignedItems[i] );
		if( AssignedItems_IfcRepresentation )
		{
			AssignedItems_IfcRepresentation->m_LayerAssignments_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcRepresentationItem>  AssignedItems_IfcRepresentationItem = dynamic_pointer_cast<IfcRepresentationItem>( m_AssignedItems[i] );
		if( AssignedItems_IfcRepresentationItem )
		{
			AssignedItems_IfcRepresentationItem->m_LayerAssignment_inverse.push_back( ptr_self );
		}
	}
}
void IfcPresentationLayerAssignment::unlinkFromInverseCounterparts()
{
	for( size_t i=0; i<m_AssignedItems.size(); ++i )
	{
		shared_ptr<IfcRepresentation>  AssignedItems_IfcRepresentation = dynamic_pointer_cast<IfcRepresentation>( m_AssignedItems[i] );
		if( AssignedItems_IfcRepresentation )
		{
			std::vector<weak_ptr<IfcPresentationLayerAssignment> >& LayerAssignments_inverse = AssignedItems_IfcRepresentation->m_LayerAssignments_inverse;
			for( auto it_LayerAssignments_inverse = LayerAssignments_inverse.begin(); it_LayerAssignments_inverse != LayerAssignments_inverse.end(); )
			{
				shared_ptr<IfcPresentationLayerAssignment> self_candidate( *it_LayerAssignments_inverse );
				if( self_candidate.get() == this )
				{
					it_LayerAssignments_inverse= LayerAssignments_inverse.erase( it_LayerAssignments_inverse );
				}
				else
				{
					++it_LayerAssignments_inverse;
				}
			}
		}
		shared_ptr<IfcRepresentationItem>  AssignedItems_IfcRepresentationItem = dynamic_pointer_cast<IfcRepresentationItem>( m_AssignedItems[i] );
		if( AssignedItems_IfcRepresentationItem )
		{
			std::vector<weak_ptr<IfcPresentationLayerAssignment> >& LayerAssignment_inverse = AssignedItems_IfcRepresentationItem->m_LayerAssignment_inverse;
			for( auto it_LayerAssignment_inverse = LayerAssignment_inverse.begin(); it_LayerAssignment_inverse != LayerAssignment_inverse.end(); )
			{
				shared_ptr<IfcPresentationLayerAssignment> self_candidate( *it_LayerAssignment_inverse );
				if( self_candidate.get() == this )
				{
					it_LayerAssignment_inverse= LayerAssignment_inverse.erase( it_LayerAssignment_inverse );
				}
				else
				{
					++it_LayerAssignment_inverse;
				}
			}
		}
	}
}
