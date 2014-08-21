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
#include "include/IfcFurnitureTypeEnum.h"

// TYPE IfcFurnitureTypeEnum = ENUMERATION OF	(CHAIR	,TABLE	,DESK	,BED	,FILECABINET	,SHELF	,SOFA	,USERDEFINED	,NOTDEFINED);
IfcFurnitureTypeEnum::IfcFurnitureTypeEnum() {}
IfcFurnitureTypeEnum::~IfcFurnitureTypeEnum() {}
shared_ptr<IfcPPObject> IfcFurnitureTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFurnitureTypeEnum> copy_self( new IfcFurnitureTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFurnitureTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFURNITURETYPEENUM("; }
	if( m_enum == ENUM_CHAIR )
	{
		stream << ".CHAIR.";
	}
	else if( m_enum == ENUM_TABLE )
	{
		stream << ".TABLE.";
	}
	else if( m_enum == ENUM_DESK )
	{
		stream << ".DESK.";
	}
	else if( m_enum == ENUM_BED )
	{
		stream << ".BED.";
	}
	else if( m_enum == ENUM_FILECABINET )
	{
		stream << ".FILECABINET.";
	}
	else if( m_enum == ENUM_SHELF )
	{
		stream << ".SHELF.";
	}
	else if( m_enum == ENUM_SOFA )
	{
		stream << ".SOFA.";
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
shared_ptr<IfcFurnitureTypeEnum> IfcFurnitureTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFurnitureTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFurnitureTypeEnum>(); }
	shared_ptr<IfcFurnitureTypeEnum> type_object( new IfcFurnitureTypeEnum() );
	if( boost::iequals( arg, L".CHAIR." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_CHAIR;
	}
	else if( boost::iequals( arg, L".TABLE." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_TABLE;
	}
	else if( boost::iequals( arg, L".DESK." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_DESK;
	}
	else if( boost::iequals( arg, L".BED." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_BED;
	}
	else if( boost::iequals( arg, L".FILECABINET." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_FILECABINET;
	}
	else if( boost::iequals( arg, L".SHELF." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_SHELF;
	}
	else if( boost::iequals( arg, L".SOFA." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_SOFA;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFurnitureTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
