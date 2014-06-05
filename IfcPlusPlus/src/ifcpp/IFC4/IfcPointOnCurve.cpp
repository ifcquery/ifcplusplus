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
#include "include/IfcParameterValue.h"
#include "include/IfcPointOnCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcPointOnCurve 
IfcPointOnCurve::IfcPointOnCurve() {}
IfcPointOnCurve::IfcPointOnCurve( int id ) { m_id = id; }
IfcPointOnCurve::~IfcPointOnCurve() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPointOnCurve::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPointOnCurve> other = dynamic_pointer_cast<IfcPointOnCurve>(other_entity);
	if( !other) { return; }
	m_BasisCurve = other->m_BasisCurve;
	m_PointParameter = other->m_PointParameter;
}
void IfcPointOnCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPOINTONCURVE" << "(";
	if( m_BasisCurve ) { stream << "#" << m_BasisCurve->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PointParameter ) { m_PointParameter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPointOnCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPointOnCurve::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPointOnCurve, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcPointOnCurve, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BasisCurve, map );
	m_PointParameter = IfcParameterValue::createObjectFromStepData( args[1] );
}
void IfcPointOnCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPoint::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisCurve", m_BasisCurve ) );
	vec_attributes.push_back( std::make_pair( "PointParameter", m_PointParameter ) );
}
void IfcPointOnCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPointOnCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPoint::setInverseCounterparts( ptr_self_entity );
}
void IfcPointOnCurve::unlinkSelf()
{
	IfcPoint::unlinkSelf();
}
