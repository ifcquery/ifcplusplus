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
#include "include/IfcKnotType.h"

// TYPE IfcKnotType = ENUMERATION OF	(UNIFORM_KNOTS	,QUASI_UNIFORM_KNOTS	,PIECEWISE_BEZIER_KNOTS	,UNSPECIFIED);
IfcKnotType::IfcKnotType() {}
IfcKnotType::~IfcKnotType() {}
shared_ptr<IfcPPObject> IfcKnotType::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcKnotType> copy_self( new IfcKnotType() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcKnotType::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCKNOTTYPE("; }
	if( m_enum == ENUM_UNIFORM_KNOTS )
	{
		stream << ".UNIFORM_KNOTS.";
	}
	else if( m_enum == ENUM_QUASI_UNIFORM_KNOTS )
	{
		stream << ".QUASI_UNIFORM_KNOTS.";
	}
	else if( m_enum == ENUM_PIECEWISE_BEZIER_KNOTS )
	{
		stream << ".PIECEWISE_BEZIER_KNOTS.";
	}
	else if( m_enum == ENUM_UNSPECIFIED )
	{
		stream << ".UNSPECIFIED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcKnotType> IfcKnotType::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcKnotType>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcKnotType>(); }
	shared_ptr<IfcKnotType> type_object( new IfcKnotType() );
	if( boost::iequals( arg, L".UNIFORM_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_UNIFORM_KNOTS;
	}
	else if( boost::iequals( arg, L".QUASI_UNIFORM_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_QUASI_UNIFORM_KNOTS;
	}
	else if( boost::iequals( arg, L".PIECEWISE_BEZIER_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_PIECEWISE_BEZIER_KNOTS;
	}
	else if( boost::iequals( arg, L".UNSPECIFIED." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_UNSPECIFIED;
	}
	return type_object;
}
