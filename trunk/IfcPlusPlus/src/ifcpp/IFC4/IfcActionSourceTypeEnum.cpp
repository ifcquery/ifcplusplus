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

// TYPE IfcActionSourceTypeEnum = ENUMERATION OF	(DEAD_LOAD_G	,COMPLETION_G1	,LIVE_LOAD_Q	,SNOW_S	,WIND_W	,PRESTRESSING_P	,SETTLEMENT_U	,TEMPERATURE_T	,EARTHQUAKE_E	,FIRE	,IMPULSE	,IMPACT	,TRANSPORT	,ERECTION	,PROPPING	,SYSTEM_IMPERFECTION	,SHRINKAGE	,CREEP	,LACK_OF_FIT	,BUOYANCY	,ICE	,CURRENT	,WAVE	,RAIN	,BRAKES	,USERDEFINED	,NOTDEFINED);
IfcActionSourceTypeEnum::IfcActionSourceTypeEnum() {}
IfcActionSourceTypeEnum::~IfcActionSourceTypeEnum() {}
shared_ptr<IfcPPObject> IfcActionSourceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcActionSourceTypeEnum> copy_self( new IfcActionSourceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcActionSourceTypeEnum> IfcActionSourceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcActionSourceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcActionSourceTypeEnum>(); }
	shared_ptr<IfcActionSourceTypeEnum> type_object( new IfcActionSourceTypeEnum() );
	if( boost::iequals( arg, L".DEAD_LOAD_G." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_DEAD_LOAD_G;
	}
	else if( boost::iequals( arg, L".COMPLETION_G1." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_COMPLETION_G1;
	}
	else if( boost::iequals( arg, L".LIVE_LOAD_Q." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_LIVE_LOAD_Q;
	}
	else if( boost::iequals( arg, L".SNOW_S." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_SNOW_S;
	}
	else if( boost::iequals( arg, L".WIND_W." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_WIND_W;
	}
	else if( boost::iequals( arg, L".PRESTRESSING_P." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_PRESTRESSING_P;
	}
	else if( boost::iequals( arg, L".SETTLEMENT_U." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_SETTLEMENT_U;
	}
	else if( boost::iequals( arg, L".TEMPERATURE_T." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_TEMPERATURE_T;
	}
	else if( boost::iequals( arg, L".EARTHQUAKE_E." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_EARTHQUAKE_E;
	}
	else if( boost::iequals( arg, L".FIRE." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_FIRE;
	}
	else if( boost::iequals( arg, L".IMPULSE." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_IMPULSE;
	}
	else if( boost::iequals( arg, L".IMPACT." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_IMPACT;
	}
	else if( boost::iequals( arg, L".TRANSPORT." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_TRANSPORT;
	}
	else if( boost::iequals( arg, L".ERECTION." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_ERECTION;
	}
	else if( boost::iequals( arg, L".PROPPING." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_PROPPING;
	}
	else if( boost::iequals( arg, L".SYSTEM_IMPERFECTION." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_SYSTEM_IMPERFECTION;
	}
	else if( boost::iequals( arg, L".SHRINKAGE." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_SHRINKAGE;
	}
	else if( boost::iequals( arg, L".CREEP." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_CREEP;
	}
	else if( boost::iequals( arg, L".LACK_OF_FIT." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_LACK_OF_FIT;
	}
	else if( boost::iequals( arg, L".BUOYANCY." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_BUOYANCY;
	}
	else if( boost::iequals( arg, L".ICE." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_ICE;
	}
	else if( boost::iequals( arg, L".CURRENT." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_CURRENT;
	}
	else if( boost::iequals( arg, L".WAVE." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_WAVE;
	}
	else if( boost::iequals( arg, L".RAIN." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_RAIN;
	}
	else if( boost::iequals( arg, L".BRAKES." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_BRAKES;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcActionSourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
