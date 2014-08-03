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
#include "include/IfcAirTerminalTypeEnum.h"

// TYPE IfcAirTerminalTypeEnum 
IfcAirTerminalTypeEnum::IfcAirTerminalTypeEnum() {}
IfcAirTerminalTypeEnum::~IfcAirTerminalTypeEnum() {}
shared_ptr<IfcPPObject> IfcAirTerminalTypeEnum::getDeepCopy()
{
	shared_ptr<IfcAirTerminalTypeEnum> copy_self( new IfcAirTerminalTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcAirTerminalTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCAIRTERMINALTYPEENUM("; }
	if( m_enum == ENUM_DIFFUSER )
	{
		stream << ".DIFFUSER.";
	}
	else if( m_enum == ENUM_GRILLE )
	{
		stream << ".GRILLE.";
	}
	else if( m_enum == ENUM_LOUVRE )
	{
		stream << ".LOUVRE.";
	}
	else if( m_enum == ENUM_REGISTER )
	{
		stream << ".REGISTER.";
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
shared_ptr<IfcAirTerminalTypeEnum> IfcAirTerminalTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcAirTerminalTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcAirTerminalTypeEnum>(); }
	shared_ptr<IfcAirTerminalTypeEnum> type_object( new IfcAirTerminalTypeEnum() );
	if( boost::iequals( arg, L".DIFFUSER." ) )
	{
		type_object->m_enum = IfcAirTerminalTypeEnum::ENUM_DIFFUSER;
	}
	else if( boost::iequals( arg, L".GRILLE." ) )
	{
		type_object->m_enum = IfcAirTerminalTypeEnum::ENUM_GRILLE;
	}
	else if( boost::iequals( arg, L".LOUVRE." ) )
	{
		type_object->m_enum = IfcAirTerminalTypeEnum::ENUM_LOUVRE;
	}
	else if( boost::iequals( arg, L".REGISTER." ) )
	{
		type_object->m_enum = IfcAirTerminalTypeEnum::ENUM_REGISTER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcAirTerminalTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcAirTerminalTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
