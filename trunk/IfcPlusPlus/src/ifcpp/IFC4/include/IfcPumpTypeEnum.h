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

// TYPE IfcPumpTypeEnum = ENUMERATION OF	(CIRCULATOR	,ENDSUCTION	,SPLITCASE	,SUBMERSIBLEPUMP	,SUMPPUMP	,VERTICALINLINE	,VERTICALTURBINE	,USERDEFINED	,NOTDEFINED);
class IfcPumpTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcPumpTypeEnumEnum
	{
		ENUM_CIRCULATOR,
		ENUM_ENDSUCTION,
		ENUM_SPLITCASE,
		ENUM_SUBMERSIBLEPUMP,
		ENUM_SUMPPUMP,
		ENUM_VERTICALINLINE,
		ENUM_VERTICALTURBINE,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcPumpTypeEnum();
	IfcPumpTypeEnum( IfcPumpTypeEnumEnum e ) { m_enum = e; }
	~IfcPumpTypeEnum();
	virtual const char* classname() const { return "IfcPumpTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcPumpTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcPumpTypeEnumEnum m_enum;
};

