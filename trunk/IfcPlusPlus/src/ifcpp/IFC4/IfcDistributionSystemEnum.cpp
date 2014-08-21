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
#include "include/IfcDistributionSystemEnum.h"

// TYPE IfcDistributionSystemEnum = ENUMERATION OF	(AIRCONDITIONING	,AUDIOVISUAL	,CHEMICAL	,CHILLEDWATER	,COMMUNICATION	,COMPRESSEDAIR	,CONDENSERWATER	,CONTROL	,CONVEYING	,DATA	,DISPOSAL	,DOMESTICCOLDWATER	,DOMESTICHOTWATER	,DRAINAGE	,EARTHING	,ELECTRICAL	,ELECTROACOUSTIC	,EXHAUST	,FIREPROTECTION	,FUEL	,GAS	,HAZARDOUS	,HEATING	,LIGHTING	,LIGHTNINGPROTECTION	,MUNICIPALSOLIDWASTE	,OIL	,OPERATIONAL	,POWERGENERATION	,RAINWATER	,REFRIGERATION	,SECURITY	,SEWAGE	,SIGNAL	,STORMWATER	,TELEPHONE	,TV	,VACUUM	,VENT	,VENTILATION	,WASTEWATER	,WATERSUPPLY	,USERDEFINED	,NOTDEFINED);
IfcDistributionSystemEnum::IfcDistributionSystemEnum() {}
IfcDistributionSystemEnum::~IfcDistributionSystemEnum() {}
shared_ptr<IfcPPObject> IfcDistributionSystemEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDistributionSystemEnum> copy_self( new IfcDistributionSystemEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDistributionSystemEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDISTRIBUTIONSYSTEMENUM("; }
	if( m_enum == ENUM_AIRCONDITIONING )
	{
		stream << ".AIRCONDITIONING.";
	}
	else if( m_enum == ENUM_AUDIOVISUAL )
	{
		stream << ".AUDIOVISUAL.";
	}
	else if( m_enum == ENUM_CHEMICAL )
	{
		stream << ".CHEMICAL.";
	}
	else if( m_enum == ENUM_CHILLEDWATER )
	{
		stream << ".CHILLEDWATER.";
	}
	else if( m_enum == ENUM_COMMUNICATION )
	{
		stream << ".COMMUNICATION.";
	}
	else if( m_enum == ENUM_COMPRESSEDAIR )
	{
		stream << ".COMPRESSEDAIR.";
	}
	else if( m_enum == ENUM_CONDENSERWATER )
	{
		stream << ".CONDENSERWATER.";
	}
	else if( m_enum == ENUM_CONTROL )
	{
		stream << ".CONTROL.";
	}
	else if( m_enum == ENUM_CONVEYING )
	{
		stream << ".CONVEYING.";
	}
	else if( m_enum == ENUM_DATA )
	{
		stream << ".DATA.";
	}
	else if( m_enum == ENUM_DISPOSAL )
	{
		stream << ".DISPOSAL.";
	}
	else if( m_enum == ENUM_DOMESTICCOLDWATER )
	{
		stream << ".DOMESTICCOLDWATER.";
	}
	else if( m_enum == ENUM_DOMESTICHOTWATER )
	{
		stream << ".DOMESTICHOTWATER.";
	}
	else if( m_enum == ENUM_DRAINAGE )
	{
		stream << ".DRAINAGE.";
	}
	else if( m_enum == ENUM_EARTHING )
	{
		stream << ".EARTHING.";
	}
	else if( m_enum == ENUM_ELECTRICAL )
	{
		stream << ".ELECTRICAL.";
	}
	else if( m_enum == ENUM_ELECTROACOUSTIC )
	{
		stream << ".ELECTROACOUSTIC.";
	}
	else if( m_enum == ENUM_EXHAUST )
	{
		stream << ".EXHAUST.";
	}
	else if( m_enum == ENUM_FIREPROTECTION )
	{
		stream << ".FIREPROTECTION.";
	}
	else if( m_enum == ENUM_FUEL )
	{
		stream << ".FUEL.";
	}
	else if( m_enum == ENUM_GAS )
	{
		stream << ".GAS.";
	}
	else if( m_enum == ENUM_HAZARDOUS )
	{
		stream << ".HAZARDOUS.";
	}
	else if( m_enum == ENUM_HEATING )
	{
		stream << ".HEATING.";
	}
	else if( m_enum == ENUM_LIGHTING )
	{
		stream << ".LIGHTING.";
	}
	else if( m_enum == ENUM_LIGHTNINGPROTECTION )
	{
		stream << ".LIGHTNINGPROTECTION.";
	}
	else if( m_enum == ENUM_MUNICIPALSOLIDWASTE )
	{
		stream << ".MUNICIPALSOLIDWASTE.";
	}
	else if( m_enum == ENUM_OIL )
	{
		stream << ".OIL.";
	}
	else if( m_enum == ENUM_OPERATIONAL )
	{
		stream << ".OPERATIONAL.";
	}
	else if( m_enum == ENUM_POWERGENERATION )
	{
		stream << ".POWERGENERATION.";
	}
	else if( m_enum == ENUM_RAINWATER )
	{
		stream << ".RAINWATER.";
	}
	else if( m_enum == ENUM_REFRIGERATION )
	{
		stream << ".REFRIGERATION.";
	}
	else if( m_enum == ENUM_SECURITY )
	{
		stream << ".SECURITY.";
	}
	else if( m_enum == ENUM_SEWAGE )
	{
		stream << ".SEWAGE.";
	}
	else if( m_enum == ENUM_SIGNAL )
	{
		stream << ".SIGNAL.";
	}
	else if( m_enum == ENUM_STORMWATER )
	{
		stream << ".STORMWATER.";
	}
	else if( m_enum == ENUM_TELEPHONE )
	{
		stream << ".TELEPHONE.";
	}
	else if( m_enum == ENUM_TV )
	{
		stream << ".TV.";
	}
	else if( m_enum == ENUM_VACUUM )
	{
		stream << ".VACUUM.";
	}
	else if( m_enum == ENUM_VENT )
	{
		stream << ".VENT.";
	}
	else if( m_enum == ENUM_VENTILATION )
	{
		stream << ".VENTILATION.";
	}
	else if( m_enum == ENUM_WASTEWATER )
	{
		stream << ".WASTEWATER.";
	}
	else if( m_enum == ENUM_WATERSUPPLY )
	{
		stream << ".WATERSUPPLY.";
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
shared_ptr<IfcDistributionSystemEnum> IfcDistributionSystemEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDistributionSystemEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDistributionSystemEnum>(); }
	shared_ptr<IfcDistributionSystemEnum> type_object( new IfcDistributionSystemEnum() );
	if( boost::iequals( arg, L".AIRCONDITIONING." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_AIRCONDITIONING;
	}
	else if( boost::iequals( arg, L".AUDIOVISUAL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_AUDIOVISUAL;
	}
	else if( boost::iequals( arg, L".CHEMICAL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CHEMICAL;
	}
	else if( boost::iequals( arg, L".CHILLEDWATER." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CHILLEDWATER;
	}
	else if( boost::iequals( arg, L".COMMUNICATION." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_COMMUNICATION;
	}
	else if( boost::iequals( arg, L".COMPRESSEDAIR." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_COMPRESSEDAIR;
	}
	else if( boost::iequals( arg, L".CONDENSERWATER." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CONDENSERWATER;
	}
	else if( boost::iequals( arg, L".CONTROL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CONTROL;
	}
	else if( boost::iequals( arg, L".CONVEYING." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CONVEYING;
	}
	else if( boost::iequals( arg, L".DATA." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DATA;
	}
	else if( boost::iequals( arg, L".DISPOSAL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DISPOSAL;
	}
	else if( boost::iequals( arg, L".DOMESTICCOLDWATER." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DOMESTICCOLDWATER;
	}
	else if( boost::iequals( arg, L".DOMESTICHOTWATER." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DOMESTICHOTWATER;
	}
	else if( boost::iequals( arg, L".DRAINAGE." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DRAINAGE;
	}
	else if( boost::iequals( arg, L".EARTHING." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_EARTHING;
	}
	else if( boost::iequals( arg, L".ELECTRICAL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_ELECTRICAL;
	}
	else if( boost::iequals( arg, L".ELECTROACOUSTIC." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_ELECTROACOUSTIC;
	}
	else if( boost::iequals( arg, L".EXHAUST." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_EXHAUST;
	}
	else if( boost::iequals( arg, L".FIREPROTECTION." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_FIREPROTECTION;
	}
	else if( boost::iequals( arg, L".FUEL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_FUEL;
	}
	else if( boost::iequals( arg, L".GAS." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_GAS;
	}
	else if( boost::iequals( arg, L".HAZARDOUS." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_HAZARDOUS;
	}
	else if( boost::iequals( arg, L".HEATING." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_HEATING;
	}
	else if( boost::iequals( arg, L".LIGHTING." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_LIGHTING;
	}
	else if( boost::iequals( arg, L".LIGHTNINGPROTECTION." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_LIGHTNINGPROTECTION;
	}
	else if( boost::iequals( arg, L".MUNICIPALSOLIDWASTE." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_MUNICIPALSOLIDWASTE;
	}
	else if( boost::iequals( arg, L".OIL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_OIL;
	}
	else if( boost::iequals( arg, L".OPERATIONAL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_OPERATIONAL;
	}
	else if( boost::iequals( arg, L".POWERGENERATION." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_POWERGENERATION;
	}
	else if( boost::iequals( arg, L".RAINWATER." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_RAINWATER;
	}
	else if( boost::iequals( arg, L".REFRIGERATION." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_REFRIGERATION;
	}
	else if( boost::iequals( arg, L".SECURITY." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_SECURITY;
	}
	else if( boost::iequals( arg, L".SEWAGE." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_SEWAGE;
	}
	else if( boost::iequals( arg, L".SIGNAL." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_SIGNAL;
	}
	else if( boost::iequals( arg, L".STORMWATER." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_STORMWATER;
	}
	else if( boost::iequals( arg, L".TELEPHONE." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_TELEPHONE;
	}
	else if( boost::iequals( arg, L".TV." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_TV;
	}
	else if( boost::iequals( arg, L".VACUUM." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_VACUUM;
	}
	else if( boost::iequals( arg, L".VENT." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_VENT;
	}
	else if( boost::iequals( arg, L".VENTILATION." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_VENTILATION;
	}
	else if( boost::iequals( arg, L".WASTEWATER." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_WASTEWATER;
	}
	else if( boost::iequals( arg, L".WATERSUPPLY." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_WATERSUPPLY;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
