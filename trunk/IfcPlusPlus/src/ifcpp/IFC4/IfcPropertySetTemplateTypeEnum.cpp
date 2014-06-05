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
shared_ptr<IfcPropertySetTemplateTypeEnum> IfcPropertySetTemplateTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPropertySetTemplateTypeEnum>(); }
	shared_ptr<IfcPropertySetTemplateTypeEnum> type_object( new IfcPropertySetTemplateTypeEnum() );
	if( _stricmp( arg.c_str(), ".PSET_TYPEDRIVENONLY." ) == 0 )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_PSET_TYPEDRIVENONLY;
	}
	else if( _stricmp( arg.c_str(), ".PSET_TYPEDRIVENOVERRIDE." ) == 0 )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_PSET_TYPEDRIVENOVERRIDE;
	}
	else if( _stricmp( arg.c_str(), ".PSET_OCCURRENCEDRIVEN." ) == 0 )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_PSET_OCCURRENCEDRIVEN;
	}
	else if( _stricmp( arg.c_str(), ".PSET_PERFORMANCEDRIVEN." ) == 0 )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_PSET_PERFORMANCEDRIVEN;
	}
	else if( _stricmp( arg.c_str(), ".QTO_TYPEDRIVENONLY." ) == 0 )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_QTO_TYPEDRIVENONLY;
	}
	else if( _stricmp( arg.c_str(), ".QTO_TYPEDRIVENOVERRIDE." ) == 0 )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_QTO_TYPEDRIVENOVERRIDE;
	}
	else if( _stricmp( arg.c_str(), ".QTO_OCCURRENCEDRIVEN." ) == 0 )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_QTO_OCCURRENCEDRIVEN;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPropertySetTemplateTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
