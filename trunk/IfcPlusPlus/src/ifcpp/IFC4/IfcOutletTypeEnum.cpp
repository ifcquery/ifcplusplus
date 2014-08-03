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
#include "include/IfcOutletTypeEnum.h"

// TYPE IfcOutletTypeEnum 
IfcOutletTypeEnum::IfcOutletTypeEnum() {}
IfcOutletTypeEnum::~IfcOutletTypeEnum() {}
shared_ptr<IfcPPObject> IfcOutletTypeEnum::getDeepCopy()
{
	shared_ptr<IfcOutletTypeEnum> copy_self( new IfcOutletTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcOutletTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOUTLETTYPEENUM("; }
	if( m_enum == ENUM_AUDIOVISUALOUTLET )
	{
		stream << ".AUDIOVISUALOUTLET.";
	}
	else if( m_enum == ENUM_COMMUNICATIONSOUTLET )
	{
		stream << ".COMMUNICATIONSOUTLET.";
	}
	else if( m_enum == ENUM_POWEROUTLET )
	{
		stream << ".POWEROUTLET.";
	}
	else if( m_enum == ENUM_DATAOUTLET )
	{
		stream << ".DATAOUTLET.";
	}
	else if( m_enum == ENUM_TELEPHONEOUTLET )
	{
		stream << ".TELEPHONEOUTLET.";
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
shared_ptr<IfcOutletTypeEnum> IfcOutletTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcOutletTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcOutletTypeEnum>(); }
	shared_ptr<IfcOutletTypeEnum> type_object( new IfcOutletTypeEnum() );
	if( boost::iequals( arg, L".AUDIOVISUALOUTLET." ) )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_AUDIOVISUALOUTLET;
	}
	else if( boost::iequals( arg, L".COMMUNICATIONSOUTLET." ) )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_COMMUNICATIONSOUTLET;
	}
	else if( boost::iequals( arg, L".POWEROUTLET." ) )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_POWEROUTLET;
	}
	else if( boost::iequals( arg, L".DATAOUTLET." ) )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_DATAOUTLET;
	}
	else if( boost::iequals( arg, L".TELEPHONEOUTLET." ) )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_TELEPHONEOUTLET;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcOutletTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
