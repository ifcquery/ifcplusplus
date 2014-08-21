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
#include "include/IfcTendonTypeEnum.h"

// TYPE IfcTendonTypeEnum = ENUMERATION OF	(BAR	,COATED	,STRAND	,WIRE	,USERDEFINED	,NOTDEFINED);
IfcTendonTypeEnum::IfcTendonTypeEnum() {}
IfcTendonTypeEnum::~IfcTendonTypeEnum() {}
shared_ptr<IfcPPObject> IfcTendonTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTendonTypeEnum> copy_self( new IfcTendonTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTendonTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTENDONTYPEENUM("; }
	if( m_enum == ENUM_BAR )
	{
		stream << ".BAR.";
	}
	else if( m_enum == ENUM_COATED )
	{
		stream << ".COATED.";
	}
	else if( m_enum == ENUM_STRAND )
	{
		stream << ".STRAND.";
	}
	else if( m_enum == ENUM_WIRE )
	{
		stream << ".WIRE.";
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
shared_ptr<IfcTendonTypeEnum> IfcTendonTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTendonTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTendonTypeEnum>(); }
	shared_ptr<IfcTendonTypeEnum> type_object( new IfcTendonTypeEnum() );
	if( boost::iequals( arg, L".BAR." ) )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_BAR;
	}
	else if( boost::iequals( arg, L".COATED." ) )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_COATED;
	}
	else if( boost::iequals( arg, L".STRAND." ) )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_STRAND;
	}
	else if( boost::iequals( arg, L".WIRE." ) )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_WIRE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTendonTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
