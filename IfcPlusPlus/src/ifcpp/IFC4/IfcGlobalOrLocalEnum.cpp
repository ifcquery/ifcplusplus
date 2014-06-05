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
#include "include/IfcGlobalOrLocalEnum.h"

// TYPE IfcGlobalOrLocalEnum 
IfcGlobalOrLocalEnum::IfcGlobalOrLocalEnum() {}
IfcGlobalOrLocalEnum::~IfcGlobalOrLocalEnum() {}
void IfcGlobalOrLocalEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCGLOBALORLOCALENUM("; }
	if( m_enum == ENUM_GLOBAL_COORDS )
	{
		stream << ".GLOBAL_COORDS.";
	}
	else if( m_enum == ENUM_LOCAL_COORDS )
	{
		stream << ".LOCAL_COORDS.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcGlobalOrLocalEnum> IfcGlobalOrLocalEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcGlobalOrLocalEnum>(); }
	shared_ptr<IfcGlobalOrLocalEnum> type_object( new IfcGlobalOrLocalEnum() );
	if( _stricmp( arg.c_str(), ".GLOBAL_COORDS." ) == 0 )
	{
		type_object->m_enum = IfcGlobalOrLocalEnum::ENUM_GLOBAL_COORDS;
	}
	else if( _stricmp( arg.c_str(), ".LOCAL_COORDS." ) == 0 )
	{
		type_object->m_enum = IfcGlobalOrLocalEnum::ENUM_LOCAL_COORDS;
	}
	return type_object;
}
