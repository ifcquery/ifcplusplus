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
#include "include/IfcCableSegmentTypeEnum.h"

// TYPE IfcCableSegmentTypeEnum = ENUMERATION OF	(BUSBARSEGMENT	,CABLESEGMENT	,CONDUCTORSEGMENT	,CORESEGMENT	,USERDEFINED	,NOTDEFINED);
IfcCableSegmentTypeEnum::IfcCableSegmentTypeEnum() {}
IfcCableSegmentTypeEnum::~IfcCableSegmentTypeEnum() {}
shared_ptr<IfcPPObject> IfcCableSegmentTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCableSegmentTypeEnum> copy_self( new IfcCableSegmentTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCableSegmentTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCABLESEGMENTTYPEENUM("; }
	if( m_enum == ENUM_BUSBARSEGMENT )
	{
		stream << ".BUSBARSEGMENT.";
	}
	else if( m_enum == ENUM_CABLESEGMENT )
	{
		stream << ".CABLESEGMENT.";
	}
	else if( m_enum == ENUM_CONDUCTORSEGMENT )
	{
		stream << ".CONDUCTORSEGMENT.";
	}
	else if( m_enum == ENUM_CORESEGMENT )
	{
		stream << ".CORESEGMENT.";
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
shared_ptr<IfcCableSegmentTypeEnum> IfcCableSegmentTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCableSegmentTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCableSegmentTypeEnum>(); }
	shared_ptr<IfcCableSegmentTypeEnum> type_object( new IfcCableSegmentTypeEnum() );
	if( boost::iequals( arg, L".BUSBARSEGMENT." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_BUSBARSEGMENT;
	}
	else if( boost::iequals( arg, L".CABLESEGMENT." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_CABLESEGMENT;
	}
	else if( boost::iequals( arg, L".CONDUCTORSEGMENT." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_CONDUCTORSEGMENT;
	}
	else if( boost::iequals( arg, L".CORESEGMENT." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_CORESEGMENT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
