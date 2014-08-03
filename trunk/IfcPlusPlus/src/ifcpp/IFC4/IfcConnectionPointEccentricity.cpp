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
#include "include/IfcConnectionPointEccentricity.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPointOrVertexPoint.h"

// ENTITY IfcConnectionPointEccentricity 
IfcConnectionPointEccentricity::IfcConnectionPointEccentricity() {}
IfcConnectionPointEccentricity::IfcConnectionPointEccentricity( int id ) { m_id = id; }
IfcConnectionPointEccentricity::~IfcConnectionPointEccentricity() {}
shared_ptr<IfcPPObject> IfcConnectionPointEccentricity::getDeepCopy()
{
	shared_ptr<IfcConnectionPointEccentricity> copy_self( new IfcConnectionPointEccentricity() );
	if( m_PointOnRelatingElement ) { copy_self->m_PointOnRelatingElement = dynamic_pointer_cast<IfcPointOrVertexPoint>( m_PointOnRelatingElement->getDeepCopy() ); }
	if( m_PointOnRelatedElement ) { copy_self->m_PointOnRelatedElement = dynamic_pointer_cast<IfcPointOrVertexPoint>( m_PointOnRelatedElement->getDeepCopy() ); }
	if( m_EccentricityInX ) { copy_self->m_EccentricityInX = dynamic_pointer_cast<IfcLengthMeasure>( m_EccentricityInX->getDeepCopy() ); }
	if( m_EccentricityInY ) { copy_self->m_EccentricityInY = dynamic_pointer_cast<IfcLengthMeasure>( m_EccentricityInY->getDeepCopy() ); }
	if( m_EccentricityInZ ) { copy_self->m_EccentricityInZ = dynamic_pointer_cast<IfcLengthMeasure>( m_EccentricityInZ->getDeepCopy() ); }
	return copy_self;
}
void IfcConnectionPointEccentricity::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCONNECTIONPOINTECCENTRICITY" << "(";
	if( m_PointOnRelatingElement ) { m_PointOnRelatingElement->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_PointOnRelatedElement ) { m_PointOnRelatedElement->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_EccentricityInX ) { m_EccentricityInX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EccentricityInY ) { m_EccentricityInY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EccentricityInZ ) { m_EccentricityInZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcConnectionPointEccentricity::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConnectionPointEccentricity::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcConnectionPointEccentricity, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcConnectionPointEccentricity, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_PointOnRelatingElement = IfcPointOrVertexPoint::createObjectFromStepData( args[0], map );
	m_PointOnRelatedElement = IfcPointOrVertexPoint::createObjectFromStepData( args[1], map );
	m_EccentricityInX = IfcLengthMeasure::createObjectFromStepData( args[2] );
	m_EccentricityInY = IfcLengthMeasure::createObjectFromStepData( args[3] );
	m_EccentricityInZ = IfcLengthMeasure::createObjectFromStepData( args[4] );
}
void IfcConnectionPointEccentricity::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConnectionPointGeometry::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "EccentricityInX", m_EccentricityInX ) );
	vec_attributes.push_back( std::make_pair( "EccentricityInY", m_EccentricityInY ) );
	vec_attributes.push_back( std::make_pair( "EccentricityInZ", m_EccentricityInZ ) );
}
void IfcConnectionPointEccentricity::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConnectionPointGeometry::getAttributesInverse( vec_attributes_inverse );
}
void IfcConnectionPointEccentricity::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConnectionPointGeometry::setInverseCounterparts( ptr_self_entity );
}
void IfcConnectionPointEccentricity::unlinkSelf()
{
	IfcConnectionPointGeometry::unlinkSelf();
}
