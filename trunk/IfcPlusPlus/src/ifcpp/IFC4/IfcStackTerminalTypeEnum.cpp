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
#include "include/IfcStackTerminalTypeEnum.h"

// TYPE IfcStackTerminalTypeEnum 
IfcStackTerminalTypeEnum::IfcStackTerminalTypeEnum() {}
IfcStackTerminalTypeEnum::~IfcStackTerminalTypeEnum() {}
shared_ptr<IfcPPObject> IfcStackTerminalTypeEnum::getDeepCopy()
{
	shared_ptr<IfcStackTerminalTypeEnum> copy_self( new IfcStackTerminalTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcStackTerminalTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTACKTERMINALTYPEENUM("; }
	if( m_enum == ENUM_BIRDCAGE )
	{
		stream << ".BIRDCAGE.";
	}
	else if( m_enum == ENUM_COWL )
	{
		stream << ".COWL.";
	}
	else if( m_enum == ENUM_RAINWATERHOPPER )
	{
		stream << ".RAINWATERHOPPER.";
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
shared_ptr<IfcStackTerminalTypeEnum> IfcStackTerminalTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcStackTerminalTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcStackTerminalTypeEnum>(); }
	shared_ptr<IfcStackTerminalTypeEnum> type_object( new IfcStackTerminalTypeEnum() );
	if( boost::iequals( arg, L".BIRDCAGE." ) )
	{
		type_object->m_enum = IfcStackTerminalTypeEnum::ENUM_BIRDCAGE;
	}
	else if( boost::iequals( arg, L".COWL." ) )
	{
		type_object->m_enum = IfcStackTerminalTypeEnum::ENUM_COWL;
	}
	else if( boost::iequals( arg, L".RAINWATERHOPPER." ) )
	{
		type_object->m_enum = IfcStackTerminalTypeEnum::ENUM_RAINWATERHOPPER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcStackTerminalTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcStackTerminalTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
