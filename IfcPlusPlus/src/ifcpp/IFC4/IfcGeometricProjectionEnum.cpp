/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
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
