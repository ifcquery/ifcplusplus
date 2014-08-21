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

// TYPE IfcMechanicalFastenerTypeEnum = ENUMERATION OF	(ANCHORBOLT	,BOLT	,DOWEL	,NAIL	,NAILPLATE	,RIVET	,SCREW	,SHEARCONNECTOR	,STAPLE	,STUDSHEARCONNECTOR	,USERDEFINED	,NOTDEFINED);
IfcMechanicalFastenerTypeEnum::IfcMechanicalFastenerTypeEnum() {}
IfcMechanicalFastenerTypeEnum::~IfcMechanicalFastenerTypeEnum() {}
shared_ptr<IfcPPObject> IfcMechanicalFastenerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMechanicalFastenerTypeEnum> copy_self( new IfcMechanicalFastenerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcMechanicalFastenerTypeEnum> IfcMechanicalFastenerTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcMechanicalFastenerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcMechanicalFastenerTypeEnum>(); }
	shared_ptr<IfcMechanicalFastenerTypeEnum> type_object( new IfcMechanicalFastenerTypeEnum() );
	if( boost::iequals( arg, L".ANCHORBOLT." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_ANCHORBOLT;
	}
	else if( boost::iequals( arg, L".BOLT." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_BOLT;
	}
	else if( boost::iequals( arg, L".DOWEL." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_DOWEL;
	}
	else if( boost::iequals( arg, L".NAIL." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NAIL;
	}
	else if( boost::iequals( arg, L".NAILPLATE." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NAILPLATE;
	}
	else if( boost::iequals( arg, L".RIVET." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_RIVET;
	}
	else if( boost::iequals( arg, L".SCREW." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_SCREW;
	}
	else if( boost::iequals( arg, L".SHEARCONNECTOR." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_SHEARCONNECTOR;
	}
	else if( boost::iequals( arg, L".STAPLE." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_STAPLE;
	}
	else if( boost::iequals( arg, L".STUDSHEARCONNECTOR." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_STUDSHEARCONNECTOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
