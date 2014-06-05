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
#include "include/IfcTrimmingPreference.h"

// TYPE IfcTrimmingPreference 
IfcTrimmingPreference::IfcTrimmingPreference() {}
IfcTrimmingPreference::~IfcTrimmingPreference() {}
void IfcTrimmingPreference::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRIMMINGPREFERENCE("; }
	if( m_enum == ENUM_CARTESIAN )
	{
		stream << ".CARTESIAN.";
	}
	else if( m_enum == ENUM_PARAMETER )
	{
		stream << ".PARAMETER.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcTrimmingPreference> IfcTrimmingPreference::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcTrimmingPreference>(); }
	shared_ptr<IfcTrimmingPreference> type_object( new IfcTrimmingPreference() );
	if( _stricmp( arg.c_str(), ".CARTESIAN." ) == 0 )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_CARTESIAN;
	}
	else if( _stricmp( arg.c_str(), ".PARAMETER." ) == 0 )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_PARAMETER;
	}
	else if( _stricmp( arg.c_str(), ".UNSPECIFIED." ) == 0 )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_UNSPECIFIED;
	}
	return type_object;
}
