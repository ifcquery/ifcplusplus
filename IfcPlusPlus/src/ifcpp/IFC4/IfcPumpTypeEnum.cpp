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
#include "include/IfcPumpTypeEnum.h"

// TYPE IfcPumpTypeEnum = ENUMERATION OF	(CIRCULATOR	,ENDSUCTION	,SPLITCASE	,SUBMERSIBLEPUMP	,SUMPPUMP	,VERTICALINLINE	,VERTICALTURBINE	,USERDEFINED	,NOTDEFINED);
IfcPumpTypeEnum::IfcPumpTypeEnum() {}
IfcPumpTypeEnum::~IfcPumpTypeEnum() {}
shared_ptr<IfcPPObject> IfcPumpTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPumpTypeEnum> copy_self( new IfcPumpTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPumpTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPUMPTYPEENUM("; }
	if( m_enum == ENUM_CIRCULATOR )
	{
		stream << ".CIRCULATOR.";
	}
	else if( m_enum == ENUM_ENDSUCTION )
	{
		stream << ".ENDSUCTION.";
	}
	else if( m_enum == ENUM_SPLITCASE )
	{
		stream << ".SPLITCASE.";
	}
	else if( m_enum == ENUM_SUBMERSIBLEPUMP )
	{
		stream << ".SUBMERSIBLEPUMP.";
	}
	else if( m_enum == ENUM_SUMPPUMP )
	{
		stream << ".SUMPPUMP.";
	}
	else if( m_enum == ENUM_VERTICALINLINE )
	{
		stream << ".VERTICALINLINE.";
	}
	else if( m_enum == ENUM_VERTICALTURBINE )
	{
		stream << ".VERTICALTURBINE.";
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
shared_ptr<IfcPumpTypeEnum> IfcPumpTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPumpTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPumpTypeEnum>(); }
	shared_ptr<IfcPumpTypeEnum> type_object( new IfcPumpTypeEnum() );
	if( boost::iequals( arg, L".CIRCULATOR." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_CIRCULATOR;
	}
	else if( boost::iequals( arg, L".ENDSUCTION." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_ENDSUCTION;
	}
	else if( boost::iequals( arg, L".SPLITCASE." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SPLITCASE;
	}
	else if( boost::iequals( arg, L".SUBMERSIBLEPUMP." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SUBMERSIBLEPUMP;
	}
	else if( boost::iequals( arg, L".SUMPPUMP." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SUMPPUMP;
	}
	else if( boost::iequals( arg, L".VERTICALINLINE." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_VERTICALINLINE;
	}
	else if( boost::iequals( arg, L".VERTICALTURBINE." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_VERTICALTURBINE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
