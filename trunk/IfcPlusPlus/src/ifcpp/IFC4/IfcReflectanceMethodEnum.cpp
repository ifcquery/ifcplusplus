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
#include "include/IfcReflectanceMethodEnum.h"

// TYPE IfcReflectanceMethodEnum 
IfcReflectanceMethodEnum::IfcReflectanceMethodEnum() {}
IfcReflectanceMethodEnum::~IfcReflectanceMethodEnum() {}
shared_ptr<IfcPPObject> IfcReflectanceMethodEnum::getDeepCopy()
{
	shared_ptr<IfcReflectanceMethodEnum> copy_self( new IfcReflectanceMethodEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcReflectanceMethodEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREFLECTANCEMETHODENUM("; }
	if( m_enum == ENUM_BLINN )
	{
		stream << ".BLINN.";
	}
	else if( m_enum == ENUM_FLAT )
	{
		stream << ".FLAT.";
	}
	else if( m_enum == ENUM_GLASS )
	{
		stream << ".GLASS.";
	}
	else if( m_enum == ENUM_MATT )
	{
		stream << ".MATT.";
	}
	else if( m_enum == ENUM_METAL )
	{
		stream << ".METAL.";
	}
	else if( m_enum == ENUM_MIRROR )
	{
		stream << ".MIRROR.";
	}
	else if( m_enum == ENUM_PHONG )
	{
		stream << ".PHONG.";
	}
	else if( m_enum == ENUM_PLASTIC )
	{
		stream << ".PLASTIC.";
	}
	else if( m_enum == ENUM_STRAUSS )
	{
		stream << ".STRAUSS.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcReflectanceMethodEnum> IfcReflectanceMethodEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcReflectanceMethodEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcReflectanceMethodEnum>(); }
	shared_ptr<IfcReflectanceMethodEnum> type_object( new IfcReflectanceMethodEnum() );
	if( boost::iequals( arg, L".BLINN." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_BLINN;
	}
	else if( boost::iequals( arg, L".FLAT." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_FLAT;
	}
	else if( boost::iequals( arg, L".GLASS." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_GLASS;
	}
	else if( boost::iequals( arg, L".MATT." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_MATT;
	}
	else if( boost::iequals( arg, L".METAL." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_METAL;
	}
	else if( boost::iequals( arg, L".MIRROR." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_MIRROR;
	}
	else if( boost::iequals( arg, L".PHONG." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_PHONG;
	}
	else if( boost::iequals( arg, L".PLASTIC." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_PLASTIC;
	}
	else if( boost::iequals( arg, L".STRAUSS." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_STRAUSS;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
