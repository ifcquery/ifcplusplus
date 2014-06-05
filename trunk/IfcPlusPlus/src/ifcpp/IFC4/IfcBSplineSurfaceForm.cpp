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
#include "include/IfcBSplineSurfaceForm.h"

// TYPE IfcBSplineSurfaceForm 
IfcBSplineSurfaceForm::IfcBSplineSurfaceForm() {}
IfcBSplineSurfaceForm::~IfcBSplineSurfaceForm() {}
void IfcBSplineSurfaceForm::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBSPLINESURFACEFORM("; }
	if( m_enum == ENUM_PLANE_SURF )
	{
		stream << ".PLANE_SURF.";
	}
	else if( m_enum == ENUM_CYLINDRICAL_SURF )
	{
		stream << ".CYLINDRICAL_SURF.";
	}
	else if( m_enum == ENUM_CONICAL_SURF )
	{
		stream << ".CONICAL_SURF.";
	}
	else if( m_enum == ENUM_SPHERICAL_SURF )
	{
		stream << ".SPHERICAL_SURF.";
	}
	else if( m_enum == ENUM_TOROIDAL_SURF )
	{
		stream << ".TOROIDAL_SURF.";
	}
	else if( m_enum == ENUM_SURF_OF_REVOLUTION )
	{
		stream << ".SURF_OF_REVOLUTION.";
	}
	else if( m_enum == ENUM_RULED_SURF )
	{
		stream << ".RULED_SURF.";
	}
	else if( m_enum == ENUM_GENERALISED_CONE )
	{
		stream << ".GENERALISED_CONE.";
	}
	else if( m_enum == ENUM_QUADRIC_SURF )
	{
		stream << ".QUADRIC_SURF.";
	}
	else if( m_enum == ENUM_SURF_OF_LINEAR_EXTRUSION )
	{
		stream << ".SURF_OF_LINEAR_EXTRUSION.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBSplineSurfaceForm> IfcBSplineSurfaceForm::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcBSplineSurfaceForm>(); }
	shared_ptr<IfcBSplineSurfaceForm> type_object( new IfcBSplineSurfaceForm() );
	if( _stricmp( arg.c_str(), ".PLANE_SURF." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_PLANE_SURF;
	}
	else if( _stricmp( arg.c_str(), ".CYLINDRICAL_SURF." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_CYLINDRICAL_SURF;
	}
	else if( _stricmp( arg.c_str(), ".CONICAL_SURF." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_CONICAL_SURF;
	}
	else if( _stricmp( arg.c_str(), ".SPHERICAL_SURF." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_SPHERICAL_SURF;
	}
	else if( _stricmp( arg.c_str(), ".TOROIDAL_SURF." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_TOROIDAL_SURF;
	}
	else if( _stricmp( arg.c_str(), ".SURF_OF_REVOLUTION." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_SURF_OF_REVOLUTION;
	}
	else if( _stricmp( arg.c_str(), ".RULED_SURF." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_RULED_SURF;
	}
	else if( _stricmp( arg.c_str(), ".GENERALISED_CONE." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_GENERALISED_CONE;
	}
	else if( _stricmp( arg.c_str(), ".QUADRIC_SURF." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_QUADRIC_SURF;
	}
	else if( _stricmp( arg.c_str(), ".SURF_OF_LINEAR_EXTRUSION." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_SURF_OF_LINEAR_EXTRUSION;
	}
	else if( _stricmp( arg.c_str(), ".UNSPECIFIED." ) == 0 )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_UNSPECIFIED;
	}
	return type_object;
}
