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

// TYPE IfcAnalysisTheoryTypeEnum = ENUMERATION OF	(FIRST_ORDER_THEORY	,SECOND_ORDER_THEORY	,THIRD_ORDER_THEORY	,FULL_NONLINEAR_THEORY	,USERDEFINED	,NOTDEFINED);
class IfcAnalysisTheoryTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcAnalysisTheoryTypeEnumEnum
	{
		ENUM_FIRST_ORDER_THEORY,
		ENUM_SECOND_ORDER_THEORY,
		ENUM_THIRD_ORDER_THEORY,
		ENUM_FULL_NONLINEAR_THEORY,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcAnalysisTheoryTypeEnum();
	IfcAnalysisTheoryTypeEnum( IfcAnalysisTheoryTypeEnumEnum e ) { m_enum = e; }
	~IfcAnalysisTheoryTypeEnum();
	virtual const char* classname() const { return "IfcAnalysisTheoryTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcAnalysisTheoryTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcAnalysisTheoryTypeEnumEnum m_enum;
};

