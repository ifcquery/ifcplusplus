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
#include "include/IfcGeometricProjectionEnum.h"

// TYPE IfcGeometricProjectionEnum = ENUMERATION OF	(GRAPH_VIEW	,SKETCH_VIEW	,MODEL_VIEW	,PLAN_VIEW	,REFLECTED_PLAN_VIEW	,SECTION_VIEW	,ELEVATION_VIEW	,USERDEFINED	,NOTDEFINED);
IfcGeometricProjectionEnum::IfcGeometricProjectionEnum() {}
IfcGeometricProjectionEnum::~IfcGeometricProjectionEnum() {}
shared_ptr<IfcPPObject> IfcGeometricProjectionEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcGeometricProjectionEnum> copy_self( new IfcGeometricProjectionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcGeometricProjectionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCGEOMETRICPROJECTIONENUM("; }
	if( m_enum == ENUM_GRAPH_VIEW )
	{
		stream << ".GRAPH_VIEW.";
	}
	else if( m_enum == ENUM_SKETCH_VIEW )
	{
		stream << ".SKETCH_VIEW.";
	}
	else if( m_enum == ENUM_MODEL_VIEW )
	{
		stream << ".MODEL_VIEW.";
	}
	else if( m_enum == ENUM_PLAN_VIEW )
	{
		stream << ".PLAN_VIEW.";
	}
	else if( m_enum == ENUM_REFLECTED_PLAN_VIEW )
	{
		stream << ".REFLECTED_PLAN_VIEW.";
	}
	else if( m_enum == ENUM_SECTION_VIEW )
	{
		stream << ".SECTION_VIEW.";
	}
	else if( m_enum == ENUM_ELEVATION_VIEW )
	{
		stream << ".ELEVATION_VIEW.";
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
shared_ptr<IfcGeometricProjectionEnum> IfcGeometricProjectionEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcGeometricProjectionEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcGeometricProjectionEnum>(); }
	shared_ptr<IfcGeometricProjectionEnum> type_object( new IfcGeometricProjectionEnum() );
	if( boost::iequals( arg, L".GRAPH_VIEW." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_GRAPH_VIEW;
	}
	else if( boost::iequals( arg, L".SKETCH_VIEW." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_SKETCH_VIEW;
	}
	else if( boost::iequals( arg, L".MODEL_VIEW." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_MODEL_VIEW;
	}
	else if( boost::iequals( arg, L".PLAN_VIEW." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_PLAN_VIEW;
	}
	else if( boost::iequals( arg, L".REFLECTED_PLAN_VIEW." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_REFLECTED_PLAN_VIEW;
	}
	else if( boost::iequals( arg, L".SECTION_VIEW." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_SECTION_VIEW;
	}
	else if( boost::iequals( arg, L".ELEVATION_VIEW." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_ELEVATION_VIEW;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcGeometricProjectionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
