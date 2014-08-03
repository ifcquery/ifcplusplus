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
#include "include/IfcPileConstructionEnum.h"

// TYPE IfcPileConstructionEnum 
IfcPileConstructionEnum::IfcPileConstructionEnum() {}
IfcPileConstructionEnum::~IfcPileConstructionEnum() {}
shared_ptr<IfcPPObject> IfcPileConstructionEnum::getDeepCopy()
{
	shared_ptr<IfcPileConstructionEnum> copy_self( new IfcPileConstructionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPileConstructionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPILECONSTRUCTIONENUM("; }
	if( m_enum == ENUM_CAST_IN_PLACE )
	{
		stream << ".CAST_IN_PLACE.";
	}
	else if( m_enum == ENUM_COMPOSITE )
	{
		stream << ".COMPOSITE.";
	}
	else if( m_enum == ENUM_PRECAST_CONCRETE )
	{
		stream << ".PRECAST_CONCRETE.";
	}
	else if( m_enum == ENUM_PREFAB_STEEL )
	{
		stream << ".PREFAB_STEEL.";
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
shared_ptr<IfcPileConstructionEnum> IfcPileConstructionEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPileConstructionEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPileConstructionEnum>(); }
	shared_ptr<IfcPileConstructionEnum> type_object( new IfcPileConstructionEnum() );
	if( boost::iequals( arg, L".CAST_IN_PLACE." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_CAST_IN_PLACE;
	}
	else if( boost::iequals( arg, L".COMPOSITE." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_COMPOSITE;
	}
	else if( boost::iequals( arg, L".PRECAST_CONCRETE." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_PRECAST_CONCRETE;
	}
	else if( boost::iequals( arg, L".PREFAB_STEEL." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_PREFAB_STEEL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
