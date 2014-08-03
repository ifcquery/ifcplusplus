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

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcDimensionalExponents.h"

// ENTITY IfcDimensionalExponents 
IfcDimensionalExponents::IfcDimensionalExponents() {}
IfcDimensionalExponents::IfcDimensionalExponents( int id ) { m_id = id; }
IfcDimensionalExponents::~IfcDimensionalExponents() {}
shared_ptr<IfcPPObject> IfcDimensionalExponents::getDeepCopy()
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
	if( m_LengthExponent == m_LengthExponent ){ stream << m_LengthExponent; } else { stream << "$"; }
	stream << ",";
	if( m_MassExponent == m_MassExponent ){ stream << m_MassExponent; } else { stream << "$"; }
	stream << ",";
	if( m_TimeExponent == m_TimeExponent ){ stream << m_TimeExponent; } else { stream << "$"; }
	stream << ",";
	if( m_ElectricCurrentExponent == m_ElectricCurrentExponent ){ stream << m_ElectricCurrentExponent; } else { stream << "$"; }
	stream << ",";
	if( m_ThermodynamicTemperatureExponent == m_ThermodynamicTemperatureExponent ){ stream << m_ThermodynamicTemperatureExponent; } else { stream << "$"; }
	stream << ",";
	if( m_AmountOfSubstanceExponent == m_AmountOfSubstanceExponent ){ stream << m_AmountOfSubstanceExponent; } else { stream << "$"; }
	stream << ",";
	if( m_LuminousIntensityExponent == m_LuminousIntensityExponent ){ stream << m_LuminousIntensityExponent; } else { stream << "$"; }
	stream << ");";
}
void IfcDimensionalExponents::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDimensionalExponents::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDimensionalExponents, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcDimensionalExponents, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
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
	vec_attributes.push_back( std::make_pair( "LengthExponent", shared_ptr<IfcPPInt>( new IfcPPInt( m_LengthExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "MassExponent", shared_ptr<IfcPPInt>( new IfcPPInt( m_MassExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "TimeExponent", shared_ptr<IfcPPInt>( new IfcPPInt( m_TimeExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "ElectricCurrentExponent", shared_ptr<IfcPPInt>( new IfcPPInt( m_ElectricCurrentExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "ThermodynamicTemperatureExponent", shared_ptr<IfcPPInt>( new IfcPPInt( m_ThermodynamicTemperatureExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "AmountOfSubstanceExponent", shared_ptr<IfcPPInt>( new IfcPPInt( m_AmountOfSubstanceExponent ) ) ) );
	vec_attributes.push_back( std::make_pair( "LuminousIntensityExponent", shared_ptr<IfcPPInt>( new IfcPPInt( m_LuminousIntensityExponent ) ) ) );
}
void IfcDimensionalExponents::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcDimensionalExponents::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcDimensionalExponents::unlinkSelf()
{
}
