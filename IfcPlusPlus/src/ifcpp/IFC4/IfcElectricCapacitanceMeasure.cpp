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
#include "include/IfcElectricCapacitanceMeasure.h"

// TYPE IfcElectricCapacitanceMeasure 
IfcElectricCapacitanceMeasure::IfcElectricCapacitanceMeasure() {}
IfcElectricCapacitanceMeasure::IfcElectricCapacitanceMeasure( double value ) { m_value = value; }
IfcElectricCapacitanceMeasure::~IfcElectricCapacitanceMeasure() {}
void IfcElectricCapacitanceMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICCAPACITANCEMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcElectricCapacitanceMeasure> IfcElectricCapacitanceMeasure::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcElectricCapacitanceMeasure>(); }
	shared_ptr<IfcElectricCapacitanceMeasure> type_object( new IfcElectricCapacitanceMeasure() );
	type_object->readArgument( arg );
	return type_object;
}
