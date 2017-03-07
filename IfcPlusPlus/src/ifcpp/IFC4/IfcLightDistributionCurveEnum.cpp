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
#include "include/IfcLightDistributionCurveEnum.h"

// TYPE IfcLightDistributionCurveEnum = ENUMERATION OF	(TYPE_A	,TYPE_B	,TYPE_C	,NOTDEFINED);
IfcLightDistributionCurveEnum::IfcLightDistributionCurveEnum() {}
IfcLightDistributionCurveEnum::~IfcLightDistributionCurveEnum() {}
shared_ptr<IfcPPObject> IfcLightDistributionCurveEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLightDistributionCurveEnum> copy_self( new IfcLightDistributionCurveEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcLightDistributionCurveEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLIGHTDISTRIBUTIONCURVEENUM("; }
	if( m_enum == ENUM_TYPE_A )
	{
		stream << ".TYPE_A.";
	}
	else if( m_enum == ENUM_TYPE_B )
	{
		stream << ".TYPE_B.";
	}
	else if( m_enum == ENUM_TYPE_C )
	{
		stream << ".TYPE_C.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcLightDistributionCurveEnum> IfcLightDistributionCurveEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcLightDistributionCurveEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcLightDistributionCurveEnum>(); }
	shared_ptr<IfcLightDistributionCurveEnum> type_object( new IfcLightDistributionCurveEnum() );
	if( boost::iequals( arg, L".TYPE_A." ) )
	{
		type_object->m_enum = IfcLightDistributionCurveEnum::ENUM_TYPE_A;
	}
	else if( boost::iequals( arg, L".TYPE_B." ) )
	{
		type_object->m_enum = IfcLightDistributionCurveEnum::ENUM_TYPE_B;
	}
	else if( boost::iequals( arg, L".TYPE_C." ) )
	{
		type_object->m_enum = IfcLightDistributionCurveEnum::ENUM_TYPE_C;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcLightDistributionCurveEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
