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
#include "include/IfcFilterTypeEnum.h"

// TYPE IfcFilterTypeEnum = ENUMERATION OF	(AIRPARTICLEFILTER	,COMPRESSEDAIRFILTER	,ODORFILTER	,OILFILTER	,STRAINER	,WATERFILTER	,USERDEFINED	,NOTDEFINED);
IfcFilterTypeEnum::IfcFilterTypeEnum() {}
IfcFilterTypeEnum::~IfcFilterTypeEnum() {}
shared_ptr<IfcPPObject> IfcFilterTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFilterTypeEnum> copy_self( new IfcFilterTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFilterTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFILTERTYPEENUM("; }
	if( m_enum == ENUM_AIRPARTICLEFILTER )
	{
		stream << ".AIRPARTICLEFILTER.";
	}
	else if( m_enum == ENUM_COMPRESSEDAIRFILTER )
	{
		stream << ".COMPRESSEDAIRFILTER.";
	}
	else if( m_enum == ENUM_ODORFILTER )
	{
		stream << ".ODORFILTER.";
	}
	else if( m_enum == ENUM_OILFILTER )
	{
		stream << ".OILFILTER.";
	}
	else if( m_enum == ENUM_STRAINER )
	{
		stream << ".STRAINER.";
	}
	else if( m_enum == ENUM_WATERFILTER )
	{
		stream << ".WATERFILTER.";
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
shared_ptr<IfcFilterTypeEnum> IfcFilterTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFilterTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFilterTypeEnum>(); }
	shared_ptr<IfcFilterTypeEnum> type_object( new IfcFilterTypeEnum() );
	if( boost::iequals( arg, L".AIRPARTICLEFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_AIRPARTICLEFILTER;
	}
	else if( boost::iequals( arg, L".COMPRESSEDAIRFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_COMPRESSEDAIRFILTER;
	}
	else if( boost::iequals( arg, L".ODORFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_ODORFILTER;
	}
	else if( boost::iequals( arg, L".OILFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_OILFILTER;
	}
	else if( boost::iequals( arg, L".STRAINER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_STRAINER;
	}
	else if( boost::iequals( arg, L".WATERFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_WATERFILTER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
