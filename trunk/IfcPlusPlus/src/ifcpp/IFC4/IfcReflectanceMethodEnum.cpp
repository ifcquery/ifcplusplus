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
#include "include/IfcReflectanceMethodEnum.h"

// TYPE IfcReflectanceMethodEnum 
IfcReflectanceMethodEnum::IfcReflectanceMethodEnum() {}
IfcReflectanceMethodEnum::~IfcReflectanceMethodEnum() {}
void IfcReflectanceMethodEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREFLECTANCEMETHODENUM("; }
	if( m_enum == ENUM_BLINN )
	{
		stream << ".BLINN.";
	}
	else if( m_enum == ENUM_FLAT )
	{
		stream << ".FLAT.";
	}
	else if( m_enum == ENUM_GLASS )
	{
		stream << ".GLASS.";
	}
	else if( m_enum == ENUM_MATT )
	{
		stream << ".MATT.";
	}
	else if( m_enum == ENUM_METAL )
	{
		stream << ".METAL.";
	}
	else if( m_enum == ENUM_MIRROR )
	{
		stream << ".MIRROR.";
	}
	else if( m_enum == ENUM_PHONG )
	{
		stream << ".PHONG.";
	}
	else if( m_enum == ENUM_PLASTIC )
	{
		stream << ".PLASTIC.";
	}
	else if( m_enum == ENUM_STRAUSS )
	{
		stream << ".STRAUSS.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcReflectanceMethodEnum> IfcReflectanceMethodEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcReflectanceMethodEnum>(); }
	shared_ptr<IfcReflectanceMethodEnum> type_object( new IfcReflectanceMethodEnum() );
	if( _stricmp( arg.c_str(), ".BLINN." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_BLINN;
	}
	else if( _stricmp( arg.c_str(), ".FLAT." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_FLAT;
	}
	else if( _stricmp( arg.c_str(), ".GLASS." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_GLASS;
	}
	else if( _stricmp( arg.c_str(), ".MATT." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_MATT;
	}
	else if( _stricmp( arg.c_str(), ".METAL." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_METAL;
	}
	else if( _stricmp( arg.c_str(), ".MIRROR." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_MIRROR;
	}
	else if( _stricmp( arg.c_str(), ".PHONG." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_PHONG;
	}
	else if( _stricmp( arg.c_str(), ".PLASTIC." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_PLASTIC;
	}
	else if( _stricmp( arg.c_str(), ".STRAUSS." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_STRAUSS;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
