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
#include "include/IfcAlarmTypeEnum.h"

// TYPE IfcAlarmTypeEnum = ENUMERATION OF	(BELL	,BREAKGLASSBUTTON	,LIGHT	,MANUALPULLBOX	,SIREN	,WHISTLE	,USERDEFINED	,NOTDEFINED);
IfcAlarmTypeEnum::IfcAlarmTypeEnum() {}
IfcAlarmTypeEnum::~IfcAlarmTypeEnum() {}
shared_ptr<IfcPPObject> IfcAlarmTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAlarmTypeEnum> copy_self( new IfcAlarmTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcAlarmTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCALARMTYPEENUM("; }
	if( m_enum == ENUM_BELL )
	{
		stream << ".BELL.";
	}
	else if( m_enum == ENUM_BREAKGLASSBUTTON )
	{
		stream << ".BREAKGLASSBUTTON.";
	}
	else if( m_enum == ENUM_LIGHT )
	{
		stream << ".LIGHT.";
	}
	else if( m_enum == ENUM_MANUALPULLBOX )
	{
		stream << ".MANUALPULLBOX.";
	}
	else if( m_enum == ENUM_SIREN )
	{
		stream << ".SIREN.";
	}
	else if( m_enum == ENUM_WHISTLE )
	{
		stream << ".WHISTLE.";
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
shared_ptr<IfcAlarmTypeEnum> IfcAlarmTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcAlarmTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcAlarmTypeEnum>(); }
	shared_ptr<IfcAlarmTypeEnum> type_object( new IfcAlarmTypeEnum() );
	if( boost::iequals( arg, L".BELL." ) )
	{
		type_object->m_enum = IfcAlarmTypeEnum::ENUM_BELL;
	}
	else if( boost::iequals( arg, L".BREAKGLASSBUTTON." ) )
	{
		type_object->m_enum = IfcAlarmTypeEnum::ENUM_BREAKGLASSBUTTON;
	}
	else if( boost::iequals( arg, L".LIGHT." ) )
	{
		type_object->m_enum = IfcAlarmTypeEnum::ENUM_LIGHT;
	}
	else if( boost::iequals( arg, L".MANUALPULLBOX." ) )
	{
		type_object->m_enum = IfcAlarmTypeEnum::ENUM_MANUALPULLBOX;
	}
	else if( boost::iequals( arg, L".SIREN." ) )
	{
		type_object->m_enum = IfcAlarmTypeEnum::ENUM_SIREN;
	}
	else if( boost::iequals( arg, L".WHISTLE." ) )
	{
		type_object->m_enum = IfcAlarmTypeEnum::ENUM_WHISTLE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcAlarmTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcAlarmTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
