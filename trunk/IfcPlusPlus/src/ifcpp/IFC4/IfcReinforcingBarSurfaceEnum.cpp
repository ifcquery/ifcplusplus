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
#include "include/IfcReinforcingBarSurfaceEnum.h"

// TYPE IfcReinforcingBarSurfaceEnum 
IfcReinforcingBarSurfaceEnum::IfcReinforcingBarSurfaceEnum() {}
IfcReinforcingBarSurfaceEnum::~IfcReinforcingBarSurfaceEnum() {}
shared_ptr<IfcPPObject> IfcReinforcingBarSurfaceEnum::getDeepCopy()
{
	shared_ptr<IfcReinforcingBarSurfaceEnum> copy_self( new IfcReinforcingBarSurfaceEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcReinforcingBarSurfaceEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREINFORCINGBARSURFACEENUM("; }
	if( m_enum == ENUM_PLAIN )
	{
		stream << ".PLAIN.";
	}
	else if( m_enum == ENUM_TEXTURED )
	{
		stream << ".TEXTURED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcReinforcingBarSurfaceEnum> IfcReinforcingBarSurfaceEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcReinforcingBarSurfaceEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcReinforcingBarSurfaceEnum>(); }
	shared_ptr<IfcReinforcingBarSurfaceEnum> type_object( new IfcReinforcingBarSurfaceEnum() );
	if( boost::iequals( arg, L".PLAIN." ) )
	{
		type_object->m_enum = IfcReinforcingBarSurfaceEnum::ENUM_PLAIN;
	}
	else if( boost::iequals( arg, L".TEXTURED." ) )
	{
		type_object->m_enum = IfcReinforcingBarSurfaceEnum::ENUM_TEXTURED;
	}
	return type_object;
}
