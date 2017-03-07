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
#include "include/IfcSurfaceSide.h"

// TYPE IfcSurfaceSide = ENUMERATION OF	(POSITIVE	,NEGATIVE	,BOTH);
IfcSurfaceSide::IfcSurfaceSide() {}
IfcSurfaceSide::~IfcSurfaceSide() {}
shared_ptr<IfcPPObject> IfcSurfaceSide::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceSide> copy_self( new IfcSurfaceSide() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSurfaceSide::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSURFACESIDE("; }
	if( m_enum == ENUM_POSITIVE )
	{
		stream << ".POSITIVE.";
	}
	else if( m_enum == ENUM_NEGATIVE )
	{
		stream << ".NEGATIVE.";
	}
	else if( m_enum == ENUM_BOTH )
	{
		stream << ".BOTH.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSurfaceSide> IfcSurfaceSide::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSurfaceSide>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSurfaceSide>(); }
	shared_ptr<IfcSurfaceSide> type_object( new IfcSurfaceSide() );
	if( boost::iequals( arg, L".POSITIVE." ) )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_POSITIVE;
	}
	else if( boost::iequals( arg, L".NEGATIVE." ) )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_NEGATIVE;
	}
	else if( boost::iequals( arg, L".BOTH." ) )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_BOTH;
	}
	return type_object;
}
