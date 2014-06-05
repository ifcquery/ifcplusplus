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

// TYPE IfcDistributionPortTypeEnum = ENUMERATION OF	(CABLE	,CABLECARRIER	,DUCT	,PIPE	,USERDEFINED	,NOTDEFINED);
class IfcDistributionPortTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcDistributionPortTypeEnumEnum
	{
		ENUM_CABLE,
		ENUM_CABLECARRIER,
		ENUM_DUCT,
		ENUM_PIPE,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDistributionPortTypeEnum();
	IfcDistributionPortTypeEnum( IfcDistributionPortTypeEnumEnum e ) { m_enum = e; }
	~IfcDistributionPortTypeEnum();
	virtual const char* classname() const { return "IfcDistributionPortTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcDistributionPortTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcDistributionPortTypeEnumEnum m_enum;
};

