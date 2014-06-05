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

// TYPE IfcLayerSetDirectionEnum = ENUMERATION OF	(AXIS1	,AXIS2	,AXIS3);
class IfcLayerSetDirectionEnum : virtual public IfcPPObject
{
public:
	enum IfcLayerSetDirectionEnumEnum
	{
		ENUM_AXIS1,
		ENUM_AXIS2,
		ENUM_AXIS3
	};

	IfcLayerSetDirectionEnum();
	IfcLayerSetDirectionEnum( IfcLayerSetDirectionEnumEnum e ) { m_enum = e; }
	~IfcLayerSetDirectionEnum();
	virtual const char* classname() const { return "IfcLayerSetDirectionEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcLayerSetDirectionEnum> createObjectFromStepData( const std::string& arg );
	IfcLayerSetDirectionEnumEnum m_enum;
};

