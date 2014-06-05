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

// TYPE IfcPropertySetTemplateTypeEnum = ENUMERATION OF	(PSET_TYPEDRIVENONLY	,PSET_TYPEDRIVENOVERRIDE	,PSET_OCCURRENCEDRIVEN	,PSET_PERFORMANCEDRIVEN	,QTO_TYPEDRIVENONLY	,QTO_TYPEDRIVENOVERRIDE	,QTO_OCCURRENCEDRIVEN	,NOTDEFINED);
class IfcPropertySetTemplateTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcPropertySetTemplateTypeEnumEnum
	{
		ENUM_PSET_TYPEDRIVENONLY,
		ENUM_PSET_TYPEDRIVENOVERRIDE,
		ENUM_PSET_OCCURRENCEDRIVEN,
		ENUM_PSET_PERFORMANCEDRIVEN,
		ENUM_QTO_TYPEDRIVENONLY,
		ENUM_QTO_TYPEDRIVENOVERRIDE,
		ENUM_QTO_OCCURRENCEDRIVEN,
		ENUM_NOTDEFINED
	};

	IfcPropertySetTemplateTypeEnum();
	IfcPropertySetTemplateTypeEnum( IfcPropertySetTemplateTypeEnumEnum e ) { m_enum = e; }
	~IfcPropertySetTemplateTypeEnum();
	virtual const char* classname() const { return "IfcPropertySetTemplateTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcPropertySetTemplateTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcPropertySetTemplateTypeEnumEnum m_enum;
};

