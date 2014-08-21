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
#include "include/IfcFireSuppressionTerminalTypeEnum.h"

// TYPE IfcFireSuppressionTerminalTypeEnum = ENUMERATION OF	(BREECHINGINLET	,FIREHYDRANT	,HOSEREEL	,SPRINKLER	,SPRINKLERDEFLECTOR	,USERDEFINED	,NOTDEFINED);
IfcFireSuppressionTerminalTypeEnum::IfcFireSuppressionTerminalTypeEnum() {}
IfcFireSuppressionTerminalTypeEnum::~IfcFireSuppressionTerminalTypeEnum() {}
shared_ptr<IfcPPObject> IfcFireSuppressionTerminalTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFireSuppressionTerminalTypeEnum> copy_self( new IfcFireSuppressionTerminalTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFireSuppressionTerminalTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFIRESUPPRESSIONTERMINALTYPEENUM("; }
	if( m_enum == ENUM_BREECHINGINLET )
	{
		stream << ".BREECHINGINLET.";
	}
	else if( m_enum == ENUM_FIREHYDRANT )
	{
		stream << ".FIREHYDRANT.";
	}
	else if( m_enum == ENUM_HOSEREEL )
	{
		stream << ".HOSEREEL.";
	}
	else if( m_enum == ENUM_SPRINKLER )
	{
		stream << ".SPRINKLER.";
	}
	else if( m_enum == ENUM_SPRINKLERDEFLECTOR )
	{
		stream << ".SPRINKLERDEFLECTOR.";
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
shared_ptr<IfcFireSuppressionTerminalTypeEnum> IfcFireSuppressionTerminalTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFireSuppressionTerminalTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFireSuppressionTerminalTypeEnum>(); }
	shared_ptr<IfcFireSuppressionTerminalTypeEnum> type_object( new IfcFireSuppressionTerminalTypeEnum() );
	if( boost::iequals( arg, L".BREECHINGINLET." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_BREECHINGINLET;
	}
	else if( boost::iequals( arg, L".FIREHYDRANT." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_FIREHYDRANT;
	}
	else if( boost::iequals( arg, L".HOSEREEL." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_HOSEREEL;
	}
	else if( boost::iequals( arg, L".SPRINKLER." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_SPRINKLER;
	}
	else if( boost::iequals( arg, L".SPRINKLERDEFLECTOR." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_SPRINKLERDEFLECTOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFireSuppressionTerminalTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
