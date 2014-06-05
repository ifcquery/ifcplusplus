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
#include "include/IfcEvaporatorTypeEnum.h"

// TYPE IfcEvaporatorTypeEnum 
IfcEvaporatorTypeEnum::IfcEvaporatorTypeEnum() {}
IfcEvaporatorTypeEnum::~IfcEvaporatorTypeEnum() {}
void IfcEvaporatorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCEVAPORATORTYPEENUM("; }
	if( m_enum == ENUM_DIRECTEXPANSION )
	{
		stream << ".DIRECTEXPANSION.";
	}
	else if( m_enum == ENUM_DIRECTEXPANSIONSHELLANDTUBE )
	{
		stream << ".DIRECTEXPANSIONSHELLANDTUBE.";
	}
	else if( m_enum == ENUM_DIRECTEXPANSIONTUBEINTUBE )
	{
		stream << ".DIRECTEXPANSIONTUBEINTUBE.";
	}
	else if( m_enum == ENUM_DIRECTEXPANSIONBRAZEDPLATE )
	{
		stream << ".DIRECTEXPANSIONBRAZEDPLATE.";
	}
	else if( m_enum == ENUM_FLOODEDSHELLANDTUBE )
	{
		stream << ".FLOODEDSHELLANDTUBE.";
	}
	else if( m_enum == ENUM_SHELLANDCOIL )
	{
		stream << ".SHELLANDCOIL.";
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
shared_ptr<IfcEvaporatorTypeEnum> IfcEvaporatorTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcEvaporatorTypeEnum>(); }
	shared_ptr<IfcEvaporatorTypeEnum> type_object( new IfcEvaporatorTypeEnum() );
	if( _stricmp( arg.c_str(), ".DIRECTEXPANSION." ) == 0 )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_DIRECTEXPANSION;
	}
	else if( _stricmp( arg.c_str(), ".DIRECTEXPANSIONSHELLANDTUBE." ) == 0 )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_DIRECTEXPANSIONSHELLANDTUBE;
	}
	else if( _stricmp( arg.c_str(), ".DIRECTEXPANSIONTUBEINTUBE." ) == 0 )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_DIRECTEXPANSIONTUBEINTUBE;
	}
	else if( _stricmp( arg.c_str(), ".DIRECTEXPANSIONBRAZEDPLATE." ) == 0 )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_DIRECTEXPANSIONBRAZEDPLATE;
	}
	else if( _stricmp( arg.c_str(), ".FLOODEDSHELLANDTUBE." ) == 0 )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_FLOODEDSHELLANDTUBE;
	}
	else if( _stricmp( arg.c_str(), ".SHELLANDCOIL." ) == 0 )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_SHELLANDCOIL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcEvaporatorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
