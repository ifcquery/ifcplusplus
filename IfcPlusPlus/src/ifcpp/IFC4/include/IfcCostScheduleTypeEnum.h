/* -*-c++-*- IfcPlusPlus - www.ifcquery.com - Copyright (C) 2011 Fabian Gerold
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
#include "ifcpp/model/IfcPPGlobal.h"

// TYPE IfcCostScheduleTypeEnum = ENUMERATION OF	(BUDGET	,COSTPLAN	,ESTIMATE	,TENDER	,PRICEDBILLOFQUANTITIES	,UNPRICEDBILLOFQUANTITIES	,SCHEDULEOFRATES	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcCostScheduleTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcCostScheduleTypeEnumEnum
	{
		ENUM_BUDGET,
		ENUM_COSTPLAN,
		ENUM_ESTIMATE,
		ENUM_TENDER,
		ENUM_PRICEDBILLOFQUANTITIES,
		ENUM_UNPRICEDBILLOFQUANTITIES,
		ENUM_SCHEDULEOFRATES,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcCostScheduleTypeEnum();
	IfcCostScheduleTypeEnum( IfcCostScheduleTypeEnumEnum e ) { m_enum = e; }
	~IfcCostScheduleTypeEnum();
	virtual const char* className() const { return "IfcCostScheduleTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcCostScheduleTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcCostScheduleTypeEnumEnum m_enum;
};

