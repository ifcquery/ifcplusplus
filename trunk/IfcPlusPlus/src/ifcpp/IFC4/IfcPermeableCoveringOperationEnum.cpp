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
#include "include/IfcPermeableCoveringOperationEnum.h"

// TYPE IfcPermeableCoveringOperationEnum 
IfcPermeableCoveringOperationEnum::IfcPermeableCoveringOperationEnum() {}
IfcPermeableCoveringOperationEnum::~IfcPermeableCoveringOperationEnum() {}
void IfcPermeableCoveringOperationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPERMEABLECOVERINGOPERATIONENUM("; }
	if( m_enum == ENUM_GRILL )
	{
		stream << ".GRILL.";
	}
	else if( m_enum == ENUM_LOUVER )
	{
		stream << ".LOUVER.";
	}
	else if( m_enum == ENUM_SCREEN )
	{
		stream << ".SCREEN.";
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
shared_ptr<IfcPermeableCoveringOperationEnum> IfcPermeableCoveringOperationEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPermeableCoveringOperationEnum>(); }
	shared_ptr<IfcPermeableCoveringOperationEnum> type_object( new IfcPermeableCoveringOperationEnum() );
	if( _stricmp( arg.c_str(), ".GRILL." ) == 0 )
	{
		type_object->m_enum = IfcPermeableCoveringOperationEnum::ENUM_GRILL;
	}
	else if( _stricmp( arg.c_str(), ".LOUVER." ) == 0 )
	{
		type_object->m_enum = IfcPermeableCoveringOperationEnum::ENUM_LOUVER;
	}
	else if( _stricmp( arg.c_str(), ".SCREEN." ) == 0 )
	{
		type_object->m_enum = IfcPermeableCoveringOperationEnum::ENUM_SCREEN;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPermeableCoveringOperationEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcPermeableCoveringOperationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
