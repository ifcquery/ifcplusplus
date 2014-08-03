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
#include "include/IfcPumpTypeEnum.h"

// TYPE IfcPumpTypeEnum 
IfcPumpTypeEnum::IfcPumpTypeEnum() {}
IfcPumpTypeEnum::~IfcPumpTypeEnum() {}
shared_ptr<IfcPPObject> IfcPumpTypeEnum::getDeepCopy()
{
	shared_ptr<IfcPumpTypeEnum> copy_self( new IfcPumpTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPumpTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPUMPTYPEENUM("; }
	if( m_enum == ENUM_CIRCULATOR )
	{
		stream << ".CIRCULATOR.";
	}
	else if( m_enum == ENUM_ENDSUCTION )
	{
		stream << ".ENDSUCTION.";
	}
	else if( m_enum == ENUM_SPLITCASE )
	{
		stream << ".SPLITCASE.";
	}
	else if( m_enum == ENUM_SUBMERSIBLEPUMP )
	{
		stream << ".SUBMERSIBLEPUMP.";
	}
	else if( m_enum == ENUM_SUMPPUMP )
	{
		stream << ".SUMPPUMP.";
	}
	else if( m_enum == ENUM_VERTICALINLINE )
	{
		stream << ".VERTICALINLINE.";
	}
	else if( m_enum == ENUM_VERTICALTURBINE )
	{
		stream << ".VERTICALTURBINE.";
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
shared_ptr<IfcPumpTypeEnum> IfcPumpTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPumpTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPumpTypeEnum>(); }
	shared_ptr<IfcPumpTypeEnum> type_object( new IfcPumpTypeEnum() );
	if( boost::iequals( arg, L".CIRCULATOR." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_CIRCULATOR;
	}
	else if( boost::iequals( arg, L".ENDSUCTION." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_ENDSUCTION;
	}
	else if( boost::iequals( arg, L".SPLITCASE." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SPLITCASE;
	}
	else if( boost::iequals( arg, L".SUBMERSIBLEPUMP." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SUBMERSIBLEPUMP;
	}
	else if( boost::iequals( arg, L".SUMPPUMP." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_SUMPPUMP;
	}
	else if( boost::iequals( arg, L".VERTICALINLINE." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_VERTICALINLINE;
	}
	else if( boost::iequals( arg, L".VERTICALTURBINE." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_VERTICALTURBINE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPumpTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
