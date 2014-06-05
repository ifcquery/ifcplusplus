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
shared_ptr<IfcInterceptorTypeEnum> IfcInterceptorTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcInterceptorTypeEnum>(); }
	shared_ptr<IfcInterceptorTypeEnum> type_object( new IfcInterceptorTypeEnum() );
	if( _stricmp( arg.c_str(), ".CYCLONIC." ) == 0 )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_CYCLONIC;
	}
	else if( _stricmp( arg.c_str(), ".GREASE." ) == 0 )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_GREASE;
	}
	else if( _stricmp( arg.c_str(), ".OIL." ) == 0 )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_OIL;
	}
	else if( _stricmp( arg.c_str(), ".PETROL." ) == 0 )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_PETROL;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcInterceptorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
