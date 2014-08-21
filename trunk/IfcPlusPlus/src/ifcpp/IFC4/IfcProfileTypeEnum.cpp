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
#include "include/IfcProfileTypeEnum.h"

// TYPE IfcProfileTypeEnum = ENUMERATION OF	(CURVE	,AREA);
IfcProfileTypeEnum::IfcProfileTypeEnum() {}
IfcProfileTypeEnum::~IfcProfileTypeEnum() {}
shared_ptr<IfcPPObject> IfcProfileTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcProfileTypeEnum> copy_self( new IfcProfileTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcProfileTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROFILETYPEENUM("; }
	if( m_enum == ENUM_CURVE )
	{
		stream << ".CURVE.";
	}
	else if( m_enum == ENUM_AREA )
	{
		stream << ".AREA.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcProfileTypeEnum> IfcProfileTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcProfileTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcProfileTypeEnum>(); }
	shared_ptr<IfcProfileTypeEnum> type_object( new IfcProfileTypeEnum() );
	if( boost::iequals( arg, L".CURVE." ) )
	{
		type_object->m_enum = IfcProfileTypeEnum::ENUM_CURVE;
	}
	else if( boost::iequals( arg, L".AREA." ) )
	{
		type_object->m_enum = IfcProfileTypeEnum::ENUM_AREA;
	}
	return type_object;
}
