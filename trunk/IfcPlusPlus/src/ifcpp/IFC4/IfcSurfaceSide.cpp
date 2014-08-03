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
#include "include/IfcSurfaceSide.h"

// TYPE IfcSurfaceSide 
IfcSurfaceSide::IfcSurfaceSide() {}
IfcSurfaceSide::~IfcSurfaceSide() {}
shared_ptr<IfcPPObject> IfcSurfaceSide::getDeepCopy()
{
	shared_ptr<IfcSurfaceSide> copy_self( new IfcSurfaceSide() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSurfaceSide::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSURFACESIDE("; }
	if( m_enum == ENUM_POSITIVE )
	{
		stream << ".POSITIVE.";
	}
	else if( m_enum == ENUM_NEGATIVE )
	{
		stream << ".NEGATIVE.";
	}
	else if( m_enum == ENUM_BOTH )
	{
		stream << ".BOTH.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSurfaceSide> IfcSurfaceSide::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSurfaceSide>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSurfaceSide>(); }
	shared_ptr<IfcSurfaceSide> type_object( new IfcSurfaceSide() );
	if( boost::iequals( arg, L".POSITIVE." ) )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_POSITIVE;
	}
	else if( boost::iequals( arg, L".NEGATIVE." ) )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_NEGATIVE;
	}
	else if( boost::iequals( arg, L".BOTH." ) )
	{
		type_object->m_enum = IfcSurfaceSide::ENUM_BOTH;
	}
	return type_object;
}
