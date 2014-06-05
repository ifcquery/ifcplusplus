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
#include "include/IfcBSplineCurveForm.h"

// TYPE IfcBSplineCurveForm 
IfcBSplineCurveForm::IfcBSplineCurveForm() {}
IfcBSplineCurveForm::~IfcBSplineCurveForm() {}
void IfcBSplineCurveForm::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBSPLINECURVEFORM("; }
	if( m_enum == ENUM_POLYLINE_FORM )
	{
		stream << ".POLYLINE_FORM.";
	}
	else if( m_enum == ENUM_CIRCULAR_ARC )
	{
		stream << ".CIRCULAR_ARC.";
	}
	else if( m_enum == ENUM_ELLIPTIC_ARC )
	{
		stream << ".ELLIPTIC_ARC.";
	}
	else if( m_enum == ENUM_PARABOLIC_ARC )
	{
		stream << ".PARABOLIC_ARC.";
	}
	else if( m_enum == ENUM_HYPERBOLIC_ARC )
	{
		stream << ".HYPERBOLIC_ARC.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBSplineCurveForm> IfcBSplineCurveForm::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcBSplineCurveForm>(); }
	shared_ptr<IfcBSplineCurveForm> type_object( new IfcBSplineCurveForm() );
	if( _stricmp( arg.c_str(), ".POLYLINE_FORM." ) == 0 )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_POLYLINE_FORM;
	}
	else if( _stricmp( arg.c_str(), ".CIRCULAR_ARC." ) == 0 )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_CIRCULAR_ARC;
	}
	else if( _stricmp( arg.c_str(), ".ELLIPTIC_ARC." ) == 0 )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_ELLIPTIC_ARC;
	}
	else if( _stricmp( arg.c_str(), ".PARABOLIC_ARC." ) == 0 )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_PARABOLIC_ARC;
	}
	else if( _stricmp( arg.c_str(), ".HYPERBOLIC_ARC." ) == 0 )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_HYPERBOLIC_ARC;
	}
	else if( _stricmp( arg.c_str(), ".UNSPECIFIED." ) == 0 )
	{
		type_object->m_enum = IfcBSplineCurveForm::ENUM_UNSPECIFIED;
	}
	return type_object;
}
