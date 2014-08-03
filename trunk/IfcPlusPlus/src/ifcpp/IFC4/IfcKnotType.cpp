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
#include "include/IfcKnotType.h"

// TYPE IfcKnotType 
IfcKnotType::IfcKnotType() {}
IfcKnotType::~IfcKnotType() {}
shared_ptr<IfcPPObject> IfcKnotType::getDeepCopy()
{
	shared_ptr<IfcKnotType> copy_self( new IfcKnotType() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcKnotType::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCKNOTTYPE("; }
	if( m_enum == ENUM_UNIFORM_KNOTS )
	{
		stream << ".UNIFORM_KNOTS.";
	}
	else if( m_enum == ENUM_QUASI_UNIFORM_KNOTS )
	{
		stream << ".QUASI_UNIFORM_KNOTS.";
	}
	else if( m_enum == ENUM_PIECEWISE_BEZIER_KNOTS )
	{
		stream << ".PIECEWISE_BEZIER_KNOTS.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcKnotType> IfcKnotType::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcKnotType>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcKnotType>(); }
	shared_ptr<IfcKnotType> type_object( new IfcKnotType() );
	if( boost::iequals( arg, L".UNIFORM_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_UNIFORM_KNOTS;
	}
	else if( boost::iequals( arg, L".QUASI_UNIFORM_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_QUASI_UNIFORM_KNOTS;
	}
	else if( boost::iequals( arg, L".PIECEWISE_BEZIER_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_PIECEWISE_BEZIER_KNOTS;
	}
	else if( boost::iequals( arg, L".UNSPECIFIED." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_UNSPECIFIED;
	}
	return type_object;
}
