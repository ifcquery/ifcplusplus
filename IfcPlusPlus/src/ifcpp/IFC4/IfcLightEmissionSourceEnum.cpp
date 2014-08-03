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
#include "include/IfcLightEmissionSourceEnum.h"

// TYPE IfcLightEmissionSourceEnum 
IfcLightEmissionSourceEnum::IfcLightEmissionSourceEnum() {}
IfcLightEmissionSourceEnum::~IfcLightEmissionSourceEnum() {}
shared_ptr<IfcPPObject> IfcLightEmissionSourceEnum::getDeepCopy()
{
	shared_ptr<IfcLightEmissionSourceEnum> copy_self( new IfcLightEmissionSourceEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcLightEmissionSourceEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLIGHTEMISSIONSOURCEENUM("; }
	if( m_enum == ENUM_COMPACTFLUORESCENT )
	{
		stream << ".COMPACTFLUORESCENT.";
	}
	else if( m_enum == ENUM_FLUORESCENT )
	{
		stream << ".FLUORESCENT.";
	}
	else if( m_enum == ENUM_HIGHPRESSUREMERCURY )
	{
		stream << ".HIGHPRESSUREMERCURY.";
	}
	else if( m_enum == ENUM_HIGHPRESSURESODIUM )
	{
		stream << ".HIGHPRESSURESODIUM.";
	}
	else if( m_enum == ENUM_LIGHTEMITTINGDIODE )
	{
		stream << ".LIGHTEMITTINGDIODE.";
	}
	else if( m_enum == ENUM_LOWPRESSURESODIUM )
	{
		stream << ".LOWPRESSURESODIUM.";
	}
	else if( m_enum == ENUM_LOWVOLTAGEHALOGEN )
	{
		stream << ".LOWVOLTAGEHALOGEN.";
	}
	else if( m_enum == ENUM_MAINVOLTAGEHALOGEN )
	{
		stream << ".MAINVOLTAGEHALOGEN.";
	}
	else if( m_enum == ENUM_METALHALIDE )
	{
		stream << ".METALHALIDE.";
	}
	else if( m_enum == ENUM_TUNGSTENFILAMENT )
	{
		stream << ".TUNGSTENFILAMENT.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcLightEmissionSourceEnum> IfcLightEmissionSourceEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcLightEmissionSourceEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcLightEmissionSourceEnum>(); }
	shared_ptr<IfcLightEmissionSourceEnum> type_object( new IfcLightEmissionSourceEnum() );
	if( boost::iequals( arg, L".COMPACTFLUORESCENT." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_COMPACTFLUORESCENT;
	}
	else if( boost::iequals( arg, L".FLUORESCENT." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_FLUORESCENT;
	}
	else if( boost::iequals( arg, L".HIGHPRESSUREMERCURY." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_HIGHPRESSUREMERCURY;
	}
	else if( boost::iequals( arg, L".HIGHPRESSURESODIUM." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_HIGHPRESSURESODIUM;
	}
	else if( boost::iequals( arg, L".LIGHTEMITTINGDIODE." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_LIGHTEMITTINGDIODE;
	}
	else if( boost::iequals( arg, L".LOWPRESSURESODIUM." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_LOWPRESSURESODIUM;
	}
	else if( boost::iequals( arg, L".LOWVOLTAGEHALOGEN." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_LOWVOLTAGEHALOGEN;
	}
	else if( boost::iequals( arg, L".MAINVOLTAGEHALOGEN." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_MAINVOLTAGEHALOGEN;
	}
	else if( boost::iequals( arg, L".METALHALIDE." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_METALHALIDE;
	}
	else if( boost::iequals( arg, L".TUNGSTENFILAMENT." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_TUNGSTENFILAMENT;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcLightEmissionSourceEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
