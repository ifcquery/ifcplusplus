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
#include "include/IfcValveTypeEnum.h"

// TYPE IfcValveTypeEnum 
IfcValveTypeEnum::IfcValveTypeEnum() {}
IfcValveTypeEnum::~IfcValveTypeEnum() {}
shared_ptr<IfcPPObject> IfcValveTypeEnum::getDeepCopy()
{
	shared_ptr<IfcValveTypeEnum> copy_self( new IfcValveTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcValveTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCVALVETYPEENUM("; }
	if( m_enum == ENUM_AIRRELEASE )
	{
		stream << ".AIRRELEASE.";
	}
	else if( m_enum == ENUM_ANTIVACUUM )
	{
		stream << ".ANTIVACUUM.";
	}
	else if( m_enum == ENUM_CHANGEOVER )
	{
		stream << ".CHANGEOVER.";
	}
	else if( m_enum == ENUM_CHECK )
	{
		stream << ".CHECK.";
	}
	else if( m_enum == ENUM_COMMISSIONING )
	{
		stream << ".COMMISSIONING.";
	}
	else if( m_enum == ENUM_DIVERTING )
	{
		stream << ".DIVERTING.";
	}
	else if( m_enum == ENUM_DRAWOFFCOCK )
	{
		stream << ".DRAWOFFCOCK.";
	}
	else if( m_enum == ENUM_DOUBLECHECK )
	{
		stream << ".DOUBLECHECK.";
	}
	else if( m_enum == ENUM_DOUBLEREGULATING )
	{
		stream << ".DOUBLEREGULATING.";
	}
	else if( m_enum == ENUM_FAUCET )
	{
		stream << ".FAUCET.";
	}
	else if( m_enum == ENUM_FLUSHING )
	{
		stream << ".FLUSHING.";
	}
	else if( m_enum == ENUM_GASCOCK )
	{
		stream << ".GASCOCK.";
	}
	else if( m_enum == ENUM_GASTAP )
	{
		stream << ".GASTAP.";
	}
	else if( m_enum == ENUM_ISOLATING )
	{
		stream << ".ISOLATING.";
	}
	else if( m_enum == ENUM_MIXING )
	{
		stream << ".MIXING.";
	}
	else if( m_enum == ENUM_PRESSUREREDUCING )
	{
		stream << ".PRESSUREREDUCING.";
	}
	else if( m_enum == ENUM_PRESSURERELIEF )
	{
		stream << ".PRESSURERELIEF.";
	}
	else if( m_enum == ENUM_REGULATING )
	{
		stream << ".REGULATING.";
	}
	else if( m_enum == ENUM_SAFETYCUTOFF )
	{
		stream << ".SAFETYCUTOFF.";
	}
	else if( m_enum == ENUM_STEAMTRAP )
	{
		stream << ".STEAMTRAP.";
	}
	else if( m_enum == ENUM_STOPCOCK )
	{
		stream << ".STOPCOCK.";
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
shared_ptr<IfcValveTypeEnum> IfcValveTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcValveTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcValveTypeEnum>(); }
	shared_ptr<IfcValveTypeEnum> type_object( new IfcValveTypeEnum() );
	if( boost::iequals( arg, L".AIRRELEASE." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_AIRRELEASE;
	}
	else if( boost::iequals( arg, L".ANTIVACUUM." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_ANTIVACUUM;
	}
	else if( boost::iequals( arg, L".CHANGEOVER." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_CHANGEOVER;
	}
	else if( boost::iequals( arg, L".CHECK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_CHECK;
	}
	else if( boost::iequals( arg, L".COMMISSIONING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_COMMISSIONING;
	}
	else if( boost::iequals( arg, L".DIVERTING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DIVERTING;
	}
	else if( boost::iequals( arg, L".DRAWOFFCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DRAWOFFCOCK;
	}
	else if( boost::iequals( arg, L".DOUBLECHECK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DOUBLECHECK;
	}
	else if( boost::iequals( arg, L".DOUBLEREGULATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DOUBLEREGULATING;
	}
	else if( boost::iequals( arg, L".FAUCET." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_FAUCET;
	}
	else if( boost::iequals( arg, L".FLUSHING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_FLUSHING;
	}
	else if( boost::iequals( arg, L".GASCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_GASCOCK;
	}
	else if( boost::iequals( arg, L".GASTAP." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_GASTAP;
	}
	else if( boost::iequals( arg, L".ISOLATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_ISOLATING;
	}
	else if( boost::iequals( arg, L".MIXING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_MIXING;
	}
	else if( boost::iequals( arg, L".PRESSUREREDUCING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_PRESSUREREDUCING;
	}
	else if( boost::iequals( arg, L".PRESSURERELIEF." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_PRESSURERELIEF;
	}
	else if( boost::iequals( arg, L".REGULATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_REGULATING;
	}
	else if( boost::iequals( arg, L".SAFETYCUTOFF." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_SAFETYCUTOFF;
	}
	else if( boost::iequals( arg, L".STEAMTRAP." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_STEAMTRAP;
	}
	else if( boost::iequals( arg, L".STOPCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_STOPCOCK;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
