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
#include "include/IfcPipeFittingTypeEnum.h"

// TYPE IfcPipeFittingTypeEnum 
IfcPipeFittingTypeEnum::IfcPipeFittingTypeEnum() {}
IfcPipeFittingTypeEnum::~IfcPipeFittingTypeEnum() {}
void IfcPipeFittingTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPIPEFITTINGTYPEENUM("; }
	if( m_enum == ENUM_BEND )
	{
		stream << ".BEND.";
	}
	else if( m_enum == ENUM_CONNECTOR )
	{
		stream << ".CONNECTOR.";
	}
	else if( m_enum == ENUM_ENTRY )
	{
		stream << ".ENTRY.";
	}
	else if( m_enum == ENUM_EXIT )
	{
		stream << ".EXIT.";
	}
	else if( m_enum == ENUM_JUNCTION )
	{
		stream << ".JUNCTION.";
	}
	else if( m_enum == ENUM_OBSTRUCTION )
	{
		stream << ".OBSTRUCTION.";
	}
	else if( m_enum == ENUM_TRANSITION )
	{
		stream << ".TRANSITION.";
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
shared_ptr<IfcPipeFittingTypeEnum> IfcPipeFittingTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPipeFittingTypeEnum>(); }
	shared_ptr<IfcPipeFittingTypeEnum> type_object( new IfcPipeFittingTypeEnum() );
	if( _stricmp( arg.c_str(), ".BEND." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_BEND;
	}
	else if( _stricmp( arg.c_str(), ".CONNECTOR." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_CONNECTOR;
	}
	else if( _stricmp( arg.c_str(), ".ENTRY." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_ENTRY;
	}
	else if( _stricmp( arg.c_str(), ".EXIT." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_EXIT;
	}
	else if( _stricmp( arg.c_str(), ".JUNCTION." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_JUNCTION;
	}
	else if( _stricmp( arg.c_str(), ".OBSTRUCTION." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_OBSTRUCTION;
	}
	else if( _stricmp( arg.c_str(), ".TRANSITION." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_TRANSITION;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
