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
#include "include/IfcConstructionMaterialResourceTypeEnum.h"

// TYPE IfcConstructionMaterialResourceTypeEnum 
IfcConstructionMaterialResourceTypeEnum::IfcConstructionMaterialResourceTypeEnum() {}
IfcConstructionMaterialResourceTypeEnum::~IfcConstructionMaterialResourceTypeEnum() {}
void IfcConstructionMaterialResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONSTRUCTIONMATERIALRESOURCETYPEENUM("; }
	if( m_enum == ENUM_AGGREGATES )
	{
		stream << ".AGGREGATES.";
	}
	else if( m_enum == ENUM_CONCRETE )
	{
		stream << ".CONCRETE.";
	}
	else if( m_enum == ENUM_DRYWALL )
	{
		stream << ".DRYWALL.";
	}
	else if( m_enum == ENUM_FUEL )
	{
		stream << ".FUEL.";
	}
	else if( m_enum == ENUM_GYPSUM )
	{
		stream << ".GYPSUM.";
	}
	else if( m_enum == ENUM_MASONRY )
	{
		stream << ".MASONRY.";
	}
	else if( m_enum == ENUM_METAL )
	{
		stream << ".METAL.";
	}
	else if( m_enum == ENUM_PLASTIC )
	{
		stream << ".PLASTIC.";
	}
	else if( m_enum == ENUM_WOOD )
	{
		stream << ".WOOD.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcConstructionMaterialResourceTypeEnum> IfcConstructionMaterialResourceTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcConstructionMaterialResourceTypeEnum>(); }
	shared_ptr<IfcConstructionMaterialResourceTypeEnum> type_object( new IfcConstructionMaterialResourceTypeEnum() );
	if( _stricmp( arg.c_str(), ".AGGREGATES." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_AGGREGATES;
	}
	else if( _stricmp( arg.c_str(), ".CONCRETE." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_CONCRETE;
	}
	else if( _stricmp( arg.c_str(), ".DRYWALL." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_DRYWALL;
	}
	else if( _stricmp( arg.c_str(), ".FUEL." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_FUEL;
	}
	else if( _stricmp( arg.c_str(), ".GYPSUM." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_GYPSUM;
	}
	else if( _stricmp( arg.c_str(), ".MASONRY." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_MASONRY;
	}
	else if( _stricmp( arg.c_str(), ".METAL." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_METAL;
	}
	else if( _stricmp( arg.c_str(), ".PLASTIC." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_PLASTIC;
	}
	else if( _stricmp( arg.c_str(), ".WOOD." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_WOOD;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_NOTDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_USERDEFINED;
	}
	return type_object;
}
