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
#include "include/IfcPileTypeEnum.h"

// TYPE IfcPileTypeEnum 
IfcPileTypeEnum::IfcPileTypeEnum() {}
IfcPileTypeEnum::~IfcPileTypeEnum() {}
void IfcPileTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPILETYPEENUM("; }
	if( m_enum == ENUM_BORED )
	{
		stream << ".BORED.";
	}
	else if( m_enum == ENUM_DRIVEN )
	{
		stream << ".DRIVEN.";
	}
	else if( m_enum == ENUM_JETGROUTING )
	{
		stream << ".JETGROUTING.";
	}
	else if( m_enum == ENUM_COHESION )
	{
		stream << ".COHESION.";
	}
	else if( m_enum == ENUM_FRICTION )
	{
		stream << ".FRICTION.";
	}
	else if( m_enum == ENUM_SUPPORT )
	{
		stream << ".SUPPORT.";
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
shared_ptr<IfcPileTypeEnum> IfcPileTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPileTypeEnum>(); }
	shared_ptr<IfcPileTypeEnum> type_object( new IfcPileTypeEnum() );
	if( _stricmp( arg.c_str(), ".BORED." ) == 0 )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_BORED;
	}
	else if( _stricmp( arg.c_str(), ".DRIVEN." ) == 0 )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_DRIVEN;
	}
	else if( _stricmp( arg.c_str(), ".JETGROUTING." ) == 0 )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_JETGROUTING;
	}
	else if( _stricmp( arg.c_str(), ".COHESION." ) == 0 )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_COHESION;
	}
	else if( _stricmp( arg.c_str(), ".FRICTION." ) == 0 )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_FRICTION;
	}
	else if( _stricmp( arg.c_str(), ".SUPPORT." ) == 0 )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_SUPPORT;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPileTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
