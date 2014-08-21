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
#include "include/IfcHumidifierTypeEnum.h"

// TYPE IfcHumidifierTypeEnum = ENUMERATION OF	(STEAMINJECTION	,ADIABATICAIRWASHER	,ADIABATICPAN	,ADIABATICWETTEDELEMENT	,ADIABATICATOMIZING	,ADIABATICULTRASONIC	,ADIABATICRIGIDMEDIA	,ADIABATICCOMPRESSEDAIRNOZZLE	,ASSISTEDELECTRIC	,ASSISTEDNATURALGAS	,ASSISTEDPROPANE	,ASSISTEDBUTANE	,ASSISTEDSTEAM	,USERDEFINED	,NOTDEFINED);
IfcHumidifierTypeEnum::IfcHumidifierTypeEnum() {}
IfcHumidifierTypeEnum::~IfcHumidifierTypeEnum() {}
shared_ptr<IfcPPObject> IfcHumidifierTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcHumidifierTypeEnum> copy_self( new IfcHumidifierTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcHumidifierTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCHUMIDIFIERTYPEENUM("; }
	if( m_enum == ENUM_STEAMINJECTION )
	{
		stream << ".STEAMINJECTION.";
	}
	else if( m_enum == ENUM_ADIABATICAIRWASHER )
	{
		stream << ".ADIABATICAIRWASHER.";
	}
	else if( m_enum == ENUM_ADIABATICPAN )
	{
		stream << ".ADIABATICPAN.";
	}
	else if( m_enum == ENUM_ADIABATICWETTEDELEMENT )
	{
		stream << ".ADIABATICWETTEDELEMENT.";
	}
	else if( m_enum == ENUM_ADIABATICATOMIZING )
	{
		stream << ".ADIABATICATOMIZING.";
	}
	else if( m_enum == ENUM_ADIABATICULTRASONIC )
	{
		stream << ".ADIABATICULTRASONIC.";
	}
	else if( m_enum == ENUM_ADIABATICRIGIDMEDIA )
	{
		stream << ".ADIABATICRIGIDMEDIA.";
	}
	else if( m_enum == ENUM_ADIABATICCOMPRESSEDAIRNOZZLE )
	{
		stream << ".ADIABATICCOMPRESSEDAIRNOZZLE.";
	}
	else if( m_enum == ENUM_ASSISTEDELECTRIC )
	{
		stream << ".ASSISTEDELECTRIC.";
	}
	else if( m_enum == ENUM_ASSISTEDNATURALGAS )
	{
		stream << ".ASSISTEDNATURALGAS.";
	}
	else if( m_enum == ENUM_ASSISTEDPROPANE )
	{
		stream << ".ASSISTEDPROPANE.";
	}
	else if( m_enum == ENUM_ASSISTEDBUTANE )
	{
		stream << ".ASSISTEDBUTANE.";
	}
	else if( m_enum == ENUM_ASSISTEDSTEAM )
	{
		stream << ".ASSISTEDSTEAM.";
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
shared_ptr<IfcHumidifierTypeEnum> IfcHumidifierTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcHumidifierTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcHumidifierTypeEnum>(); }
	shared_ptr<IfcHumidifierTypeEnum> type_object( new IfcHumidifierTypeEnum() );
	if( boost::iequals( arg, L".STEAMINJECTION." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_STEAMINJECTION;
	}
	else if( boost::iequals( arg, L".ADIABATICAIRWASHER." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ADIABATICAIRWASHER;
	}
	else if( boost::iequals( arg, L".ADIABATICPAN." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ADIABATICPAN;
	}
	else if( boost::iequals( arg, L".ADIABATICWETTEDELEMENT." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ADIABATICWETTEDELEMENT;
	}
	else if( boost::iequals( arg, L".ADIABATICATOMIZING." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ADIABATICATOMIZING;
	}
	else if( boost::iequals( arg, L".ADIABATICULTRASONIC." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ADIABATICULTRASONIC;
	}
	else if( boost::iequals( arg, L".ADIABATICRIGIDMEDIA." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ADIABATICRIGIDMEDIA;
	}
	else if( boost::iequals( arg, L".ADIABATICCOMPRESSEDAIRNOZZLE." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ADIABATICCOMPRESSEDAIRNOZZLE;
	}
	else if( boost::iequals( arg, L".ASSISTEDELECTRIC." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ASSISTEDELECTRIC;
	}
	else if( boost::iequals( arg, L".ASSISTEDNATURALGAS." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ASSISTEDNATURALGAS;
	}
	else if( boost::iequals( arg, L".ASSISTEDPROPANE." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ASSISTEDPROPANE;
	}
	else if( boost::iequals( arg, L".ASSISTEDBUTANE." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ASSISTEDBUTANE;
	}
	else if( boost::iequals( arg, L".ASSISTEDSTEAM." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_ASSISTEDSTEAM;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcHumidifierTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
