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
#include "include/IfcPipeSegmentTypeEnum.h"

// TYPE IfcPipeSegmentTypeEnum = ENUMERATION OF	(CULVERT	,FLEXIBLESEGMENT	,RIGIDSEGMENT	,GUTTER	,SPOOL	,USERDEFINED	,NOTDEFINED);
IfcPipeSegmentTypeEnum::IfcPipeSegmentTypeEnum() {}
IfcPipeSegmentTypeEnum::~IfcPipeSegmentTypeEnum() {}
shared_ptr<IfcPPObject> IfcPipeSegmentTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPipeSegmentTypeEnum> copy_self( new IfcPipeSegmentTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPipeSegmentTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPIPESEGMENTTYPEENUM("; }
	if( m_enum == ENUM_CULVERT )
	{
		stream << ".CULVERT.";
	}
	else if( m_enum == ENUM_FLEXIBLESEGMENT )
	{
		stream << ".FLEXIBLESEGMENT.";
	}
	else if( m_enum == ENUM_RIGIDSEGMENT )
	{
		stream << ".RIGIDSEGMENT.";
	}
	else if( m_enum == ENUM_GUTTER )
	{
		stream << ".GUTTER.";
	}
	else if( m_enum == ENUM_SPOOL )
	{
		stream << ".SPOOL.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcPipeSegmentTypeEnum> IfcPipeSegmentTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPipeSegmentTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPipeSegmentTypeEnum>(); }
	shared_ptr<IfcPipeSegmentTypeEnum> type_object( new IfcPipeSegmentTypeEnum() );
	if( boost::iequals( arg, L".CULVERT." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_CULVERT;
	}
	else if( boost::iequals( arg, L".FLEXIBLESEGMENT." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_FLEXIBLESEGMENT;
	}
	else if( boost::iequals( arg, L".RIGIDSEGMENT." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_RIGIDSEGMENT;
	}
	else if( boost::iequals( arg, L".GUTTER." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_GUTTER;
	}
	else if( boost::iequals( arg, L".SPOOL." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_SPOOL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
