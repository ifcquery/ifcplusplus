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
#include "include/IfcSectionTypeEnum.h"

// TYPE IfcSectionTypeEnum 
IfcSectionTypeEnum::IfcSectionTypeEnum() {}
IfcSectionTypeEnum::~IfcSectionTypeEnum() {}
shared_ptr<IfcPPObject> IfcSectionTypeEnum::getDeepCopy()
{
	shared_ptr<IfcSectionTypeEnum> copy_self( new IfcSectionTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSectionTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSECTIONTYPEENUM("; }
	if( m_enum == ENUM_UNIFORM )
	{
		stream << ".UNIFORM.";
	}
	else if( m_enum == ENUM_TAPERED )
	{
		stream << ".TAPERED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSectionTypeEnum> IfcSectionTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSectionTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSectionTypeEnum>(); }
	shared_ptr<IfcSectionTypeEnum> type_object( new IfcSectionTypeEnum() );
	if( boost::iequals( arg, L".UNIFORM." ) )
	{
		type_object->m_enum = IfcSectionTypeEnum::ENUM_UNIFORM;
	}
	else if( boost::iequals( arg, L".TAPERED." ) )
	{
		type_object->m_enum = IfcSectionTypeEnum::ENUM_TAPERED;
	}
	return type_object;
}
