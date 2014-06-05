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
#include "include/IfcAddressTypeEnum.h"

// TYPE IfcAddressTypeEnum 
IfcAddressTypeEnum::IfcAddressTypeEnum() {}
IfcAddressTypeEnum::~IfcAddressTypeEnum() {}
void IfcAddressTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCADDRESSTYPEENUM("; }
	if( m_enum == ENUM_OFFICE )
	{
		stream << ".OFFICE.";
	}
	else if( m_enum == ENUM_SITE )
	{
		stream << ".SITE.";
	}
	else if( m_enum == ENUM_HOME )
	{
		stream << ".HOME.";
	}
	else if( m_enum == ENUM_DISTRIBUTIONPOINT )
	{
		stream << ".DISTRIBUTIONPOINT.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcAddressTypeEnum> IfcAddressTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcAddressTypeEnum>(); }
	shared_ptr<IfcAddressTypeEnum> type_object( new IfcAddressTypeEnum() );
	if( _stricmp( arg.c_str(), ".OFFICE." ) == 0 )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_OFFICE;
	}
	else if( _stricmp( arg.c_str(), ".SITE." ) == 0 )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_SITE;
	}
	else if( _stricmp( arg.c_str(), ".HOME." ) == 0 )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_HOME;
	}
	else if( _stricmp( arg.c_str(), ".DISTRIBUTIONPOINT." ) == 0 )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_DISTRIBUTIONPOINT;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAddressTypeEnum::ENUM_USERDEFINED;
	}
	return type_object;
}
