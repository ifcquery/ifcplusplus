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
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcBSplineCurveForm.h"

// TYPE IfcBSplineCurveForm = ENUMERATION OF	(POLYLINE_FORM	,CIRCULAR_ARC	,ELLIPTIC_ARC	,PARABOLIC_ARC	,HYPERBOLIC_ARC	,UNSPECIFIED);
IfcBSplineCurveForm::IfcBSplineCurveForm() {}
IfcBSplineCurveForm::~IfcBSplineCurveForm() {}
shared_ptr<IfcPPObject> IfcBSplineCurveForm::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBSplineCurveForm> copy_self( new IfcBSplineCurveForm() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBSplineCurveForm::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBSPLINECURVEFORM("; }
	if( m_enum == ENUM_POLYLINE_FORM )
	{
		stream << ".POLYLINE_FORM.";
	}
	else if( m_enum == ENUM_CIRCULAR_ARC )
	{
		stream << ".CIRCULAR_ARC.";
	}
	else if( m_enum == ENUM_ELLIPTIC_ARC )
	{
		stream << ".ELLIPTIC_ARC.";
	}
	else if( m_enum == ENUM_PARABOLIC_ARC )
	{
		stream << ".PARABOLIC_ARC.";
	}
	else if( m_enum == ENUM_HYPERBOLIC_ARC )
	{
		stream << ".HYPERBOLIC_ARC.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBSplineCurveForm> IfcBSplineCurveForm::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBSplineCurveForm>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBSplineCurveForm>(); }
	shared_ptr<IfcBSplineCurveForm> type_object( new IfcBSplineCurveForm() );
	if( boost::iequals( arg, L".POLYLINE_FORM." ) )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_POLYLINE_FORM;
	}
	else if( boost::iequals( arg, L".CIRCULAR_ARC." ) )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_CIRCULAR_ARC;
	}
	else if( boost::iequals( arg, L".ELLIPTIC_ARC." ) )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_ELLIPTIC_ARC;
	}
	else if( boost::iequals( arg, L".PARABOLIC_ARC." ) )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_PARABOLIC_ARC;
	}
	else if( boost::iequals( arg, L".HYPERBOLIC_ARC." ) )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_HYPERBOLIC_ARC;
	}
	else if( boost::iequals( arg, L".UNSPECIFIED." ) )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_UNSPECIFIED;
	}
	return type_object;
}
