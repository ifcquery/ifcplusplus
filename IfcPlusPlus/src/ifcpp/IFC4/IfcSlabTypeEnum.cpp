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
#include "include/IfcSlabTypeEnum.h"

// TYPE IfcSlabTypeEnum 
IfcSlabTypeEnum::IfcSlabTypeEnum() {}
IfcSlabTypeEnum::~IfcSlabTypeEnum() {}
shared_ptr<IfcPPObject> IfcSlabTypeEnum::getDeepCopy()
{
	shared_ptr<IfcSlabTypeEnum> copy_self( new IfcSlabTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSlabTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSLABTYPEENUM("; }
	if( m_enum == ENUM_FLOOR )
	{
		stream << ".FLOOR.";
	}
	else if( m_enum == ENUM_ROOF )
	{
		stream << ".ROOF.";
	}
	else if( m_enum == ENUM_LANDING )
	{
		stream << ".LANDING.";
	}
	else if( m_enum == ENUM_BASESLAB )
	{
		stream << ".BASESLAB.";
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
shared_ptr<IfcSlabTypeEnum> IfcSlabTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSlabTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSlabTypeEnum>(); }
	shared_ptr<IfcSlabTypeEnum> type_object( new IfcSlabTypeEnum() );
	if( boost::iequals( arg, L".FLOOR." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_FLOOR;
	}
	else if( boost::iequals( arg, L".ROOF." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_ROOF;
	}
	else if( boost::iequals( arg, L".LANDING." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_LANDING;
	}
	else if( boost::iequals( arg, L".BASESLAB." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_BASESLAB;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
