/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcConstructionMaterialResourceTypeEnum.h"

// TYPE IfcConstructionMaterialResourceTypeEnum = ENUMERATION OF	(AGGREGATES	,CONCRETE	,DRYWALL	,FUEL	,GYPSUM	,MASONRY	,METAL	,PLASTIC	,WOOD	,NOTDEFINED	,USERDEFINED);
IfcConstructionMaterialResourceTypeEnum::IfcConstructionMaterialResourceTypeEnum() {}
IfcConstructionMaterialResourceTypeEnum::~IfcConstructionMaterialResourceTypeEnum() {}
shared_ptr<IfcPPObject> IfcConstructionMaterialResourceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcConstructionMaterialResourceTypeEnum> copy_self( new IfcConstructionMaterialResourceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcConstructionMaterialResourceTypeEnum> IfcConstructionMaterialResourceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcConstructionMaterialResourceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcConstructionMaterialResourceTypeEnum>(); }
	shared_ptr<IfcConstructionMaterialResourceTypeEnum> type_object( new IfcConstructionMaterialResourceTypeEnum() );
	if( boost::iequals( arg, L".AGGREGATES." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_AGGREGATES;
	}
	else if( boost::iequals( arg, L".CONCRETE." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_CONCRETE;
	}
	else if( boost::iequals( arg, L".DRYWALL." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_DRYWALL;
	}
	else if( boost::iequals( arg, L".FUEL." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_FUEL;
	}
	else if( boost::iequals( arg, L".GYPSUM." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_GYPSUM;
	}
	else if( boost::iequals( arg, L".MASONRY." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_MASONRY;
	}
	else if( boost::iequals( arg, L".METAL." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_METAL;
	}
	else if( boost::iequals( arg, L".PLASTIC." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_PLASTIC;
	}
	else if( boost::iequals( arg, L".WOOD." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_WOOD;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_NOTDEFINED;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcConstructionMaterialResourceTypeEnum::ENUM_USERDEFINED;
	}
	return type_object;
}
