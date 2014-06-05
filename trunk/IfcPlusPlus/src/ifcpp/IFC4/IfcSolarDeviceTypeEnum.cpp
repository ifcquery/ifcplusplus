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
#include "include/IfcSolarDeviceTypeEnum.h"

// TYPE IfcSolarDeviceTypeEnum 
IfcSolarDeviceTypeEnum::IfcSolarDeviceTypeEnum() {}
IfcSolarDeviceTypeEnum::~IfcSolarDeviceTypeEnum() {}
void IfcSolarDeviceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSOLARDEVICETYPEENUM("; }
	if( m_enum == ENUM_SOLARCOLLECTOR )
	{
		stream << ".SOLARCOLLECTOR.";
	}
	else if( m_enum == ENUM_SOLARPANEL )
	{
		stream << ".SOLARPANEL.";
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
shared_ptr<IfcSolarDeviceTypeEnum> IfcSolarDeviceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcSolarDeviceTypeEnum>(); }
	shared_ptr<IfcSolarDeviceTypeEnum> type_object( new IfcSolarDeviceTypeEnum() );
	if( _stricmp( arg.c_str(), ".SOLARCOLLECTOR." ) == 0 )
	{
		type_object->m_enum = IfcSolarDeviceTypeEnum::ENUM_SOLARCOLLECTOR;
	}
	else if( _stricmp( arg.c_str(), ".SOLARPANEL." ) == 0 )
	{
		type_object->m_enum = IfcSolarDeviceTypeEnum::ENUM_SOLARPANEL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcSolarDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcSolarDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
