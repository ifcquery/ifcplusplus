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
#include "include/IfcTimeSeriesDataTypeEnum.h"

// TYPE IfcTimeSeriesDataTypeEnum = ENUMERATION OF	(CONTINUOUS	,DISCRETE	,DISCRETEBINARY	,PIECEWISEBINARY	,PIECEWISECONSTANT	,PIECEWISECONTINUOUS	,NOTDEFINED);
IfcTimeSeriesDataTypeEnum::IfcTimeSeriesDataTypeEnum() {}
IfcTimeSeriesDataTypeEnum::~IfcTimeSeriesDataTypeEnum() {}
shared_ptr<IfcPPObject> IfcTimeSeriesDataTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTimeSeriesDataTypeEnum> copy_self( new IfcTimeSeriesDataTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTimeSeriesDataTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTIMESERIESDATATYPEENUM("; }
	if( m_enum == ENUM_CONTINUOUS )
	{
		stream << ".CONTINUOUS.";
	}
	else if( m_enum == ENUM_DISCRETE )
	{
		stream << ".DISCRETE.";
	}
	else if( m_enum == ENUM_DISCRETEBINARY )
	{
		stream << ".DISCRETEBINARY.";
	}
	else if( m_enum == ENUM_PIECEWISEBINARY )
	{
		stream << ".PIECEWISEBINARY.";
	}
	else if( m_enum == ENUM_PIECEWISECONSTANT )
	{
		stream << ".PIECEWISECONSTANT.";
	}
	else if( m_enum == ENUM_PIECEWISECONTINUOUS )
	{
		stream << ".PIECEWISECONTINUOUS.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcTimeSeriesDataTypeEnum> IfcTimeSeriesDataTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTimeSeriesDataTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTimeSeriesDataTypeEnum>(); }
	shared_ptr<IfcTimeSeriesDataTypeEnum> type_object( new IfcTimeSeriesDataTypeEnum() );
	if( boost::iequals( arg, L".CONTINUOUS." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_CONTINUOUS;
	}
	else if( boost::iequals( arg, L".DISCRETE." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_DISCRETE;
	}
	else if( boost::iequals( arg, L".DISCRETEBINARY." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_DISCRETEBINARY;
	}
	else if( boost::iequals( arg, L".PIECEWISEBINARY." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_PIECEWISEBINARY;
	}
	else if( boost::iequals( arg, L".PIECEWISECONSTANT." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_PIECEWISECONSTANT;
	}
	else if( boost::iequals( arg, L".PIECEWISECONTINUOUS." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_PIECEWISECONTINUOUS;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
