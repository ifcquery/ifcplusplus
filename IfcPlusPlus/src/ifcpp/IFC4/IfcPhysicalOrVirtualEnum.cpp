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
#include "include/IfcPhysicalOrVirtualEnum.h"

// TYPE IfcPhysicalOrVirtualEnum = ENUMERATION OF	(PHYSICAL	,VIRTUAL	,NOTDEFINED);
IfcPhysicalOrVirtualEnum::IfcPhysicalOrVirtualEnum() {}
IfcPhysicalOrVirtualEnum::~IfcPhysicalOrVirtualEnum() {}
shared_ptr<IfcPPObject> IfcPhysicalOrVirtualEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPhysicalOrVirtualEnum> copy_self( new IfcPhysicalOrVirtualEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPhysicalOrVirtualEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPHYSICALORVIRTUALENUM("; }
	if( m_enum == ENUM_PHYSICAL )
	{
		stream << ".PHYSICAL.";
	}
	else if( m_enum == ENUM_VIRTUAL )
	{
		stream << ".VIRTUAL.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcPhysicalOrVirtualEnum> IfcPhysicalOrVirtualEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPhysicalOrVirtualEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPhysicalOrVirtualEnum>(); }
	shared_ptr<IfcPhysicalOrVirtualEnum> type_object( new IfcPhysicalOrVirtualEnum() );
	if( boost::iequals( arg, L".PHYSICAL." ) )
	{
		type_object->m_enum = IfcPhysicalOrVirtualEnum::ENUM_PHYSICAL;
	}
	else if( boost::iequals( arg, L".VIRTUAL." ) )
	{
		type_object->m_enum = IfcPhysicalOrVirtualEnum::ENUM_VIRTUAL;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPhysicalOrVirtualEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
