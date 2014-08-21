/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcMeasureValue.h"
#include "include/IfcComplexNumber.h"

// TYPE IfcComplexNumber = ARRAY [1:2] OF REAL;
IfcComplexNumber::IfcComplexNumber() {}
IfcComplexNumber::~IfcComplexNumber() {}
shared_ptr<IfcPPObject> IfcComplexNumber::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcComplexNumber> copy_self( new IfcComplexNumber() );
	for( size_t ii=0; ii<m_vec.size(); ++ii )
	{
		double item_ii = m_vec[ii];
		copy_self->m_vec.push_back( item_ii );
	}
	return copy_self;
}
void IfcComplexNumber::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOMPLEXNUMBER("; }
	writeDoubleList( stream, m_vec );
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcComplexNumber> IfcComplexNumber::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcComplexNumber>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcComplexNumber>(); }
	shared_ptr<IfcComplexNumber> type_object( new IfcComplexNumber() );
	readRealList( arg, type_object->m_vec );
	return type_object;
}
