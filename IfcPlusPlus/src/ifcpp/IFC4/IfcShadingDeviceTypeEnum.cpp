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
#include "include/IfcShadingDeviceTypeEnum.h"

// TYPE IfcShadingDeviceTypeEnum = ENUMERATION OF	(JALOUSIE	,SHUTTER	,AWNING	,USERDEFINED	,NOTDEFINED);
IfcShadingDeviceTypeEnum::IfcShadingDeviceTypeEnum() {}
IfcShadingDeviceTypeEnum::~IfcShadingDeviceTypeEnum() {}
shared_ptr<IfcPPObject> IfcShadingDeviceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcShadingDeviceTypeEnum> copy_self( new IfcShadingDeviceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcShadingDeviceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSHADINGDEVICETYPEENUM("; }
	if( m_enum == ENUM_JALOUSIE )
	{
		stream << ".JALOUSIE.";
	}
	else if( m_enum == ENUM_SHUTTER )
	{
		stream << ".SHUTTER.";
	}
	else if( m_enum == ENUM_AWNING )
	{
		stream << ".AWNING.";
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
shared_ptr<IfcShadingDeviceTypeEnum> IfcShadingDeviceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcShadingDeviceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcShadingDeviceTypeEnum>(); }
	shared_ptr<IfcShadingDeviceTypeEnum> type_object( new IfcShadingDeviceTypeEnum() );
	if( boost::iequals( arg, L".JALOUSIE." ) )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_JALOUSIE;
	}
	else if( boost::iequals( arg, L".SHUTTER." ) )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_SHUTTER;
	}
	else if( boost::iequals( arg, L".AWNING." ) )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_AWNING;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
