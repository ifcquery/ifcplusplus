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
#include "include/IfcProtectiveDeviceTypeEnum.h"

// TYPE IfcProtectiveDeviceTypeEnum = ENUMERATION OF	(CIRCUITBREAKER	,EARTHLEAKAGECIRCUITBREAKER	,EARTHINGSWITCH	,FUSEDISCONNECTOR	,RESIDUALCURRENTCIRCUITBREAKER	,RESIDUALCURRENTSWITCH	,VARISTOR	,USERDEFINED	,NOTDEFINED);
IfcProtectiveDeviceTypeEnum::IfcProtectiveDeviceTypeEnum() {}
IfcProtectiveDeviceTypeEnum::~IfcProtectiveDeviceTypeEnum() {}
shared_ptr<IfcPPObject> IfcProtectiveDeviceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcProtectiveDeviceTypeEnum> copy_self( new IfcProtectiveDeviceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcProtectiveDeviceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROTECTIVEDEVICETYPEENUM("; }
	if( m_enum == ENUM_CIRCUITBREAKER )
	{
		stream << ".CIRCUITBREAKER.";
	}
	else if( m_enum == ENUM_EARTHLEAKAGECIRCUITBREAKER )
	{
		stream << ".EARTHLEAKAGECIRCUITBREAKER.";
	}
	else if( m_enum == ENUM_EARTHINGSWITCH )
	{
		stream << ".EARTHINGSWITCH.";
	}
	else if( m_enum == ENUM_FUSEDISCONNECTOR )
	{
		stream << ".FUSEDISCONNECTOR.";
	}
	else if( m_enum == ENUM_RESIDUALCURRENTCIRCUITBREAKER )
	{
		stream << ".RESIDUALCURRENTCIRCUITBREAKER.";
	}
	else if( m_enum == ENUM_RESIDUALCURRENTSWITCH )
	{
		stream << ".RESIDUALCURRENTSWITCH.";
	}
	else if( m_enum == ENUM_VARISTOR )
	{
		stream << ".VARISTOR.";
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
shared_ptr<IfcProtectiveDeviceTypeEnum> IfcProtectiveDeviceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTypeEnum>(); }
	shared_ptr<IfcProtectiveDeviceTypeEnum> type_object( new IfcProtectiveDeviceTypeEnum() );
	if( boost::iequals( arg, L".CIRCUITBREAKER." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_CIRCUITBREAKER;
	}
	else if( boost::iequals( arg, L".EARTHLEAKAGECIRCUITBREAKER." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_EARTHLEAKAGECIRCUITBREAKER;
	}
	else if( boost::iequals( arg, L".EARTHINGSWITCH." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_EARTHINGSWITCH;
	}
	else if( boost::iequals( arg, L".FUSEDISCONNECTOR." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_FUSEDISCONNECTOR;
	}
	else if( boost::iequals( arg, L".RESIDUALCURRENTCIRCUITBREAKER." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_RESIDUALCURRENTCIRCUITBREAKER;
	}
	else if( boost::iequals( arg, L".RESIDUALCURRENTSWITCH." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_RESIDUALCURRENTSWITCH;
	}
	else if( boost::iequals( arg, L".VARISTOR." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_VARISTOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
