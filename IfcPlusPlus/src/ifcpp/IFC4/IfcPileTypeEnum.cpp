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
#include "include/IfcPileTypeEnum.h"

// TYPE IfcPileTypeEnum = ENUMERATION OF	(BORED	,DRIVEN	,JETGROUTING	,COHESION	,FRICTION	,SUPPORT	,USERDEFINED	,NOTDEFINED);
IfcPileTypeEnum::IfcPileTypeEnum() {}
IfcPileTypeEnum::~IfcPileTypeEnum() {}
shared_ptr<IfcPPObject> IfcPileTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPileTypeEnum> copy_self( new IfcPileTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPileTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPILETYPEENUM("; }
	if( m_enum == ENUM_BORED )
	{
		stream << ".BORED.";
	}
	else if( m_enum == ENUM_DRIVEN )
	{
		stream << ".DRIVEN.";
	}
	else if( m_enum == ENUM_JETGROUTING )
	{
		stream << ".JETGROUTING.";
	}
	else if( m_enum == ENUM_COHESION )
	{
		stream << ".COHESION.";
	}
	else if( m_enum == ENUM_FRICTION )
	{
		stream << ".FRICTION.";
	}
	else if( m_enum == ENUM_SUPPORT )
	{
		stream << ".SUPPORT.";
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
shared_ptr<IfcPileTypeEnum> IfcPileTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPileTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPileTypeEnum>(); }
	shared_ptr<IfcPileTypeEnum> type_object( new IfcPileTypeEnum() );
	if( boost::iequals( arg, L".BORED." ) )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_BORED;
	}
	else if( boost::iequals( arg, L".DRIVEN." ) )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_DRIVEN;
	}
	else if( boost::iequals( arg, L".JETGROUTING." ) )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_JETGROUTING;
	}
	else if( boost::iequals( arg, L".COHESION." ) )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_COHESION;
	}
	else if( boost::iequals( arg, L".FRICTION." ) )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_FRICTION;
	}
	else if( boost::iequals( arg, L".SUPPORT." ) )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_SUPPORT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
