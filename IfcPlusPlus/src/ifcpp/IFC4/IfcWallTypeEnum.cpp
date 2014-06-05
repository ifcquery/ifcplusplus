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
#include "include/IfcWallTypeEnum.h"

// TYPE IfcWallTypeEnum 
IfcWallTypeEnum::IfcWallTypeEnum() {}
IfcWallTypeEnum::~IfcWallTypeEnum() {}
void IfcWallTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWALLTYPEENUM("; }
	if( m_enum == ENUM_MOVABLE )
	{
		stream << ".MOVABLE.";
	}
	else if( m_enum == ENUM_PARAPET )
	{
		stream << ".PARAPET.";
	}
	else if( m_enum == ENUM_PARTITIONING )
	{
		stream << ".PARTITIONING.";
	}
	else if( m_enum == ENUM_PLUMBINGWALL )
	{
		stream << ".PLUMBINGWALL.";
	}
	else if( m_enum == ENUM_SHEAR )
	{
		stream << ".SHEAR.";
	}
	else if( m_enum == ENUM_SOLIDWALL )
	{
		stream << ".SOLIDWALL.";
	}
	else if( m_enum == ENUM_STANDARD )
	{
		stream << ".STANDARD.";
	}
	else if( m_enum == ENUM_POLYGONAL )
	{
		stream << ".POLYGONAL.";
	}
	else if( m_enum == ENUM_ELEMENTEDWALL )
	{
		stream << ".ELEMENTEDWALL.";
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
shared_ptr<IfcWallTypeEnum> IfcWallTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcWallTypeEnum>(); }
	shared_ptr<IfcWallTypeEnum> type_object( new IfcWallTypeEnum() );
	if( _stricmp( arg.c_str(), ".MOVABLE." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_MOVABLE;
	}
	else if( _stricmp( arg.c_str(), ".PARAPET." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_PARAPET;
	}
	else if( _stricmp( arg.c_str(), ".PARTITIONING." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_PARTITIONING;
	}
	else if( _stricmp( arg.c_str(), ".PLUMBINGWALL." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_PLUMBINGWALL;
	}
	else if( _stricmp( arg.c_str(), ".SHEAR." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_SHEAR;
	}
	else if( _stricmp( arg.c_str(), ".SOLIDWALL." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_SOLIDWALL;
	}
	else if( _stricmp( arg.c_str(), ".STANDARD." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_STANDARD;
	}
	else if( _stricmp( arg.c_str(), ".POLYGONAL." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_POLYGONAL;
	}
	else if( _stricmp( arg.c_str(), ".ELEMENTEDWALL." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_ELEMENTEDWALL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
