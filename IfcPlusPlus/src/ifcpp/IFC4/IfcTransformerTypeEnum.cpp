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
#include "include/IfcTransformerTypeEnum.h"

// TYPE IfcTransformerTypeEnum 
IfcTransformerTypeEnum::IfcTransformerTypeEnum() {}
IfcTransformerTypeEnum::~IfcTransformerTypeEnum() {}
shared_ptr<IfcPPObject> IfcTransformerTypeEnum::getDeepCopy()
{
	shared_ptr<IfcTransformerTypeEnum> copy_self( new IfcTransformerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTransformerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRANSFORMERTYPEENUM("; }
	if( m_enum == ENUM_CURRENT )
	{
		stream << ".CURRENT.";
	}
	else if( m_enum == ENUM_FREQUENCY )
	{
		stream << ".FREQUENCY.";
	}
	else if( m_enum == ENUM_INVERTER )
	{
		stream << ".INVERTER.";
	}
	else if( m_enum == ENUM_RECTIFIER )
	{
		stream << ".RECTIFIER.";
	}
	else if( m_enum == ENUM_VOLTAGE )
	{
		stream << ".VOLTAGE.";
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
shared_ptr<IfcTransformerTypeEnum> IfcTransformerTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTransformerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTransformerTypeEnum>(); }
	shared_ptr<IfcTransformerTypeEnum> type_object( new IfcTransformerTypeEnum() );
	if( boost::iequals( arg, L".CURRENT." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_CURRENT;
	}
	else if( boost::iequals( arg, L".FREQUENCY." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_FREQUENCY;
	}
	else if( boost::iequals( arg, L".INVERTER." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_INVERTER;
	}
	else if( boost::iequals( arg, L".RECTIFIER." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_RECTIFIER;
	}
	else if( boost::iequals( arg, L".VOLTAGE." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_VOLTAGE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
