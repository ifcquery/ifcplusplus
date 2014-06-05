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

// TYPE IfcSIPrefix = ENUMERATION OF	(EXA	,PETA	,TERA	,GIGA	,MEGA	,KILO	,HECTO	,DECA	,DECI	,CENTI	,MILLI	,MICRO	,NANO	,PICO	,FEMTO	,ATTO);
class IfcSIPrefix : virtual public IfcPPObject
{
public:
	enum IfcSIPrefixEnum
	{
		ENUM_EXA,
		ENUM_PETA,
		ENUM_TERA,
		ENUM_GIGA,
		ENUM_MEGA,
		ENUM_KILO,
		ENUM_HECTO,
		ENUM_DECA,
		ENUM_DECI,
		ENUM_CENTI,
		ENUM_MILLI,
		ENUM_MICRO,
		ENUM_NANO,
		ENUM_PICO,
		ENUM_FEMTO,
		ENUM_ATTO
	};

	IfcSIPrefix();
	IfcSIPrefix( IfcSIPrefixEnum e ) { m_enum = e; }
	~IfcSIPrefix();
	virtual const char* classname() const { return "IfcSIPrefix"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcSIPrefix> createObjectFromStepData( const std::string& arg );
	IfcSIPrefixEnum m_enum;
};

