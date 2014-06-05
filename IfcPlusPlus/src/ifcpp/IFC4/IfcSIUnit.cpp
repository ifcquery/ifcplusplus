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
#include "include/IfcSIPrefix.h"
#include "include/IfcSIUnit.h"
#include "include/IfcSIUnitName.h"
#include "include/IfcUnitEnum.h"

// ENTITY IfcSIUnit 
IfcSIUnit::IfcSIUnit() {}
IfcSIUnit::IfcSIUnit( int id ) { m_id = id; }
IfcSIUnit::~IfcSIUnit() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSIUnit::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSIUnit> other = dynamic_pointer_cast<IfcSIUnit>(other_entity);
	if( !other) { return; }
	m_Dimensions = other->m_Dimensions;
	m_UnitType = other->m_UnitType;
	m_Prefix = other->m_Prefix;
	m_Name = other->m_Name;
}
void IfcSIUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSIUNIT" << "(";
	if( m_Dimensions ) { stream << "#" << m_Dimensions->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Prefix ) { m_Prefix->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSIUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSIUnit::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSIUnit, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcSIUnit, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Dimensions, map );
	m_UnitType = IfcUnitEnum::createObjectFromStepData( args[1] );
	m_Prefix = IfcSIPrefix::createObjectFromStepData( args[2] );
	m_Name = IfcSIUnitName::createObjectFromStepData( args[3] );
}
void IfcSIUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcNamedUnit::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Prefix", m_Prefix ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
}
void IfcSIUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSIUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcNamedUnit::setInverseCounterparts( ptr_self_entity );
}
void IfcSIUnit::unlinkSelf()
{
	IfcNamedUnit::unlinkSelf();
}
