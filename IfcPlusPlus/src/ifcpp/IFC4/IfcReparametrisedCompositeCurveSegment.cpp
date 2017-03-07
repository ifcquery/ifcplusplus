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
#include "include/IfcBoolean.h"
#include "include/IfcCompositeCurve.h"
#include "include/IfcCurve.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcReparametrisedCompositeCurveSegment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTransitionCode.h"

// ENTITY IfcReparametrisedCompositeCurveSegment 
IfcReparametrisedCompositeCurveSegment::IfcReparametrisedCompositeCurveSegment() { m_entity_enum = IFCREPARAMETRISEDCOMPOSITECURVESEGMENT; }
IfcReparametrisedCompositeCurveSegment::IfcReparametrisedCompositeCurveSegment( int id ) { m_id = id; m_entity_enum = IFCREPARAMETRISEDCOMPOSITECURVESEGMENT; }
IfcReparametrisedCompositeCurveSegment::~IfcReparametrisedCompositeCurveSegment() {}
shared_ptr<IfcPPObject> IfcReparametrisedCompositeCurveSegment::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReparametrisedCompositeCurveSegment> copy_self( new IfcReparametrisedCompositeCurveSegment() );
	if( m_Transition ) { copy_self->m_Transition = dynamic_pointer_cast<IfcTransitionCode>( m_Transition->getDeepCopy(options) ); }
	if( m_SameSense ) { copy_self->m_SameSense = dynamic_pointer_cast<IfcBoolean>( m_SameSense->getDeepCopy(options) ); }
	if( m_ParentCurve ) { copy_self->m_ParentCurve = dynamic_pointer_cast<IfcCurve>( m_ParentCurve->getDeepCopy(options) ); }
	if( m_ParamLength ) { copy_self->m_ParamLength = dynamic_pointer_cast<IfcParameterValue>( m_ParamLength->getDeepCopy(options) ); }
	return copy_self;
}
void IfcReparametrisedCompositeCurveSegment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREPARAMETRISEDCOMPOSITECURVESEGMENT" << "(";
	if( m_Transition ) { m_Transition->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_SameSense ) { m_SameSense->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ParentCurve ) { stream << "#" << m_ParentCurve->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_ParamLength ) { m_ParamLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcReparametrisedCompositeCurveSegment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcReparametrisedCompositeCurveSegment::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcReparametrisedCompositeCurveSegment, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Transition = IfcTransitionCode::createObjectFromSTEP( args[0] );
	m_SameSense = IfcBoolean::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_ParentCurve, map );
	m_ParamLength = IfcParameterValue::createObjectFromSTEP( args[3] );
}
void IfcReparametrisedCompositeCurveSegment::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCompositeCurveSegment::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ParamLength", m_ParamLength ) );
}
void IfcReparametrisedCompositeCurveSegment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCompositeCurveSegment::getAttributesInverse( vec_attributes_inverse );
}
void IfcReparametrisedCompositeCurveSegment::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCompositeCurveSegment::setInverseCounterparts( ptr_self_entity );
}
void IfcReparametrisedCompositeCurveSegment::unlinkFromInverseCounterparts()
{
	IfcCompositeCurveSegment::unlinkFromInverseCounterparts();
}
