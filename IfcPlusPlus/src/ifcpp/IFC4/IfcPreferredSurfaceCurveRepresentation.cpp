/* -*-c++-*- IfcPlusPlus - www.ifcquery.com - Copyright (C) 2011 Fabian Gerold
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
#include "include/IfcPreferredSurfaceCurveRepresentation.h"

// TYPE IfcPreferredSurfaceCurveRepresentation = ENUMERATION OF	(CURVE3D	,PCURVE_S1	,PCURVE_S2);
IfcPreferredSurfaceCurveRepresentation::IfcPreferredSurfaceCurveRepresentation() {}
IfcPreferredSurfaceCurveRepresentation::~IfcPreferredSurfaceCurveRepresentation() {}
shared_ptr<IfcPPObject> IfcPreferredSurfaceCurveRepresentation::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPreferredSurfaceCurveRepresentation> copy_self( new IfcPreferredSurfaceCurveRepresentation() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPreferredSurfaceCurveRepresentation::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPREFERREDSURFACECURVEREPRESENTATION("; }
	if( m_enum == ENUM_CURVE3D )
	{
		stream << ".CURVE3D.";
	}
	else if( m_enum == ENUM_PCURVE_S1 )
	{
		stream << ".PCURVE_S1.";
	}
	else if( m_enum == ENUM_PCURVE_S2 )
	{
		stream << ".PCURVE_S2.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcPreferredSurfaceCurveRepresentation> IfcPreferredSurfaceCurveRepresentation::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPreferredSurfaceCurveRepresentation>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPreferredSurfaceCurveRepresentation>(); }
	shared_ptr<IfcPreferredSurfaceCurveRepresentation> type_object( new IfcPreferredSurfaceCurveRepresentation() );
	if( boost::iequals( arg, L".CURVE3D." ) )
	{
		type_object->m_enum = IfcPreferredSurfaceCurveRepresentation::ENUM_CURVE3D;
	}
	else if( boost::iequals( arg, L".PCURVE_S1." ) )
	{
		type_object->m_enum = IfcPreferredSurfaceCurveRepresentation::ENUM_PCURVE_S1;
	}
	else if( boost::iequals( arg, L".PCURVE_S2." ) )
	{
		type_object->m_enum = IfcPreferredSurfaceCurveRepresentation::ENUM_PCURVE_S2;
	}
	return type_object;
}
