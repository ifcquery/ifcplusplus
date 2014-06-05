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
#include "include/IfcLinearMomentMeasure.h"

// TYPE IfcLinearMomentMeasure 
IfcLinearMomentMeasure::IfcLinearMomentMeasure() {}
IfcLinearMomentMeasure::IfcLinearMomentMeasure( double value ) { m_value = value; }
IfcLinearMomentMeasure::~IfcLinearMomentMeasure() {}
void IfcLinearMomentMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLINEARMOMENTMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcLinearMomentMeasure> IfcLinearMomentMeasure::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcLinearMomentMeasure>(); }
	shared_ptr<IfcLinearMomentMeasure> type_object( new IfcLinearMomentMeasure() );
	type_object->readArgument( arg );
	return type_object;
}
