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
#include "include/IfcCoilTypeEnum.h"

// TYPE IfcCoilTypeEnum 
IfcCoilTypeEnum::IfcCoilTypeEnum() {}
IfcCoilTypeEnum::~IfcCoilTypeEnum() {}
void IfcCoilTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOILTYPEENUM("; }
	if( m_enum == ENUM_DXCOOLINGCOIL )
	{
		stream << ".DXCOOLINGCOIL.";
	}
	else if( m_enum == ENUM_ELECTRICHEATINGCOIL )
	{
		stream << ".ELECTRICHEATINGCOIL.";
	}
	else if( m_enum == ENUM_GASHEATINGCOIL )
	{
		stream << ".GASHEATINGCOIL.";
	}
	else if( m_enum == ENUM_HYDRONICCOIL )
	{
		stream << ".HYDRONICCOIL.";
	}
	else if( m_enum == ENUM_STEAMHEATINGCOIL )
	{
		stream << ".STEAMHEATINGCOIL.";
	}
	else if( m_enum == ENUM_WATERCOOLINGCOIL )
	{
		stream << ".WATERCOOLINGCOIL.";
	}
	else if( m_enum == ENUM_WATERHEATINGCOIL )
	{
		stream << ".WATERHEATINGCOIL.";
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
shared_ptr<IfcCoilTypeEnum> IfcCoilTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcCoilTypeEnum>(); }
	shared_ptr<IfcCoilTypeEnum> type_object( new IfcCoilTypeEnum() );
	if( _stricmp( arg.c_str(), ".DXCOOLINGCOIL." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_DXCOOLINGCOIL;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICHEATINGCOIL." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_ELECTRICHEATINGCOIL;
	}
	else if( _stricmp( arg.c_str(), ".GASHEATINGCOIL." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_GASHEATINGCOIL;
	}
	else if( _stricmp( arg.c_str(), ".HYDRONICCOIL." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_HYDRONICCOIL;
	}
	else if( _stricmp( arg.c_str(), ".STEAMHEATINGCOIL." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_STEAMHEATINGCOIL;
	}
	else if( _stricmp( arg.c_str(), ".WATERCOOLINGCOIL." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_WATERCOOLINGCOIL;
	}
	else if( _stricmp( arg.c_str(), ".WATERHEATINGCOIL." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_WATERHEATINGCOIL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
