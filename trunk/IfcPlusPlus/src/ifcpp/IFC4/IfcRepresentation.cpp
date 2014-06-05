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

// method setEntity takes over all attributes from another instance of the class
void IfcRepresentation::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRepresentation> other = dynamic_pointer_cast<IfcRepresentation>(other_entity);
	if( !other) { return; }
	m_ContextOfItems = other->m_ContextOfItems;
	m_RepresentationIdentifier = other->m_RepresentationIdentifier;
	m_RepresentationType = other->m_RepresentationType;
	m_Items = other->m_Items;
}
void IfcRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCREPRESENTATION" << "(";
	if( m_ContextOfItems ) { stream << "#" << m_ContextOfItems->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RepresentationIdentifier ) { m_RepresentationIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RepresentationType ) { m_RepresentationType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Items );
	stream << ");";
}
void IfcRepresentation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRepresentation::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRepresentation, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcRepresentation, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_ContextOfItems, map );
	m_RepresentationIdentifier = IfcLabel::createObjectFromStepData( args[1] );
	m_RepresentationType = IfcLabel::createObjectFromStepData( args[2] );
	readEntityReferenceList( args[3], m_Items, map );
}
void IfcRepresentation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "ContextOfItems", m_ContextOfItems ) );
	vec_attributes.push_back( std::make_pair( "RepresentationIdentifier", m_RepresentationIdentifier ) );
	vec_attributes.push_back( std::make_pair( "RepresentationType", m_RepresentationType ) );
}
void IfcRepresentation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> RepresentationMap_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_RepresentationMap_inverse.size(); ++i ) { RepresentationMap_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRepresentationMap>( m_RepresentationMap_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "RepresentationMap_inverse", RepresentationMap_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> LayerAssignments_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_LayerAssignments_inverse.size(); ++i ) { LayerAssignments_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPresentationLayerAssignment>( m_LayerAssignments_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "LayerAssignments_inverse", LayerAssignments_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> OfProductRepresentation_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_OfProductRepresentation_inverse.size(); ++i ) { OfProductRepresentation_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcProductRepresentation>( m_OfProductRepresentation_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "OfProductRepresentation_inverse", OfProductRepresentation_inverse_vec_obj ) );
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
		std::vector<weak_ptr<IfcRepresentation> >::iterator it_RepresentationsInContext_inverse;
		for( it_RepresentationsInContext_inverse = RepresentationsInContext_inverse.begin(); it_RepresentationsInContext_inverse != RepresentationsInContext_inverse.end(); ++it_RepresentationsInContext_inverse)
		{
			shared_ptr<IfcRepresentation> self_candidate( *it_RepresentationsInContext_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				RepresentationsInContext_inverse.erase( it_RepresentationsInContext_inverse );
				break;
			}
		}
	}
}
