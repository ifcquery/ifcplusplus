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
#include "include/IfcFastenerTypeEnum.h"

// TYPE IfcFastenerTypeEnum = ENUMERATION OF	(GLUE	,MORTAR	,WELD	,USERDEFINED	,NOTDEFINED);
IfcFastenerTypeEnum::IfcFastenerTypeEnum() {}
IfcFastenerTypeEnum::~IfcFastenerTypeEnum() {}
shared_ptr<IfcPPObject> IfcFastenerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFastenerTypeEnum> copy_self( new IfcFastenerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFastenerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFASTENERTYPEENUM("; }
	if( m_enum == ENUM_GLUE )
	{
		stream << ".GLUE.";
	}
	else if( m_enum == ENUM_MORTAR )
	{
		stream << ".MORTAR.";
	}
	else if( m_enum == ENUM_WELD )
	{
		stream << ".WELD.";
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
shared_ptr<IfcFastenerTypeEnum> IfcFastenerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFastenerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFastenerTypeEnum>(); }
	shared_ptr<IfcFastenerTypeEnum> type_object( new IfcFastenerTypeEnum() );
	if( boost::iequals( arg, L".GLUE." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_GLUE;
	}
	else if( boost::iequals( arg, L".MORTAR." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_MORTAR;
	}
	else if( boost::iequals( arg, L".WELD." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_WELD;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFastenerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
