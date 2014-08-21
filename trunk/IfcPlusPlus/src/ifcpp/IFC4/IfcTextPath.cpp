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
#include "include/IfcTextPath.h"

// TYPE IfcTextPath = ENUMERATION OF	(LEFT	,RIGHT	,UP	,DOWN);
IfcTextPath::IfcTextPath() {}
IfcTextPath::~IfcTextPath() {}
shared_ptr<IfcPPObject> IfcTextPath::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextPath> copy_self( new IfcTextPath() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTextPath::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTEXTPATH("; }
	if( m_enum == ENUM_LEFT )
	{
		stream << ".LEFT.";
	}
	else if( m_enum == ENUM_RIGHT )
	{
		stream << ".RIGHT.";
	}
	else if( m_enum == ENUM_UP )
	{
		stream << ".UP.";
	}
	else if( m_enum == ENUM_DOWN )
	{
		stream << ".DOWN.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcTextPath> IfcTextPath::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTextPath>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTextPath>(); }
	shared_ptr<IfcTextPath> type_object( new IfcTextPath() );
	if( boost::iequals( arg, L".LEFT." ) )
	{
		type_object->m_enum = IfcTextPath::ENUM_LEFT;
	}
	else if( boost::iequals( arg, L".RIGHT." ) )
	{
		type_object->m_enum = IfcTextPath::ENUM_RIGHT;
	}
	else if( boost::iequals( arg, L".UP." ) )
	{
		type_object->m_enum = IfcTextPath::ENUM_UP;
	}
	else if( boost::iequals( arg, L".DOWN." ) )
	{
		type_object->m_enum = IfcTextPath::ENUM_DOWN;
	}
	return type_object;
}
