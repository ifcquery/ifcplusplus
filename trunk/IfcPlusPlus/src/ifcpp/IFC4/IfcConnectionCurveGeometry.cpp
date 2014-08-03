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
#include "include/IfcConnectionCurveGeometry.h"
#include "include/IfcCurveOrEdgeCurve.h"

// ENTITY IfcConnectionCurveGeometry 
IfcConnectionCurveGeometry::IfcConnectionCurveGeometry() {}
IfcConnectionCurveGeometry::IfcConnectionCurveGeometry( int id ) { m_id = id; }
IfcConnectionCurveGeometry::~IfcConnectionCurveGeometry() {}
shared_ptr<IfcPPObject> IfcConnectionCurveGeometry::getDeepCopy()
{
	shared_ptr<IfcConnectionCurveGeometry> copy_self( new IfcConnectionCurveGeometry() );
	if( m_CurveOnRelatingElement ) { copy_self->m_CurveOnRelatingElement = dynamic_pointer_cast<IfcCurveOrEdgeCurve>( m_CurveOnRelatingElement->getDeepCopy() ); }
	if( m_CurveOnRelatedElement ) { copy_self->m_CurveOnRelatedElement = dynamic_pointer_cast<IfcCurveOrEdgeCurve>( m_CurveOnRelatedElement->getDeepCopy() ); }
	return copy_self;
}
void IfcConnectionCurveGeometry::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCONNECTIONCURVEGEOMETRY" << "(";
	if( m_CurveOnRelatingElement ) { m_CurveOnRelatingElement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_CurveOnRelatedElement ) { m_CurveOnRelatedElement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcConnectionCurveGeometry::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConnectionCurveGeometry::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcConnectionCurveGeometry, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcConnectionCurveGeometry, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_CurveOnRelatingElement = IfcCurveOrEdgeCurve::createObjectFromStepData( args[0], map );
	m_CurveOnRelatedElement = IfcCurveOrEdgeCurve::createObjectFromStepData( args[1], map );
}
void IfcConnectionCurveGeometry::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConnectionGeometry::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "CurveOnRelatingElement", m_CurveOnRelatingElement ) );
	vec_attributes.push_back( std::make_pair( "CurveOnRelatedElement", m_CurveOnRelatedElement ) );
}
void IfcConnectionCurveGeometry::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConnectionGeometry::getAttributesInverse( vec_attributes_inverse );
}
void IfcConnectionCurveGeometry::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConnectionGeometry::setInverseCounterparts( ptr_self_entity );
}
void IfcConnectionCurveGeometry::unlinkSelf()
{
	IfcConnectionGeometry::unlinkSelf();
}
