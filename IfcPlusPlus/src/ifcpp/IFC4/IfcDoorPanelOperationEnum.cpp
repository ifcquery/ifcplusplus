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
#include "include/IfcDoorPanelOperationEnum.h"

// TYPE IfcDoorPanelOperationEnum 
IfcDoorPanelOperationEnum::IfcDoorPanelOperationEnum() {}
IfcDoorPanelOperationEnum::~IfcDoorPanelOperationEnum() {}
void IfcDoorPanelOperationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDOORPANELOPERATIONENUM("; }
	if( m_enum == ENUM_SWINGING )
	{
		stream << ".SWINGING.";
	}
	else if( m_enum == ENUM_DOUBLE_ACTING )
	{
		stream << ".DOUBLE_ACTING.";
	}
	else if( m_enum == ENUM_SLIDING )
	{
		stream << ".SLIDING.";
	}
	else if( m_enum == ENUM_FOLDING )
	{
		stream << ".FOLDING.";
	}
	else if( m_enum == ENUM_REVOLVING )
	{
		stream << ".REVOLVING.";
	}
	else if( m_enum == ENUM_ROLLINGUP )
	{
		stream << ".ROLLINGUP.";
	}
	else if( m_enum == ENUM_FIXEDPANEL )
	{
		stream << ".FIXEDPANEL.";
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
shared_ptr<IfcDoorPanelOperationEnum> IfcDoorPanelOperationEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcDoorPanelOperationEnum>(); }
	shared_ptr<IfcDoorPanelOperationEnum> type_object( new IfcDoorPanelOperationEnum() );
	if( _stricmp( arg.c_str(), ".SWINGING." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_SWINGING;
	}
	else if( _stricmp( arg.c_str(), ".DOUBLE_ACTING." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_DOUBLE_ACTING;
	}
	else if( _stricmp( arg.c_str(), ".SLIDING." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_SLIDING;
	}
	else if( _stricmp( arg.c_str(), ".FOLDING." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_FOLDING;
	}
	else if( _stricmp( arg.c_str(), ".REVOLVING." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_REVOLVING;
	}
	else if( _stricmp( arg.c_str(), ".ROLLINGUP." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_ROLLINGUP;
	}
	else if( _stricmp( arg.c_str(), ".FIXEDPANEL." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_FIXEDPANEL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
