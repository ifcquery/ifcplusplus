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
#include "include/IfcDoorPanelOperationEnum.h"

// TYPE IfcDoorPanelOperationEnum = ENUMERATION OF	(SWINGING	,DOUBLE_ACTING	,SLIDING	,FOLDING	,REVOLVING	,ROLLINGUP	,FIXEDPANEL	,USERDEFINED	,NOTDEFINED);
IfcDoorPanelOperationEnum::IfcDoorPanelOperationEnum() {}
IfcDoorPanelOperationEnum::~IfcDoorPanelOperationEnum() {}
shared_ptr<IfcPPObject> IfcDoorPanelOperationEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDoorPanelOperationEnum> copy_self( new IfcDoorPanelOperationEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDoorPanelOperationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDOORPANELOPERATIONENUM("; }
	if( m_enum == ENUM_SWINGING )
	{
		stream << ".SWINGING.";
	}
	else if( m_enum == ENUM_DOUBLE_ACTING )
	{
		stream << ".DOUBLE_ACTING.";
	}
	else if( m_enum == ENUM_SLIDING )
	{
		stream << ".SLIDING.";
	}
	else if( m_enum == ENUM_FOLDING )
	{
		stream << ".FOLDING.";
	}
	else if( m_enum == ENUM_REVOLVING )
	{
		stream << ".REVOLVING.";
	}
	else if( m_enum == ENUM_ROLLINGUP )
	{
		stream << ".ROLLINGUP.";
	}
	else if( m_enum == ENUM_FIXEDPANEL )
	{
		stream << ".FIXEDPANEL.";
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
shared_ptr<IfcDoorPanelOperationEnum> IfcDoorPanelOperationEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDoorPanelOperationEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDoorPanelOperationEnum>(); }
	shared_ptr<IfcDoorPanelOperationEnum> type_object( new IfcDoorPanelOperationEnum() );
	if( boost::iequals( arg, L".SWINGING." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_SWINGING;
	}
	else if( boost::iequals( arg, L".DOUBLE_ACTING." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_DOUBLE_ACTING;
	}
	else if( boost::iequals( arg, L".SLIDING." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_SLIDING;
	}
	else if( boost::iequals( arg, L".FOLDING." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_FOLDING;
	}
	else if( boost::iequals( arg, L".REVOLVING." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_REVOLVING;
	}
	else if( boost::iequals( arg, L".ROLLINGUP." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_ROLLINGUP;
	}
	else if( boost::iequals( arg, L".FIXEDPANEL." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_FIXEDPANEL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDoorPanelOperationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
