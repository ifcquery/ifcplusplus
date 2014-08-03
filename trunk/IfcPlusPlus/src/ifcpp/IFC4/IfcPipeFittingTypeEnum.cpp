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
#include "include/IfcPipeFittingTypeEnum.h"

// TYPE IfcPipeFittingTypeEnum 
IfcPipeFittingTypeEnum::IfcPipeFittingTypeEnum() {}
IfcPipeFittingTypeEnum::~IfcPipeFittingTypeEnum() {}
shared_ptr<IfcPPObject> IfcPipeFittingTypeEnum::getDeepCopy()
{
	shared_ptr<IfcPipeFittingTypeEnum> copy_self( new IfcPipeFittingTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPipeFittingTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPIPEFITTINGTYPEENUM("; }
	if( m_enum == ENUM_BEND )
	{
		stream << ".BEND.";
	}
	else if( m_enum == ENUM_CONNECTOR )
	{
		stream << ".CONNECTOR.";
	}
	else if( m_enum == ENUM_ENTRY )
	{
		stream << ".ENTRY.";
	}
	else if( m_enum == ENUM_EXIT )
	{
		stream << ".EXIT.";
	}
	else if( m_enum == ENUM_JUNCTION )
	{
		stream << ".JUNCTION.";
	}
	else if( m_enum == ENUM_OBSTRUCTION )
	{
		stream << ".OBSTRUCTION.";
	}
	else if( m_enum == ENUM_TRANSITION )
	{
		stream << ".TRANSITION.";
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
shared_ptr<IfcPipeFittingTypeEnum> IfcPipeFittingTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPipeFittingTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPipeFittingTypeEnum>(); }
	shared_ptr<IfcPipeFittingTypeEnum> type_object( new IfcPipeFittingTypeEnum() );
	if( boost::iequals( arg, L".BEND." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_BEND;
	}
	else if( boost::iequals( arg, L".CONNECTOR." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_CONNECTOR;
	}
	else if( boost::iequals( arg, L".ENTRY." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_ENTRY;
	}
	else if( boost::iequals( arg, L".EXIT." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_EXIT;
	}
	else if( boost::iequals( arg, L".JUNCTION." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_JUNCTION;
	}
	else if( boost::iequals( arg, L".OBSTRUCTION." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_OBSTRUCTION;
	}
	else if( boost::iequals( arg, L".TRANSITION." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_TRANSITION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPipeFittingTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
