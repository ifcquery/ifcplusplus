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
#include "include/IfcAxis2Placement.h"
#include "include/IfcEllipse.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcEllipse 
IfcEllipse::IfcEllipse() { m_entity_enum = IFCELLIPSE; }
IfcEllipse::IfcEllipse( int id ) { m_id = id; m_entity_enum = IFCELLIPSE; }
IfcEllipse::~IfcEllipse() {}
shared_ptr<IfcPPObject> IfcEllipse::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcEllipse> copy_self( new IfcEllipse() );
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement>( m_Position->getDeepCopy(options) ); }
	if( m_SemiAxis1 ) { copy_self->m_SemiAxis1 = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_SemiAxis1->getDeepCopy(options) ); }
	if( m_SemiAxis2 ) { copy_self->m_SemiAxis2 = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_SemiAxis2->getDeepCopy(options) ); }
	return copy_self;
}
void IfcEllipse::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCELLIPSE" << "(";
	if( m_Position ) { m_Position->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_SemiAxis1 ) { m_SemiAxis1->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SemiAxis2 ) { m_SemiAxis2->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcEllipse::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcEllipse::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcEllipse, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Position = IfcAxis2Placement::createObjectFromSTEP( args[0], map );
	m_SemiAxis1 = IfcPositiveLengthMeasure::createObjectFromSTEP( args[1] );
	m_SemiAxis2 = IfcPositiveLengthMeasure::createObjectFromSTEP( args[2] );
}
void IfcEllipse::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConic::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SemiAxis1", m_SemiAxis1 ) );
	vec_attributes.push_back( std::make_pair( "SemiAxis2", m_SemiAxis2 ) );
}
void IfcEllipse::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConic::getAttributesInverse( vec_attributes_inverse );
}
void IfcEllipse::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConic::setInverseCounterparts( ptr_self_entity );
}
void IfcEllipse::unlinkFromInverseCounterparts()
{
	IfcConic::unlinkFromInverseCounterparts();
}
