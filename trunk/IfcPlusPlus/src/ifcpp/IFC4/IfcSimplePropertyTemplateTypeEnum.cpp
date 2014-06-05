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
#include "include/IfcSimplePropertyTemplateTypeEnum.h"

// TYPE IfcSimplePropertyTemplateTypeEnum 
IfcSimplePropertyTemplateTypeEnum::IfcSimplePropertyTemplateTypeEnum() {}
IfcSimplePropertyTemplateTypeEnum::~IfcSimplePropertyTemplateTypeEnum() {}
void IfcSimplePropertyTemplateTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSIMPLEPROPERTYTEMPLATETYPEENUM("; }
	if( m_enum == ENUM_P_SINGLEVALUE )
	{
		stream << ".P_SINGLEVALUE.";
	}
	else if( m_enum == ENUM_P_ENUMERATEDVALUE )
	{
		stream << ".P_ENUMERATEDVALUE.";
	}
	else if( m_enum == ENUM_P_BOUNDEDVALUE )
	{
		stream << ".P_BOUNDEDVALUE.";
	}
	else if( m_enum == ENUM_P_LISTVALUE )
	{
		stream << ".P_LISTVALUE.";
	}
	else if( m_enum == ENUM_P_TABLEVALUE )
	{
		stream << ".P_TABLEVALUE.";
	}
	else if( m_enum == ENUM_P_REFERENCEVALUE )
	{
		stream << ".P_REFERENCEVALUE.";
	}
	else if( m_enum == ENUM_Q_LENGTH )
	{
		stream << ".Q_LENGTH.";
	}
	else if( m_enum == ENUM_Q_AREA )
	{
		stream << ".Q_AREA.";
	}
	else if( m_enum == ENUM_Q_VOLUME )
	{
		stream << ".Q_VOLUME.";
	}
	else if( m_enum == ENUM_Q_COUNT )
	{
		stream << ".Q_COUNT.";
	}
	else if( m_enum == ENUM_Q_WEIGHT )
	{
		stream << ".Q_WEIGHT.";
	}
	else if( m_enum == ENUM_Q_TIME )
	{
		stream << ".Q_TIME.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSimplePropertyTemplateTypeEnum> IfcSimplePropertyTemplateTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcSimplePropertyTemplateTypeEnum>(); }
	shared_ptr<IfcSimplePropertyTemplateTypeEnum> type_object( new IfcSimplePropertyTemplateTypeEnum() );
	if( _stricmp( arg.c_str(), ".P_SINGLEVALUE." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_SINGLEVALUE;
	}
	else if( _stricmp( arg.c_str(), ".P_ENUMERATEDVALUE." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_ENUMERATEDVALUE;
	}
	else if( _stricmp( arg.c_str(), ".P_BOUNDEDVALUE." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_BOUNDEDVALUE;
	}
	else if( _stricmp( arg.c_str(), ".P_LISTVALUE." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_LISTVALUE;
	}
	else if( _stricmp( arg.c_str(), ".P_TABLEVALUE." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_TABLEVALUE;
	}
	else if( _stricmp( arg.c_str(), ".P_REFERENCEVALUE." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_REFERENCEVALUE;
	}
	else if( _stricmp( arg.c_str(), ".Q_LENGTH." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_LENGTH;
	}
	else if( _stricmp( arg.c_str(), ".Q_AREA." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_AREA;
	}
	else if( _stricmp( arg.c_str(), ".Q_VOLUME." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_VOLUME;
	}
	else if( _stricmp( arg.c_str(), ".Q_COUNT." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_COUNT;
	}
	else if( _stricmp( arg.c_str(), ".Q_WEIGHT." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_WEIGHT;
	}
	else if( _stricmp( arg.c_str(), ".Q_TIME." ) == 0 )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_TIME;
	}
	return type_object;
}
