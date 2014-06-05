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
#include "include/IfcPoint.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVertexPoint.h"

// ENTITY IfcVertexPoint 
IfcVertexPoint::IfcVertexPoint() {}
IfcVertexPoint::IfcVertexPoint( int id ) { m_id = id; }
IfcVertexPoint::~IfcVertexPoint() {}

// method setEntity takes over all attributes from another instance of the class
void IfcVertexPoint::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcVertexPoint> other = dynamic_pointer_cast<IfcVertexPoint>(other_entity);
	if( !other) { return; }
	m_VertexGeometry = other->m_VertexGeometry;
}
void IfcVertexPoint::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCVERTEXPOINT" << "(";
	if( m_VertexGeometry ) { stream << "#" << m_VertexGeometry->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcVertexPoint::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcVertexPoint::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcVertexPoint, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcVertexPoint, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_VertexGeometry, map );
}
void IfcVertexPoint::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcVertex::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "VertexGeometry", m_VertexGeometry ) );
}
void IfcVertexPoint::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcVertexPoint::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcVertex::setInverseCounterparts( ptr_self_entity );
}
void IfcVertexPoint::unlinkSelf()
{
	IfcVertex::unlinkSelf();
}
