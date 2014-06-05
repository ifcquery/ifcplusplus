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
#include "include/IfcActionSourceTypeEnum.h"

// TYPE IfcActionSourceTypeEnum 
IfcActionSourceTypeEnum::IfcActionSourceTypeEnum() {}
IfcActionSourceTypeEnum::~IfcActionSourceTypeEnum() {}
void IfcActionSourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCACTIONSOURCETYPEENUM("; }
	if( m_enum == ENUM_DEAD_LOAD_G )
	{
		stream << ".DEAD_LOAD_G.";
	}
	else if( m_enum == ENUM_COMPLETION_G1 )
	{
		stream << ".COMPLETION_G1.";
	}
	else if( m_enum == ENUM_LIVE_LOAD_Q )
	{
		stream << ".LIVE_LOAD_Q.";
	}
	else if( m_enum == ENUM_SNOW_S )
	{
		stream << ".SNOW_S.";
	}
	else if( m_enum == ENUM_WIND_W )
	{
		stream << ".WIND_W.";
	}
	else if( m_enum == ENUM_PRESTRESSING_P )
	{
		stream << ".PRESTRESSING_P.";
	}
	else if( m_enum == ENUM_SETTLEMENT_U )
	{
		stream << ".SETTLEMENT_U.";
	}
	else if( m_enum == ENUM_TEMPERATURE_T )
	{
		stream << ".TEMPERATURE_T.";
	}
	else if( m_enum == ENUM_EARTHQUAKE_E )
	{
		stream << ".EARTHQUAKE_E.";
	}
	else if( m_enum == ENUM_FIRE )
	{
		stream << ".FIRE.";
	}
	else if( m_enum == ENUM_IMPULSE )
	{
		stream << ".IMPULSE.";
	}
	else if( m_enum == ENUM_IMPACT )
	{
		stream << ".IMPACT.";
	}
	else if( m_enum == ENUM_TRANSPORT )
	{
		stream << ".TRANSPORT.";
	}
	else if( m_enum == ENUM_ERECTION )
	{
		stream << ".ERECTION.";
	}
	else if( m_enum == ENUM_PROPPING )
	{
		stream << ".PROPPING.";
	}
	else if( m_enum == ENUM_SYSTEM_IMPERFECTION )
	{
		stream << ".SYSTEM_IMPERFECTION.";
	}
	else if( m_enum == ENUM_SHRINKAGE )
	{
		stream << ".SHRINKAGE.";
	}
	else if( m_enum == ENUM_CREEP )
	{
		stream << ".CREEP.";
	}
	else if( m_enum == ENUM_LACK_OF_FIT )
	{
		stream << ".LACK_OF_FIT.";
	}
	else if( m_enum == ENUM_BUOYANCY )
	{
		stream << ".BUOYANCY.";
	}
	else if( m_enum == ENUM_ICE )
	{
		stream << ".ICE.";
	}
	else if( m_enum == ENUM_CURRENT )
	{
		stream << ".CURRENT.";
	}
	else if( m_enum == ENUM_WAVE )
	{
		stream << ".WAVE.";
	}
	else if( m_enum == ENUM_RAIN )
	{
		stream << ".RAIN.";
	}
	else if( m_enum == ENUM_BRAKES )
	{
		stream << ".BRAKES.";
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
shared_ptr<IfcActionSourceTypeEnum> IfcActionSourceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcActionSourceTypeEnum>(); }
	shared_ptr<IfcActionSourceTypeEnum> type_object( new IfcActionSourceTypeEnum() );
	if( _stricmp( arg.c_str(), ".DEAD_LOAD_G." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_DEAD_LOAD_G;
	}
	else if( _stricmp( arg.c_str(), ".COMPLETION_G1." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_COMPLETION_G1;
	}
	else if( _stricmp( arg.c_str(), ".LIVE_LOAD_Q." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_LIVE_LOAD_Q;
	}
	else if( _stricmp( arg.c_str(), ".SNOW_S." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_SNOW_S;
	}
	else if( _stricmp( arg.c_str(), ".WIND_W." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_WIND_W;
	}
	else if( _stricmp( arg.c_str(), ".PRESTRESSING_P." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_PRESTRESSING_P;
	}
	else if( _stricmp( arg.c_str(), ".SETTLEMENT_U." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_SETTLEMENT_U;
	}
	else if( _stricmp( arg.c_str(), ".TEMPERATURE_T." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_TEMPERATURE_T;
	}
	else if( _stricmp( arg.c_str(), ".EARTHQUAKE_E." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_EARTHQUAKE_E;
	}
	else if( _stricmp( arg.c_str(), ".FIRE." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_FIRE;
	}
	else if( _stricmp( arg.c_str(), ".IMPULSE." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_IMPULSE;
	}
	else if( _stricmp( arg.c_str(), ".IMPACT." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_IMPACT;
	}
	else if( _stricmp( arg.c_str(), ".TRANSPORT." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_TRANSPORT;
	}
	else if( _stricmp( arg.c_str(), ".ERECTION." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_ERECTION;
	}
	else if( _stricmp( arg.c_str(), ".PROPPING." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_PROPPING;
	}
	else if( _stricmp( arg.c_str(), ".SYSTEM_IMPERFECTION." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_SYSTEM_IMPERFECTION;
	}
	else if( _stricmp( arg.c_str(), ".SHRINKAGE." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_SHRINKAGE;
	}
	else if( _stricmp( arg.c_str(), ".CREEP." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_CREEP;
	}
	else if( _stricmp( arg.c_str(), ".LACK_OF_FIT." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_LACK_OF_FIT;
	}
	else if( _stricmp( arg.c_str(), ".BUOYANCY." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_BUOYANCY;
	}
	else if( _stricmp( arg.c_str(), ".ICE." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_ICE;
	}
	else if( _stricmp( arg.c_str(), ".CURRENT." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_CURRENT;
	}
	else if( _stricmp( arg.c_str(), ".WAVE." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_WAVE;
	}
	else if( _stricmp( arg.c_str(), ".RAIN." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_RAIN;
	}
	else if( _stricmp( arg.c_str(), ".BRAKES." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_BRAKES;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
