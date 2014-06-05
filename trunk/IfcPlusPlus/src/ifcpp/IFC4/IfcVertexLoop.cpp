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
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVertex.h"
#include "include/IfcVertexLoop.h"

// ENTITY IfcVertexLoop 
IfcVertexLoop::IfcVertexLoop() {}
IfcVertexLoop::IfcVertexLoop( int id ) { m_id = id; }
IfcVertexLoop::~IfcVertexLoop() {}

// method setEntity takes over all attributes from another instance of the class
void IfcVertexLoop::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcVertexLoop> other = dynamic_pointer_cast<IfcVertexLoop>(other_entity);
	if( !other) { return; }
	m_LoopVertex = other->m_LoopVertex;
}
void IfcVertexLoop::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCVERTEXLOOP" << "(";
	if( m_LoopVertex ) { stream << "#" << m_LoopVertex->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcVertexLoop::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcVertexLoop::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcVertexLoop, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcVertexLoop, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_LoopVertex, map );
}
void IfcVertexLoop::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcLoop::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LoopVertex", m_LoopVertex ) );
}
void IfcVertexLoop::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcVertexLoop::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLoop::setInverseCounterparts( ptr_self_entity );
}
void IfcVertexLoop::unlinkSelf()
{
	IfcLoop::unlinkSelf();
}
