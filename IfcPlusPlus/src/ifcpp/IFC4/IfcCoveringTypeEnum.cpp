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
#include "include/IfcCoveringTypeEnum.h"

// TYPE IfcCoveringTypeEnum = ENUMERATION OF	(CEILING	,FLOORING	,CLADDING	,ROOFING	,MOLDING	,SKIRTINGBOARD	,INSULATION	,MEMBRANE	,SLEEVING	,WRAPPING	,USERDEFINED	,NOTDEFINED);
IfcCoveringTypeEnum::IfcCoveringTypeEnum() {}
IfcCoveringTypeEnum::~IfcCoveringTypeEnum() {}
shared_ptr<IfcPPObject> IfcCoveringTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCoveringTypeEnum> copy_self( new IfcCoveringTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCoveringTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOVERINGTYPEENUM("; }
	if( m_enum == ENUM_CEILING )
	{
		stream << ".CEILING.";
	}
	else if( m_enum == ENUM_FLOORING )
	{
		stream << ".FLOORING.";
	}
	else if( m_enum == ENUM_CLADDING )
	{
		stream << ".CLADDING.";
	}
	else if( m_enum == ENUM_ROOFING )
	{
		stream << ".ROOFING.";
	}
	else if( m_enum == ENUM_MOLDING )
	{
		stream << ".MOLDING.";
	}
	else if( m_enum == ENUM_SKIRTINGBOARD )
	{
		stream << ".SKIRTINGBOARD.";
	}
	else if( m_enum == ENUM_INSULATION )
	{
		stream << ".INSULATION.";
	}
	else if( m_enum == ENUM_MEMBRANE )
	{
		stream << ".MEMBRANE.";
	}
	else if( m_enum == ENUM_SLEEVING )
	{
		stream << ".SLEEVING.";
	}
	else if( m_enum == ENUM_WRAPPING )
	{
		stream << ".WRAPPING.";
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
shared_ptr<IfcCoveringTypeEnum> IfcCoveringTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCoveringTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCoveringTypeEnum>(); }
	shared_ptr<IfcCoveringTypeEnum> type_object( new IfcCoveringTypeEnum() );
	if( boost::iequals( arg, L".CEILING." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_CEILING;
	}
	else if( boost::iequals( arg, L".FLOORING." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_FLOORING;
	}
	else if( boost::iequals( arg, L".CLADDING." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_CLADDING;
	}
	else if( boost::iequals( arg, L".ROOFING." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_ROOFING;
	}
	else if( boost::iequals( arg, L".MOLDING." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_MOLDING;
	}
	else if( boost::iequals( arg, L".SKIRTINGBOARD." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_SKIRTINGBOARD;
	}
	else if( boost::iequals( arg, L".INSULATION." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_INSULATION;
	}
	else if( boost::iequals( arg, L".MEMBRANE." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_MEMBRANE;
	}
	else if( boost::iequals( arg, L".SLEEVING." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_SLEEVING;
	}
	else if( boost::iequals( arg, L".WRAPPING." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_WRAPPING;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
