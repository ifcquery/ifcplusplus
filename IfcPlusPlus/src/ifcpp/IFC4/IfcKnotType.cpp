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
#include "include/IfcKnotType.h"

// TYPE IfcKnotType 
IfcKnotType::IfcKnotType() {}
IfcKnotType::~IfcKnotType() {}
void IfcKnotType::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCKNOTTYPE("; }
	if( m_enum == ENUM_UNIFORM_KNOTS )
	{
		stream << ".UNIFORM_KNOTS.";
	}
	else if( m_enum == ENUM_QUASI_UNIFORM_KNOTS )
	{
		stream << ".QUASI_UNIFORM_KNOTS.";
	}
	else if( m_enum == ENUM_PIECEWISE_BEZIER_KNOTS )
	{
		stream << ".PIECEWISE_BEZIER_KNOTS.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcKnotType> IfcKnotType::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcKnotType>(); }
	shared_ptr<IfcKnotType> type_object( new IfcKnotType() );
	if( _stricmp( arg.c_str(), ".UNIFORM_KNOTS." ) == 0 )
	{
		type_object->m_enum = IfcKnotType::ENUM_UNIFORM_KNOTS;
	}
	else if( _stricmp( arg.c_str(), ".QUASI_UNIFORM_KNOTS." ) == 0 )
	{
		type_object->m_enum = IfcKnotType::ENUM_QUASI_UNIFORM_KNOTS;
	}
	else if( _stricmp( arg.c_str(), ".PIECEWISE_BEZIER_KNOTS." ) == 0 )
	{
		type_object->m_enum = IfcKnotType::ENUM_PIECEWISE_BEZIER_KNOTS;
	}
	else if( _stricmp( arg.c_str(), ".UNSPECIFIED." ) == 0 )
	{
		type_object->m_enum = IfcKnotType::ENUM_UNSPECIFIED;
	}
	return type_object;
}
