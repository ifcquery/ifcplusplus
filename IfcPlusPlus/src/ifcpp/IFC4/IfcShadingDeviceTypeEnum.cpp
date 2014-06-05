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
#include "include/IfcShadingDeviceTypeEnum.h"

// TYPE IfcShadingDeviceTypeEnum 
IfcShadingDeviceTypeEnum::IfcShadingDeviceTypeEnum() {}
IfcShadingDeviceTypeEnum::~IfcShadingDeviceTypeEnum() {}
void IfcShadingDeviceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSHADINGDEVICETYPEENUM("; }
	if( m_enum == ENUM_JALOUSIE )
	{
		stream << ".JALOUSIE.";
	}
	else if( m_enum == ENUM_SHUTTER )
	{
		stream << ".SHUTTER.";
	}
	else if( m_enum == ENUM_AWNING )
	{
		stream << ".AWNING.";
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
shared_ptr<IfcShadingDeviceTypeEnum> IfcShadingDeviceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcShadingDeviceTypeEnum>(); }
	shared_ptr<IfcShadingDeviceTypeEnum> type_object( new IfcShadingDeviceTypeEnum() );
	if( _stricmp( arg.c_str(), ".JALOUSIE." ) == 0 )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_JALOUSIE;
	}
	else if( _stricmp( arg.c_str(), ".SHUTTER." ) == 0 )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_SHUTTER;
	}
	else if( _stricmp( arg.c_str(), ".AWNING." ) == 0 )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_AWNING;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcShadingDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
