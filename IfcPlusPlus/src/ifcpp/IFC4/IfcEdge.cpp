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
#include "include/IfcEdge.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVertex.h"

// ENTITY IfcEdge 
IfcEdge::IfcEdge() {}
IfcEdge::IfcEdge( int id ) { m_id = id; }
IfcEdge::~IfcEdge() {}

// method setEntity takes over all attributes from another instance of the class
void IfcEdge::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcEdge> other = dynamic_pointer_cast<IfcEdge>(other_entity);
	if( !other) { return; }
	m_EdgeStart = other->m_EdgeStart;
	m_EdgeEnd = other->m_EdgeEnd;
}
void IfcEdge::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCEDGE" << "(";
	if( m_EdgeStart ) { stream << "#" << m_EdgeStart->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_EdgeEnd ) { stream << "#" << m_EdgeEnd->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcEdge::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcEdge::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcEdge, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcEdge, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_EdgeStart, map );
	readEntityReference( args[1], m_EdgeEnd, map );
}
void IfcEdge::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTopologicalRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "EdgeStart", m_EdgeStart ) );
	vec_attributes.push_back( std::make_pair( "EdgeEnd", m_EdgeEnd ) );
}
void IfcEdge::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcEdge::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTopologicalRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcEdge::unlinkSelf()
{
	IfcTopologicalRepresentationItem::unlinkSelf();
}
