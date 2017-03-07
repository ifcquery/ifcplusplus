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
#include "include/IfcTransitionCode.h"

// TYPE IfcTransitionCode = ENUMERATION OF	(DISCONTINUOUS	,CONTINUOUS	,CONTSAMEGRADIENT	,CONTSAMEGRADIENTSAMECURVATURE);
IfcTransitionCode::IfcTransitionCode() {}
IfcTransitionCode::~IfcTransitionCode() {}
shared_ptr<IfcPPObject> IfcTransitionCode::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTransitionCode> copy_self( new IfcTransitionCode() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTransitionCode::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRANSITIONCODE("; }
	if( m_enum == ENUM_DISCONTINUOUS )
	{
		stream << ".DISCONTINUOUS.";
	}
	else if( m_enum == ENUM_CONTINUOUS )
	{
		stream << ".CONTINUOUS.";
	}
	else if( m_enum == ENUM_CONTSAMEGRADIENT )
	{
		stream << ".CONTSAMEGRADIENT.";
	}
	else if( m_enum == ENUM_CONTSAMEGRADIENTSAMECURVATURE )
	{
		stream << ".CONTSAMEGRADIENTSAMECURVATURE.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcTransitionCode> IfcTransitionCode::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTransitionCode>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTransitionCode>(); }
	shared_ptr<IfcTransitionCode> type_object( new IfcTransitionCode() );
	if( boost::iequals( arg, L".DISCONTINUOUS." ) )
	{
		type_object->m_enum = IfcTransitionCode::ENUM_DISCONTINUOUS;
	}
	else if( boost::iequals( arg, L".CONTINUOUS." ) )
	{
		type_object->m_enum = IfcTransitionCode::ENUM_CONTINUOUS;
	}
	else if( boost::iequals( arg, L".CONTSAMEGRADIENT." ) )
	{
		type_object->m_enum = IfcTransitionCode::ENUM_CONTSAMEGRADIENT;
	}
	else if( boost::iequals( arg, L".CONTSAMEGRADIENTSAMECURVATURE." ) )
	{
		type_object->m_enum = IfcTransitionCode::ENUM_CONTSAMEGRADIENTSAMECURVATURE;
	}
	return type_object;
}
