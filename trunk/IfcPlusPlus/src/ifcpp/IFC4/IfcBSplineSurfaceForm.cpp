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

// TYPE IfcBSplineSurfaceForm = ENUMERATION OF	(PLANE_SURF	,CYLINDRICAL_SURF	,CONICAL_SURF	,SPHERICAL_SURF	,TOROIDAL_SURF	,SURF_OF_REVOLUTION	,RULED_SURF	,GENERALISED_CONE	,QUADRIC_SURF	,SURF_OF_LINEAR_EXTRUSION	,UNSPECIFIED);
IfcBSplineSurfaceForm::IfcBSplineSurfaceForm() {}
IfcBSplineSurfaceForm::~IfcBSplineSurfaceForm() {}
shared_ptr<IfcPPObject> IfcBSplineSurfaceForm::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBSplineSurfaceForm> copy_self( new IfcBSplineSurfaceForm() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
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
shared_ptr<IfcBSplineSurfaceForm> IfcBSplineSurfaceForm::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBSplineSurfaceForm>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBSplineSurfaceForm>(); }
	shared_ptr<IfcBSplineSurfaceForm> type_object( new IfcBSplineSurfaceForm() );
	if( boost::iequals( arg, L".PLANE_SURF." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_PLANE_SURF;
	}
	else if( boost::iequals( arg, L".CYLINDRICAL_SURF." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_CYLINDRICAL_SURF;
	}
	else if( boost::iequals( arg, L".CONICAL_SURF." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_CONICAL_SURF;
	}
	else if( boost::iequals( arg, L".SPHERICAL_SURF." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_SPHERICAL_SURF;
	}
	else if( boost::iequals( arg, L".TOROIDAL_SURF." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_TOROIDAL_SURF;
	}
	else if( boost::iequals( arg, L".SURF_OF_REVOLUTION." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_SURF_OF_REVOLUTION;
	}
	else if( boost::iequals( arg, L".RULED_SURF." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_RULED_SURF;
	}
	else if( boost::iequals( arg, L".GENERALISED_CONE." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_GENERALISED_CONE;
	}
	else if( boost::iequals( arg, L".QUADRIC_SURF." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_QUADRIC_SURF;
	}
	else if( boost::iequals( arg, L".SURF_OF_LINEAR_EXTRUSION." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_SURF_OF_LINEAR_EXTRUSION;
	}
	else if( boost::iequals( arg, L".UNSPECIFIED." ) )
	{
		type_object->m_enum = IfcBSplineSurfaceForm::ENUM_UNSPECIFIED;
	}
	return type_object;
}
