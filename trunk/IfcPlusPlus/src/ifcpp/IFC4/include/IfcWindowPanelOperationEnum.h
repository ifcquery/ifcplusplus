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

// TYPE IfcWindowPanelOperationEnum = ENUMERATION OF	(SIDEHUNGRIGHTHAND	,SIDEHUNGLEFTHAND	,TILTANDTURNRIGHTHAND	,TILTANDTURNLEFTHAND	,TOPHUNG	,BOTTOMHUNG	,PIVOTHORIZONTAL	,PIVOTVERTICAL	,SLIDINGHORIZONTAL	,SLIDINGVERTICAL	,REMOVABLECASEMENT	,FIXEDCASEMENT	,OTHEROPERATION	,NOTDEFINED);
class IfcWindowPanelOperationEnum : virtual public IfcPPObject
{
public:
	enum IfcWindowPanelOperationEnumEnum
	{
		ENUM_SIDEHUNGRIGHTHAND,
		ENUM_SIDEHUNGLEFTHAND,
		ENUM_TILTANDTURNRIGHTHAND,
		ENUM_TILTANDTURNLEFTHAND,
		ENUM_TOPHUNG,
		ENUM_BOTTOMHUNG,
		ENUM_PIVOTHORIZONTAL,
		ENUM_PIVOTVERTICAL,
		ENUM_SLIDINGHORIZONTAL,
		ENUM_SLIDINGVERTICAL,
		ENUM_REMOVABLECASEMENT,
		ENUM_FIXEDCASEMENT,
		ENUM_OTHEROPERATION,
		ENUM_NOTDEFINED
	};

	IfcWindowPanelOperationEnum();
	IfcWindowPanelOperationEnum( IfcWindowPanelOperationEnumEnum e ) { m_enum = e; }
	~IfcWindowPanelOperationEnum();
	virtual const char* classname() const { return "IfcWindowPanelOperationEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcWindowPanelOperationEnum> createObjectFromStepData( const std::string& arg );
	IfcWindowPanelOperationEnumEnum m_enum;
};

