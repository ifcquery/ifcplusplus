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
#include "include/IfcSurfaceSide.h"

// TYPE IfcSurfaceSide 
IfcSurfaceSide::IfcSurfaceSide() {}
IfcSurfaceSide::~IfcSurfaceSide() {}
void IfcSurfaceSide::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSURFACESIDE("; }
	if( m_enum == ENUM_POSITIVE )
	{
		stream << ".POSITIVE.";
	}
	else if( m_enum == ENUM_NEGATIVE )
	{
		stream << ".NEGATIVE.";
	}
	else if( m_enum == ENUM_BOTH )
	{
		stream << ".BOTH.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSurfaceSide> IfcSurfaceSide::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcSurfaceSide>(); }
	shared_ptr<IfcSurfaceSide> type_object( new IfcSurfaceSide() );
	if( _stricmp( arg.c_str(), ".POSITIVE." ) == 0 )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_POSITIVE;
	}
	else if( _stricmp( arg.c_str(), ".NEGATIVE." ) == 0 )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_NEGATIVE;
	}
	else if( _stricmp( arg.c_str(), ".BOTH." ) == 0 )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_BOTH;
	}
	return type_object;
}
