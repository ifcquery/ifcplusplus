/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcWindowPanelOperationEnum.h"

// TYPE IfcWindowPanelOperationEnum = ENUMERATION OF	(SIDEHUNGRIGHTHAND	,SIDEHUNGLEFTHAND	,TILTANDTURNRIGHTHAND	,TILTANDTURNLEFTHAND	,TOPHUNG	,BOTTOMHUNG	,PIVOTHORIZONTAL	,PIVOTVERTICAL	,SLIDINGHORIZONTAL	,SLIDINGVERTICAL	,REMOVABLECASEMENT	,FIXEDCASEMENT	,OTHEROPERATION	,NOTDEFINED);
IfcWindowPanelOperationEnum::IfcWindowPanelOperationEnum() {}
IfcWindowPanelOperationEnum::~IfcWindowPanelOperationEnum() {}
shared_ptr<IfcPPObject> IfcWindowPanelOperationEnum::getDeepCopy( IfcPPCopyOptions& options )
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
shared_ptr<IfcWindowPanelOperationEnum> IfcWindowPanelOperationEnum::createObjectFromSTEP( const std::wstring& arg )
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
