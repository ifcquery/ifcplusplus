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
