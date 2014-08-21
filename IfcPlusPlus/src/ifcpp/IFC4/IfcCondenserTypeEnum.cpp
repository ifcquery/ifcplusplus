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
#include "include/IfcCondenserTypeEnum.h"

// TYPE IfcCondenserTypeEnum = ENUMERATION OF	(AIRCOOLED	,EVAPORATIVECOOLED	,WATERCOOLED	,WATERCOOLEDBRAZEDPLATE	,WATERCOOLEDSHELLCOIL	,WATERCOOLEDSHELLTUBE	,WATERCOOLEDTUBEINTUBE	,USERDEFINED	,NOTDEFINED);
IfcCondenserTypeEnum::IfcCondenserTypeEnum() {}
IfcCondenserTypeEnum::~IfcCondenserTypeEnum() {}
shared_ptr<IfcPPObject> IfcCondenserTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCondenserTypeEnum> copy_self( new IfcCondenserTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCondenserTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONDENSERTYPEENUM("; }
	if( m_enum == ENUM_AIRCOOLED )
	{
		stream << ".AIRCOOLED.";
	}
	else if( m_enum == ENUM_EVAPORATIVECOOLED )
	{
		stream << ".EVAPORATIVECOOLED.";
	}
	else if( m_enum == ENUM_WATERCOOLED )
	{
		stream << ".WATERCOOLED.";
	}
	else if( m_enum == ENUM_WATERCOOLEDBRAZEDPLATE )
	{
		stream << ".WATERCOOLEDBRAZEDPLATE.";
	}
	else if( m_enum == ENUM_WATERCOOLEDSHELLCOIL )
	{
		stream << ".WATERCOOLEDSHELLCOIL.";
	}
	else if( m_enum == ENUM_WATERCOOLEDSHELLTUBE )
	{
		stream << ".WATERCOOLEDSHELLTUBE.";
	}
	else if( m_enum == ENUM_WATERCOOLEDTUBEINTUBE )
	{
		stream << ".WATERCOOLEDTUBEINTUBE.";
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
shared_ptr<IfcCondenserTypeEnum> IfcCondenserTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCondenserTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCondenserTypeEnum>(); }
	shared_ptr<IfcCondenserTypeEnum> type_object( new IfcCondenserTypeEnum() );
	if( boost::iequals( arg, L".AIRCOOLED." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_AIRCOOLED;
	}
	else if( boost::iequals( arg, L".EVAPORATIVECOOLED." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_EVAPORATIVECOOLED;
	}
	else if( boost::iequals( arg, L".WATERCOOLED." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_WATERCOOLED;
	}
	else if( boost::iequals( arg, L".WATERCOOLEDBRAZEDPLATE." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_WATERCOOLEDBRAZEDPLATE;
	}
	else if( boost::iequals( arg, L".WATERCOOLEDSHELLCOIL." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_WATERCOOLEDSHELLCOIL;
	}
	else if( boost::iequals( arg, L".WATERCOOLEDSHELLTUBE." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_WATERCOOLEDSHELLTUBE;
	}
	else if( boost::iequals( arg, L".WATERCOOLEDTUBEINTUBE." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_WATERCOOLEDTUBEINTUBE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCondenserTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
