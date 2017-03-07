/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcConnectionPointEccentricity.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPointOrVertexPoint.h"

// ENTITY IfcConnectionPointEccentricity 
IfcConnectionPointEccentricity::IfcConnectionPointEccentricity() { m_entity_enum = IFCCONNECTIONPOINTECCENTRICITY; }
IfcConnectionPointEccentricity::IfcConnectionPointEccentricity( int id ) { m_id = id; m_entity_enum = IFCCONNECTIONPOINTECCENTRICITY; }
IfcConnectionPointEccentricity::~IfcConnectionPointEccentricity() {}
shared_ptr<IfcPPObject> IfcConnectionPointEccentricity::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcConnectionPointEccentricity> copy_self( new IfcConnectionPointEccentricity() );
	if( m_PointOnRelatingElement ) { copy_self->m_PointOnRelatingElement = dynamic_pointer_cast<IfcPointOrVertexPoint>( m_PointOnRelatingElement->getDeepCopy(options) ); }
	if( m_PointOnRelatedElement ) { copy_self->m_PointOnRelatedElement = dynamic_pointer_cast<IfcPointOrVertexPoint>( m_PointOnRelatedElement->getDeepCopy(options) ); }
	if( m_EccentricityInX ) { copy_self->m_EccentricityInX = dynamic_pointer_cast<IfcLengthMeasure>( m_EccentricityInX->getDeepCopy(options) ); }
	if( m_EccentricityInY ) { copy_self->m_EccentricityInY = dynamic_pointer_cast<IfcLengthMeasure>( m_EccentricityInY->getDeepCopy(options) ); }
	if( m_EccentricityInZ ) { copy_self->m_EccentricityInZ = dynamic_pointer_cast<IfcLengthMeasure>( m_EccentricityInZ->getDeepCopy(options) ); }
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
void IfcConnectionPointEccentricity::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcConnectionPointEccentricity, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_PointOnRelatingElement = IfcPointOrVertexPoint::createObjectFromSTEP( args[0], map );
	m_PointOnRelatedElement = IfcPointOrVertexPoint::createObjectFromSTEP( args[1], map );
	m_EccentricityInX = IfcLengthMeasure::createObjectFromSTEP( args[2] );
	m_EccentricityInY = IfcLengthMeasure::createObjectFromSTEP( args[3] );
	m_EccentricityInZ = IfcLengthMeasure::createObjectFromSTEP( args[4] );
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
void IfcConnectionPointEccentricity::unlinkFromInverseCounterparts()
{
	IfcConnectionPointGeometry::unlinkFromInverseCounterparts();
}
