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
#include "include/IfcModulusOfRotationalSubgradeReactionSelect.h"
#include "include/IfcModulusOfSubgradeReactionSelect.h"
#include "include/IfcModulusOfTranslationalSubgradeReactionSelect.h"
#include "include/IfcRotationalStiffnessSelect.h"
#include "include/IfcSimpleValue.h"
#include "include/IfcTranslationalStiffnessSelect.h"
#include "include/IfcWarpingStiffnessSelect.h"
#include "include/IfcBoolean.h"

// TYPE IfcBoolean = BOOLEAN;
IfcBoolean::IfcBoolean() {}
IfcBoolean::IfcBoolean( bool value ) { m_value = value; }
IfcBoolean::~IfcBoolean() {}
shared_ptr<IfcPPObject> IfcBoolean::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBoolean> copy_self( new IfcBoolean() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcBoolean::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBOOLEAN("; }
	if( m_value == false )
	{
		stream << ".F.";
	}
	else if( m_value == true )
	{
		stream << ".T.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcBoolean> IfcBoolean::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBoolean>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBoolean>(); }
	shared_ptr<IfcBoolean> type_object( new IfcBoolean() );
	readBool( arg, type_object->m_value );
	return type_object;
}
