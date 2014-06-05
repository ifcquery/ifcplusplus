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
#include "include/IfcStructuralSurfaceMemberTypeEnum.h"

// TYPE IfcStructuralSurfaceMemberTypeEnum 
IfcStructuralSurfaceMemberTypeEnum::IfcStructuralSurfaceMemberTypeEnum() {}
IfcStructuralSurfaceMemberTypeEnum::~IfcStructuralSurfaceMemberTypeEnum() {}
void IfcStructuralSurfaceMemberTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTRUCTURALSURFACEMEMBERTYPEENUM("; }
	if( m_enum == ENUM_BENDING_ELEMENT )
	{
		stream << ".BENDING_ELEMENT.";
	}
	else if( m_enum == ENUM_MEMBRANE_ELEMENT )
	{
		stream << ".MEMBRANE_ELEMENT.";
	}
	else if( m_enum == ENUM_SHELL )
	{
		stream << ".SHELL.";
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
shared_ptr<IfcStructuralSurfaceMemberTypeEnum> IfcStructuralSurfaceMemberTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcStructuralSurfaceMemberTypeEnum>(); }
	shared_ptr<IfcStructuralSurfaceMemberTypeEnum> type_object( new IfcStructuralSurfaceMemberTypeEnum() );
	if( _stricmp( arg.c_str(), ".BENDING_ELEMENT." ) == 0 )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_BENDING_ELEMENT;
	}
	else if( _stricmp( arg.c_str(), ".MEMBRANE_ELEMENT." ) == 0 )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_MEMBRANE_ELEMENT;
	}
	else if( _stricmp( arg.c_str(), ".SHELL." ) == 0 )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_SHELL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
