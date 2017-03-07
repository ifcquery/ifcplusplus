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
#include "include/IfcDirectionSenseEnum.h"

// TYPE IfcDirectionSenseEnum = ENUMERATION OF	(POSITIVE	,NEGATIVE);
IfcDirectionSenseEnum::IfcDirectionSenseEnum() {}
IfcDirectionSenseEnum::~IfcDirectionSenseEnum() {}
shared_ptr<IfcPPObject> IfcDirectionSenseEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDirectionSenseEnum> copy_self( new IfcDirectionSenseEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDirectionSenseEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDIRECTIONSENSEENUM("; }
	if( m_enum == ENUM_POSITIVE )
	{
		stream << ".POSITIVE.";
	}
	else if( m_enum == ENUM_NEGATIVE )
	{
		stream << ".NEGATIVE.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcDirectionSenseEnum> IfcDirectionSenseEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDirectionSenseEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDirectionSenseEnum>(); }
	shared_ptr<IfcDirectionSenseEnum> type_object( new IfcDirectionSenseEnum() );
	if( boost::iequals( arg, L".POSITIVE." ) )
	{
		type_object->m_enum = IfcDirectionSenseEnum::ENUM_POSITIVE;
	}
	else if( boost::iequals( arg, L".NEGATIVE." ) )
	{
		type_object->m_enum = IfcDirectionSenseEnum::ENUM_NEGATIVE;
	}
	return type_object;
}
