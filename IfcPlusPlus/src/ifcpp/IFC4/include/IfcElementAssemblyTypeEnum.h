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

// TYPE IfcElementAssemblyTypeEnum = ENUMERATION OF	(ACCESSORY_ASSEMBLY	,ARCH	,BEAM_GRID	,BRACED_FRAME	,GIRDER	,REINFORCEMENT_UNIT	,RIGID_FRAME	,SLAB_FIELD	,TRUSS	,USERDEFINED	,NOTDEFINED);
class IfcElementAssemblyTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcElementAssemblyTypeEnumEnum
	{
		ENUM_ACCESSORY_ASSEMBLY,
		ENUM_ARCH,
		ENUM_BEAM_GRID,
		ENUM_BRACED_FRAME,
		ENUM_GIRDER,
		ENUM_REINFORCEMENT_UNIT,
		ENUM_RIGID_FRAME,
		ENUM_SLAB_FIELD,
		ENUM_TRUSS,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcElementAssemblyTypeEnum();
	IfcElementAssemblyTypeEnum( IfcElementAssemblyTypeEnumEnum e ) { m_enum = e; }
	~IfcElementAssemblyTypeEnum();
	virtual const char* classname() const { return "IfcElementAssemblyTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcElementAssemblyTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcElementAssemblyTypeEnumEnum m_enum;
};

