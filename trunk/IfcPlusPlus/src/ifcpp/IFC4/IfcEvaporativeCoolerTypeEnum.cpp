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
#include "include/IfcEvaporativeCoolerTypeEnum.h"

// TYPE IfcEvaporativeCoolerTypeEnum = ENUMERATION OF	(DIRECTEVAPORATIVERANDOMMEDIAAIRCOOLER	,DIRECTEVAPORATIVERIGIDMEDIAAIRCOOLER	,DIRECTEVAPORATIVESLINGERSPACKAGEDAIRCOOLER	,DIRECTEVAPORATIVEPACKAGEDROTARYAIRCOOLER	,DIRECTEVAPORATIVEAIRWASHER	,INDIRECTEVAPORATIVEPACKAGEAIRCOOLER	,INDIRECTEVAPORATIVEWETCOIL	,INDIRECTEVAPORATIVECOOLINGTOWERORCOILCOOLER	,INDIRECTDIRECTCOMBINATION	,USERDEFINED	,NOTDEFINED);
IfcEvaporativeCoolerTypeEnum::IfcEvaporativeCoolerTypeEnum() {}
IfcEvaporativeCoolerTypeEnum::~IfcEvaporativeCoolerTypeEnum() {}
shared_ptr<IfcPPObject> IfcEvaporativeCoolerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcEvaporativeCoolerTypeEnum> copy_self( new IfcEvaporativeCoolerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcEvaporativeCoolerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCEVAPORATIVECOOLERTYPEENUM("; }
	if( m_enum == ENUM_DIRECTEVAPORATIVERANDOMMEDIAAIRCOOLER )
	{
		stream << ".DIRECTEVAPORATIVERANDOMMEDIAAIRCOOLER.";
	}
	else if( m_enum == ENUM_DIRECTEVAPORATIVERIGIDMEDIAAIRCOOLER )
	{
		stream << ".DIRECTEVAPORATIVERIGIDMEDIAAIRCOOLER.";
	}
	else if( m_enum == ENUM_DIRECTEVAPORATIVESLINGERSPACKAGEDAIRCOOLER )
	{
		stream << ".DIRECTEVAPORATIVESLINGERSPACKAGEDAIRCOOLER.";
	}
	else if( m_enum == ENUM_DIRECTEVAPORATIVEPACKAGEDROTARYAIRCOOLER )
	{
		stream << ".DIRECTEVAPORATIVEPACKAGEDROTARYAIRCOOLER.";
	}
	else if( m_enum == ENUM_DIRECTEVAPORATIVEAIRWASHER )
	{
		stream << ".DIRECTEVAPORATIVEAIRWASHER.";
	}
	else if( m_enum == ENUM_INDIRECTEVAPORATIVEPACKAGEAIRCOOLER )
	{
		stream << ".INDIRECTEVAPORATIVEPACKAGEAIRCOOLER.";
	}
	else if( m_enum == ENUM_INDIRECTEVAPORATIVEWETCOIL )
	{
		stream << ".INDIRECTEVAPORATIVEWETCOIL.";
	}
	else if( m_enum == ENUM_INDIRECTEVAPORATIVECOOLINGTOWERORCOILCOOLER )
	{
		stream << ".INDIRECTEVAPORATIVECOOLINGTOWERORCOILCOOLER.";
	}
	else if( m_enum == ENUM_INDIRECTDIRECTCOMBINATION )
	{
		stream << ".INDIRECTDIRECTCOMBINATION.";
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
shared_ptr<IfcEvaporativeCoolerTypeEnum> IfcEvaporativeCoolerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcEvaporativeCoolerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcEvaporativeCoolerTypeEnum>(); }
	shared_ptr<IfcEvaporativeCoolerTypeEnum> type_object( new IfcEvaporativeCoolerTypeEnum() );
	if( boost::iequals( arg, L".DIRECTEVAPORATIVERANDOMMEDIAAIRCOOLER." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVERANDOMMEDIAAIRCOOLER;
	}
	else if( boost::iequals( arg, L".DIRECTEVAPORATIVERIGIDMEDIAAIRCOOLER." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVERIGIDMEDIAAIRCOOLER;
	}
	else if( boost::iequals( arg, L".DIRECTEVAPORATIVESLINGERSPACKAGEDAIRCOOLER." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVESLINGERSPACKAGEDAIRCOOLER;
	}
	else if( boost::iequals( arg, L".DIRECTEVAPORATIVEPACKAGEDROTARYAIRCOOLER." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVEPACKAGEDROTARYAIRCOOLER;
	}
	else if( boost::iequals( arg, L".DIRECTEVAPORATIVEAIRWASHER." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVEAIRWASHER;
	}
	else if( boost::iequals( arg, L".INDIRECTEVAPORATIVEPACKAGEAIRCOOLER." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_INDIRECTEVAPORATIVEPACKAGEAIRCOOLER;
	}
	else if( boost::iequals( arg, L".INDIRECTEVAPORATIVEWETCOIL." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_INDIRECTEVAPORATIVEWETCOIL;
	}
	else if( boost::iequals( arg, L".INDIRECTEVAPORATIVECOOLINGTOWERORCOILCOOLER." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_INDIRECTEVAPORATIVECOOLINGTOWERORCOILCOOLER;
	}
	else if( boost::iequals( arg, L".INDIRECTDIRECTCOMBINATION." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_INDIRECTDIRECTCOMBINATION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
