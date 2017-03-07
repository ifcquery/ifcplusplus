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
#include "include/IfcReinforcingBarSurfaceEnum.h"

// TYPE IfcReinforcingBarSurfaceEnum = ENUMERATION OF	(PLAIN	,TEXTURED);
IfcReinforcingBarSurfaceEnum::IfcReinforcingBarSurfaceEnum() {}
IfcReinforcingBarSurfaceEnum::~IfcReinforcingBarSurfaceEnum() {}
shared_ptr<IfcPPObject> IfcReinforcingBarSurfaceEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReinforcingBarSurfaceEnum> copy_self( new IfcReinforcingBarSurfaceEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcReinforcingBarSurfaceEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREINFORCINGBARSURFACEENUM("; }
	if( m_enum == ENUM_PLAIN )
	{
		stream << ".PLAIN.";
	}
	else if( m_enum == ENUM_TEXTURED )
	{
		stream << ".TEXTURED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcReinforcingBarSurfaceEnum> IfcReinforcingBarSurfaceEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcReinforcingBarSurfaceEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcReinforcingBarSurfaceEnum>(); }
	shared_ptr<IfcReinforcingBarSurfaceEnum> type_object( new IfcReinforcingBarSurfaceEnum() );
	if( boost::iequals( arg, L".PLAIN." ) )
	{
		type_object->m_enum = IfcReinforcingBarSurfaceEnum::ENUM_PLAIN;
	}
	else if( boost::iequals( arg, L".TEXTURED." ) )
	{
		type_object->m_enum = IfcReinforcingBarSurfaceEnum::ENUM_TEXTURED;
	}
	return type_object;
}
