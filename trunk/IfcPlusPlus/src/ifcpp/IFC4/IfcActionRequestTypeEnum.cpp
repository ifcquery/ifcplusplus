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
#include "include/IfcActionRequestTypeEnum.h"

// TYPE IfcActionRequestTypeEnum = ENUMERATION OF	(EMAIL	,FAX	,PHONE	,POST	,VERBAL	,USERDEFINED	,NOTDEFINED);
IfcActionRequestTypeEnum::IfcActionRequestTypeEnum() {}
IfcActionRequestTypeEnum::~IfcActionRequestTypeEnum() {}
shared_ptr<IfcPPObject> IfcActionRequestTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcActionRequestTypeEnum> copy_self( new IfcActionRequestTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcActionRequestTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCACTIONREQUESTTYPEENUM("; }
	if( m_enum == ENUM_EMAIL )
	{
		stream << ".EMAIL.";
	}
	else if( m_enum == ENUM_FAX )
	{
		stream << ".FAX.";
	}
	else if( m_enum == ENUM_PHONE )
	{
		stream << ".PHONE.";
	}
	else if( m_enum == ENUM_POST )
	{
		stream << ".POST.";
	}
	else if( m_enum == ENUM_VERBAL )
	{
		stream << ".VERBAL.";
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
shared_ptr<IfcActionRequestTypeEnum> IfcActionRequestTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcActionRequestTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcActionRequestTypeEnum>(); }
	shared_ptr<IfcActionRequestTypeEnum> type_object( new IfcActionRequestTypeEnum() );
	if( boost::iequals( arg, L".EMAIL." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_EMAIL;
	}
	else if( boost::iequals( arg, L".FAX." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_FAX;
	}
	else if( boost::iequals( arg, L".PHONE." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_PHONE;
	}
	else if( boost::iequals( arg, L".POST." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_POST;
	}
	else if( boost::iequals( arg, L".VERBAL." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_VERBAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
