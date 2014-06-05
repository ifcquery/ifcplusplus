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

// TYPE IfcFlowDirectionEnum = ENUMERATION OF	(SOURCE	,SINK	,SOURCEANDSINK	,NOTDEFINED);
class IfcFlowDirectionEnum : virtual public IfcPPObject
{
public:
	enum IfcFlowDirectionEnumEnum
	{
		ENUM_SOURCE,
		ENUM_SINK,
		ENUM_SOURCEANDSINK,
		ENUM_NOTDEFINED
	};

	IfcFlowDirectionEnum();
	IfcFlowDirectionEnum( IfcFlowDirectionEnumEnum e ) { m_enum = e; }
	~IfcFlowDirectionEnum();
	virtual const char* classname() const { return "IfcFlowDirectionEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcFlowDirectionEnum> createObjectFromStepData( const std::string& arg );
	IfcFlowDirectionEnumEnum m_enum;
};

