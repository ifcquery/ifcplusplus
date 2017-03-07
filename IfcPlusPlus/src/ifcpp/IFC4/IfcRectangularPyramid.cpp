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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRectangularPyramid.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcRectangularPyramid 
IfcRectangularPyramid::IfcRectangularPyramid() { m_entity_enum = IFCRECTANGULARPYRAMID; }
IfcRectangularPyramid::IfcRectangularPyramid( int id ) { m_id = id; m_entity_enum = IFCRECTANGULARPYRAMID; }
IfcRectangularPyramid::~IfcRectangularPyramid() {}
shared_ptr<IfcPPObject> IfcRectangularPyramid::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRectangularPyramid> copy_self( new IfcRectangularPyramid() );
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_XLength ) { copy_self->m_XLength = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_XLength->getDeepCopy(options) ); }
	if( m_YLength ) { copy_self->m_YLength = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_YLength->getDeepCopy(options) ); }
	if( m_Height ) { copy_self->m_Height = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Height->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRectangularPyramid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRECTANGULARPYRAMID" << "(";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_XLength ) { m_XLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_YLength ) { m_YLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Height ) { m_Height->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRectangularPyramid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRectangularPyramid::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRectangularPyramid, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Position, map );
	m_XLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[1] );
	m_YLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[2] );
	m_Height = IfcPositiveLengthMeasure::createObjectFromSTEP( args[3] );
}
void IfcRectangularPyramid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCsgPrimitive3D::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "XLength", m_XLength ) );
	vec_attributes.push_back( std::make_pair( "YLength", m_YLength ) );
	vec_attributes.push_back( std::make_pair( "Height", m_Height ) );
}
void IfcRectangularPyramid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCsgPrimitive3D::getAttributesInverse( vec_attributes_inverse );
}
void IfcRectangularPyramid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCsgPrimitive3D::setInverseCounterparts( ptr_self_entity );
}
void IfcRectangularPyramid::unlinkFromInverseCounterparts()
{
	IfcCsgPrimitive3D::unlinkFromInverseCounterparts();
}
