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
#include "include/IfcPumpTypeEnum.h"

// TYPE IfcPumpTypeEnum 
IfcPumpTypeEnum::IfcPumpTypeEnum() {}
IfcPumpTypeEnum::~IfcPumpTypeEnum() {}
void IfcPumpTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPUMPTYPEENUM("; }
	if( m_enum == ENUM_CIRCULATOR )
	{
		stream << ".CIRCULATOR.";
	}
	else if( m_enum == ENUM_ENDSUCTION )
	{
		stream << ".ENDSUCTION.";
	}
	else if( m_enum == ENUM_SPLITCASE )
	{
		stream << ".SPLITCASE.";
	}
	else if( m_enum == ENUM_SUBMERSIBLEPUMP )
	{
		stream << ".SUBMERSIBLEPUMP.";
	}
	else if( m_enum == ENUM_SUMPPUMP )
	{
		stream << ".SUMPPUMP.";
	}
	else if( m_enum == ENUM_VERTICALINLINE )
	{
		stream << ".VERTICALINLINE.";
	}
	else if( m_enum == ENUM_VERTICALTURBINE )
	{
		stream << ".VERTICALTURBINE.";
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
shared_ptr<IfcPumpTypeEnum> IfcPumpTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPumpTypeEnum>(); }
	shared_ptr<IfcPumpTypeEnum> type_object( new IfcPumpTypeEnum() );
	if( _stricmp( arg.c_str(), ".CIRCULATOR." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_CIRCULATOR;
	}
	else if( _stricmp( arg.c_str(), ".ENDSUCTION." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_ENDSUCTION;
	}
	else if( _stricmp( arg.c_str(), ".SPLITCASE." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SPLITCASE;
	}
	else if( _stricmp( arg.c_str(), ".SUBMERSIBLEPUMP." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SUBMERSIBLEPUMP;
	}
	else if( _stricmp( arg.c_str(), ".SUMPPUMP." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SUMPPUMP;
	}
	else if( _stricmp( arg.c_str(), ".VERTICALINLINE." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_VERTICALINLINE;
	}
	else if( _stricmp( arg.c_str(), ".VERTICALTURBINE." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_VERTICALTURBINE;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
