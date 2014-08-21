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
#include "include/IfcGridTypeEnum.h"

// TYPE IfcGridTypeEnum = ENUMERATION OF	(RECTANGULAR	,RADIAL	,TRIANGULAR	,IRREGULAR	,USERDEFINED	,NOTDEFINED);
IfcGridTypeEnum::IfcGridTypeEnum() {}
IfcGridTypeEnum::~IfcGridTypeEnum() {}
shared_ptr<IfcPPObject> IfcGridTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcGridTypeEnum> copy_self( new IfcGridTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcGridTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCGRIDTYPEENUM("; }
	if( m_enum == ENUM_RECTANGULAR )
	{
		stream << ".RECTANGULAR.";
	}
	else if( m_enum == ENUM_RADIAL )
	{
		stream << ".RADIAL.";
	}
	else if( m_enum == ENUM_TRIANGULAR )
	{
		stream << ".TRIANGULAR.";
	}
	else if( m_enum == ENUM_IRREGULAR )
	{
		stream << ".IRREGULAR.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcGridTypeEnum> IfcGridTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcGridTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcGridTypeEnum>(); }
	shared_ptr<IfcGridTypeEnum> type_object( new IfcGridTypeEnum() );
	if( boost::iequals( arg, L".RECTANGULAR." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_RECTANGULAR;
	}
	else if( boost::iequals( arg, L".RADIAL." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_RADIAL;
	}
	else if( boost::iequals( arg, L".TRIANGULAR." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_TRIANGULAR;
	}
	else if( boost::iequals( arg, L".IRREGULAR." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_IRREGULAR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
