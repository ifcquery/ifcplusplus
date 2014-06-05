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
#include "include/IfcGridTypeEnum.h"

// TYPE IfcGridTypeEnum 
IfcGridTypeEnum::IfcGridTypeEnum() {}
IfcGridTypeEnum::~IfcGridTypeEnum() {}
void IfcGridTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCGRIDTYPEENUM("; }
	if( m_enum == ENUM_RECTANGULAR )
	{
		stream << ".RECTANGULAR.";
	}
	else if( m_enum == ENUM_RADIAL )
	{
		stream << ".RADIAL.";
	}
	else if( m_enum == ENUM_TRIANGULAR )
	{
		stream << ".TRIANGULAR.";
	}
	else if( m_enum == ENUM_IRREGULAR )
	{
		stream << ".IRREGULAR.";
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
shared_ptr<IfcGridTypeEnum> IfcGridTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcGridTypeEnum>(); }
	shared_ptr<IfcGridTypeEnum> type_object( new IfcGridTypeEnum() );
	if( _stricmp( arg.c_str(), ".RECTANGULAR." ) == 0 )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_RECTANGULAR;
	}
	else if( _stricmp( arg.c_str(), ".RADIAL." ) == 0 )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_RADIAL;
	}
	else if( _stricmp( arg.c_str(), ".TRIANGULAR." ) == 0 )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_TRIANGULAR;
	}
	else if( _stricmp( arg.c_str(), ".IRREGULAR." ) == 0 )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_IRREGULAR;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
