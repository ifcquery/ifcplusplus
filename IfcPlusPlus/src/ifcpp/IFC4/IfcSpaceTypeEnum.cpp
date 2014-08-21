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
#include "include/IfcSpaceTypeEnum.h"

// TYPE IfcSpaceTypeEnum = ENUMERATION OF	(SPACE	,PARKING	,GFA	,INTERNAL	,EXTERNAL	,USERDEFINED	,NOTDEFINED);
IfcSpaceTypeEnum::IfcSpaceTypeEnum() {}
IfcSpaceTypeEnum::~IfcSpaceTypeEnum() {}
shared_ptr<IfcPPObject> IfcSpaceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSpaceTypeEnum> copy_self( new IfcSpaceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSpaceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSPACETYPEENUM("; }
	if( m_enum == ENUM_SPACE )
	{
		stream << ".SPACE.";
	}
	else if( m_enum == ENUM_PARKING )
	{
		stream << ".PARKING.";
	}
	else if( m_enum == ENUM_GFA )
	{
		stream << ".GFA.";
	}
	else if( m_enum == ENUM_INTERNAL )
	{
		stream << ".INTERNAL.";
	}
	else if( m_enum == ENUM_EXTERNAL )
	{
		stream << ".EXTERNAL.";
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
shared_ptr<IfcSpaceTypeEnum> IfcSpaceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSpaceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSpaceTypeEnum>(); }
	shared_ptr<IfcSpaceTypeEnum> type_object( new IfcSpaceTypeEnum() );
	if( boost::iequals( arg, L".SPACE." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_SPACE;
	}
	else if( boost::iequals( arg, L".PARKING." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_PARKING;
	}
	else if( boost::iequals( arg, L".GFA." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_GFA;
	}
	else if( boost::iequals( arg, L".INTERNAL." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_INTERNAL;
	}
	else if( boost::iequals( arg, L".EXTERNAL." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_EXTERNAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
