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
#include "include/IfcBoilerTypeEnum.h"

// TYPE IfcBoilerTypeEnum = ENUMERATION OF	(WATER	,STEAM	,USERDEFINED	,NOTDEFINED);
IfcBoilerTypeEnum::IfcBoilerTypeEnum() {}
IfcBoilerTypeEnum::~IfcBoilerTypeEnum() {}
shared_ptr<IfcPPObject> IfcBoilerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBoilerTypeEnum> copy_self( new IfcBoilerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBoilerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBOILERTYPEENUM("; }
	if( m_enum == ENUM_WATER )
	{
		stream << ".WATER.";
	}
	else if( m_enum == ENUM_STEAM )
	{
		stream << ".STEAM.";
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
shared_ptr<IfcBoilerTypeEnum> IfcBoilerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBoilerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBoilerTypeEnum>(); }
	shared_ptr<IfcBoilerTypeEnum> type_object( new IfcBoilerTypeEnum() );
	if( boost::iequals( arg, L".WATER." ) )
	{
		type_object->m_enum = IfcBoilerTypeEnum::ENUM_WATER;
	}
	else if( boost::iequals( arg, L".STEAM." ) )
	{
		type_object->m_enum = IfcBoilerTypeEnum::ENUM_STEAM;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcBoilerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcBoilerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
