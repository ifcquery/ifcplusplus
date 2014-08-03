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
#include "include/IfcCooledBeamTypeEnum.h"

// TYPE IfcCooledBeamTypeEnum 
IfcCooledBeamTypeEnum::IfcCooledBeamTypeEnum() {}
IfcCooledBeamTypeEnum::~IfcCooledBeamTypeEnum() {}
shared_ptr<IfcPPObject> IfcCooledBeamTypeEnum::getDeepCopy()
{
	shared_ptr<IfcCooledBeamTypeEnum> copy_self( new IfcCooledBeamTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCooledBeamTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOOLEDBEAMTYPEENUM("; }
	if( m_enum == ENUM_ACTIVE )
	{
		stream << ".ACTIVE.";
	}
	else if( m_enum == ENUM_PASSIVE )
	{
		stream << ".PASSIVE.";
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
shared_ptr<IfcCooledBeamTypeEnum> IfcCooledBeamTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCooledBeamTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCooledBeamTypeEnum>(); }
	shared_ptr<IfcCooledBeamTypeEnum> type_object( new IfcCooledBeamTypeEnum() );
	if( boost::iequals( arg, L".ACTIVE." ) )
	{
		type_object->m_enum = IfcCooledBeamTypeEnum::ENUM_ACTIVE;
	}
	else if( boost::iequals( arg, L".PASSIVE." ) )
	{
		type_object->m_enum = IfcCooledBeamTypeEnum::ENUM_PASSIVE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCooledBeamTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCooledBeamTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
