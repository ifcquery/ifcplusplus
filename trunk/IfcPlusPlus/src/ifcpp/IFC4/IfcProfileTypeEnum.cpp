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
#include "include/IfcProfileTypeEnum.h"

// TYPE IfcProfileTypeEnum 
IfcProfileTypeEnum::IfcProfileTypeEnum() {}
IfcProfileTypeEnum::~IfcProfileTypeEnum() {}
void IfcProfileTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROFILETYPEENUM("; }
	if( m_enum == ENUM_CURVE )
	{
		stream << ".CURVE.";
	}
	else if( m_enum == ENUM_AREA )
	{
		stream << ".AREA.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcProfileTypeEnum> IfcProfileTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcProfileTypeEnum>(); }
	shared_ptr<IfcProfileTypeEnum> type_object( new IfcProfileTypeEnum() );
	if( _stricmp( arg.c_str(), ".CURVE." ) == 0 )
	{
		type_object->m_enum = IfcProfileTypeEnum::ENUM_CURVE;
	}
	else if( _stricmp( arg.c_str(), ".AREA." ) == 0 )
	{
		type_object->m_enum = IfcProfileTypeEnum::ENUM_AREA;
	}
	return type_object;
}
