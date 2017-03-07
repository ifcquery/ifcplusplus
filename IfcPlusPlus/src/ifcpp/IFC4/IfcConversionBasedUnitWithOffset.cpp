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
#include "include/IfcConversionBasedUnitWithOffset.h"
#include "include/IfcDimensionalExponents.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcMeasureWithUnit.h"
#include "include/IfcReal.h"
#include "include/IfcUnitEnum.h"

// ENTITY IfcConversionBasedUnitWithOffset 
IfcConversionBasedUnitWithOffset::IfcConversionBasedUnitWithOffset() { m_entity_enum = IFCCONVERSIONBASEDUNITWITHOFFSET; }
IfcConversionBasedUnitWithOffset::IfcConversionBasedUnitWithOffset( int id ) { m_id = id; m_entity_enum = IFCCONVERSIONBASEDUNITWITHOFFSET; }
IfcConversionBasedUnitWithOffset::~IfcConversionBasedUnitWithOffset() {}
shared_ptr<IfcPPObject> IfcConversionBasedUnitWithOffset::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcConversionBasedUnitWithOffset> copy_self( new IfcConversionBasedUnitWithOffset() );
	if( m_Dimensions ) { copy_self->m_Dimensions = dynamic_pointer_cast<IfcDimensionalExponents>( m_Dimensions->getDeepCopy(options) ); }
	if( m_UnitType ) { copy_self->m_UnitType = dynamic_pointer_cast<IfcUnitEnum>( m_UnitType->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_ConversionFactor ) { copy_self->m_ConversionFactor = dynamic_pointer_cast<IfcMeasureWithUnit>( m_ConversionFactor->getDeepCopy(options) ); }
	if( m_ConversionOffset ) { copy_self->m_ConversionOffset = dynamic_pointer_cast<IfcReal>( m_ConversionOffset->getDeepCopy(options) ); }
	return copy_self;
}
void IfcConversionBasedUnitWithOffset::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCONVERSIONBASEDUNITWITHOFFSET" << "(";
	if( m_Dimensions ) { stream << "#" << m_Dimensions->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ConversionFactor ) { stream << "#" << m_ConversionFactor->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_ConversionOffset ) { m_ConversionOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcConversionBasedUnitWithOffset::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConversionBasedUnitWithOffset::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcConversionBasedUnitWithOffset, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Dimensions, map );
	m_UnitType = IfcUnitEnum::createObjectFromSTEP( args[1] );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	readEntityReference( args[3], m_ConversionFactor, map );
	m_ConversionOffset = IfcReal::createObjectFromSTEP( args[4] );
}
void IfcConversionBasedUnitWithOffset::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConversionBasedUnit::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ConversionOffset", m_ConversionOffset ) );
}
void IfcConversionBasedUnitWithOffset::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConversionBasedUnit::getAttributesInverse( vec_attributes_inverse );
}
void IfcConversionBasedUnitWithOffset::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConversionBasedUnit::setInverseCounterparts( ptr_self_entity );
}
void IfcConversionBasedUnitWithOffset::unlinkFromInverseCounterparts()
{
	IfcConversionBasedUnit::unlinkFromInverseCounterparts();
}
