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
#include "include/IfcWindowPanelPositionEnum.h"

// TYPE IfcWindowPanelPositionEnum = ENUMERATION OF	(LEFT	,MIDDLE	,RIGHT	,BOTTOM	,TOP	,NOTDEFINED);
IfcWindowPanelPositionEnum::IfcWindowPanelPositionEnum() {}
IfcWindowPanelPositionEnum::~IfcWindowPanelPositionEnum() {}
shared_ptr<IfcPPObject> IfcWindowPanelPositionEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWindowPanelPositionEnum> copy_self( new IfcWindowPanelPositionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWindowPanelPositionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWINDOWPANELPOSITIONENUM("; }
	if( m_enum == ENUM_LEFT )
	{
		stream << ".LEFT.";
	}
	else if( m_enum == ENUM_MIDDLE )
	{
		stream << ".MIDDLE.";
	}
	else if( m_enum == ENUM_RIGHT )
	{
		stream << ".RIGHT.";
	}
	else if( m_enum == ENUM_BOTTOM )
	{
		stream << ".BOTTOM.";
	}
	else if( m_enum == ENUM_TOP )
	{
		stream << ".TOP.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcWindowPanelPositionEnum> IfcWindowPanelPositionEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWindowPanelPositionEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWindowPanelPositionEnum>(); }
	shared_ptr<IfcWindowPanelPositionEnum> type_object( new IfcWindowPanelPositionEnum() );
	if( boost::iequals( arg, L".LEFT." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_LEFT;
	}
	else if( boost::iequals( arg, L".MIDDLE." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_MIDDLE;
	}
	else if( boost::iequals( arg, L".RIGHT." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_RIGHT;
	}
	else if( boost::iequals( arg, L".BOTTOM." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_BOTTOM;
	}
	else if( boost::iequals( arg, L".TOP." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_TOP;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWindowPanelPositionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
