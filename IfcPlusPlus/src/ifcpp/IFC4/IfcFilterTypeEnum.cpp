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
#include "include/IfcFilterTypeEnum.h"

// TYPE IfcFilterTypeEnum = ENUMERATION OF	(AIRPARTICLEFILTER	,COMPRESSEDAIRFILTER	,ODORFILTER	,OILFILTER	,STRAINER	,WATERFILTER	,USERDEFINED	,NOTDEFINED);
IfcFilterTypeEnum::IfcFilterTypeEnum() {}
IfcFilterTypeEnum::~IfcFilterTypeEnum() {}
shared_ptr<IfcPPObject> IfcFilterTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFilterTypeEnum> copy_self( new IfcFilterTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFilterTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFILTERTYPEENUM("; }
	if( m_enum == ENUM_AIRPARTICLEFILTER )
	{
		stream << ".AIRPARTICLEFILTER.";
	}
	else if( m_enum == ENUM_COMPRESSEDAIRFILTER )
	{
		stream << ".COMPRESSEDAIRFILTER.";
	}
	else if( m_enum == ENUM_ODORFILTER )
	{
		stream << ".ODORFILTER.";
	}
	else if( m_enum == ENUM_OILFILTER )
	{
		stream << ".OILFILTER.";
	}
	else if( m_enum == ENUM_STRAINER )
	{
		stream << ".STRAINER.";
	}
	else if( m_enum == ENUM_WATERFILTER )
	{
		stream << ".WATERFILTER.";
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
shared_ptr<IfcFilterTypeEnum> IfcFilterTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFilterTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFilterTypeEnum>(); }
	shared_ptr<IfcFilterTypeEnum> type_object( new IfcFilterTypeEnum() );
	if( boost::iequals( arg, L".AIRPARTICLEFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_AIRPARTICLEFILTER;
	}
	else if( boost::iequals( arg, L".COMPRESSEDAIRFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_COMPRESSEDAIRFILTER;
	}
	else if( boost::iequals( arg, L".ODORFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_ODORFILTER;
	}
	else if( boost::iequals( arg, L".OILFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_OILFILTER;
	}
	else if( boost::iequals( arg, L".STRAINER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_STRAINER;
	}
	else if( boost::iequals( arg, L".WATERFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_WATERFILTER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
