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
#include "include/IfcMotorConnectionTypeEnum.h"

// TYPE IfcMotorConnectionTypeEnum 
IfcMotorConnectionTypeEnum::IfcMotorConnectionTypeEnum() {}
IfcMotorConnectionTypeEnum::~IfcMotorConnectionTypeEnum() {}
shared_ptr<IfcPPObject> IfcMotorConnectionTypeEnum::getDeepCopy()
{
	shared_ptr<IfcMotorConnectionTypeEnum> copy_self( new IfcMotorConnectionTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcMotorConnectionTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCMOTORCONNECTIONTYPEENUM("; }
	if( m_enum == ENUM_BELTDRIVE )
	{
		stream << ".BELTDRIVE.";
	}
	else if( m_enum == ENUM_COUPLING )
	{
		stream << ".COUPLING.";
	}
	else if( m_enum == ENUM_DIRECTDRIVE )
	{
		stream << ".DIRECTDRIVE.";
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
shared_ptr<IfcMotorConnectionTypeEnum> IfcMotorConnectionTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcMotorConnectionTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcMotorConnectionTypeEnum>(); }
	shared_ptr<IfcMotorConnectionTypeEnum> type_object( new IfcMotorConnectionTypeEnum() );
	if( boost::iequals( arg, L".BELTDRIVE." ) )
	{
		type_object->m_enum = IfcMotorConnectionTypeEnum::ENUM_BELTDRIVE;
	}
	else if( boost::iequals( arg, L".COUPLING." ) )
	{
		type_object->m_enum = IfcMotorConnectionTypeEnum::ENUM_COUPLING;
	}
	else if( boost::iequals( arg, L".DIRECTDRIVE." ) )
	{
		type_object->m_enum = IfcMotorConnectionTypeEnum::ENUM_DIRECTDRIVE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcMotorConnectionTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcMotorConnectionTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
