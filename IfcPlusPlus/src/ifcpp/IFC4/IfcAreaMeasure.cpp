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
#include "include/IfcMeasureValue.h"
#include "include/IfcAreaMeasure.h"

// TYPE IfcAreaMeasure = REAL;
IfcAreaMeasure::IfcAreaMeasure() {}
IfcAreaMeasure::IfcAreaMeasure( double value ) { m_value = value; }
IfcAreaMeasure::~IfcAreaMeasure() {}
shared_ptr<IfcPPObject> IfcAreaMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAreaMeasure> copy_self( new IfcAreaMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcAreaMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCAREAMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcAreaMeasure> IfcAreaMeasure::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcAreaMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcAreaMeasure>(); }
	shared_ptr<IfcAreaMeasure> type_object( new IfcAreaMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
