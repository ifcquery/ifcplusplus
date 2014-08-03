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
#include "include/IfcDerivedMeasureValue.h"
#include "include/IfcCompoundPlaneAngleMeasure.h"

// TYPE IfcCompoundPlaneAngleMeasure 
IfcCompoundPlaneAngleMeasure::IfcCompoundPlaneAngleMeasure() {}
IfcCompoundPlaneAngleMeasure::~IfcCompoundPlaneAngleMeasure() {}
shared_ptr<IfcPPObject> IfcCompoundPlaneAngleMeasure::getDeepCopy()
{
	shared_ptr<IfcCompoundPlaneAngleMeasure> copy_self( new IfcCompoundPlaneAngleMeasure() );
	for( size_t ii=0; ii<m_vec.size(); ++ii )
	{
		int item_ii = m_vec[ii];
		copy_self->m_vec.push_back( item_ii );
	}
	return copy_self;
}
void IfcCompoundPlaneAngleMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOMPOUNDPLANEANGLEMEASURE("; }
	writeIntList( stream, m_vec );
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcCompoundPlaneAngleMeasure> IfcCompoundPlaneAngleMeasure::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCompoundPlaneAngleMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCompoundPlaneAngleMeasure>(); }
	shared_ptr<IfcCompoundPlaneAngleMeasure> type_object( new IfcCompoundPlaneAngleMeasure() );
	readIntList(  arg, type_object->m_vec );
	return type_object;
}
