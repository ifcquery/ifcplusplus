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
#include "include/IfcElementCompositionEnum.h"

// TYPE IfcElementCompositionEnum 
IfcElementCompositionEnum::IfcElementCompositionEnum() {}
IfcElementCompositionEnum::~IfcElementCompositionEnum() {}
void IfcElementCompositionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELEMENTCOMPOSITIONENUM("; }
	if( m_enum == ENUM_COMPLEX )
	{
		stream << ".COMPLEX.";
	}
	else if( m_enum == ENUM_ELEMENT )
	{
		stream << ".ELEMENT.";
	}
	else if( m_enum == ENUM_PARTIAL )
	{
		stream << ".PARTIAL.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcElementCompositionEnum> IfcElementCompositionEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcElementCompositionEnum>(); }
	shared_ptr<IfcElementCompositionEnum> type_object( new IfcElementCompositionEnum() );
	if( _stricmp( arg.c_str(), ".COMPLEX." ) == 0 )
	{
		type_object->m_enum = IfcElementCompositionEnum::ENUM_COMPLEX;
	}
	else if( _stricmp( arg.c_str(), ".ELEMENT." ) == 0 )
	{
		type_object->m_enum = IfcElementCompositionEnum::ENUM_ELEMENT;
	}
	else if( _stricmp( arg.c_str(), ".PARTIAL." ) == 0 )
	{
		type_object->m_enum = IfcElementCompositionEnum::ENUM_PARTIAL;
	}
	return type_object;
}
