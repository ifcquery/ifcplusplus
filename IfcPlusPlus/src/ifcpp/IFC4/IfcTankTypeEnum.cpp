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
#include "include/IfcTankTypeEnum.h"

// TYPE IfcTankTypeEnum = ENUMERATION OF	(BASIN	,BREAKPRESSURE	,EXPANSION	,FEEDANDEXPANSION	,PRESSUREVESSEL	,STORAGE	,VESSEL	,USERDEFINED	,NOTDEFINED);
IfcTankTypeEnum::IfcTankTypeEnum() {}
IfcTankTypeEnum::~IfcTankTypeEnum() {}
shared_ptr<IfcPPObject> IfcTankTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTankTypeEnum> copy_self( new IfcTankTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTankTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTANKTYPEENUM("; }
	if( m_enum == ENUM_BASIN )
	{
		stream << ".BASIN.";
	}
	else if( m_enum == ENUM_BREAKPRESSURE )
	{
		stream << ".BREAKPRESSURE.";
	}
	else if( m_enum == ENUM_EXPANSION )
	{
		stream << ".EXPANSION.";
	}
	else if( m_enum == ENUM_FEEDANDEXPANSION )
	{
		stream << ".FEEDANDEXPANSION.";
	}
	else if( m_enum == ENUM_PRESSUREVESSEL )
	{
		stream << ".PRESSUREVESSEL.";
	}
	else if( m_enum == ENUM_STORAGE )
	{
		stream << ".STORAGE.";
	}
	else if( m_enum == ENUM_VESSEL )
	{
		stream << ".VESSEL.";
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
shared_ptr<IfcTankTypeEnum> IfcTankTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTankTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTankTypeEnum>(); }
	shared_ptr<IfcTankTypeEnum> type_object( new IfcTankTypeEnum() );
	if( boost::iequals( arg, L".BASIN." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_BASIN;
	}
	else if( boost::iequals( arg, L".BREAKPRESSURE." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_BREAKPRESSURE;
	}
	else if( boost::iequals( arg, L".EXPANSION." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_EXPANSION;
	}
	else if( boost::iequals( arg, L".FEEDANDEXPANSION." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_FEEDANDEXPANSION;
	}
	else if( boost::iequals( arg, L".PRESSUREVESSEL." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_PRESSUREVESSEL;
	}
	else if( boost::iequals( arg, L".STORAGE." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_STORAGE;
	}
	else if( boost::iequals( arg, L".VESSEL." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_VESSEL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
