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
#include "include/IfcRightCircularCone.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcRightCircularCone 
IfcRightCircularCone::IfcRightCircularCone() { m_entity_enum = IFCRIGHTCIRCULARCONE; }
IfcRightCircularCone::IfcRightCircularCone( int id ) { m_id = id; m_entity_enum = IFCRIGHTCIRCULARCONE; }
IfcRightCircularCone::~IfcRightCircularCone() {}
shared_ptr<IfcPPObject> IfcRightCircularCone::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRightCircularCone> copy_self( new IfcRightCircularCone() );
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_Height ) { copy_self->m_Height = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Height->getDeepCopy(options) ); }
	if( m_BottomRadius ) { copy_self->m_BottomRadius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_BottomRadius->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRightCircularCone::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRIGHTCIRCULARCONE" << "(";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Height ) { m_Height->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomRadius ) { m_BottomRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRightCircularCone::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRightCircularCone::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRightCircularCone, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Position, map );
	m_Height = IfcPositiveLengthMeasure::createObjectFromSTEP( args[1] );
	m_BottomRadius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[2] );
}
void IfcRightCircularCone::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCsgPrimitive3D::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Height", m_Height ) );
	vec_attributes.push_back( std::make_pair( "BottomRadius", m_BottomRadius ) );
}
void IfcRightCircularCone::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCsgPrimitive3D::getAttributesInverse( vec_attributes_inverse );
}
void IfcRightCircularCone::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCsgPrimitive3D::setInverseCounterparts( ptr_self_entity );
}
void IfcRightCircularCone::unlinkFromInverseCounterparts()
{
	IfcCsgPrimitive3D::unlinkFromInverseCounterparts();
}
