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

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"

// TYPE IfcDistributionSystemEnum = ENUMERATION OF	(AIRCONDITIONING	,AUDIOVISUAL	,CHEMICAL	,CHILLEDWATER	,COMMUNICATION	,COMPRESSEDAIR	,CONDENSERWATER	,CONTROL	,CONVEYING	,DATA	,DISPOSAL	,DOMESTICCOLDWATER	,DOMESTICHOTWATER	,DRAINAGE	,EARTHING	,ELECTRICAL	,ELECTROACOUSTIC	,EXHAUST	,FIREPROTECTION	,FUEL	,GAS	,HAZARDOUS	,HEATING	,LIGHTING	,LIGHTNINGPROTECTION	,MUNICIPALSOLIDWASTE	,OIL	,OPERATIONAL	,POWERGENERATION	,RAINWATER	,REFRIGERATION	,SECURITY	,SEWAGE	,SIGNAL	,STORMWATER	,TELEPHONE	,TV	,VACUUM	,VENT	,VENTILATION	,WASTEWATER	,WATERSUPPLY	,USERDEFINED	,NOTDEFINED);
class IfcDistributionSystemEnum : virtual public IfcPPObject
{
public:
	enum IfcDistributionSystemEnumEnum
	{
		ENUM_AIRCONDITIONING,
		ENUM_AUDIOVISUAL,
		ENUM_CHEMICAL,
		ENUM_CHILLEDWATER,
		ENUM_COMMUNICATION,
		ENUM_COMPRESSEDAIR,
		ENUM_CONDENSERWATER,
		ENUM_CONTROL,
		ENUM_CONVEYING,
		ENUM_DATA,
		ENUM_DISPOSAL,
		ENUM_DOMESTICCOLDWATER,
		ENUM_DOMESTICHOTWATER,
		ENUM_DRAINAGE,
		ENUM_EARTHING,
		ENUM_ELECTRICAL,
		ENUM_ELECTROACOUSTIC,
		ENUM_EXHAUST,
		ENUM_FIREPROTECTION,
		ENUM_FUEL,
		ENUM_GAS,
		ENUM_HAZARDOUS,
		ENUM_HEATING,
		ENUM_LIGHTING,
		ENUM_LIGHTNINGPROTECTION,
		ENUM_MUNICIPALSOLIDWASTE,
		ENUM_OIL,
		ENUM_OPERATIONAL,
		ENUM_POWERGENERATION,
		ENUM_RAINWATER,
		ENUM_REFRIGERATION,
		ENUM_SECURITY,
		ENUM_SEWAGE,
		ENUM_SIGNAL,
		ENUM_STORMWATER,
		ENUM_TELEPHONE,
		ENUM_TV,
		ENUM_VACUUM,
		ENUM_VENT,
		ENUM_VENTILATION,
		ENUM_WASTEWATER,
		ENUM_WATERSUPPLY,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDistributionSystemEnum();
	IfcDistributionSystemEnum( IfcDistributionSystemEnumEnum e ) { m_enum = e; }
	~IfcDistributionSystemEnum();
	virtual const char* classname() const { return "IfcDistributionSystemEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcDistributionSystemEnum> createObjectFromStepData( const std::string& arg );
	IfcDistributionSystemEnumEnum m_enum;
};

