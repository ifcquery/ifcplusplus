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
#include "include/IfcCurveStyleFontAndScaling.h"
#include "include/IfcCurveStyleFontSelect.h"
#include "include/IfcLabel.h"
#include "include/IfcPositiveRatioMeasure.h"

// ENTITY IfcCurveStyleFontAndScaling 
IfcCurveStyleFontAndScaling::IfcCurveStyleFontAndScaling() { m_entity_enum = IFCCURVESTYLEFONTANDSCALING; }
IfcCurveStyleFontAndScaling::IfcCurveStyleFontAndScaling( int id ) { m_id = id; m_entity_enum = IFCCURVESTYLEFONTANDSCALING; }
IfcCurveStyleFontAndScaling::~IfcCurveStyleFontAndScaling() {}
shared_ptr<IfcPPObject> IfcCurveStyleFontAndScaling::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCurveStyleFontAndScaling> copy_self( new IfcCurveStyleFontAndScaling() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_CurveFont ) { copy_self->m_CurveFont = dynamic_pointer_cast<IfcCurveStyleFontSelect>( m_CurveFont->getDeepCopy(options) ); }
	if( m_CurveFontScaling ) { copy_self->m_CurveFontScaling = dynamic_pointer_cast<IfcPositiveRatioMeasure>( m_CurveFontScaling->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCurveStyleFontAndScaling::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCURVESTYLEFONTANDSCALING" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CurveFont ) { m_CurveFont->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_CurveFontScaling ) { m_CurveFontScaling->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCurveStyleFontAndScaling::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurveStyleFontAndScaling::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCurveStyleFontAndScaling, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_CurveFont = IfcCurveStyleFontSelect::createObjectFromSTEP( args[1], map );
	m_CurveFontScaling = IfcPositiveRatioMeasure::createObjectFromSTEP( args[2] );
}
void IfcCurveStyleFontAndScaling::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "CurveFont", m_CurveFont ) );
	vec_attributes.push_back( std::make_pair( "CurveFontScaling", m_CurveFontScaling ) );
}
void IfcCurveStyleFontAndScaling::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcCurveStyleFontAndScaling::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveStyleFontAndScaling::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}
