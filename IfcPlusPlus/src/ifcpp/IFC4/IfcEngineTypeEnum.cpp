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
#include "include/IfcEngineTypeEnum.h"

// TYPE IfcEngineTypeEnum = ENUMERATION OF	(EXTERNALCOMBUSTION	,INTERNALCOMBUSTION	,USERDEFINED	,NOTDEFINED);
IfcEngineTypeEnum::IfcEngineTypeEnum() {}
IfcEngineTypeEnum::~IfcEngineTypeEnum() {}
shared_ptr<IfcPPObject> IfcEngineTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcEngineTypeEnum> copy_self( new IfcEngineTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcEngineTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCENGINETYPEENUM("; }
	if( m_enum == ENUM_EXTERNALCOMBUSTION )
	{
		stream << ".EXTERNALCOMBUSTION.";
	}
	else if( m_enum == ENUM_INTERNALCOMBUSTION )
	{
		stream << ".INTERNALCOMBUSTION.";
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
shared_ptr<IfcEngineTypeEnum> IfcEngineTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcEngineTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcEngineTypeEnum>(); }
	shared_ptr<IfcEngineTypeEnum> type_object( new IfcEngineTypeEnum() );
	if( boost::iequals( arg, L".EXTERNALCOMBUSTION." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_EXTERNALCOMBUSTION;
	}
	else if( boost::iequals( arg, L".INTERNALCOMBUSTION." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_INTERNALCOMBUSTION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
