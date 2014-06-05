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

// TYPE IfcDistributionSystemEnum 
IfcDistributionSystemEnum::IfcDistributionSystemEnum() {}
IfcDistributionSystemEnum::~IfcDistributionSystemEnum() {}
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
shared_ptr<IfcDistributionSystemEnum> IfcDistributionSystemEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcDistributionSystemEnum>(); }
	shared_ptr<IfcDistributionSystemEnum> type_object( new IfcDistributionSystemEnum() );
	if( _stricmp( arg.c_str(), ".AIRCONDITIONING." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_AIRCONDITIONING;
	}
	else if( _stricmp( arg.c_str(), ".AUDIOVISUAL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_AUDIOVISUAL;
	}
	else if( _stricmp( arg.c_str(), ".CHEMICAL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CHEMICAL;
	}
	else if( _stricmp( arg.c_str(), ".CHILLEDWATER." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CHILLEDWATER;
	}
	else if( _stricmp( arg.c_str(), ".COMMUNICATION." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_COMMUNICATION;
	}
	else if( _stricmp( arg.c_str(), ".COMPRESSEDAIR." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_COMPRESSEDAIR;
	}
	else if( _stricmp( arg.c_str(), ".CONDENSERWATER." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CONDENSERWATER;
	}
	else if( _stricmp( arg.c_str(), ".CONTROL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CONTROL;
	}
	else if( _stricmp( arg.c_str(), ".CONVEYING." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_CONVEYING;
	}
	else if( _stricmp( arg.c_str(), ".DATA." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DATA;
	}
	else if( _stricmp( arg.c_str(), ".DISPOSAL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DISPOSAL;
	}
	else if( _stricmp( arg.c_str(), ".DOMESTICCOLDWATER." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DOMESTICCOLDWATER;
	}
	else if( _stricmp( arg.c_str(), ".DOMESTICHOTWATER." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DOMESTICHOTWATER;
	}
	else if( _stricmp( arg.c_str(), ".DRAINAGE." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_DRAINAGE;
	}
	else if( _stricmp( arg.c_str(), ".EARTHING." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_EARTHING;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRICAL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_ELECTRICAL;
	}
	else if( _stricmp( arg.c_str(), ".ELECTROACOUSTIC." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_ELECTROACOUSTIC;
	}
	else if( _stricmp( arg.c_str(), ".EXHAUST." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_EXHAUST;
	}
	else if( _stricmp( arg.c_str(), ".FIREPROTECTION." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_FIREPROTECTION;
	}
	else if( _stricmp( arg.c_str(), ".FUEL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_FUEL;
	}
	else if( _stricmp( arg.c_str(), ".GAS." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_GAS;
	}
	else if( _stricmp( arg.c_str(), ".HAZARDOUS." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_HAZARDOUS;
	}
	else if( _stricmp( arg.c_str(), ".HEATING." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_HEATING;
	}
	else if( _stricmp( arg.c_str(), ".LIGHTING." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_LIGHTING;
	}
	else if( _stricmp( arg.c_str(), ".LIGHTNINGPROTECTION." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_LIGHTNINGPROTECTION;
	}
	else if( _stricmp( arg.c_str(), ".MUNICIPALSOLIDWASTE." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_MUNICIPALSOLIDWASTE;
	}
	else if( _stricmp( arg.c_str(), ".OIL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_OIL;
	}
	else if( _stricmp( arg.c_str(), ".OPERATIONAL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_OPERATIONAL;
	}
	else if( _stricmp( arg.c_str(), ".POWERGENERATION." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_POWERGENERATION;
	}
	else if( _stricmp( arg.c_str(), ".RAINWATER." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_RAINWATER;
	}
	else if( _stricmp( arg.c_str(), ".REFRIGERATION." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_REFRIGERATION;
	}
	else if( _stricmp( arg.c_str(), ".SECURITY." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_SECURITY;
	}
	else if( _stricmp( arg.c_str(), ".SEWAGE." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_SEWAGE;
	}
	else if( _stricmp( arg.c_str(), ".SIGNAL." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_SIGNAL;
	}
	else if( _stricmp( arg.c_str(), ".STORMWATER." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_STORMWATER;
	}
	else if( _stricmp( arg.c_str(), ".TELEPHONE." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_TELEPHONE;
	}
	else if( _stricmp( arg.c_str(), ".TV." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_TV;
	}
	else if( _stricmp( arg.c_str(), ".VACUUM." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_VACUUM;
	}
	else if( _stricmp( arg.c_str(), ".VENT." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_VENT;
	}
	else if( _stricmp( arg.c_str(), ".VENTILATION." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_VENTILATION;
	}
	else if( _stricmp( arg.c_str(), ".WASTEWATER." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_WASTEWATER;
	}
	else if( _stricmp( arg.c_str(), ".WATERSUPPLY." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_WATERSUPPLY;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcDistributionSystemEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
