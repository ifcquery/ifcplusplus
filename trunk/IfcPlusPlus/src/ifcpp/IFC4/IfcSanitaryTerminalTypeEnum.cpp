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
#include "include/IfcSanitaryTerminalTypeEnum.h"

// TYPE IfcSanitaryTerminalTypeEnum = ENUMERATION OF	(BATH	,BIDET	,CISTERN	,SHOWER	,SINK	,SANITARYFOUNTAIN	,TOILETPAN	,URINAL	,WASHHANDBASIN	,WCSEAT	,USERDEFINED	,NOTDEFINED);
IfcSanitaryTerminalTypeEnum::IfcSanitaryTerminalTypeEnum() {}
IfcSanitaryTerminalTypeEnum::~IfcSanitaryTerminalTypeEnum() {}
shared_ptr<IfcPPObject> IfcSanitaryTerminalTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSanitaryTerminalTypeEnum> copy_self( new IfcSanitaryTerminalTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSanitaryTerminalTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSANITARYTERMINALTYPEENUM("; }
	if( m_enum == ENUM_BATH )
	{
		stream << ".BATH.";
	}
	else if( m_enum == ENUM_BIDET )
	{
		stream << ".BIDET.";
	}
	else if( m_enum == ENUM_CISTERN )
	{
		stream << ".CISTERN.";
	}
	else if( m_enum == ENUM_SHOWER )
	{
		stream << ".SHOWER.";
	}
	else if( m_enum == ENUM_SINK )
	{
		stream << ".SINK.";
	}
	else if( m_enum == ENUM_SANITARYFOUNTAIN )
	{
		stream << ".SANITARYFOUNTAIN.";
	}
	else if( m_enum == ENUM_TOILETPAN )
	{
		stream << ".TOILETPAN.";
	}
	else if( m_enum == ENUM_URINAL )
	{
		stream << ".URINAL.";
	}
	else if( m_enum == ENUM_WASHHANDBASIN )
	{
		stream << ".WASHHANDBASIN.";
	}
	else if( m_enum == ENUM_WCSEAT )
	{
		stream << ".WCSEAT.";
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
shared_ptr<IfcSanitaryTerminalTypeEnum> IfcSanitaryTerminalTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSanitaryTerminalTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSanitaryTerminalTypeEnum>(); }
	shared_ptr<IfcSanitaryTerminalTypeEnum> type_object( new IfcSanitaryTerminalTypeEnum() );
	if( boost::iequals( arg, L".BATH." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_BATH;
	}
	else if( boost::iequals( arg, L".BIDET." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_BIDET;
	}
	else if( boost::iequals( arg, L".CISTERN." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_CISTERN;
	}
	else if( boost::iequals( arg, L".SHOWER." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_SHOWER;
	}
	else if( boost::iequals( arg, L".SINK." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_SINK;
	}
	else if( boost::iequals( arg, L".SANITARYFOUNTAIN." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_SANITARYFOUNTAIN;
	}
	else if( boost::iequals( arg, L".TOILETPAN." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_TOILETPAN;
	}
	else if( boost::iequals( arg, L".URINAL." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_URINAL;
	}
	else if( boost::iequals( arg, L".WASHHANDBASIN." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_WASHHANDBASIN;
	}
	else if( boost::iequals( arg, L".WCSEAT." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_WCSEAT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSanitaryTerminalTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
