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
#include "include/IfcControllerTypeEnum.h"

// TYPE IfcControllerTypeEnum 
IfcControllerTypeEnum::IfcControllerTypeEnum() {}
IfcControllerTypeEnum::~IfcControllerTypeEnum() {}
shared_ptr<IfcPPObject> IfcControllerTypeEnum::getDeepCopy()
{
	shared_ptr<IfcControllerTypeEnum> copy_self( new IfcControllerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcControllerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONTROLLERTYPEENUM("; }
	if( m_enum == ENUM_FLOATING )
	{
		stream << ".FLOATING.";
	}
	else if( m_enum == ENUM_PROGRAMMABLE )
	{
		stream << ".PROGRAMMABLE.";
	}
	else if( m_enum == ENUM_PROPORTIONAL )
	{
		stream << ".PROPORTIONAL.";
	}
	else if( m_enum == ENUM_MULTIPOSITION )
	{
		stream << ".MULTIPOSITION.";
	}
	else if( m_enum == ENUM_TWOPOSITION )
	{
		stream << ".TWOPOSITION.";
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
shared_ptr<IfcControllerTypeEnum> IfcControllerTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcControllerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcControllerTypeEnum>(); }
	shared_ptr<IfcControllerTypeEnum> type_object( new IfcControllerTypeEnum() );
	if( boost::iequals( arg, L".FLOATING." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_FLOATING;
	}
	else if( boost::iequals( arg, L".PROGRAMMABLE." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_PROGRAMMABLE;
	}
	else if( boost::iequals( arg, L".PROPORTIONAL." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_PROPORTIONAL;
	}
	else if( boost::iequals( arg, L".MULTIPOSITION." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_MULTIPOSITION;
	}
	else if( boost::iequals( arg, L".TWOPOSITION." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_TWOPOSITION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcControllerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
