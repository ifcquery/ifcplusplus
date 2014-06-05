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
#include "include/IfcWindowStyleConstructionEnum.h"

// TYPE IfcWindowStyleConstructionEnum 
IfcWindowStyleConstructionEnum::IfcWindowStyleConstructionEnum() {}
IfcWindowStyleConstructionEnum::~IfcWindowStyleConstructionEnum() {}
void IfcWindowStyleConstructionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWINDOWSTYLECONSTRUCTIONENUM("; }
	if( m_enum == ENUM_ALUMINIUM )
	{
		stream << ".ALUMINIUM.";
	}
	else if( m_enum == ENUM_HIGH_GRADE_STEEL )
	{
		stream << ".HIGH_GRADE_STEEL.";
	}
	else if( m_enum == ENUM_STEEL )
	{
		stream << ".STEEL.";
	}
	else if( m_enum == ENUM_WOOD )
	{
		stream << ".WOOD.";
	}
	else if( m_enum == ENUM_ALUMINIUM_WOOD )
	{
		stream << ".ALUMINIUM_WOOD.";
	}
	else if( m_enum == ENUM_PLASTIC )
	{
		stream << ".PLASTIC.";
	}
	else if( m_enum == ENUM_OTHER_CONSTRUCTION )
	{
		stream << ".OTHER_CONSTRUCTION.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcWindowStyleConstructionEnum> IfcWindowStyleConstructionEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcWindowStyleConstructionEnum>(); }
	shared_ptr<IfcWindowStyleConstructionEnum> type_object( new IfcWindowStyleConstructionEnum() );
	if( _stricmp( arg.c_str(), ".ALUMINIUM." ) == 0 )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_ALUMINIUM;
	}
	else if( _stricmp( arg.c_str(), ".HIGH_GRADE_STEEL." ) == 0 )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_HIGH_GRADE_STEEL;
	}
	else if( _stricmp( arg.c_str(), ".STEEL." ) == 0 )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_STEEL;
	}
	else if( _stricmp( arg.c_str(), ".WOOD." ) == 0 )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_WOOD;
	}
	else if( _stricmp( arg.c_str(), ".ALUMINIUM_WOOD." ) == 0 )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_ALUMINIUM_WOOD;
	}
	else if( _stricmp( arg.c_str(), ".PLASTIC." ) == 0 )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_PLASTIC;
	}
	else if( _stricmp( arg.c_str(), ".OTHER_CONSTRUCTION." ) == 0 )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_OTHER_CONSTRUCTION;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
