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
#include "include/IfcNamedUnit.h"
#include "include/IfcUnitEnum.h"

// ENTITY IfcNamedUnit 
IfcNamedUnit::IfcNamedUnit() {}
IfcNamedUnit::IfcNamedUnit( int id ) { m_id = id; }
IfcNamedUnit::~IfcNamedUnit() {}

// method setEntity takes over all attributes from another instance of the class
void IfcNamedUnit::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcNamedUnit> other = dynamic_pointer_cast<IfcNamedUnit>(other_entity);
	if( !other) { return; }
	m_Dimensions = other->m_Dimensions;
	m_UnitType = other->m_UnitType;
}
void IfcNamedUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCNAMEDUNIT" << "(";
	if( m_Dimensions ) { stream << "#" << m_Dimensions->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcNamedUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcNamedUnit::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcNamedUnit, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcNamedUnit, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Dimensions, map );
	m_UnitType = IfcUnitEnum::createObjectFromStepData( args[1] );
}
void IfcNamedUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Dimensions", m_Dimensions ) );
	vec_attributes.push_back( std::make_pair( "UnitType", m_UnitType ) );
}
void IfcNamedUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcNamedUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcNamedUnit::unlinkSelf()
{
}
