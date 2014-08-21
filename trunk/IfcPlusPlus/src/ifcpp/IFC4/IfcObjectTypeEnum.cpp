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
#include "include/IfcObjectTypeEnum.h"

// TYPE IfcObjectTypeEnum = ENUMERATION OF	(PRODUCT	,PROCESS	,CONTROL	,RESOURCE	,ACTOR	,GROUP	,PROJECT	,NOTDEFINED);
IfcObjectTypeEnum::IfcObjectTypeEnum() {}
IfcObjectTypeEnum::~IfcObjectTypeEnum() {}
shared_ptr<IfcPPObject> IfcObjectTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcObjectTypeEnum> copy_self( new IfcObjectTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcObjectTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOBJECTTYPEENUM("; }
	if( m_enum == ENUM_PRODUCT )
	{
		stream << ".PRODUCT.";
	}
	else if( m_enum == ENUM_PROCESS )
	{
		stream << ".PROCESS.";
	}
	else if( m_enum == ENUM_CONTROL )
	{
		stream << ".CONTROL.";
	}
	else if( m_enum == ENUM_RESOURCE )
	{
		stream << ".RESOURCE.";
	}
	else if( m_enum == ENUM_ACTOR )
	{
		stream << ".ACTOR.";
	}
	else if( m_enum == ENUM_GROUP )
	{
		stream << ".GROUP.";
	}
	else if( m_enum == ENUM_PROJECT )
	{
		stream << ".PROJECT.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcObjectTypeEnum> IfcObjectTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcObjectTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcObjectTypeEnum>(); }
	shared_ptr<IfcObjectTypeEnum> type_object( new IfcObjectTypeEnum() );
	if( boost::iequals( arg, L".PRODUCT." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PRODUCT;
	}
	else if( boost::iequals( arg, L".PROCESS." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PROCESS;
	}
	else if( boost::iequals( arg, L".CONTROL." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_CONTROL;
	}
	else if( boost::iequals( arg, L".RESOURCE." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_RESOURCE;
	}
	else if( boost::iequals( arg, L".ACTOR." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_ACTOR;
	}
	else if( boost::iequals( arg, L".GROUP." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_GROUP;
	}
	else if( boost::iequals( arg, L".PROJECT." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_PROJECT;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcObjectTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
