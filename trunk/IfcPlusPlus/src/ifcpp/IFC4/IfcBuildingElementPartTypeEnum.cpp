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
#include "include/IfcBuildingElementPartTypeEnum.h"

// TYPE IfcBuildingElementPartTypeEnum = ENUMERATION OF	(INSULATION	,PRECASTPANEL	,USERDEFINED	,NOTDEFINED);
IfcBuildingElementPartTypeEnum::IfcBuildingElementPartTypeEnum() {}
IfcBuildingElementPartTypeEnum::~IfcBuildingElementPartTypeEnum() {}
shared_ptr<IfcPPObject> IfcBuildingElementPartTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBuildingElementPartTypeEnum> copy_self( new IfcBuildingElementPartTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBuildingElementPartTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBUILDINGELEMENTPARTTYPEENUM("; }
	if( m_enum == ENUM_INSULATION )
	{
		stream << ".INSULATION.";
	}
	else if( m_enum == ENUM_PRECASTPANEL )
	{
		stream << ".PRECASTPANEL.";
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
shared_ptr<IfcBuildingElementPartTypeEnum> IfcBuildingElementPartTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBuildingElementPartTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBuildingElementPartTypeEnum>(); }
	shared_ptr<IfcBuildingElementPartTypeEnum> type_object( new IfcBuildingElementPartTypeEnum() );
	if( boost::iequals( arg, L".INSULATION." ) )
	{
		type_object->m_enum = IfcBuildingElementPartTypeEnum::ENUM_INSULATION;
	}
	else if( boost::iequals( arg, L".PRECASTPANEL." ) )
	{
		type_object->m_enum = IfcBuildingElementPartTypeEnum::ENUM_PRECASTPANEL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcBuildingElementPartTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcBuildingElementPartTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
