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
#include "include/IfcCoveringTypeEnum.h"

// TYPE IfcCoveringTypeEnum 
IfcCoveringTypeEnum::IfcCoveringTypeEnum() {}
IfcCoveringTypeEnum::~IfcCoveringTypeEnum() {}
void IfcCoveringTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOVERINGTYPEENUM("; }
	if( m_enum == ENUM_CEILING )
	{
		stream << ".CEILING.";
	}
	else if( m_enum == ENUM_FLOORING )
	{
		stream << ".FLOORING.";
	}
	else if( m_enum == ENUM_CLADDING )
	{
		stream << ".CLADDING.";
	}
	else if( m_enum == ENUM_ROOFING )
	{
		stream << ".ROOFING.";
	}
	else if( m_enum == ENUM_MOLDING )
	{
		stream << ".MOLDING.";
	}
	else if( m_enum == ENUM_SKIRTINGBOARD )
	{
		stream << ".SKIRTINGBOARD.";
	}
	else if( m_enum == ENUM_INSULATION )
	{
		stream << ".INSULATION.";
	}
	else if( m_enum == ENUM_MEMBRANE )
	{
		stream << ".MEMBRANE.";
	}
	else if( m_enum == ENUM_SLEEVING )
	{
		stream << ".SLEEVING.";
	}
	else if( m_enum == ENUM_WRAPPING )
	{
		stream << ".WRAPPING.";
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
shared_ptr<IfcCoveringTypeEnum> IfcCoveringTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcCoveringTypeEnum>(); }
	shared_ptr<IfcCoveringTypeEnum> type_object( new IfcCoveringTypeEnum() );
	if( _stricmp( arg.c_str(), ".CEILING." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_CEILING;
	}
	else if( _stricmp( arg.c_str(), ".FLOORING." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_FLOORING;
	}
	else if( _stricmp( arg.c_str(), ".CLADDING." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_CLADDING;
	}
	else if( _stricmp( arg.c_str(), ".ROOFING." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_ROOFING;
	}
	else if( _stricmp( arg.c_str(), ".MOLDING." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_MOLDING;
	}
	else if( _stricmp( arg.c_str(), ".SKIRTINGBOARD." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_SKIRTINGBOARD;
	}
	else if( _stricmp( arg.c_str(), ".INSULATION." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_INSULATION;
	}
	else if( _stricmp( arg.c_str(), ".MEMBRANE." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_MEMBRANE;
	}
	else if( _stricmp( arg.c_str(), ".SLEEVING." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_SLEEVING;
	}
	else if( _stricmp( arg.c_str(), ".WRAPPING." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_WRAPPING;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcCoveringTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
