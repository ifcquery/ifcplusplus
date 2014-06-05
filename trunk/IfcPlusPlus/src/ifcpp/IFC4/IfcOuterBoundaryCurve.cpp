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
#include "include/IfcCompositeCurveSegment.h"
#include "include/IfcOuterBoundaryCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcOuterBoundaryCurve 
IfcOuterBoundaryCurve::IfcOuterBoundaryCurve() {}
IfcOuterBoundaryCurve::IfcOuterBoundaryCurve( int id ) { m_id = id; }
IfcOuterBoundaryCurve::~IfcOuterBoundaryCurve() {}

// method setEntity takes over all attributes from another instance of the class
void IfcOuterBoundaryCurve::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcOuterBoundaryCurve> other = dynamic_pointer_cast<IfcOuterBoundaryCurve>(other_entity);
	if( !other) { return; }
	m_Segments = other->m_Segments;
	m_SelfIntersect = other->m_SelfIntersect;
}
void IfcOuterBoundaryCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCOUTERBOUNDARYCURVE" << "(";
	writeEntityList( stream, m_Segments );
	stream << ",";
	if( m_SelfIntersect == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_SelfIntersect == LOGICAL_TRUE ) { stream << ".T."; }
	else if( m_SelfIntersect == LOGICAL_UNKNOWN ) { stream << ".U."; }
	stream << ");";
}
void IfcOuterBoundaryCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOuterBoundaryCurve::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcOuterBoundaryCurve, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcOuterBoundaryCurve, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Segments, map );
	if( _stricmp( args[1].c_str(), ".F." ) == 0 ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( _stricmp( args[1].c_str(), ".T." ) == 0 ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( _stricmp( args[1].c_str(), ".U." ) == 0 ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
}
void IfcOuterBoundaryCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundaryCurve::getAttributes( vec_attributes );
}
void IfcOuterBoundaryCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcOuterBoundaryCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundaryCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcOuterBoundaryCurve::unlinkSelf()
{
	IfcBoundaryCurve::unlinkSelf();
}
