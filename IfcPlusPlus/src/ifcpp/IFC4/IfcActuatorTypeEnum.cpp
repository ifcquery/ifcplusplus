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
#include "include/IfcActuatorTypeEnum.h"

// TYPE IfcActuatorTypeEnum = ENUMERATION OF	(ELECTRICACTUATOR	,HANDOPERATEDACTUATOR	,HYDRAULICACTUATOR	,PNEUMATICACTUATOR	,THERMOSTATICACTUATOR	,USERDEFINED	,NOTDEFINED);
IfcActuatorTypeEnum::IfcActuatorTypeEnum() {}
IfcActuatorTypeEnum::~IfcActuatorTypeEnum() {}
shared_ptr<IfcPPObject> IfcActuatorTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcActuatorTypeEnum> copy_self( new IfcActuatorTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcActuatorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCACTUATORTYPEENUM("; }
	if( m_enum == ENUM_ELECTRICACTUATOR )
	{
		stream << ".ELECTRICACTUATOR.";
	}
	else if( m_enum == ENUM_HANDOPERATEDACTUATOR )
	{
		stream << ".HANDOPERATEDACTUATOR.";
	}
	else if( m_enum == ENUM_HYDRAULICACTUATOR )
	{
		stream << ".HYDRAULICACTUATOR.";
	}
	else if( m_enum == ENUM_PNEUMATICACTUATOR )
	{
		stream << ".PNEUMATICACTUATOR.";
	}
	else if( m_enum == ENUM_THERMOSTATICACTUATOR )
	{
		stream << ".THERMOSTATICACTUATOR.";
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
shared_ptr<IfcActuatorTypeEnum> IfcActuatorTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcActuatorTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcActuatorTypeEnum>(); }
	shared_ptr<IfcActuatorTypeEnum> type_object( new IfcActuatorTypeEnum() );
	if( boost::iequals( arg, L".ELECTRICACTUATOR." ) )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_ELECTRICACTUATOR;
	}
	else if( boost::iequals( arg, L".HANDOPERATEDACTUATOR." ) )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_HANDOPERATEDACTUATOR;
	}
	else if( boost::iequals( arg, L".HYDRAULICACTUATOR." ) )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_HYDRAULICACTUATOR;
	}
	else if( boost::iequals( arg, L".PNEUMATICACTUATOR." ) )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_PNEUMATICACTUATOR;
	}
	else if( boost::iequals( arg, L".THERMOSTATICACTUATOR." ) )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_THERMOSTATICACTUATOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcActuatorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
