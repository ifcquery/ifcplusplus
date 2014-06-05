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
#include "include/IfcTendonTypeEnum.h"

// TYPE IfcTendonTypeEnum 
IfcTendonTypeEnum::IfcTendonTypeEnum() {}
IfcTendonTypeEnum::~IfcTendonTypeEnum() {}
void IfcTendonTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTENDONTYPEENUM("; }
	if( m_enum == ENUM_BAR )
	{
		stream << ".BAR.";
	}
	else if( m_enum == ENUM_COATED )
	{
		stream << ".COATED.";
	}
	else if( m_enum == ENUM_STRAND )
	{
		stream << ".STRAND.";
	}
	else if( m_enum == ENUM_WIRE )
	{
		stream << ".WIRE.";
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
shared_ptr<IfcTendonTypeEnum> IfcTendonTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcTendonTypeEnum>(); }
	shared_ptr<IfcTendonTypeEnum> type_object( new IfcTendonTypeEnum() );
	if( _stricmp( arg.c_str(), ".BAR." ) == 0 )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_BAR;
	}
	else if( _stricmp( arg.c_str(), ".COATED." ) == 0 )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_COATED;
	}
	else if( _stricmp( arg.c_str(), ".STRAND." ) == 0 )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_STRAND;
	}
	else if( _stricmp( arg.c_str(), ".WIRE." ) == 0 )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_WIRE;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
