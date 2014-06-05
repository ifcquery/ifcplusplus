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
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcLayeredItem.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRepresentation.h"
#include "include/IfcRepresentationItem.h"
#include "include/IfcText.h"

// ENTITY IfcPresentationLayerAssignment 
IfcPresentationLayerAssignment::IfcPresentationLayerAssignment() {}
IfcPresentationLayerAssignment::IfcPresentationLayerAssignment( int id ) { m_id = id; }
IfcPresentationLayerAssignment::~IfcPresentationLayerAssignment() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPresentationLayerAssignment::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPresentationLayerAssignment> other = dynamic_pointer_cast<IfcPresentationLayerAssignment>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_AssignedItems = other->m_AssignedItems;
	m_Identifier = other->m_Identifier;
}
void IfcPresentationLayerAssignment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPRESENTATIONLAYERASSIGNMENT" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_AssignedItems, true );
	stream << ",";
	if( m_Identifier ) { m_Identifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPresentationLayerAssignment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPresentationLayerAssignment::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPresentationLayerAssignment, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcPresentationLayerAssignment, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readSelectList( args[2], m_AssignedItems, map );
	m_Identifier = IfcIdentifier::createObjectFromStepData( args[3] );
}
void IfcPresentationLayerAssignment::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	shared_ptr<IfcPPAttributeObjectVector> AssignedItems_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_AssignedItems.begin(), m_AssignedItems.end(), std::back_inserter( AssignedItems_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "AssignedItems", AssignedItems_vec_object ) );
	vec_attributes.push_back( std::make_pair( "Identifier", m_Identifier ) );
}
void IfcPresentationLayerAssignment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPresentationLayerAssignment::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcPresentationLayerAssignment> ptr_self = dynamic_pointer_cast<IfcPresentationLayerAssignment>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcPresentationLayerAssignment::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_AssignedItems.size(); ++i )
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
void IfcPresentationLayerAssignment::unlinkSelf()
{
	for( int i=0; i<m_AssignedItems.size(); ++i )
	{
		shared_ptr<IfcRepresentation>  AssignedItems_IfcRepresentation = dynamic_pointer_cast<IfcRepresentation>( m_AssignedItems[i] );
		if( AssignedItems_IfcRepresentation )
		{
			std::vector<weak_ptr<IfcPresentationLayerAssignment> >& LayerAssignments_inverse = AssignedItems_IfcRepresentation->m_LayerAssignments_inverse;
			std::vector<weak_ptr<IfcPresentationLayerAssignment> >::iterator it_LayerAssignments_inverse;
			for( it_LayerAssignments_inverse = LayerAssignments_inverse.begin(); it_LayerAssignments_inverse != LayerAssignments_inverse.end(); ++it_LayerAssignments_inverse)
			{
				shared_ptr<IfcPresentationLayerAssignment> self_candidate( *it_LayerAssignments_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					LayerAssignments_inverse.erase( it_LayerAssignments_inverse );
					break;
				}
			}
		}
		shared_ptr<IfcRepresentationItem>  AssignedItems_IfcRepresentationItem = dynamic_pointer_cast<IfcRepresentationItem>( m_AssignedItems[i] );
		if( AssignedItems_IfcRepresentationItem )
		{
			std::vector<weak_ptr<IfcPresentationLayerAssignment> >& LayerAssignment_inverse = AssignedItems_IfcRepresentationItem->m_LayerAssignment_inverse;
			std::vector<weak_ptr<IfcPresentationLayerAssignment> >::iterator it_LayerAssignment_inverse;
			for( it_LayerAssignment_inverse = LayerAssignment_inverse.begin(); it_LayerAssignment_inverse != LayerAssignment_inverse.end(); ++it_LayerAssignment_inverse)
			{
				shared_ptr<IfcPresentationLayerAssignment> self_candidate( *it_LayerAssignment_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					LayerAssignment_inverse.erase( it_LayerAssignment_inverse );
					break;
				}
			}
		}
	}
}
