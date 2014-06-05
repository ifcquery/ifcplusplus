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
#include "include/IfcAirToAirHeatRecoveryTypeEnum.h"

// TYPE IfcAirToAirHeatRecoveryTypeEnum 
IfcAirToAirHeatRecoveryTypeEnum::IfcAirToAirHeatRecoveryTypeEnum() {}
IfcAirToAirHeatRecoveryTypeEnum::~IfcAirToAirHeatRecoveryTypeEnum() {}
void IfcAirToAirHeatRecoveryTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCAIRTOAIRHEATRECOVERYTYPEENUM("; }
	if( m_enum == ENUM_FIXEDPLATECOUNTERFLOWEXCHANGER )
	{
		stream << ".FIXEDPLATECOUNTERFLOWEXCHANGER.";
	}
	else if( m_enum == ENUM_FIXEDPLATECROSSFLOWEXCHANGER )
	{
		stream << ".FIXEDPLATECROSSFLOWEXCHANGER.";
	}
	else if( m_enum == ENUM_FIXEDPLATEPARALLELFLOWEXCHANGER )
	{
		stream << ".FIXEDPLATEPARALLELFLOWEXCHANGER.";
	}
	else if( m_enum == ENUM_ROTARYWHEEL )
	{
		stream << ".ROTARYWHEEL.";
	}
	else if( m_enum == ENUM_RUNAROUNDCOILLOOP )
	{
		stream << ".RUNAROUNDCOILLOOP.";
	}
	else if( m_enum == ENUM_HEATPIPE )
	{
		stream << ".HEATPIPE.";
	}
	else if( m_enum == ENUM_TWINTOWERENTHALPYRECOVERYLOOPS )
	{
		stream << ".TWINTOWERENTHALPYRECOVERYLOOPS.";
	}
	else if( m_enum == ENUM_THERMOSIPHONSEALEDTUBEHEATEXCHANGERS )
	{
		stream << ".THERMOSIPHONSEALEDTUBEHEATEXCHANGERS.";
	}
	else if( m_enum == ENUM_THERMOSIPHONCOILTYPEHEATEXCHANGERS )
	{
		stream << ".THERMOSIPHONCOILTYPEHEATEXCHANGERS.";
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
shared_ptr<IfcAirToAirHeatRecoveryTypeEnum> IfcAirToAirHeatRecoveryTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcAirToAirHeatRecoveryTypeEnum>(); }
	shared_ptr<IfcAirToAirHeatRecoveryTypeEnum> type_object( new IfcAirToAirHeatRecoveryTypeEnum() );
	if( _stricmp( arg.c_str(), ".FIXEDPLATECOUNTERFLOWEXCHANGER." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_FIXEDPLATECOUNTERFLOWEXCHANGER;
	}
	else if( _stricmp( arg.c_str(), ".FIXEDPLATECROSSFLOWEXCHANGER." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_FIXEDPLATECROSSFLOWEXCHANGER;
	}
	else if( _stricmp( arg.c_str(), ".FIXEDPLATEPARALLELFLOWEXCHANGER." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_FIXEDPLATEPARALLELFLOWEXCHANGER;
	}
	else if( _stricmp( arg.c_str(), ".ROTARYWHEEL." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_ROTARYWHEEL;
	}
	else if( _stricmp( arg.c_str(), ".RUNAROUNDCOILLOOP." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_RUNAROUNDCOILLOOP;
	}
	else if( _stricmp( arg.c_str(), ".HEATPIPE." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_HEATPIPE;
	}
	else if( _stricmp( arg.c_str(), ".TWINTOWERENTHALPYRECOVERYLOOPS." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_TWINTOWERENTHALPYRECOVERYLOOPS;
	}
	else if( _stricmp( arg.c_str(), ".THERMOSIPHONSEALEDTUBEHEATEXCHANGERS." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_THERMOSIPHONSEALEDTUBEHEATEXCHANGERS;
	}
	else if( _stricmp( arg.c_str(), ".THERMOSIPHONCOILTYPEHEATEXCHANGERS." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_THERMOSIPHONCOILTYPEHEATEXCHANGERS;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
