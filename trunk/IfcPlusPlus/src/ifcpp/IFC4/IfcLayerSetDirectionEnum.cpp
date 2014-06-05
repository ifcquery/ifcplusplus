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
#include "include/IfcLayerSetDirectionEnum.h"

// TYPE IfcLayerSetDirectionEnum 
IfcLayerSetDirectionEnum::IfcLayerSetDirectionEnum() {}
IfcLayerSetDirectionEnum::~IfcLayerSetDirectionEnum() {}
void IfcLayerSetDirectionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLAYERSETDIRECTIONENUM("; }
	if( m_enum == ENUM_AXIS1 )
	{
		stream << ".AXIS1.";
	}
	else if( m_enum == ENUM_AXIS2 )
	{
		stream << ".AXIS2.";
	}
	else if( m_enum == ENUM_AXIS3 )
	{
		stream << ".AXIS3.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcLayerSetDirectionEnum> IfcLayerSetDirectionEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcLayerSetDirectionEnum>(); }
	shared_ptr<IfcLayerSetDirectionEnum> type_object( new IfcLayerSetDirectionEnum() );
	if( _stricmp( arg.c_str(), ".AXIS1." ) == 0 )
	{
		type_object->m_enum = IfcLayerSetDirectionEnum::ENUM_AXIS1;
	}
	else if( _stricmp( arg.c_str(), ".AXIS2." ) == 0 )
	{
		type_object->m_enum = IfcLayerSetDirectionEnum::ENUM_AXIS2;
	}
	else if( _stricmp( arg.c_str(), ".AXIS3." ) == 0 )
	{
		type_object->m_enum = IfcLayerSetDirectionEnum::ENUM_AXIS3;
	}
	return type_object;
}
