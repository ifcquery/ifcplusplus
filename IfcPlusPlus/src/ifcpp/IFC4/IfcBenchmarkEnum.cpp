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
#include "include/IfcBenchmarkEnum.h"

// TYPE IfcBenchmarkEnum 
IfcBenchmarkEnum::IfcBenchmarkEnum() {}
IfcBenchmarkEnum::~IfcBenchmarkEnum() {}
void IfcBenchmarkEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBENCHMARKENUM("; }
	if( m_enum == ENUM_GREATERTHAN )
	{
		stream << ".GREATERTHAN.";
	}
	else if( m_enum == ENUM_GREATERTHANOREQUALTO )
	{
		stream << ".GREATERTHANOREQUALTO.";
	}
	else if( m_enum == ENUM_LESSTHAN )
	{
		stream << ".LESSTHAN.";
	}
	else if( m_enum == ENUM_LESSTHANOREQUALTO )
	{
		stream << ".LESSTHANOREQUALTO.";
	}
	else if( m_enum == ENUM_EQUALTO )
	{
		stream << ".EQUALTO.";
	}
	else if( m_enum == ENUM_NOTEQUALTO )
	{
		stream << ".NOTEQUALTO.";
	}
	else if( m_enum == ENUM_INCLUDES )
	{
		stream << ".INCLUDES.";
	}
	else if( m_enum == ENUM_NOTINCLUDES )
	{
		stream << ".NOTINCLUDES.";
	}
	else if( m_enum == ENUM_INCLUDEDIN )
	{
		stream << ".INCLUDEDIN.";
	}
	else if( m_enum == ENUM_NOTINCLUDEDIN )
	{
		stream << ".NOTINCLUDEDIN.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBenchmarkEnum> IfcBenchmarkEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcBenchmarkEnum>(); }
	shared_ptr<IfcBenchmarkEnum> type_object( new IfcBenchmarkEnum() );
	if( _stricmp( arg.c_str(), ".GREATERTHAN." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_GREATERTHAN;
	}
	else if( _stricmp( arg.c_str(), ".GREATERTHANOREQUALTO." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_GREATERTHANOREQUALTO;
	}
	else if( _stricmp( arg.c_str(), ".LESSTHAN." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_LESSTHAN;
	}
	else if( _stricmp( arg.c_str(), ".LESSTHANOREQUALTO." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_LESSTHANOREQUALTO;
	}
	else if( _stricmp( arg.c_str(), ".EQUALTO." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_EQUALTO;
	}
	else if( _stricmp( arg.c_str(), ".NOTEQUALTO." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_NOTEQUALTO;
	}
	else if( _stricmp( arg.c_str(), ".INCLUDES." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_INCLUDES;
	}
	else if( _stricmp( arg.c_str(), ".NOTINCLUDES." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_NOTINCLUDES;
	}
	else if( _stricmp( arg.c_str(), ".INCLUDEDIN." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_INCLUDEDIN;
	}
	else if( _stricmp( arg.c_str(), ".NOTINCLUDEDIN." ) == 0 )
	{
		type_object->m_enum = IfcBenchmarkEnum::ENUM_NOTINCLUDEDIN;
	}
	return type_object;
}
