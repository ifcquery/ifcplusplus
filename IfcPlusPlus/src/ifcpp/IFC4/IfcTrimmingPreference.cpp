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
#include "include/IfcTrimmingPreference.h"

// TYPE IfcTrimmingPreference = ENUMERATION OF	(CARTESIAN	,PARAMETER	,UNSPECIFIED);
IfcTrimmingPreference::IfcTrimmingPreference() {}
IfcTrimmingPreference::~IfcTrimmingPreference() {}
shared_ptr<IfcPPObject> IfcTrimmingPreference::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTrimmingPreference> copy_self( new IfcTrimmingPreference() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTrimmingPreference::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRIMMINGPREFERENCE("; }
	if( m_enum == ENUM_CARTESIAN )
	{
		stream << ".CARTESIAN.";
	}
	else if( m_enum == ENUM_PARAMETER )
	{
		stream << ".PARAMETER.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcTrimmingPreference> IfcTrimmingPreference::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTrimmingPreference>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTrimmingPreference>(); }
	shared_ptr<IfcTrimmingPreference> type_object( new IfcTrimmingPreference() );
	if( boost::iequals( arg, L".CARTESIAN." ) )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_CARTESIAN;
	}
	else if( boost::iequals( arg, L".PARAMETER." ) )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_PARAMETER;
	}
	else if( boost::iequals( arg, L".UNSPECIFIED." ) )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_UNSPECIFIED;
	}
	return type_object;
}
