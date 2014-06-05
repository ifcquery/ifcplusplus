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
#include "include/IfcMechanicalFastenerTypeEnum.h"

// TYPE IfcMechanicalFastenerTypeEnum 
IfcMechanicalFastenerTypeEnum::IfcMechanicalFastenerTypeEnum() {}
IfcMechanicalFastenerTypeEnum::~IfcMechanicalFastenerTypeEnum() {}
void IfcMechanicalFastenerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCMECHANICALFASTENERTYPEENUM("; }
	if( m_enum == ENUM_ANCHORBOLT )
	{
		stream << ".ANCHORBOLT.";
	}
	else if( m_enum == ENUM_BOLT )
	{
		stream << ".BOLT.";
	}
	else if( m_enum == ENUM_DOWEL )
	{
		stream << ".DOWEL.";
	}
	else if( m_enum == ENUM_NAIL )
	{
		stream << ".NAIL.";
	}
	else if( m_enum == ENUM_NAILPLATE )
	{
		stream << ".NAILPLATE.";
	}
	else if( m_enum == ENUM_RIVET )
	{
		stream << ".RIVET.";
	}
	else if( m_enum == ENUM_SCREW )
	{
		stream << ".SCREW.";
	}
	else if( m_enum == ENUM_SHEARCONNECTOR )
	{
		stream << ".SHEARCONNECTOR.";
	}
	else if( m_enum == ENUM_STAPLE )
	{
		stream << ".STAPLE.";
	}
	else if( m_enum == ENUM_STUDSHEARCONNECTOR )
	{
		stream << ".STUDSHEARCONNECTOR.";
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
shared_ptr<IfcMechanicalFastenerTypeEnum> IfcMechanicalFastenerTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcMechanicalFastenerTypeEnum>(); }
	shared_ptr<IfcMechanicalFastenerTypeEnum> type_object( new IfcMechanicalFastenerTypeEnum() );
	if( _stricmp( arg.c_str(), ".ANCHORBOLT." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_ANCHORBOLT;
	}
	else if( _stricmp( arg.c_str(), ".BOLT." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_BOLT;
	}
	else if( _stricmp( arg.c_str(), ".DOWEL." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_DOWEL;
	}
	else if( _stricmp( arg.c_str(), ".NAIL." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NAIL;
	}
	else if( _stricmp( arg.c_str(), ".NAILPLATE." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NAILPLATE;
	}
	else if( _stricmp( arg.c_str(), ".RIVET." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_RIVET;
	}
	else if( _stricmp( arg.c_str(), ".SCREW." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_SCREW;
	}
	else if( _stricmp( arg.c_str(), ".SHEARCONNECTOR." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_SHEARCONNECTOR;
	}
	else if( _stricmp( arg.c_str(), ".STAPLE." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_STAPLE;
	}
	else if( _stricmp( arg.c_str(), ".STUDSHEARCONNECTOR." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_STUDSHEARCONNECTOR;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
