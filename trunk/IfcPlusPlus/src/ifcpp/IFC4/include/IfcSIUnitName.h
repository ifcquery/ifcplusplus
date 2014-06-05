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

// TYPE IfcSIUnitName = ENUMERATION OF	(AMPERE	,BECQUEREL	,CANDELA	,COULOMB	,CUBIC_METRE	,DEGREE_CELSIUS	,FARAD	,GRAM	,GRAY	,HENRY	,HERTZ	,JOULE	,KELVIN	,LUMEN	,LUX	,METRE	,MOLE	,NEWTON	,OHM	,PASCAL	,RADIAN	,SECOND	,SIEMENS	,SIEVERT	,SQUARE_METRE	,STERADIAN	,TESLA	,VOLT	,WATT	,WEBER);
class IfcSIUnitName : virtual public IfcPPObject
{
public:
	enum IfcSIUnitNameEnum
	{
		ENUM_AMPERE,
		ENUM_BECQUEREL,
		ENUM_CANDELA,
		ENUM_COULOMB,
		ENUM_CUBIC_METRE,
		ENUM_DEGREE_CELSIUS,
		ENUM_FARAD,
		ENUM_GRAM,
		ENUM_GRAY,
		ENUM_HENRY,
		ENUM_HERTZ,
		ENUM_JOULE,
		ENUM_KELVIN,
		ENUM_LUMEN,
		ENUM_LUX,
		ENUM_METRE,
		ENUM_MOLE,
		ENUM_NEWTON,
		ENUM_OHM,
		ENUM_PASCAL,
		ENUM_RADIAN,
		ENUM_SECOND,
		ENUM_SIEMENS,
		ENUM_SIEVERT,
		ENUM_SQUARE_METRE,
		ENUM_STERADIAN,
		ENUM_TESLA,
		ENUM_VOLT,
		ENUM_WATT,
		ENUM_WEBER
	};

	IfcSIUnitName();
	IfcSIUnitName( IfcSIUnitNameEnum e ) { m_enum = e; }
	~IfcSIUnitName();
	virtual const char* classname() const { return "IfcSIUnitName"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcSIUnitName> createObjectFromStepData( const std::string& arg );
	IfcSIUnitNameEnum m_enum;
};

