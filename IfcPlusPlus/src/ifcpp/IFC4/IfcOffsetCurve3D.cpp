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
#include "include/IfcDirection.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcOffsetCurve3D.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcOffsetCurve3D 
IfcOffsetCurve3D::IfcOffsetCurve3D() {}
IfcOffsetCurve3D::IfcOffsetCurve3D( int id ) { m_id = id; }
IfcOffsetCurve3D::~IfcOffsetCurve3D() {}

// method setEntity takes over all attributes from another instance of the class
void IfcOffsetCurve3D::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcOffsetCurve3D> other = dynamic_pointer_cast<IfcOffsetCurve3D>(other_entity);
	if( !other) { return; }
	m_BasisCurve = other->m_BasisCurve;
	m_Distance = other->m_Distance;
	m_SelfIntersect = other->m_SelfIntersect;
	m_RefDirection = other->m_RefDirection;
}
void IfcOffsetCurve3D::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCOFFSETCURVE3D" << "(";
	if( m_BasisCurve ) { stream << "#" << m_BasisCurve->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Distance ) { m_Distance->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SelfIntersect == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_SelfIntersect == LOGICAL_TRUE ) { stream << ".T."; }
	else if( m_SelfIntersect == LOGICAL_UNKNOWN ) { stream << ".U."; }
	stream << ",";
	if( m_RefDirection ) { stream << "#" << m_RefDirection->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcOffsetCurve3D::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOffsetCurve3D::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcOffsetCurve3D, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcOffsetCurve3D, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BasisCurve, map );
	m_Distance = IfcLengthMeasure::createObjectFromStepData( args[1] );
	if( _stricmp( args[2].c_str(), ".F." ) == 0 ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( _stricmp( args[2].c_str(), ".T." ) == 0 ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( _stricmp( args[2].c_str(), ".U." ) == 0 ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
	readEntityReference( args[3], m_RefDirection, map );
}
void IfcOffsetCurve3D::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisCurve", m_BasisCurve ) );
	vec_attributes.push_back( std::make_pair( "Distance", m_Distance ) );
	vec_attributes.push_back( std::make_pair( "SelfIntersect", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_SelfIntersect ) ) ) );
	vec_attributes.push_back( std::make_pair( "RefDirection", m_RefDirection ) );
}
void IfcOffsetCurve3D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcOffsetCurve3D::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcOffsetCurve3D::unlinkSelf()
{
	IfcCurve::unlinkSelf();
}
