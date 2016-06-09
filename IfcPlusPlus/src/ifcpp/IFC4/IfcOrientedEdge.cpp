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
#include "include/IfcBoolean.h"
#include "include/IfcEdge.h"
#include "include/IfcOrientedEdge.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVertex.h"

// ENTITY IfcOrientedEdge 
IfcOrientedEdge::IfcOrientedEdge() { m_entity_enum = IFCORIENTEDEDGE; }
IfcOrientedEdge::IfcOrientedEdge( int id ) { m_id = id; m_entity_enum = IFCORIENTEDEDGE; }
IfcOrientedEdge::~IfcOrientedEdge() {}
shared_ptr<IfcPPObject> IfcOrientedEdge::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcOrientedEdge> copy_self( new IfcOrientedEdge() );
	if( m_EdgeStart ) { copy_self->m_EdgeStart = dynamic_pointer_cast<IfcVertex>( m_EdgeStart->getDeepCopy(options) ); }
	if( m_EdgeEnd ) { copy_self->m_EdgeEnd = dynamic_pointer_cast<IfcVertex>( m_EdgeEnd->getDeepCopy(options) ); }
	if( m_EdgeElement ) { copy_self->m_EdgeElement = dynamic_pointer_cast<IfcEdge>( m_EdgeElement->getDeepCopy(options) ); }
	if( m_Orientation ) { copy_self->m_Orientation = dynamic_pointer_cast<IfcBoolean>( m_Orientation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcOrientedEdge::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCORIENTEDEDGE" << "(";
	if( m_EdgeStart ) { stream << "#" << m_EdgeStart->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_EdgeEnd ) { stream << "#" << m_EdgeEnd->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_EdgeElement ) { stream << "#" << m_EdgeElement->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Orientation ) { m_Orientation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcOrientedEdge::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOrientedEdge::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcOrientedEdge, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_EdgeStart, map );
	readEntityReference( args[1], m_EdgeEnd, map );
	readEntityReference( args[2], m_EdgeElement, map );
	m_Orientation = IfcBoolean::createObjectFromSTEP( args[3] );
}
void IfcOrientedEdge::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcEdge::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "EdgeElement", m_EdgeElement ) );
	vec_attributes.push_back( std::make_pair( "Orientation", m_Orientation ) );
}
void IfcOrientedEdge::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcEdge::getAttributesInverse( vec_attributes_inverse );
}
void IfcOrientedEdge::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcEdge::setInverseCounterparts( ptr_self_entity );
}
void IfcOrientedEdge::unlinkFromInverseCounterparts()
{
	IfcEdge::unlinkFromInverseCounterparts();
}
