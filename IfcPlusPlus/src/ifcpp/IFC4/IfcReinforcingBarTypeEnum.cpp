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
#include "include/IfcReinforcingBarTypeEnum.h"

// TYPE IfcReinforcingBarTypeEnum 
IfcReinforcingBarTypeEnum::IfcReinforcingBarTypeEnum() {}
IfcReinforcingBarTypeEnum::~IfcReinforcingBarTypeEnum() {}
shared_ptr<IfcPPObject> IfcReinforcingBarTypeEnum::getDeepCopy()
{
	shared_ptr<IfcReinforcingBarTypeEnum> copy_self( new IfcReinforcingBarTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcReinforcingBarTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREINFORCINGBARTYPEENUM("; }
	if( m_enum == ENUM_ANCHORING )
	{
		stream << ".ANCHORING.";
	}
	else if( m_enum == ENUM_EDGE )
	{
		stream << ".EDGE.";
	}
	else if( m_enum == ENUM_LIGATURE )
	{
		stream << ".LIGATURE.";
	}
	else if( m_enum == ENUM_MAIN )
	{
		stream << ".MAIN.";
	}
	else if( m_enum == ENUM_PUNCHING )
	{
		stream << ".PUNCHING.";
	}
	else if( m_enum == ENUM_RING )
	{
		stream << ".RING.";
	}
	else if( m_enum == ENUM_SHEAR )
	{
		stream << ".SHEAR.";
	}
	else if( m_enum == ENUM_STUD )
	{
		stream << ".STUD.";
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
shared_ptr<IfcReinforcingBarTypeEnum> IfcReinforcingBarTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcReinforcingBarTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcReinforcingBarTypeEnum>(); }
	shared_ptr<IfcReinforcingBarTypeEnum> type_object( new IfcReinforcingBarTypeEnum() );
	if( boost::iequals( arg, L".ANCHORING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_ANCHORING;
	}
	else if( boost::iequals( arg, L".EDGE." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_EDGE;
	}
	else if( boost::iequals( arg, L".LIGATURE." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_LIGATURE;
	}
	else if( boost::iequals( arg, L".MAIN." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_MAIN;
	}
	else if( boost::iequals( arg, L".PUNCHING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_PUNCHING;
	}
	else if( boost::iequals( arg, L".RING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_RING;
	}
	else if( boost::iequals( arg, L".SHEAR." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_SHEAR;
	}
	else if( boost::iequals( arg, L".STUD." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_STUD;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
