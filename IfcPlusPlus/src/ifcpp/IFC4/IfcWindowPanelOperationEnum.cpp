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
#include "include/IfcWindowPanelOperationEnum.h"

// TYPE IfcWindowPanelOperationEnum 
IfcWindowPanelOperationEnum::IfcWindowPanelOperationEnum() {}
IfcWindowPanelOperationEnum::~IfcWindowPanelOperationEnum() {}
shared_ptr<IfcPPObject> IfcWindowPanelOperationEnum::getDeepCopy()
{
	shared_ptr<IfcWindowPanelOperationEnum> copy_self( new IfcWindowPanelOperationEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWindowPanelOperationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWINDOWPANELOPERATIONENUM("; }
	if( m_enum == ENUM_SIDEHUNGRIGHTHAND )
	{
		stream << ".SIDEHUNGRIGHTHAND.";
	}
	else if( m_enum == ENUM_SIDEHUNGLEFTHAND )
	{
		stream << ".SIDEHUNGLEFTHAND.";
	}
	else if( m_enum == ENUM_TILTANDTURNRIGHTHAND )
	{
		stream << ".TILTANDTURNRIGHTHAND.";
	}
	else if( m_enum == ENUM_TILTANDTURNLEFTHAND )
	{
		stream << ".TILTANDTURNLEFTHAND.";
	}
	else if( m_enum == ENUM_TOPHUNG )
	{
		stream << ".TOPHUNG.";
	}
	else if( m_enum == ENUM_BOTTOMHUNG )
	{
		stream << ".BOTTOMHUNG.";
	}
	else if( m_enum == ENUM_PIVOTHORIZONTAL )
	{
		stream << ".PIVOTHORIZONTAL.";
	}
	else if( m_enum == ENUM_PIVOTVERTICAL )
	{
		stream << ".PIVOTVERTICAL.";
	}
	else if( m_enum == ENUM_SLIDINGHORIZONTAL )
	{
		stream << ".SLIDINGHORIZONTAL.";
	}
	else if( m_enum == ENUM_SLIDINGVERTICAL )
	{
		stream << ".SLIDINGVERTICAL.";
	}
	else if( m_enum == ENUM_REMOVABLECASEMENT )
	{
		stream << ".REMOVABLECASEMENT.";
	}
	else if( m_enum == ENUM_FIXEDCASEMENT )
	{
		stream << ".FIXEDCASEMENT.";
	}
	else if( m_enum == ENUM_OTHEROPERATION )
	{
		stream << ".OTHEROPERATION.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcWindowPanelOperationEnum> IfcWindowPanelOperationEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWindowPanelOperationEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWindowPanelOperationEnum>(); }
	shared_ptr<IfcWindowPanelOperationEnum> type_object( new IfcWindowPanelOperationEnum() );
	if( boost::iequals( arg, L".SIDEHUNGRIGHTHAND." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_SIDEHUNGRIGHTHAND;
	}
	else if( boost::iequals( arg, L".SIDEHUNGLEFTHAND." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_SIDEHUNGLEFTHAND;
	}
	else if( boost::iequals( arg, L".TILTANDTURNRIGHTHAND." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_TILTANDTURNRIGHTHAND;
	}
	else if( boost::iequals( arg, L".TILTANDTURNLEFTHAND." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_TILTANDTURNLEFTHAND;
	}
	else if( boost::iequals( arg, L".TOPHUNG." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_TOPHUNG;
	}
	else if( boost::iequals( arg, L".BOTTOMHUNG." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_BOTTOMHUNG;
	}
	else if( boost::iequals( arg, L".PIVOTHORIZONTAL." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_PIVOTHORIZONTAL;
	}
	else if( boost::iequals( arg, L".PIVOTVERTICAL." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_PIVOTVERTICAL;
	}
	else if( boost::iequals( arg, L".SLIDINGHORIZONTAL." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_SLIDINGHORIZONTAL;
	}
	else if( boost::iequals( arg, L".SLIDINGVERTICAL." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_SLIDINGVERTICAL;
	}
	else if( boost::iequals( arg, L".REMOVABLECASEMENT." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_REMOVABLECASEMENT;
	}
	else if( boost::iequals( arg, L".FIXEDCASEMENT." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_FIXEDCASEMENT;
	}
	else if( boost::iequals( arg, L".OTHEROPERATION." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_OTHEROPERATION;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWindowPanelOperationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
