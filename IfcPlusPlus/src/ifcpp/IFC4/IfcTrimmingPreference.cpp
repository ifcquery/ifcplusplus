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
#include "include/IfcTrimmingPreference.h"

// TYPE IfcTrimmingPreference = ENUMERATION OF	(CARTESIAN	,PARAMETER	,UNSPECIFIED);
IfcTrimmingPreference::IfcTrimmingPreference() {}
IfcTrimmingPreference::~IfcTrimmingPreference() {}
shared_ptr<IfcPPObject> IfcTrimmingPreference::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTrimmingPreference> copy_self( new IfcTrimmingPreference() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTrimmingPreference::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRIMMINGPREFERENCE("; }
	if( m_enum == ENUM_CARTESIAN )
	{
		stream << ".CARTESIAN.";
	}
	else if( m_enum == ENUM_PARAMETER )
	{
		stream << ".PARAMETER.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcTrimmingPreference> IfcTrimmingPreference::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTrimmingPreference>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTrimmingPreference>(); }
	shared_ptr<IfcTrimmingPreference> type_object( new IfcTrimmingPreference() );
	if( boost::iequals( arg, L".CARTESIAN." ) )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_CARTESIAN;
	}
	else if( boost::iequals( arg, L".PARAMETER." ) )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_PARAMETER;
	}
	else if( boost::iequals( arg, L".UNSPECIFIED." ) )
	{
		type_object->m_enum = IfcTrimmingPreference::ENUM_UNSPECIFIED;
	}
	return type_object;
}
