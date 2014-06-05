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
#include "include/IfcStructuralCurveActivityTypeEnum.h"

// TYPE IfcStructuralCurveActivityTypeEnum 
IfcStructuralCurveActivityTypeEnum::IfcStructuralCurveActivityTypeEnum() {}
IfcStructuralCurveActivityTypeEnum::~IfcStructuralCurveActivityTypeEnum() {}
void IfcStructuralCurveActivityTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTRUCTURALCURVEACTIVITYTYPEENUM("; }
	if( m_enum == ENUM_CONST )
	{
		stream << ".CONST.";
	}
	else if( m_enum == ENUM_LINEAR )
	{
		stream << ".LINEAR.";
	}
	else if( m_enum == ENUM_POLYGONAL )
	{
		stream << ".POLYGONAL.";
	}
	else if( m_enum == ENUM_EQUIDISTANT )
	{
		stream << ".EQUIDISTANT.";
	}
	else if( m_enum == ENUM_SINUS )
	{
		stream << ".SINUS.";
	}
	else if( m_enum == ENUM_PARABOLA )
	{
		stream << ".PARABOLA.";
	}
	else if( m_enum == ENUM_DISCRETE )
	{
		stream << ".DISCRETE.";
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
shared_ptr<IfcStructuralCurveActivityTypeEnum> IfcStructuralCurveActivityTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcStructuralCurveActivityTypeEnum>(); }
	shared_ptr<IfcStructuralCurveActivityTypeEnum> type_object( new IfcStructuralCurveActivityTypeEnum() );
	if( _stricmp( arg.c_str(), ".CONST." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_CONST;
	}
	else if( _stricmp( arg.c_str(), ".LINEAR." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_LINEAR;
	}
	else if( _stricmp( arg.c_str(), ".POLYGONAL." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_POLYGONAL;
	}
	else if( _stricmp( arg.c_str(), ".EQUIDISTANT." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_EQUIDISTANT;
	}
	else if( _stricmp( arg.c_str(), ".SINUS." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_SINUS;
	}
	else if( _stricmp( arg.c_str(), ".PARABOLA." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_PARABOLA;
	}
	else if( _stricmp( arg.c_str(), ".DISCRETE." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_DISCRETE;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcStructuralCurveActivityTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
