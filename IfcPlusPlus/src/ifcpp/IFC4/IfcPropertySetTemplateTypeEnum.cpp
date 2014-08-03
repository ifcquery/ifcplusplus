/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcPropertySetTemplateTypeEnum.h"

// TYPE IfcPropertySetTemplateTypeEnum 
IfcPropertySetTemplateTypeEnum::IfcPropertySetTemplateTypeEnum() {}
IfcPropertySetTemplateTypeEnum::~IfcPropertySetTemplateTypeEnum() {}
shared_ptr<IfcPPObject> IfcPropertySetTemplateTypeEnum::getDeepCopy()
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
shared_ptr<IfcPropertySetTemplateTypeEnum> IfcPropertySetTemplateTypeEnum::createObjectFromStepData( const std::wstring& arg )
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
