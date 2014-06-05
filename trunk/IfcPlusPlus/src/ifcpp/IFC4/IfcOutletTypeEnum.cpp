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
#include "include/IfcOutletTypeEnum.h"

// TYPE IfcOutletTypeEnum 
IfcOutletTypeEnum::IfcOutletTypeEnum() {}
IfcOutletTypeEnum::~IfcOutletTypeEnum() {}
void IfcOutletTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOUTLETTYPEENUM("; }
	if( m_enum == ENUM_AUDIOVISUALOUTLET )
	{
		stream << ".AUDIOVISUALOUTLET.";
	}
	else if( m_enum == ENUM_COMMUNICATIONSOUTLET )
	{
		stream << ".COMMUNICATIONSOUTLET.";
	}
	else if( m_enum == ENUM_POWEROUTLET )
	{
		stream << ".POWEROUTLET.";
	}
	else if( m_enum == ENUM_DATAOUTLET )
	{
		stream << ".DATAOUTLET.";
	}
	else if( m_enum == ENUM_TELEPHONEOUTLET )
	{
		stream << ".TELEPHONEOUTLET.";
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
shared_ptr<IfcOutletTypeEnum> IfcOutletTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcOutletTypeEnum>(); }
	shared_ptr<IfcOutletTypeEnum> type_object( new IfcOutletTypeEnum() );
	if( _stricmp( arg.c_str(), ".AUDIOVISUALOUTLET." ) == 0 )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_AUDIOVISUALOUTLET;
	}
	else if( _stricmp( arg.c_str(), ".COMMUNICATIONSOUTLET." ) == 0 )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_COMMUNICATIONSOUTLET;
	}
	else if( _stricmp( arg.c_str(), ".POWEROUTLET." ) == 0 )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_POWEROUTLET;
	}
	else if( _stricmp( arg.c_str(), ".DATAOUTLET." ) == 0 )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_DATAOUTLET;
	}
	else if( _stricmp( arg.c_str(), ".TELEPHONEOUTLET." ) == 0 )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_TELEPHONEOUTLET;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
