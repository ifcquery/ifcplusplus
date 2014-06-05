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
#include "include/IfcTendonAnchorTypeEnum.h"

// TYPE IfcTendonAnchorTypeEnum 
IfcTendonAnchorTypeEnum::IfcTendonAnchorTypeEnum() {}
IfcTendonAnchorTypeEnum::~IfcTendonAnchorTypeEnum() {}
void IfcTendonAnchorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTENDONANCHORTYPEENUM("; }
	if( m_enum == ENUM_COUPLER )
	{
		stream << ".COUPLER.";
	}
	else if( m_enum == ENUM_FIXED_END )
	{
		stream << ".FIXED_END.";
	}
	else if( m_enum == ENUM_TENSIONING_END )
	{
		stream << ".TENSIONING_END.";
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
shared_ptr<IfcTendonAnchorTypeEnum> IfcTendonAnchorTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcTendonAnchorTypeEnum>(); }
	shared_ptr<IfcTendonAnchorTypeEnum> type_object( new IfcTendonAnchorTypeEnum() );
	if( _stricmp( arg.c_str(), ".COUPLER." ) == 0 )
	{
		type_object->m_enum = IfcTendonAnchorTypeEnum::ENUM_COUPLER;
	}
	else if( _stricmp( arg.c_str(), ".FIXED_END." ) == 0 )
	{
		type_object->m_enum = IfcTendonAnchorTypeEnum::ENUM_FIXED_END;
	}
	else if( _stricmp( arg.c_str(), ".TENSIONING_END." ) == 0 )
	{
		type_object->m_enum = IfcTendonAnchorTypeEnum::ENUM_TENSIONING_END;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcTendonAnchorTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcTendonAnchorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
