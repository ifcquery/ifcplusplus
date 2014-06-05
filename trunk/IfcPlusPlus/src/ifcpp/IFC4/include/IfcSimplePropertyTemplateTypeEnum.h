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

// TYPE IfcSimplePropertyTemplateTypeEnum = ENUMERATION OF	(P_SINGLEVALUE	,P_ENUMERATEDVALUE	,P_BOUNDEDVALUE	,P_LISTVALUE	,P_TABLEVALUE	,P_REFERENCEVALUE	,Q_LENGTH	,Q_AREA	,Q_VOLUME	,Q_COUNT	,Q_WEIGHT	,Q_TIME);
class IfcSimplePropertyTemplateTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcSimplePropertyTemplateTypeEnumEnum
	{
		ENUM_P_SINGLEVALUE,
		ENUM_P_ENUMERATEDVALUE,
		ENUM_P_BOUNDEDVALUE,
		ENUM_P_LISTVALUE,
		ENUM_P_TABLEVALUE,
		ENUM_P_REFERENCEVALUE,
		ENUM_Q_LENGTH,
		ENUM_Q_AREA,
		ENUM_Q_VOLUME,
		ENUM_Q_COUNT,
		ENUM_Q_WEIGHT,
		ENUM_Q_TIME
	};

	IfcSimplePropertyTemplateTypeEnum();
	IfcSimplePropertyTemplateTypeEnum( IfcSimplePropertyTemplateTypeEnumEnum e ) { m_enum = e; }
	~IfcSimplePropertyTemplateTypeEnum();
	virtual const char* classname() const { return "IfcSimplePropertyTemplateTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcSimplePropertyTemplateTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcSimplePropertyTemplateTypeEnumEnum m_enum;
};

