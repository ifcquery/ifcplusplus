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
#include "include/IfcFanTypeEnum.h"

// TYPE IfcFanTypeEnum = ENUMERATION OF	(CENTRIFUGALFORWARDCURVED	,CENTRIFUGALRADIAL	,CENTRIFUGALBACKWARDINCLINEDCURVED	,CENTRIFUGALAIRFOIL	,TUBEAXIAL	,VANEAXIAL	,PROPELLORAXIAL	,USERDEFINED	,NOTDEFINED);
IfcFanTypeEnum::IfcFanTypeEnum() {}
IfcFanTypeEnum::~IfcFanTypeEnum() {}
shared_ptr<IfcPPObject> IfcFanTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFanTypeEnum> copy_self( new IfcFanTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFanTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFANTYPEENUM("; }
	if( m_enum == ENUM_CENTRIFUGALFORWARDCURVED )
	{
		stream << ".CENTRIFUGALFORWARDCURVED.";
	}
	else if( m_enum == ENUM_CENTRIFUGALRADIAL )
	{
		stream << ".CENTRIFUGALRADIAL.";
	}
	else if( m_enum == ENUM_CENTRIFUGALBACKWARDINCLINEDCURVED )
	{
		stream << ".CENTRIFUGALBACKWARDINCLINEDCURVED.";
	}
	else if( m_enum == ENUM_CENTRIFUGALAIRFOIL )
	{
		stream << ".CENTRIFUGALAIRFOIL.";
	}
	else if( m_enum == ENUM_TUBEAXIAL )
	{
		stream << ".TUBEAXIAL.";
	}
	else if( m_enum == ENUM_VANEAXIAL )
	{
		stream << ".VANEAXIAL.";
	}
	else if( m_enum == ENUM_PROPELLORAXIAL )
	{
		stream << ".PROPELLORAXIAL.";
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
shared_ptr<IfcFanTypeEnum> IfcFanTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFanTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFanTypeEnum>(); }
	shared_ptr<IfcFanTypeEnum> type_object( new IfcFanTypeEnum() );
	if( boost::iequals( arg, L".CENTRIFUGALFORWARDCURVED." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_CENTRIFUGALFORWARDCURVED;
	}
	else if( boost::iequals( arg, L".CENTRIFUGALRADIAL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_CENTRIFUGALRADIAL;
	}
	else if( boost::iequals( arg, L".CENTRIFUGALBACKWARDINCLINEDCURVED." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_CENTRIFUGALBACKWARDINCLINEDCURVED;
	}
	else if( boost::iequals( arg, L".CENTRIFUGALAIRFOIL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_CENTRIFUGALAIRFOIL;
	}
	else if( boost::iequals( arg, L".TUBEAXIAL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_TUBEAXIAL;
	}
	else if( boost::iequals( arg, L".VANEAXIAL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_VANEAXIAL;
	}
	else if( boost::iequals( arg, L".PROPELLORAXIAL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_PROPELLORAXIAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
