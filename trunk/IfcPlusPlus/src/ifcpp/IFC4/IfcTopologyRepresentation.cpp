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
#include "include/IfcRepresentationContext.h"
#include "include/IfcRepresentationItem.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcShapeAspect.h"
#include "include/IfcTopologyRepresentation.h"

// ENTITY IfcTopologyRepresentation 
IfcTopologyRepresentation::IfcTopologyRepresentation() {}
IfcTopologyRepresentation::IfcTopologyRepresentation( int id ) { m_id = id; }
IfcTopologyRepresentation::~IfcTopologyRepresentation() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTopologyRepresentation::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTopologyRepresentation> other = dynamic_pointer_cast<IfcTopologyRepresentation>(other_entity);
	if( !other) { return; }
	m_ContextOfItems = other->m_ContextOfItems;
	m_RepresentationIdentifier = other->m_RepresentationIdentifier;
	m_RepresentationType = other->m_RepresentationType;
	m_Items = other->m_Items;
}
void IfcTopologyRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTOPOLOGYREPRESENTATION" << "(";
	if( m_ContextOfItems ) { stream << "#" << m_ContextOfItems->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RepresentationIdentifier ) { m_RepresentationIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RepresentationType ) { m_RepresentationType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Items );
	stream << ");";
}
void IfcTopologyRepresentation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTopologyRepresentation::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTopologyRepresentation, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcTopologyRepresentation, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_ContextOfItems, map );
	m_RepresentationIdentifier = IfcLabel::createObjectFromStepData( args[1] );
	m_RepresentationType = IfcLabel::createObjectFromStepData( args[2] );
	readEntityReferenceList( args[3], m_Items, map );
}
void IfcTopologyRepresentation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcShapeModel::getAttributes( vec_attributes );
}
void IfcTopologyRepresentation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTopologyRepresentation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcShapeModel::setInverseCounterparts( ptr_self_entity );
}
void IfcTopologyRepresentation::unlinkSelf()
{
	IfcShapeModel::unlinkSelf();
}
