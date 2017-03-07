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
#include "include/IfcPropertySetTemplateTypeEnum.h"

// TYPE IfcPropertySetTemplateTypeEnum = ENUMERATION OF	(PSET_TYPEDRIVENONLY	,PSET_TYPEDRIVENOVERRIDE	,PSET_OCCURRENCEDRIVEN	,PSET_PERFORMANCEDRIVEN	,QTO_TYPEDRIVENONLY	,QTO_TYPEDRIVENOVERRIDE	,QTO_OCCURRENCEDRIVEN	,NOTDEFINED);
IfcPropertySetTemplateTypeEnum::IfcPropertySetTemplateTypeEnum() {}
IfcPropertySetTemplateTypeEnum::~IfcPropertySetTemplateTypeEnum() {}
shared_ptr<IfcPPObject> IfcPropertySetTemplateTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertySetTemplateTypeEnum> copy_self( new IfcPropertySetTemplateTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPropertySetTemplateTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROPERTYSETTEMPLATETYPEENUM("; }
	if( m_enum == ENUM_PSET_TYPEDRIVENONLY )
	{
		stream << ".PSET_TYPEDRIVENONLY.";
	}
	else if( m_enum == ENUM_PSET_TYPEDRIVENOVERRIDE )
	{
		stream << ".PSET_TYPEDRIVENOVERRIDE.";
	}
	else if( m_enum == ENUM_PSET_OCCURRENCEDRIVEN )
	{
		stream << ".PSET_OCCURRENCEDRIVEN.";
	}
	else if( m_enum == ENUM_PSET_PERFORMANCEDRIVEN )
	{
		stream << ".PSET_PERFORMANCEDRIVEN.";
	}
	else if( m_enum == ENUM_QTO_TYPEDRIVENONLY )
	{
		stream << ".QTO_TYPEDRIVENONLY.";
	}
	else if( m_enum == ENUM_QTO_TYPEDRIVENOVERRIDE )
	{
		stream << ".QTO_TYPEDRIVENOVERRIDE.";
	}
	else if( m_enum == ENUM_QTO_OCCURRENCEDRIVEN )
	{
		stream << ".QTO_OCCURRENCEDRIVEN.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcPropertySetTemplateTypeEnum> IfcPropertySetTemplateTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPropertySetTemplateTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPropertySetTemplateTypeEnum>(); }
	shared_ptr<IfcPropertySetTemplateTypeEnum> type_object( new IfcPropertySetTemplateTypeEnum() );
	if( boost::iequals( arg, L".PSET_TYPEDRIVENONLY." ) )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_PSET_TYPEDRIVENONLY;
	}
	else if( boost::iequals( arg, L".PSET_TYPEDRIVENOVERRIDE." ) )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_PSET_TYPEDRIVENOVERRIDE;
	}
	else if( boost::iequals( arg, L".PSET_OCCURRENCEDRIVEN." ) )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_PSET_OCCURRENCEDRIVEN;
	}
	else if( boost::iequals( arg, L".PSET_PERFORMANCEDRIVEN." ) )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_PSET_PERFORMANCEDRIVEN;
	}
	else if( boost::iequals( arg, L".QTO_TYPEDRIVENONLY." ) )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_QTO_TYPEDRIVENONLY;
	}
	else if( boost::iequals( arg, L".QTO_TYPEDRIVENOVERRIDE." ) )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_QTO_TYPEDRIVENOVERRIDE;
	}
	else if( boost::iequals( arg, L".QTO_OCCURRENCEDRIVEN." ) )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_QTO_OCCURRENCEDRIVEN;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
