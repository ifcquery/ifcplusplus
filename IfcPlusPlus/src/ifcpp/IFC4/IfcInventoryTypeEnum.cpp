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
#include "include/IfcInventoryTypeEnum.h"

// TYPE IfcInventoryTypeEnum 
IfcInventoryTypeEnum::IfcInventoryTypeEnum() {}
IfcInventoryTypeEnum::~IfcInventoryTypeEnum() {}
void IfcInventoryTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCINVENTORYTYPEENUM("; }
	if( m_enum == ENUM_ASSETINVENTORY )
	{
		stream << ".ASSETINVENTORY.";
	}
	else if( m_enum == ENUM_SPACEINVENTORY )
	{
		stream << ".SPACEINVENTORY.";
	}
	else if( m_enum == ENUM_FURNITUREINVENTORY )
	{
		stream << ".FURNITUREINVENTORY.";
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
shared_ptr<IfcInventoryTypeEnum> IfcInventoryTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcInventoryTypeEnum>(); }
	shared_ptr<IfcInventoryTypeEnum> type_object( new IfcInventoryTypeEnum() );
	if( _stricmp( arg.c_str(), ".ASSETINVENTORY." ) == 0 )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_ASSETINVENTORY;
	}
	else if( _stricmp( arg.c_str(), ".SPACEINVENTORY." ) == 0 )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_SPACEINVENTORY;
	}
	else if( _stricmp( arg.c_str(), ".FURNITUREINVENTORY." ) == 0 )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_FURNITUREINVENTORY;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
