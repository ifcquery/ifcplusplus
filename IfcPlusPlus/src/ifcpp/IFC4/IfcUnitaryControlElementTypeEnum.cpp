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
#include "include/IfcUnitaryControlElementTypeEnum.h"

// TYPE IfcUnitaryControlElementTypeEnum = ENUMERATION OF	(ALARMPANEL	,CONTROLPANEL	,GASDETECTIONPANEL	,INDICATORPANEL	,MIMICPANEL	,HUMIDISTAT	,THERMOSTAT	,WEATHERSTATION	,USERDEFINED	,NOTDEFINED);
IfcUnitaryControlElementTypeEnum::IfcUnitaryControlElementTypeEnum() {}
IfcUnitaryControlElementTypeEnum::~IfcUnitaryControlElementTypeEnum() {}
shared_ptr<IfcPPObject> IfcUnitaryControlElementTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcUnitaryControlElementTypeEnum> copy_self( new IfcUnitaryControlElementTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcUnitaryControlElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCUNITARYCONTROLELEMENTTYPEENUM("; }
	if( m_enum == ENUM_ALARMPANEL )
	{
		stream << ".ALARMPANEL.";
	}
	else if( m_enum == ENUM_CONTROLPANEL )
	{
		stream << ".CONTROLPANEL.";
	}
	else if( m_enum == ENUM_GASDETECTIONPANEL )
	{
		stream << ".GASDETECTIONPANEL.";
	}
	else if( m_enum == ENUM_INDICATORPANEL )
	{
		stream << ".INDICATORPANEL.";
	}
	else if( m_enum == ENUM_MIMICPANEL )
	{
		stream << ".MIMICPANEL.";
	}
	else if( m_enum == ENUM_HUMIDISTAT )
	{
		stream << ".HUMIDISTAT.";
	}
	else if( m_enum == ENUM_THERMOSTAT )
	{
		stream << ".THERMOSTAT.";
	}
	else if( m_enum == ENUM_WEATHERSTATION )
	{
		stream << ".WEATHERSTATION.";
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
shared_ptr<IfcUnitaryControlElementTypeEnum> IfcUnitaryControlElementTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcUnitaryControlElementTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcUnitaryControlElementTypeEnum>(); }
	shared_ptr<IfcUnitaryControlElementTypeEnum> type_object( new IfcUnitaryControlElementTypeEnum() );
	if( boost::iequals( arg, L".ALARMPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_ALARMPANEL;
	}
	else if( boost::iequals( arg, L".CONTROLPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_CONTROLPANEL;
	}
	else if( boost::iequals( arg, L".GASDETECTIONPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_GASDETECTIONPANEL;
	}
	else if( boost::iequals( arg, L".INDICATORPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_INDICATORPANEL;
	}
	else if( boost::iequals( arg, L".MIMICPANEL." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_MIMICPANEL;
	}
	else if( boost::iequals( arg, L".HUMIDISTAT." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_HUMIDISTAT;
	}
	else if( boost::iequals( arg, L".THERMOSTAT." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_THERMOSTAT;
	}
	else if( boost::iequals( arg, L".WEATHERSTATION." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_WEATHERSTATION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcUnitaryControlElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
