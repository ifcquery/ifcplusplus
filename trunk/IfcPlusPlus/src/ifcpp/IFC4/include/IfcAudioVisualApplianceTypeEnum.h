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

// TYPE IfcAudioVisualApplianceTypeEnum = ENUMERATION OF	(AMPLIFIER	,CAMERA	,DISPLAY	,MICROPHONE	,PLAYER	,PROJECTOR	,RECEIVER	,SPEAKER	,SWITCHER	,TELEPHONE	,TUNER	,USERDEFINED	,NOTDEFINED);
class IfcAudioVisualApplianceTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcAudioVisualApplianceTypeEnumEnum
	{
		ENUM_AMPLIFIER,
		ENUM_CAMERA,
		ENUM_DISPLAY,
		ENUM_MICROPHONE,
		ENUM_PLAYER,
		ENUM_PROJECTOR,
		ENUM_RECEIVER,
		ENUM_SPEAKER,
		ENUM_SWITCHER,
		ENUM_TELEPHONE,
		ENUM_TUNER,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcAudioVisualApplianceTypeEnum();
	IfcAudioVisualApplianceTypeEnum( IfcAudioVisualApplianceTypeEnumEnum e ) { m_enum = e; }
	~IfcAudioVisualApplianceTypeEnum();
	virtual const char* classname() const { return "IfcAudioVisualApplianceTypeEnum"; }
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcAudioVisualApplianceTypeEnum> createObjectFromStepData( const std::string& arg );
	IfcAudioVisualApplianceTypeEnumEnum m_enum;
};

