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
#include "include/IfcContextDependentUnit.h"
#include "include/IfcDimensionalExponents.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcUnitEnum.h"

// ENTITY IfcContextDependentUnit 
IfcContextDependentUnit::IfcContextDependentUnit() {}
IfcContextDependentUnit::IfcContextDependentUnit( int id ) { m_id = id; }
IfcContextDependentUnit::~IfcContextDependentUnit() {}

// method setEntity takes over all attributes from another instance of the class
void IfcContextDependentUnit::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcContextDependentUnit> other = dynamic_pointer_cast<IfcContextDependentUnit>(other_entity);
	if( !other) { return; }
	m_Dimensions = other->m_Dimensions;
	m_UnitType = other->m_UnitType;
	m_Name = other->m_Name;
}
void IfcContextDependentUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCONTEXTDEPENDENTUNIT" << "(";
	if( m_Dimensions ) { stream << "#" << m_Dimensions->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcContextDependentUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcContextDependentUnit::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcContextDependentUnit, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcContextDependentUnit, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Dimensions, map );
	m_UnitType = IfcUnitEnum::createObjectFromStepData( args[1] );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
}
void IfcContextDependentUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcNamedUnit::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
}
void IfcContextDependentUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasExternalReference_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasExternalReference_inverse.size(); ++i ) { HasExternalReference_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReference_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasExternalReference_inverse", HasExternalReference_inverse_vec_obj ) );
}
void IfcContextDependentUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcNamedUnit::setInverseCounterparts( ptr_self_entity );
}
void IfcContextDependentUnit::unlinkSelf()
{
	IfcNamedUnit::unlinkSelf();
}
