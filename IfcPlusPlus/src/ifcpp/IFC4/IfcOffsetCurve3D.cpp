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
#include "include/IfcCurve.h"
#include "include/IfcDirection.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcLogical.h"
#include "include/IfcOffsetCurve3D.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcOffsetCurve3D 
IfcOffsetCurve3D::IfcOffsetCurve3D() { m_entity_enum = IFCOFFSETCURVE3D; }
IfcOffsetCurve3D::IfcOffsetCurve3D( int id ) { m_id = id; m_entity_enum = IFCOFFSETCURVE3D; }
IfcOffsetCurve3D::~IfcOffsetCurve3D() {}
shared_ptr<IfcPPObject> IfcOffsetCurve3D::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcOffsetCurve3D> copy_self( new IfcOffsetCurve3D() );
	if( m_BasisCurve ) { copy_self->m_BasisCurve = dynamic_pointer_cast<IfcCurve>( m_BasisCurve->getDeepCopy(options) ); }
	if( m_Distance ) { copy_self->m_Distance = dynamic_pointer_cast<IfcLengthMeasure>( m_Distance->getDeepCopy(options) ); }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	if( m_RefDirection ) { copy_self->m_RefDirection = dynamic_pointer_cast<IfcDirection>( m_RefDirection->getDeepCopy(options) ); }
	return copy_self;
}
void IfcOffsetCurve3D::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCOFFSETCURVE3D" << "(";
	if( m_BasisCurve ) { stream << "#" << m_BasisCurve->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Distance ) { m_Distance->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RefDirection ) { stream << "#" << m_RefDirection->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcOffsetCurve3D::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOffsetCurve3D::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcOffsetCurve3D, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_BasisCurve, map );
	m_Distance = IfcLengthMeasure::createObjectFromSTEP( args[1] );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[2] );
	readEntityReference( args[3], m_RefDirection, map );
}
void IfcOffsetCurve3D::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisCurve", m_BasisCurve ) );
	vec_attributes.push_back( std::make_pair( "Distance", m_Distance ) );
	vec_attributes.push_back( std::make_pair( "SelfIntersect", m_SelfIntersect ) );
	vec_attributes.push_back( std::make_pair( "RefDirection", m_RefDirection ) );
}
void IfcOffsetCurve3D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcOffsetCurve3D::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcOffsetCurve3D::unlinkFromInverseCounterparts()
{
	IfcCurve::unlinkFromInverseCounterparts();
}
