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
#include "include/IfcJunctionBoxTypeEnum.h"

// TYPE IfcJunctionBoxTypeEnum = ENUMERATION OF	(DATA	,POWER	,USERDEFINED	,NOTDEFINED);
IfcJunctionBoxTypeEnum::IfcJunctionBoxTypeEnum() {}
IfcJunctionBoxTypeEnum::~IfcJunctionBoxTypeEnum() {}
shared_ptr<IfcPPObject> IfcJunctionBoxTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcJunctionBoxTypeEnum> copy_self( new IfcJunctionBoxTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcJunctionBoxTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCJUNCTIONBOXTYPEENUM("; }
	if( m_enum == ENUM_DATA )
	{
		stream << ".DATA.";
	}
	else if( m_enum == ENUM_POWER )
	{
		stream << ".POWER.";
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
shared_ptr<IfcJunctionBoxTypeEnum> IfcJunctionBoxTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcJunctionBoxTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcJunctionBoxTypeEnum>(); }
	shared_ptr<IfcJunctionBoxTypeEnum> type_object( new IfcJunctionBoxTypeEnum() );
	if( boost::iequals( arg, L".DATA." ) )
	{
		type_object->m_enum = IfcJunctionBoxTypeEnum::ENUM_DATA;
	}
	else if( boost::iequals( arg, L".POWER." ) )
	{
		type_object->m_enum = IfcJunctionBoxTypeEnum::ENUM_POWER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcJunctionBoxTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcJunctionBoxTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
