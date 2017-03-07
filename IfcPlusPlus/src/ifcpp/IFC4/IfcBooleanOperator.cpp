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
#include "include/IfcBooleanOperator.h"

// TYPE IfcBooleanOperator = ENUMERATION OF	(UNION	,INTERSECTION	,DIFFERENCE);
IfcBooleanOperator::IfcBooleanOperator() {}
IfcBooleanOperator::~IfcBooleanOperator() {}
shared_ptr<IfcPPObject> IfcBooleanOperator::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBooleanOperator> copy_self( new IfcBooleanOperator() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBooleanOperator::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBOOLEANOPERATOR("; }
	if( m_enum == ENUM_UNION )
	{
		stream << ".UNION.";
	}
	else if( m_enum == ENUM_INTERSECTION )
	{
		stream << ".INTERSECTION.";
	}
	else if( m_enum == ENUM_DIFFERENCE )
	{
		stream << ".DIFFERENCE.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBooleanOperator> IfcBooleanOperator::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBooleanOperator>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBooleanOperator>(); }
	shared_ptr<IfcBooleanOperator> type_object( new IfcBooleanOperator() );
	if( boost::iequals( arg, L".UNION." ) )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_UNION;
	}
	else if( boost::iequals( arg, L".INTERSECTION." ) )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_INTERSECTION;
	}
	else if( boost::iequals( arg, L".DIFFERENCE." ) )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_DIFFERENCE;
	}
	return type_object;
}
