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
