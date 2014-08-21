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
#include "include/IfcCoilTypeEnum.h"

// TYPE IfcCoilTypeEnum = ENUMERATION OF	(DXCOOLINGCOIL	,ELECTRICHEATINGCOIL	,GASHEATINGCOIL	,HYDRONICCOIL	,STEAMHEATINGCOIL	,WATERCOOLINGCOIL	,WATERHEATINGCOIL	,USERDEFINED	,NOTDEFINED);
IfcCoilTypeEnum::IfcCoilTypeEnum() {}
IfcCoilTypeEnum::~IfcCoilTypeEnum() {}
shared_ptr<IfcPPObject> IfcCoilTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCoilTypeEnum> copy_self( new IfcCoilTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCoilTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOILTYPEENUM("; }
	if( m_enum == ENUM_DXCOOLINGCOIL )
	{
		stream << ".DXCOOLINGCOIL.";
	}
	else if( m_enum == ENUM_ELECTRICHEATINGCOIL )
	{
		stream << ".ELECTRICHEATINGCOIL.";
	}
	else if( m_enum == ENUM_GASHEATINGCOIL )
	{
		stream << ".GASHEATINGCOIL.";
	}
	else if( m_enum == ENUM_HYDRONICCOIL )
	{
		stream << ".HYDRONICCOIL.";
	}
	else if( m_enum == ENUM_STEAMHEATINGCOIL )
	{
		stream << ".STEAMHEATINGCOIL.";
	}
	else if( m_enum == ENUM_WATERCOOLINGCOIL )
	{
		stream << ".WATERCOOLINGCOIL.";
	}
	else if( m_enum == ENUM_WATERHEATINGCOIL )
	{
		stream << ".WATERHEATINGCOIL.";
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
shared_ptr<IfcCoilTypeEnum> IfcCoilTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCoilTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCoilTypeEnum>(); }
	shared_ptr<IfcCoilTypeEnum> type_object( new IfcCoilTypeEnum() );
	if( boost::iequals( arg, L".DXCOOLINGCOIL." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_DXCOOLINGCOIL;
	}
	else if( boost::iequals( arg, L".ELECTRICHEATINGCOIL." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_ELECTRICHEATINGCOIL;
	}
	else if( boost::iequals( arg, L".GASHEATINGCOIL." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_GASHEATINGCOIL;
	}
	else if( boost::iequals( arg, L".HYDRONICCOIL." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_HYDRONICCOIL;
	}
	else if( boost::iequals( arg, L".STEAMHEATINGCOIL." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_STEAMHEATINGCOIL;
	}
	else if( boost::iequals( arg, L".WATERCOOLINGCOIL." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_WATERCOOLINGCOIL;
	}
	else if( boost::iequals( arg, L".WATERHEATINGCOIL." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_WATERHEATINGCOIL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCoilTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
