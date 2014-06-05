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
#include "include/IfcMedicalDeviceTypeEnum.h"

// TYPE IfcMedicalDeviceTypeEnum 
IfcMedicalDeviceTypeEnum::IfcMedicalDeviceTypeEnum() {}
IfcMedicalDeviceTypeEnum::~IfcMedicalDeviceTypeEnum() {}
void IfcMedicalDeviceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCMEDICALDEVICETYPEENUM("; }
	if( m_enum == ENUM_AIRSTATION )
	{
		stream << ".AIRSTATION.";
	}
	else if( m_enum == ENUM_FEEDAIRUNIT )
	{
		stream << ".FEEDAIRUNIT.";
	}
	else if( m_enum == ENUM_OXYGENGENERATOR )
	{
		stream << ".OXYGENGENERATOR.";
	}
	else if( m_enum == ENUM_OXYGENPLANT )
	{
		stream << ".OXYGENPLANT.";
	}
	else if( m_enum == ENUM_VACUUMSTATION )
	{
		stream << ".VACUUMSTATION.";
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
shared_ptr<IfcMedicalDeviceTypeEnum> IfcMedicalDeviceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcMedicalDeviceTypeEnum>(); }
	shared_ptr<IfcMedicalDeviceTypeEnum> type_object( new IfcMedicalDeviceTypeEnum() );
	if( _stricmp( arg.c_str(), ".AIRSTATION." ) == 0 )
	{
		type_object->m_enum = IfcMedicalDeviceTypeEnum::ENUM_AIRSTATION;
	}
	else if( _stricmp( arg.c_str(), ".FEEDAIRUNIT." ) == 0 )
	{
		type_object->m_enum = IfcMedicalDeviceTypeEnum::ENUM_FEEDAIRUNIT;
	}
	else if( _stricmp( arg.c_str(), ".OXYGENGENERATOR." ) == 0 )
	{
		type_object->m_enum = IfcMedicalDeviceTypeEnum::ENUM_OXYGENGENERATOR;
	}
	else if( _stricmp( arg.c_str(), ".OXYGENPLANT." ) == 0 )
	{
		type_object->m_enum = IfcMedicalDeviceTypeEnum::ENUM_OXYGENPLANT;
	}
	else if( _stricmp( arg.c_str(), ".VACUUMSTATION." ) == 0 )
	{
		type_object->m_enum = IfcMedicalDeviceTypeEnum::ENUM_VACUUMSTATION;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcMedicalDeviceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcMedicalDeviceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
