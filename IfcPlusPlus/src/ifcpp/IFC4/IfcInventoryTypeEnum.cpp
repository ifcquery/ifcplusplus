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

// TYPE IfcInventoryTypeEnum = ENUMERATION OF	(ASSETINVENTORY	,SPACEINVENTORY	,FURNITUREINVENTORY	,USERDEFINED	,NOTDEFINED);
IfcInventoryTypeEnum::IfcInventoryTypeEnum() {}
IfcInventoryTypeEnum::~IfcInventoryTypeEnum() {}
shared_ptr<IfcPPObject> IfcInventoryTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcInventoryTypeEnum> copy_self( new IfcInventoryTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcInventoryTypeEnum> IfcInventoryTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcInventoryTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcInventoryTypeEnum>(); }
	shared_ptr<IfcInventoryTypeEnum> type_object( new IfcInventoryTypeEnum() );
	if( boost::iequals( arg, L".ASSETINVENTORY." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_ASSETINVENTORY;
	}
	else if( boost::iequals( arg, L".SPACEINVENTORY." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_SPACEINVENTORY;
	}
	else if( boost::iequals( arg, L".FURNITUREINVENTORY." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_FURNITUREINVENTORY;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
