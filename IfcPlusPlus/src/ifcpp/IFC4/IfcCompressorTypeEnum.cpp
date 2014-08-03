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
#include "include/IfcCompressorTypeEnum.h"

// TYPE IfcCompressorTypeEnum 
IfcCompressorTypeEnum::IfcCompressorTypeEnum() {}
IfcCompressorTypeEnum::~IfcCompressorTypeEnum() {}
shared_ptr<IfcPPObject> IfcCompressorTypeEnum::getDeepCopy()
{
	shared_ptr<IfcCompressorTypeEnum> copy_self( new IfcCompressorTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCompressorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOMPRESSORTYPEENUM("; }
	if( m_enum == ENUM_DYNAMIC )
	{
		stream << ".DYNAMIC.";
	}
	else if( m_enum == ENUM_RECIPROCATING )
	{
		stream << ".RECIPROCATING.";
	}
	else if( m_enum == ENUM_ROTARY )
	{
		stream << ".ROTARY.";
	}
	else if( m_enum == ENUM_SCROLL )
	{
		stream << ".SCROLL.";
	}
	else if( m_enum == ENUM_TROCHOIDAL )
	{
		stream << ".TROCHOIDAL.";
	}
	else if( m_enum == ENUM_SINGLESTAGE )
	{
		stream << ".SINGLESTAGE.";
	}
	else if( m_enum == ENUM_BOOSTER )
	{
		stream << ".BOOSTER.";
	}
	else if( m_enum == ENUM_OPENTYPE )
	{
		stream << ".OPENTYPE.";
	}
	else if( m_enum == ENUM_HERMETIC )
	{
		stream << ".HERMETIC.";
	}
	else if( m_enum == ENUM_SEMIHERMETIC )
	{
		stream << ".SEMIHERMETIC.";
	}
	else if( m_enum == ENUM_WELDEDSHELLHERMETIC )
	{
		stream << ".WELDEDSHELLHERMETIC.";
	}
	else if( m_enum == ENUM_ROLLINGPISTON )
	{
		stream << ".ROLLINGPISTON.";
	}
	else if( m_enum == ENUM_ROTARYVANE )
	{
		stream << ".ROTARYVANE.";
	}
	else if( m_enum == ENUM_SINGLESCREW )
	{
		stream << ".SINGLESCREW.";
	}
	else if( m_enum == ENUM_TWINSCREW )
	{
		stream << ".TWINSCREW.";
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
shared_ptr<IfcCompressorTypeEnum> IfcCompressorTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCompressorTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCompressorTypeEnum>(); }
	shared_ptr<IfcCompressorTypeEnum> type_object( new IfcCompressorTypeEnum() );
	if( boost::iequals( arg, L".DYNAMIC." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_DYNAMIC;
	}
	else if( boost::iequals( arg, L".RECIPROCATING." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_RECIPROCATING;
	}
	else if( boost::iequals( arg, L".ROTARY." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_ROTARY;
	}
	else if( boost::iequals( arg, L".SCROLL." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_SCROLL;
	}
	else if( boost::iequals( arg, L".TROCHOIDAL." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_TROCHOIDAL;
	}
	else if( boost::iequals( arg, L".SINGLESTAGE." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_SINGLESTAGE;
	}
	else if( boost::iequals( arg, L".BOOSTER." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_BOOSTER;
	}
	else if( boost::iequals( arg, L".OPENTYPE." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_OPENTYPE;
	}
	else if( boost::iequals( arg, L".HERMETIC." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_HERMETIC;
	}
	else if( boost::iequals( arg, L".SEMIHERMETIC." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_SEMIHERMETIC;
	}
	else if( boost::iequals( arg, L".WELDEDSHELLHERMETIC." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_WELDEDSHELLHERMETIC;
	}
	else if( boost::iequals( arg, L".ROLLINGPISTON." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_ROLLINGPISTON;
	}
	else if( boost::iequals( arg, L".ROTARYVANE." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_ROTARYVANE;
	}
	else if( boost::iequals( arg, L".SINGLESCREW." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_SINGLESCREW;
	}
	else if( boost::iequals( arg, L".TWINSCREW." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_TWINSCREW;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCompressorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
