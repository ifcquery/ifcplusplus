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
