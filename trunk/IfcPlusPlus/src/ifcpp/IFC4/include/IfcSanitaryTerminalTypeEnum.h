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

// TYPE IfcSanitaryTerminalTypeEnum = ENUMERATION OF	(BATH	,BIDET	,CISTERN	,SHOWER	,SINK	,SANITARYFOUNTAIN	,TOILETPAN	,URINAL	,WASHHANDBASIN	,WCSEAT	,USERDEFINED	,NOTDEFINED);
class IfcSanitaryTerminalTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcSanitaryTerminalTypeEnumEnum
	{
		ENUM_BATH,
		ENUM_BIDET,
		ENUM_CISTERN,
		ENUM_SHOWER,
		ENUM_SINK,
		ENUM_SANITARYFOUNTAIN,
		ENUM_TOILETPAN,
		ENUM_URINAL,
		ENUM_WASHHANDBASIN,
		ENUM_WCSEAT,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcSanitaryTerminalTypeEnum();
	IfcSanitaryTerminalTypeEnum( IfcSanitaryTerminalTypeEnumEnum e ) { m_enum = e; }
	~IfcSanitaryTerminalTypeEnum();
	virtual const char* classname() const { return "IfcSanitaryTerminalTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcSanitaryTerminalTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcSanitaryTerminalTypeEnumEnum m_enum;
};

