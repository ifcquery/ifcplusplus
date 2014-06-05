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
#include "include/IfcPileConstructionEnum.h"

// TYPE IfcPileConstructionEnum 
IfcPileConstructionEnum::IfcPileConstructionEnum() {}
IfcPileConstructionEnum::~IfcPileConstructionEnum() {}
void IfcPileConstructionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPILECONSTRUCTIONENUM("; }
	if( m_enum == ENUM_CAST_IN_PLACE )
	{
		stream << ".CAST_IN_PLACE.";
	}
	else if( m_enum == ENUM_COMPOSITE )
	{
		stream << ".COMPOSITE.";
	}
	else if( m_enum == ENUM_PRECAST_CONCRETE )
	{
		stream << ".PRECAST_CONCRETE.";
	}
	else if( m_enum == ENUM_PREFAB_STEEL )
	{
		stream << ".PREFAB_STEEL.";
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
shared_ptr<IfcPileConstructionEnum> IfcPileConstructionEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPileConstructionEnum>(); }
	shared_ptr<IfcPileConstructionEnum> type_object( new IfcPileConstructionEnum() );
	if( _stricmp( arg.c_str(), ".CAST_IN_PLACE." ) == 0 )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_CAST_IN_PLACE;
	}
	else if( _stricmp( arg.c_str(), ".COMPOSITE." ) == 0 )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_COMPOSITE;
	}
	else if( _stricmp( arg.c_str(), ".PRECAST_CONCRETE." ) == 0 )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_PRECAST_CONCRETE;
	}
	else if( _stricmp( arg.c_str(), ".PREFAB_STEEL." ) == 0 )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_PREFAB_STEEL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
