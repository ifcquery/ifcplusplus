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

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcAudioVisualApplianceTypeEnum.h"

// TYPE IfcAudioVisualApplianceTypeEnum = ENUMERATION OF	(AMPLIFIER	,CAMERA	,DISPLAY	,MICROPHONE	,PLAYER	,PROJECTOR	,RECEIVER	,SPEAKER	,SWITCHER	,TELEPHONE	,TUNER	,USERDEFINED	,NOTDEFINED);
IfcAudioVisualApplianceTypeEnum::IfcAudioVisualApplianceTypeEnum() {}
IfcAudioVisualApplianceTypeEnum::~IfcAudioVisualApplianceTypeEnum() {}
shared_ptr<IfcPPObject> IfcAudioVisualApplianceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAudioVisualApplianceTypeEnum> copy_self( new IfcAudioVisualApplianceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcAudioVisualApplianceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCAUDIOVISUALAPPLIANCETYPEENUM("; }
	if( m_enum == ENUM_AMPLIFIER )
	{
		stream << ".AMPLIFIER.";
	}
	else if( m_enum == ENUM_CAMERA )
	{
		stream << ".CAMERA.";
	}
	else if( m_enum == ENUM_DISPLAY )
	{
		stream << ".DISPLAY.";
	}
	else if( m_enum == ENUM_MICROPHONE )
	{
		stream << ".MICROPHONE.";
	}
	else if( m_enum == ENUM_PLAYER )
	{
		stream << ".PLAYER.";
	}
	else if( m_enum == ENUM_PROJECTOR )
	{
		stream << ".PROJECTOR.";
	}
	else if( m_enum == ENUM_RECEIVER )
	{
		stream << ".RECEIVER.";
	}
	else if( m_enum == ENUM_SPEAKER )
	{
		stream << ".SPEAKER.";
	}
	else if( m_enum == ENUM_SWITCHER )
	{
		stream << ".SWITCHER.";
	}
	else if( m_enum == ENUM_TELEPHONE )
	{
		stream << ".TELEPHONE.";
	}
	else if( m_enum == ENUM_TUNER )
	{
		stream << ".TUNER.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcAudioVisualApplianceTypeEnum> IfcAudioVisualApplianceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcAudioVisualApplianceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcAudioVisualApplianceTypeEnum>(); }
	shared_ptr<IfcAudioVisualApplianceTypeEnum> type_object( new IfcAudioVisualApplianceTypeEnum() );
	if( boost::iequals( arg, L".AMPLIFIER." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_AMPLIFIER;
	}
	else if( boost::iequals( arg, L".CAMERA." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_CAMERA;
	}
	else if( boost::iequals( arg, L".DISPLAY." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_DISPLAY;
	}
	else if( boost::iequals( arg, L".MICROPHONE." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_MICROPHONE;
	}
	else if( boost::iequals( arg, L".PLAYER." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_PLAYER;
	}
	else if( boost::iequals( arg, L".PROJECTOR." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_PROJECTOR;
	}
	else if( boost::iequals( arg, L".RECEIVER." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_RECEIVER;
	}
	else if( boost::iequals( arg, L".SPEAKER." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_SPEAKER;
	}
	else if( boost::iequals( arg, L".SWITCHER." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_SWITCHER;
	}
	else if( boost::iequals( arg, L".TELEPHONE." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_TELEPHONE;
	}
	else if( boost::iequals( arg, L".TUNER." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_TUNER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcAudioVisualApplianceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
