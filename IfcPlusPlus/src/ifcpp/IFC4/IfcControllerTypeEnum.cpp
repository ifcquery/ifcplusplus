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
#include "include/IfcControllerTypeEnum.h"

// TYPE IfcControllerTypeEnum = ENUMERATION OF	(FLOATING	,PROGRAMMABLE	,PROPORTIONAL	,MULTIPOSITION	,TWOPOSITION	,USERDEFINED	,NOTDEFINED);
IfcControllerTypeEnum::IfcControllerTypeEnum() {}
IfcControllerTypeEnum::~IfcControllerTypeEnum() {}
shared_ptr<IfcPPObject> IfcControllerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcControllerTypeEnum> copy_self( new IfcControllerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcControllerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONTROLLERTYPEENUM("; }
	if( m_enum == ENUM_FLOATING )
	{
		stream << ".FLOATING.";
	}
	else if( m_enum == ENUM_PROGRAMMABLE )
	{
		stream << ".PROGRAMMABLE.";
	}
	else if( m_enum == ENUM_PROPORTIONAL )
	{
		stream << ".PROPORTIONAL.";
	}
	else if( m_enum == ENUM_MULTIPOSITION )
	{
		stream << ".MULTIPOSITION.";
	}
	else if( m_enum == ENUM_TWOPOSITION )
	{
		stream << ".TWOPOSITION.";
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
shared_ptr<IfcControllerTypeEnum> IfcControllerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcControllerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcControllerTypeEnum>(); }
	shared_ptr<IfcControllerTypeEnum> type_object( new IfcControllerTypeEnum() );
	if( boost::iequals( arg, L".FLOATING." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_FLOATING;
	}
	else if( boost::iequals( arg, L".PROGRAMMABLE." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_PROGRAMMABLE;
	}
	else if( boost::iequals( arg, L".PROPORTIONAL." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_PROPORTIONAL;
	}
	else if( boost::iequals( arg, L".MULTIPOSITION." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_MULTIPOSITION;
	}
	else if( boost::iequals( arg, L".TWOPOSITION." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_TWOPOSITION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
