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
#include "include/IfcAnalysisModelTypeEnum.h"

// TYPE IfcAnalysisModelTypeEnum 
IfcAnalysisModelTypeEnum::IfcAnalysisModelTypeEnum() {}
IfcAnalysisModelTypeEnum::~IfcAnalysisModelTypeEnum() {}
void IfcAnalysisModelTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCANALYSISMODELTYPEENUM("; }
	if( m_enum == ENUM_IN_PLANE_LOADING_2D )
	{
		stream << ".IN_PLANE_LOADING_2D.";
	}
	else if( m_enum == ENUM_OUT_PLANE_LOADING_2D )
	{
		stream << ".OUT_PLANE_LOADING_2D.";
	}
	else if( m_enum == ENUM_LOADING_3D )
	{
		stream << ".LOADING_3D.";
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
shared_ptr<IfcAnalysisModelTypeEnum> IfcAnalysisModelTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcAnalysisModelTypeEnum>(); }
	shared_ptr<IfcAnalysisModelTypeEnum> type_object( new IfcAnalysisModelTypeEnum() );
	if( _stricmp( arg.c_str(), ".IN_PLANE_LOADING_2D." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisModelTypeEnum::ENUM_IN_PLANE_LOADING_2D;
	}
	else if( _stricmp( arg.c_str(), ".OUT_PLANE_LOADING_2D." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisModelTypeEnum::ENUM_OUT_PLANE_LOADING_2D;
	}
	else if( _stricmp( arg.c_str(), ".LOADING_3D." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisModelTypeEnum::ENUM_LOADING_3D;
	}
	else if( _stricmp( arg.c_str(), ".USERDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisModelTypeEnum::ENUM_USERDEFINED;
	}
	else if( _stricmp( arg.c_str(), ".NOTDEFINED." ) == 0 )
	{
		type_object->m_enum = IfcAnalysisModelTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
