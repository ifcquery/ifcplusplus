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

// TYPE IfcEvaporativeCoolerTypeEnum 
IfcEvaporativeCoolerTypeEnum::IfcEvaporativeCoolerTypeEnum() {}
IfcEvaporativeCoolerTypeEnum::~IfcEvaporativeCoolerTypeEnum() {}
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
shared_ptr<IfcEvaporativeCoolerTypeEnum> IfcEvaporativeCoolerTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcEvaporativeCoolerTypeEnum>(); }
	shared_ptr<IfcEvaporativeCoolerTypeEnum> type_object( new IfcEvaporativeCoolerTypeEnum() );
	if( _stricmp( arg.c_str(), ".DIRECTEVAPORATIVERANDOMMEDIAAIRCOOLER." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVERANDOMMEDIAAIRCOOLER;
	}
	else if( _stricmp( arg.c_str(), ".DIRECTEVAPORATIVERIGIDMEDIAAIRCOOLER." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVERIGIDMEDIAAIRCOOLER;
	}
	else if( _stricmp( arg.c_str(), ".DIRECTEVAPORATIVESLINGERSPACKAGEDAIRCOOLER." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVESLINGERSPACKAGEDAIRCOOLER;
	}
	else if( _stricmp( arg.c_str(), ".DIRECTEVAPORATIVEPACKAGEDROTARYAIRCOOLER." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVEPACKAGEDROTARYAIRCOOLER;
	}
	else if( _stricmp( arg.c_str(), ".DIRECTEVAPORATIVEAIRWASHER." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_DIRECTEVAPORATIVEAIRWASHER;
	}
	else if( _stricmp( arg.c_str(), ".INDIRECTEVAPORATIVEPACKAGEAIRCOOLER." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_INDIRECTEVAPORATIVEPACKAGEAIRCOOLER;
	}
	else if( _stricmp( arg.c_str(), ".INDIRECTEVAPORATIVEWETCOIL." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_INDIRECTEVAPORATIVEWETCOIL;
	}
	else if( _stricmp( arg.c_str(), ".INDIRECTEVAPORATIVECOOLINGTOWERORCOILCOOLER." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_INDIRECTEVAPORATIVECOOLINGTOWERORCOILCOOLER;
	}
	else if( _stricmp( arg.c_str(), ".INDIRECTDIRECTCOMBINATION." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_INDIRECTDIRECTCOMBINATION;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcEvaporativeCoolerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
