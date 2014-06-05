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
#include "include/IfcCurve.h"
#include "include/IfcEdgeCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVertex.h"

// ENTITY IfcEdgeCurve 
IfcEdgeCurve::IfcEdgeCurve() {}
IfcEdgeCurve::IfcEdgeCurve( int id ) { m_id = id; }
IfcEdgeCurve::~IfcEdgeCurve() {}

// method setEntity takes over all attributes from another instance of the class
void IfcEdgeCurve::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcEdgeCurve> other = dynamic_pointer_cast<IfcEdgeCurve>(other_entity);
	if( !other) { return; }
	m_EdgeStart = other->m_EdgeStart;
	m_EdgeEnd = other->m_EdgeEnd;
	m_EdgeGeometry = other->m_EdgeGeometry;
	m_SameSense = other->m_SameSense;
}
void IfcEdgeCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCEDGECURVE" << "(";
	if( m_EdgeStart ) { stream << "#" << m_EdgeStart->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_EdgeEnd ) { stream << "#" << m_EdgeEnd->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_EdgeGeometry ) { stream << "#" << m_EdgeGeometry->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_SameSense == false ) { stream << ".F."; }
	else if( m_SameSense == true ) { stream << ".T."; }
	stream << ");";
}
void IfcEdgeCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcEdgeCurve::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcEdgeCurve, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcEdgeCurve, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_EdgeStart, map );
	readEntityReference( args[1], m_EdgeEnd, map );
	readEntityReference( args[2], m_EdgeGeometry, map );
	if( _stricmp( args[3].c_str(), ".F." ) == 0 ) { m_SameSense = false; }
	else if( _stricmp( args[3].c_str(), ".T." ) == 0 ) { m_SameSense = true; }
}
void IfcEdgeCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcEdge::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "EdgeGeometry", m_EdgeGeometry ) );
	vec_attributes.push_back( std::make_pair( "SameSense", shared_ptr<IfcPPBool>( new IfcPPBool( m_SameSense ) ) ) );
}
void IfcEdgeCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcEdgeCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcEdge::setInverseCounterparts( ptr_self_entity );
}
void IfcEdgeCurve::unlinkSelf()
{
	IfcEdge::unlinkSelf();
}
