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
#include "include/IfcLaborResourceTypeEnum.h"

// TYPE IfcLaborResourceTypeEnum 
IfcLaborResourceTypeEnum::IfcLaborResourceTypeEnum() {}
IfcLaborResourceTypeEnum::~IfcLaborResourceTypeEnum() {}
void IfcLaborResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLABORRESOURCETYPEENUM("; }
	if( m_enum == ENUM_ADMINISTRATION )
	{
		stream << ".ADMINISTRATION.";
	}
	else if( m_enum == ENUM_CARPENTRY )
	{
		stream << ".CARPENTRY.";
	}
	else if( m_enum == ENUM_CLEANING )
	{
		stream << ".CLEANING.";
	}
	else if( m_enum == ENUM_CONCRETE )
	{
		stream << ".CONCRETE.";
	}
	else if( m_enum == ENUM_DRYWALL )
	{
		stream << ".DRYWALL.";
	}
	else if( m_enum == ENUM_ELECTRIC )
	{
		stream << ".ELECTRIC.";
	}
	else if( m_enum == ENUM_FINISHING )
	{
		stream << ".FINISHING.";
	}
	else if( m_enum == ENUM_FLOORING )
	{
		stream << ".FLOORING.";
	}
	else if( m_enum == ENUM_GENERAL )
	{
		stream << ".GENERAL.";
	}
	else if( m_enum == ENUM_HVAC )
	{
		stream << ".HVAC.";
	}
	else if( m_enum == ENUM_LANDSCAPING )
	{
		stream << ".LANDSCAPING.";
	}
	else if( m_enum == ENUM_MASONRY )
	{
		stream << ".MASONRY.";
	}
	else if( m_enum == ENUM_PAINTING )
	{
		stream << ".PAINTING.";
	}
	else if( m_enum == ENUM_PAVING )
	{
		stream << ".PAVING.";
	}
	else if( m_enum == ENUM_PLUMBING )
	{
		stream << ".PLUMBING.";
	}
	else if( m_enum == ENUM_ROOFING )
	{
		stream << ".ROOFING.";
	}
	else if( m_enum == ENUM_SITEGRADING )
	{
		stream << ".SITEGRADING.";
	}
	else if( m_enum == ENUM_STEELWORK )
	{
		stream << ".STEELWORK.";
	}
	else if( m_enum == ENUM_SURVEYING )
	{
		stream << ".SURVEYING.";
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
shared_ptr<IfcLaborResourceTypeEnum> IfcLaborResourceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcLaborResourceTypeEnum>(); }
	shared_ptr<IfcLaborResourceTypeEnum> type_object( new IfcLaborResourceTypeEnum() );
	if( _stricmp( arg.c_str(), ".ADMINISTRATION." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_ADMINISTRATION;
	}
	else if( _stricmp( arg.c_str(), ".CARPENTRY." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_CARPENTRY;
	}
	else if( _stricmp( arg.c_str(), ".CLEANING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_CLEANING;
	}
	else if( _stricmp( arg.c_str(), ".CONCRETE." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_CONCRETE;
	}
	else if( _stricmp( arg.c_str(), ".DRYWALL." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_DRYWALL;
	}
	else if( _stricmp( arg.c_str(), ".ELECTRIC." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_ELECTRIC;
	}
	else if( _stricmp( arg.c_str(), ".FINISHING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_FINISHING;
	}
	else if( _stricmp( arg.c_str(), ".FLOORING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_FLOORING;
	}
	else if( _stricmp( arg.c_str(), ".GENERAL." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_GENERAL;
	}
	else if( _stricmp( arg.c_str(), ".HVAC." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_HVAC;
	}
	else if( _stricmp( arg.c_str(), ".LANDSCAPING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_LANDSCAPING;
	}
	else if( _stricmp( arg.c_str(), ".MASONRY." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_MASONRY;
	}
	else if( _stricmp( arg.c_str(), ".PAINTING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_PAINTING;
	}
	else if( _stricmp( arg.c_str(), ".PAVING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_PAVING;
	}
	else if( _stricmp( arg.c_str(), ".PLUMBING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_PLUMBING;
	}
	else if( _stricmp( arg.c_str(), ".ROOFING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_ROOFING;
	}
	else if( _stricmp( arg.c_str(), ".SITEGRADING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_SITEGRADING;
	}
	else if( _stricmp( arg.c_str(), ".STEELWORK." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_STEELWORK;
	}
	else if( _stricmp( arg.c_str(), ".SURVEYING." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_SURVEYING;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
