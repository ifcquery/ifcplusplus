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
#include "include/IfcObjectiveEnum.h"

// TYPE IfcObjectiveEnum 
IfcObjectiveEnum::IfcObjectiveEnum() {}
IfcObjectiveEnum::~IfcObjectiveEnum() {}
void IfcObjectiveEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOBJECTIVEENUM("; }
	if( m_enum == ENUM_CODECOMPLIANCE )
	{
		stream << ".CODECOMPLIANCE.";
	}
	else if( m_enum == ENUM_CODEWAIVER )
	{
		stream << ".CODEWAIVER.";
	}
	else if( m_enum == ENUM_DESIGNINTENT )
	{
		stream << ".DESIGNINTENT.";
	}
	else if( m_enum == ENUM_EXTERNAL )
	{
		stream << ".EXTERNAL.";
	}
	else if( m_enum == ENUM_HEALTHANDSAFETY )
	{
		stream << ".HEALTHANDSAFETY.";
	}
	else if( m_enum == ENUM_MERGECONFLICT )
	{
		stream << ".MERGECONFLICT.";
	}
	else if( m_enum == ENUM_MODELVIEW )
	{
		stream << ".MODELVIEW.";
	}
	else if( m_enum == ENUM_PARAMETER )
	{
		stream << ".PARAMETER.";
	}
	else if( m_enum == ENUM_REQUIREMENT )
	{
		stream << ".REQUIREMENT.";
	}
	else if( m_enum == ENUM_SPECIFICATION )
	{
		stream << ".SPECIFICATION.";
	}
	else if( m_enum == ENUM_TRIGGERCONDITION )
	{
		stream << ".TRIGGERCONDITION.";
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
shared_ptr<IfcObjectiveEnum> IfcObjectiveEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcObjectiveEnum>(); }
	shared_ptr<IfcObjectiveEnum> type_object( new IfcObjectiveEnum() );
	if( _stricmp( arg.c_str(), ".CODECOMPLIANCE." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_CODECOMPLIANCE;
	}
	else if( _stricmp( arg.c_str(), ".CODEWAIVER." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_CODEWAIVER;
	}
	else if( _stricmp( arg.c_str(), ".DESIGNINTENT." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_DESIGNINTENT;
	}
	else if( _stricmp( arg.c_str(), ".EXTERNAL." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_EXTERNAL;
	}
	else if( _stricmp( arg.c_str(), ".HEALTHANDSAFETY." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_HEALTHANDSAFETY;
	}
	else if( _stricmp( arg.c_str(), ".MERGECONFLICT." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_MERGECONFLICT;
	}
	else if( _stricmp( arg.c_str(), ".MODELVIEW." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_MODELVIEW;
	}
	else if( _stricmp( arg.c_str(), ".PARAMETER." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_PARAMETER;
	}
	else if( _stricmp( arg.c_str(), ".REQUIREMENT." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_REQUIREMENT;
	}
	else if( _stricmp( arg.c_str(), ".SPECIFICATION." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_SPECIFICATION;
	}
	else if( _stricmp( arg.c_str(), ".TRIGGERCONDITION." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_TRIGGERCONDITION;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
