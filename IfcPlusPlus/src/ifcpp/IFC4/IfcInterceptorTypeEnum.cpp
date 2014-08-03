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
#include "include/IfcInterceptorTypeEnum.h"

// TYPE IfcInterceptorTypeEnum 
IfcInterceptorTypeEnum::IfcInterceptorTypeEnum() {}
IfcInterceptorTypeEnum::~IfcInterceptorTypeEnum() {}
shared_ptr<IfcPPObject> IfcInterceptorTypeEnum::getDeepCopy()
{
	shared_ptr<IfcInterceptorTypeEnum> copy_self( new IfcInterceptorTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcInterceptorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCINTERCEPTORTYPEENUM("; }
	if( m_enum == ENUM_CYCLONIC )
	{
		stream << ".CYCLONIC.";
	}
	else if( m_enum == ENUM_GREASE )
	{
		stream << ".GREASE.";
	}
	else if( m_enum == ENUM_OIL )
	{
		stream << ".OIL.";
	}
	else if( m_enum == ENUM_PETROL )
	{
		stream << ".PETROL.";
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
shared_ptr<IfcInterceptorTypeEnum> IfcInterceptorTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcInterceptorTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcInterceptorTypeEnum>(); }
	shared_ptr<IfcInterceptorTypeEnum> type_object( new IfcInterceptorTypeEnum() );
	if( boost::iequals( arg, L".CYCLONIC." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_CYCLONIC;
	}
	else if( boost::iequals( arg, L".GREASE." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_GREASE;
	}
	else if( boost::iequals( arg, L".OIL." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_OIL;
	}
	else if( boost::iequals( arg, L".PETROL." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_PETROL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
