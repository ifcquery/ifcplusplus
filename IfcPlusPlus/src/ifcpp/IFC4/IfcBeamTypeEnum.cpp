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
#include "include/IfcBeamTypeEnum.h"

// TYPE IfcBeamTypeEnum 
IfcBeamTypeEnum::IfcBeamTypeEnum() {}
IfcBeamTypeEnum::~IfcBeamTypeEnum() {}
void IfcBeamTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBEAMTYPEENUM("; }
	if( m_enum == ENUM_BEAM )
	{
		stream << ".BEAM.";
	}
	else if( m_enum == ENUM_JOIST )
	{
		stream << ".JOIST.";
	}
	else if( m_enum == ENUM_HOLLOWCORE )
	{
		stream << ".HOLLOWCORE.";
	}
	else if( m_enum == ENUM_LINTEL )
	{
		stream << ".LINTEL.";
	}
	else if( m_enum == ENUM_SPANDREL )
	{
		stream << ".SPANDREL.";
	}
	else if( m_enum == ENUM_T_BEAM )
	{
		stream << ".T_BEAM.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBeamTypeEnum> IfcBeamTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcBeamTypeEnum>(); }
	shared_ptr<IfcBeamTypeEnum> type_object( new IfcBeamTypeEnum() );
	if( _stricmp( arg.c_str(), ".BEAM." ) == 0 )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_BEAM;
	}
	else if( _stricmp( arg.c_str(), ".JOIST." ) == 0 )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_JOIST;
	}
	else if( _stricmp( arg.c_str(), ".HOLLOWCORE." ) == 0 )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_HOLLOWCORE;
	}
	else if( _stricmp( arg.c_str(), ".LINTEL." ) == 0 )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_LINTEL;
	}
	else if( _stricmp( arg.c_str(), ".SPANDREL." ) == 0 )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_SPANDREL;
	}
	else if( _stricmp( arg.c_str(), ".T_BEAM." ) == 0 )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_T_BEAM;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
