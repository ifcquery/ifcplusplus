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
#include "include/IfcSimplePropertyTemplateTypeEnum.h"

// TYPE IfcSimplePropertyTemplateTypeEnum = ENUMERATION OF	(P_SINGLEVALUE	,P_ENUMERATEDVALUE	,P_BOUNDEDVALUE	,P_LISTVALUE	,P_TABLEVALUE	,P_REFERENCEVALUE	,Q_LENGTH	,Q_AREA	,Q_VOLUME	,Q_COUNT	,Q_WEIGHT	,Q_TIME);
IfcSimplePropertyTemplateTypeEnum::IfcSimplePropertyTemplateTypeEnum() {}
IfcSimplePropertyTemplateTypeEnum::~IfcSimplePropertyTemplateTypeEnum() {}
shared_ptr<IfcPPObject> IfcSimplePropertyTemplateTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSimplePropertyTemplateTypeEnum> copy_self( new IfcSimplePropertyTemplateTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSimplePropertyTemplateTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSIMPLEPROPERTYTEMPLATETYPEENUM("; }
	if( m_enum == ENUM_P_SINGLEVALUE )
	{
		stream << ".P_SINGLEVALUE.";
	}
	else if( m_enum == ENUM_P_ENUMERATEDVALUE )
	{
		stream << ".P_ENUMERATEDVALUE.";
	}
	else if( m_enum == ENUM_P_BOUNDEDVALUE )
	{
		stream << ".P_BOUNDEDVALUE.";
	}
	else if( m_enum == ENUM_P_LISTVALUE )
	{
		stream << ".P_LISTVALUE.";
	}
	else if( m_enum == ENUM_P_TABLEVALUE )
	{
		stream << ".P_TABLEVALUE.";
	}
	else if( m_enum == ENUM_P_REFERENCEVALUE )
	{
		stream << ".P_REFERENCEVALUE.";
	}
	else if( m_enum == ENUM_Q_LENGTH )
	{
		stream << ".Q_LENGTH.";
	}
	else if( m_enum == ENUM_Q_AREA )
	{
		stream << ".Q_AREA.";
	}
	else if( m_enum == ENUM_Q_VOLUME )
	{
		stream << ".Q_VOLUME.";
	}
	else if( m_enum == ENUM_Q_COUNT )
	{
		stream << ".Q_COUNT.";
	}
	else if( m_enum == ENUM_Q_WEIGHT )
	{
		stream << ".Q_WEIGHT.";
	}
	else if( m_enum == ENUM_Q_TIME )
	{
		stream << ".Q_TIME.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSimplePropertyTemplateTypeEnum> IfcSimplePropertyTemplateTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSimplePropertyTemplateTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSimplePropertyTemplateTypeEnum>(); }
	shared_ptr<IfcSimplePropertyTemplateTypeEnum> type_object( new IfcSimplePropertyTemplateTypeEnum() );
	if( boost::iequals( arg, L".P_SINGLEVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_SINGLEVALUE;
	}
	else if( boost::iequals( arg, L".P_ENUMERATEDVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_ENUMERATEDVALUE;
	}
	else if( boost::iequals( arg, L".P_BOUNDEDVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_BOUNDEDVALUE;
	}
	else if( boost::iequals( arg, L".P_LISTVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_LISTVALUE;
	}
	else if( boost::iequals( arg, L".P_TABLEVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_TABLEVALUE;
	}
	else if( boost::iequals( arg, L".P_REFERENCEVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_REFERENCEVALUE;
	}
	else if( boost::iequals( arg, L".Q_LENGTH." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_LENGTH;
	}
	else if( boost::iequals( arg, L".Q_AREA." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_AREA;
	}
	else if( boost::iequals( arg, L".Q_VOLUME." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_VOLUME;
	}
	else if( boost::iequals( arg, L".Q_COUNT." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_COUNT;
	}
	else if( boost::iequals( arg, L".Q_WEIGHT." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_WEIGHT;
	}
	else if( boost::iequals( arg, L".Q_TIME." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_TIME;
	}
	return type_object;
}
