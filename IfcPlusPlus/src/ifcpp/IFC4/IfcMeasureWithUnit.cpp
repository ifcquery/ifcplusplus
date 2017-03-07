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

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcMeasureWithUnit.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcMeasureWithUnit 
IfcMeasureWithUnit::IfcMeasureWithUnit() { m_entity_enum = IFCMEASUREWITHUNIT; }
IfcMeasureWithUnit::IfcMeasureWithUnit( int id ) { m_id = id; m_entity_enum = IFCMEASUREWITHUNIT; }
IfcMeasureWithUnit::~IfcMeasureWithUnit() {}
shared_ptr<IfcPPObject> IfcMeasureWithUnit::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMeasureWithUnit> copy_self( new IfcMeasureWithUnit() );
	if( m_ValueComponent ) { copy_self->m_ValueComponent = dynamic_pointer_cast<IfcValue>( m_ValueComponent->getDeepCopy(options) ); }
	if( m_UnitComponent ) { copy_self->m_UnitComponent = dynamic_pointer_cast<IfcUnit>( m_UnitComponent->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMeasureWithUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMEASUREWITHUNIT" << "(";
	if( m_ValueComponent ) { m_ValueComponent->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_UnitComponent ) { m_UnitComponent->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcMeasureWithUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMeasureWithUnit::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMeasureWithUnit, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ValueComponent = IfcValue::createObjectFromSTEP( args[0], map );
	m_UnitComponent = IfcUnit::createObjectFromSTEP( args[1], map );
}
void IfcMeasureWithUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "ValueComponent", m_ValueComponent ) );
	vec_attributes.push_back( std::make_pair( "UnitComponent", m_UnitComponent ) );
}
void IfcMeasureWithUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMeasureWithUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMeasureWithUnit::unlinkFromInverseCounterparts()
{
}
