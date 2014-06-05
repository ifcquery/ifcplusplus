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
#include "include/IfcBooleanOperator.h"

// TYPE IfcBooleanOperator 
IfcBooleanOperator::IfcBooleanOperator() {}
IfcBooleanOperator::~IfcBooleanOperator() {}
void IfcBooleanOperator::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBOOLEANOPERATOR("; }
	if( m_enum == ENUM_UNION )
	{
		stream << ".UNION.";
	}
	else if( m_enum == ENUM_INTERSECTION )
	{
		stream << ".INTERSECTION.";
	}
	else if( m_enum == ENUM_DIFFERENCE )
	{
		stream << ".DIFFERENCE.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBooleanOperator> IfcBooleanOperator::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcBooleanOperator>(); }
	shared_ptr<IfcBooleanOperator> type_object( new IfcBooleanOperator() );
	if( _stricmp( arg.c_str(), ".UNION." ) == 0 )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_UNION;
	}
	else if( _stricmp( arg.c_str(), ".INTERSECTION." ) == 0 )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_INTERSECTION;
	}
	else if( _stricmp( arg.c_str(), ".DIFFERENCE." ) == 0 )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_DIFFERENCE;
	}
	return type_object;
}
