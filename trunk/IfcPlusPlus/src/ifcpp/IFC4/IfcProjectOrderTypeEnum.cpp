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
#include "include/IfcProjectOrderTypeEnum.h"

// TYPE IfcProjectOrderTypeEnum 
IfcProjectOrderTypeEnum::IfcProjectOrderTypeEnum() {}
IfcProjectOrderTypeEnum::~IfcProjectOrderTypeEnum() {}
void IfcProjectOrderTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROJECTORDERTYPEENUM("; }
	if( m_enum == ENUM_CHANGEORDER )
	{
		stream << ".CHANGEORDER.";
	}
	else if( m_enum == ENUM_MAINTENANCEWORKORDER )
	{
		stream << ".MAINTENANCEWORKORDER.";
	}
	else if( m_enum == ENUM_MOVEORDER )
	{
		stream << ".MOVEORDER.";
	}
	else if( m_enum == ENUM_PURCHASEORDER )
	{
		stream << ".PURCHASEORDER.";
	}
	else if( m_enum == ENUM_WORKORDER )
	{
		stream << ".WORKORDER.";
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
shared_ptr<IfcProjectOrderTypeEnum> IfcProjectOrderTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcProjectOrderTypeEnum>(); }
	shared_ptr<IfcProjectOrderTypeEnum> type_object( new IfcProjectOrderTypeEnum() );
	if( _stricmp( arg.c_str(), ".CHANGEORDER." ) == 0 )
	{
		type_object->m_enum = IfcProjectOrderTypeEnum::ENUM_CHANGEORDER;
	}
	else if( _stricmp( arg.c_str(), ".MAINTENANCEWORKORDER." ) == 0 )
	{
		type_object->m_enum = IfcProjectOrderTypeEnum::ENUM_MAINTENANCEWORKORDER;
	}
	else if( _stricmp( arg.c_str(), ".MOVEORDER." ) == 0 )
	{
		type_object->m_enum = IfcProjectOrderTypeEnum::ENUM_MOVEORDER;
	}
	else if( _stricmp( arg.c_str(), ".PURCHASEORDER." ) == 0 )
	{
		type_object->m_enum = IfcProjectOrderTypeEnum::ENUM_PURCHASEORDER;
	}
	else if( _stricmp( arg.c_str(), ".WORKORDER." ) == 0 )
	{
		type_object->m_enum = IfcProjectOrderTypeEnum::ENUM_WORKORDER;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcProjectOrderTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcProjectOrderTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
