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
#include "include/IfcDiscreteAccessoryTypeEnum.h"

// TYPE IfcDiscreteAccessoryTypeEnum 
IfcDiscreteAccessoryTypeEnum::IfcDiscreteAccessoryTypeEnum() {}
IfcDiscreteAccessoryTypeEnum::~IfcDiscreteAccessoryTypeEnum() {}
shared_ptr<IfcPPObject> IfcDiscreteAccessoryTypeEnum::getDeepCopy()
{
	shared_ptr<IfcDiscreteAccessoryTypeEnum> copy_self( new IfcDiscreteAccessoryTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDiscreteAccessoryTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDISCRETEACCESSORYTYPEENUM("; }
	if( m_enum == ENUM_ANCHORPLATE )
	{
		stream << ".ANCHORPLATE.";
	}
	else if( m_enum == ENUM_BRACKET )
	{
		stream << ".BRACKET.";
	}
	else if( m_enum == ENUM_SHOE )
	{
		stream << ".SHOE.";
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
shared_ptr<IfcDiscreteAccessoryTypeEnum> IfcDiscreteAccessoryTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDiscreteAccessoryTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDiscreteAccessoryTypeEnum>(); }
	shared_ptr<IfcDiscreteAccessoryTypeEnum> type_object( new IfcDiscreteAccessoryTypeEnum() );
	if( boost::iequals( arg, L".ANCHORPLATE." ) )
	{
		type_object->m_enum = IfcDiscreteAccessoryTypeEnum::ENUM_ANCHORPLATE;
	}
	else if( boost::iequals( arg, L".BRACKET." ) )
	{
		type_object->m_enum = IfcDiscreteAccessoryTypeEnum::ENUM_BRACKET;
	}
	else if( boost::iequals( arg, L".SHOE." ) )
	{
		type_object->m_enum = IfcDiscreteAccessoryTypeEnum::ENUM_SHOE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDiscreteAccessoryTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDiscreteAccessoryTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
