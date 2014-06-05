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
#include "include/IfcProjectedOrTrueLengthEnum.h"

// TYPE IfcProjectedOrTrueLengthEnum 
IfcProjectedOrTrueLengthEnum::IfcProjectedOrTrueLengthEnum() {}
IfcProjectedOrTrueLengthEnum::~IfcProjectedOrTrueLengthEnum() {}
void IfcProjectedOrTrueLengthEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROJECTEDORTRUELENGTHENUM("; }
	if( m_enum == ENUM_PROJECTED_LENGTH )
	{
		stream << ".PROJECTED_LENGTH.";
	}
	else if( m_enum == ENUM_TRUE_LENGTH )
	{
		stream << ".TRUE_LENGTH.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcProjectedOrTrueLengthEnum> IfcProjectedOrTrueLengthEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcProjectedOrTrueLengthEnum>(); }
	shared_ptr<IfcProjectedOrTrueLengthEnum> type_object( new IfcProjectedOrTrueLengthEnum() );
	if( _stricmp( arg.c_str(), ".PROJECTED_LENGTH." ) == 0 )
	{
		type_object->m_enum = IfcProjectedOrTrueLengthEnum::ENUM_PROJECTED_LENGTH;
	}
	else if( _stricmp( arg.c_str(), ".TRUE_LENGTH." ) == 0 )
	{
		type_object->m_enum = IfcProjectedOrTrueLengthEnum::ENUM_TRUE_LENGTH;
	}
	return type_object;
}
