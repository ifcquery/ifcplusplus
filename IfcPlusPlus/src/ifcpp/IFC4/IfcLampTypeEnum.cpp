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
#include "include/IfcLampTypeEnum.h"

// TYPE IfcLampTypeEnum 
IfcLampTypeEnum::IfcLampTypeEnum() {}
IfcLampTypeEnum::~IfcLampTypeEnum() {}
shared_ptr<IfcPPObject> IfcLampTypeEnum::getDeepCopy()
{
	shared_ptr<IfcLampTypeEnum> copy_self( new IfcLampTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcLampTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLAMPTYPEENUM("; }
	if( m_enum == ENUM_COMPACTFLUORESCENT )
	{
		stream << ".COMPACTFLUORESCENT.";
	}
	else if( m_enum == ENUM_FLUORESCENT )
	{
		stream << ".FLUORESCENT.";
	}
	else if( m_enum == ENUM_HALOGEN )
	{
		stream << ".HALOGEN.";
	}
	else if( m_enum == ENUM_HIGHPRESSUREMERCURY )
	{
		stream << ".HIGHPRESSUREMERCURY.";
	}
	else if( m_enum == ENUM_HIGHPRESSURESODIUM )
	{
		stream << ".HIGHPRESSURESODIUM.";
	}
	else if( m_enum == ENUM_LED )
	{
		stream << ".LED.";
	}
	else if( m_enum == ENUM_METALHALIDE )
	{
		stream << ".METALHALIDE.";
	}
	else if( m_enum == ENUM_OLED )
	{
		stream << ".OLED.";
	}
	else if( m_enum == ENUM_TUNGSTENFILAMENT )
	{
		stream << ".TUNGSTENFILAMENT.";
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
shared_ptr<IfcLampTypeEnum> IfcLampTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcLampTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcLampTypeEnum>(); }
	shared_ptr<IfcLampTypeEnum> type_object( new IfcLampTypeEnum() );
	if( boost::iequals( arg, L".COMPACTFLUORESCENT." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_COMPACTFLUORESCENT;
	}
	else if( boost::iequals( arg, L".FLUORESCENT." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_FLUORESCENT;
	}
	else if( boost::iequals( arg, L".HALOGEN." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_HALOGEN;
	}
	else if( boost::iequals( arg, L".HIGHPRESSUREMERCURY." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_HIGHPRESSUREMERCURY;
	}
	else if( boost::iequals( arg, L".HIGHPRESSURESODIUM." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_HIGHPRESSURESODIUM;
	}
	else if( boost::iequals( arg, L".LED." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_LED;
	}
	else if( boost::iequals( arg, L".METALHALIDE." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_METALHALIDE;
	}
	else if( boost::iequals( arg, L".OLED." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_OLED;
	}
	else if( boost::iequals( arg, L".TUNGSTENFILAMENT." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_TUNGSTENFILAMENT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcLampTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
