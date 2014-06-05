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
#include "include/IfcPlateTypeEnum.h"

// TYPE IfcPlateTypeEnum 
IfcPlateTypeEnum::IfcPlateTypeEnum() {}
IfcPlateTypeEnum::~IfcPlateTypeEnum() {}
void IfcPlateTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPLATETYPEENUM("; }
	if( m_enum == ENUM_CURTAIN_PANEL )
	{
		stream << ".CURTAIN_PANEL.";
	}
	else if( m_enum == ENUM_SHEET )
	{
		stream << ".SHEET.";
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
shared_ptr<IfcPlateTypeEnum> IfcPlateTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPlateTypeEnum>(); }
	shared_ptr<IfcPlateTypeEnum> type_object( new IfcPlateTypeEnum() );
	if( _stricmp( arg.c_str(), ".CURTAIN_PANEL." ) == 0 )
	{
		type_object->m_enum = IfcPlateTypeEnum::ENUM_CURTAIN_PANEL;
	}
	else if( _stricmp( arg.c_str(), ".SHEET." ) == 0 )
	{
		type_object->m_enum = IfcPlateTypeEnum::ENUM_SHEET;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPlateTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPlateTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
