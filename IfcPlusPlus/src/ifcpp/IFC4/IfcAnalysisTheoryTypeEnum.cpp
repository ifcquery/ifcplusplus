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
#include "include/IfcAnalysisTheoryTypeEnum.h"

// TYPE IfcAnalysisTheoryTypeEnum 
IfcAnalysisTheoryTypeEnum::IfcAnalysisTheoryTypeEnum() {}
IfcAnalysisTheoryTypeEnum::~IfcAnalysisTheoryTypeEnum() {}
void IfcAnalysisTheoryTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCANALYSISTHEORYTYPEENUM("; }
	if( m_enum == ENUM_FIRST_ORDER_THEORY )
	{
		stream << ".FIRST_ORDER_THEORY.";
	}
	else if( m_enum == ENUM_SECOND_ORDER_THEORY )
	{
		stream << ".SECOND_ORDER_THEORY.";
	}
	else if( m_enum == ENUM_THIRD_ORDER_THEORY )
	{
		stream << ".THIRD_ORDER_THEORY.";
	}
	else if( m_enum == ENUM_FULL_NONLINEAR_THEORY )
	{
		stream << ".FULL_NONLINEAR_THEORY.";
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
shared_ptr<IfcAnalysisTheoryTypeEnum> IfcAnalysisTheoryTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcAnalysisTheoryTypeEnum>(); }
	shared_ptr<IfcAnalysisTheoryTypeEnum> type_object( new IfcAnalysisTheoryTypeEnum() );
	if( _stricmp( arg.c_str(), ".FIRST_ORDER_THEORY." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisTheoryTypeEnum::ENUM_FIRST_ORDER_THEORY;
	}
	else if( _stricmp( arg.c_str(), ".SECOND_ORDER_THEORY." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisTheoryTypeEnum::ENUM_SECOND_ORDER_THEORY;
	}
	else if( _stricmp( arg.c_str(), ".THIRD_ORDER_THEORY." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisTheoryTypeEnum::ENUM_THIRD_ORDER_THEORY;
	}
	else if( _stricmp( arg.c_str(), ".FULL_NONLINEAR_THEORY." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisTheoryTypeEnum::ENUM_FULL_NONLINEAR_THEORY;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisTheoryTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisTheoryTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
