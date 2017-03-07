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
#include "include/IfcDimensionalExponents.h"

// ENTITY IfcDimensionalExponents 
IfcDimensionalExponents::IfcDimensionalExponents() { m_entity_enum = IFCDIMENSIONALEXPONENTS; }
IfcDimensionalExponents::IfcDimensionalExponents( int id ) { m_id = id; m_entity_enum = IFCDIMENSIONALEXPONENTS; }
IfcDimensionalExponents::~IfcDimensionalExponents() {}
shared_ptr<IfcPPObject> IfcDimensionalExponents::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDimensionalExponents> copy_self( new IfcDimensionalExponents() );
	if( m_LengthExponent ) { copy_self->m_LengthExponent = m_LengthExponent; }
	if( m_MassExponent ) { copy_self->m_MassExponent = m_MassExponent; }
	if( m_TimeExponent ) { copy_self->m_TimeExponent = m_TimeExponent; }
	if( m_ElectricCurrentExponent ) { copy_self->m_ElectricCurrentExponent = m_ElectricCurrentExponent; }
	if( m_ThermodynamicTemperatureExponent ) { copy_self->m_ThermodynamicTemperatureExponent = m_ThermodynamicTemperatureExponent; }
	if( m_AmountOfSubstanceExponent ) { copy_self->m_AmountOfSubstanceExponent = m_AmountOfSubstanceExponent; }
	if( m_LuminousIntensityExponent ) { copy_self->m_LuminousIntensityExponent = m_LuminousIntensityExponent; }
	return copy_self;
}
void IfcDimensionalExponents::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDIMENSIONALEXPONENTS" << "(";
	stream << m_LengthExponent;
	stream << ",";
	stream << m_MassExponent;
	stream << ",";
	stream << m_TimeExponent;
	stream << ",";
	stream << m_ElectricCurrentExponent;
	stream << ",";
	stream << m_ThermodynamicTemperatureExponent;
	stream << ",";
	stream << m_AmountOfSubstanceExponent;
	stream << ",";
	stream << m_LuminousIntensityExponent;
	stream << ");";
}
void IfcDimensionalExponents::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDimensionalExponents::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcDimensionalExponents, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readIntValue( args[0], m_LengthExponent );
	readIntValue( args[1], m_MassExponent );
	readIntValue( args[2], m_TimeExponent );
	readIntValue( args[3], m_ElectricCurrentExponent );
	readIntValue( args[4], m_ThermodynamicTemperatureExponent );
	readIntValue( args[5], m_AmountOfSubstanceExponent );
	readIntValue( args[6], m_LuminousIntensityExponent );
}
void IfcDimensionalExponents::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "LengthExponent", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_LengthExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "MassExponent", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_MassExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "TimeExponent", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_TimeExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "ElectricCurrentExponent", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_ElectricCurrentExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "ThermodynamicTemperatureExponent", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_ThermodynamicTemperatureExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "AmountOfSubstanceExponent", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_AmountOfSubstanceExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "LuminousIntensityExponent", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_LuminousIntensityExponent ) ) ) );
}
void IfcDimensionalExponents::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcDimensionalExponents::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcDimensionalExponents::unlinkFromInverseCounterparts()
{
}
