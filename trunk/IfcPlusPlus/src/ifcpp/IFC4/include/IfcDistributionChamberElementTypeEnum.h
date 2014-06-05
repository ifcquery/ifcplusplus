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

// TYPE IfcDistributionChamberElementTypeEnum = ENUMERATION OF	(FORMEDDUCT	,INSPECTIONCHAMBER	,INSPECTIONPIT	,MANHOLE	,METERCHAMBER	,SUMP	,TRENCH	,VALVECHAMBER	,USERDEFINED	,NOTDEFINED);
class IfcDistributionChamberElementTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcDistributionChamberElementTypeEnumEnum
	{
		ENUM_FORMEDDUCT,
		ENUM_INSPECTIONCHAMBER,
		ENUM_INSPECTIONPIT,
		ENUM_MANHOLE,
		ENUM_METERCHAMBER,
		ENUM_SUMP,
		ENUM_TRENCH,
		ENUM_VALVECHAMBER,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDistributionChamberElementTypeEnum();
	IfcDistributionChamberElementTypeEnum( IfcDistributionChamberElementTypeEnumEnum e ) { m_enum = e; }
	~IfcDistributionChamberElementTypeEnum();
	virtual const char* classname() const { return "IfcDistributionChamberElementTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcDistributionChamberElementTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcDistributionChamberElementTypeEnumEnum m_enum;
};

