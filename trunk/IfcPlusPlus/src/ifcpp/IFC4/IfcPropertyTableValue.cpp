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
#include "include/IfcComplexProperty.h"
#include "include/IfcCurveInterpolationEnum.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcPropertyTableValue.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyTableValue 
IfcPropertyTableValue::IfcPropertyTableValue() {}
IfcPropertyTableValue::IfcPropertyTableValue( int id ) { m_id = id; }
IfcPropertyTableValue::~IfcPropertyTableValue() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertyTableValue::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertyTableValue> other = dynamic_pointer_cast<IfcPropertyTableValue>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_DefiningValues = other->m_DefiningValues;
	m_DefinedValues = other->m_DefinedValues;
	m_Expression = other->m_Expression;
	m_DefiningUnit = other->m_DefiningUnit;
	m_DefinedUnit = other->m_DefinedUnit;
	m_CurveInterpolation = other->m_CurveInterpolation;
}
void IfcPropertyTableValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYTABLEVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_DefiningValues, true );
	stream << ",";
	writeTypeList( stream, m_DefinedValues, true );
	stream << ",";
	if( m_Expression ) { m_Expression->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DefiningUnit ) { m_DefiningUnit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_DefinedUnit ) { m_DefinedUnit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_CurveInterpolation ) { m_CurveInterpolation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertyTableValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyTableValue::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertyTableValue, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>8 ){ std::cout << "Wrong parameter count for entity IfcPropertyTableValue, expecting 8, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readSelectList( args[2], m_DefiningValues, map );
	readSelectList( args[3], m_DefinedValues, map );
	m_Expression = IfcText::createObjectFromStepData( args[4] );
	m_DefiningUnit = IfcUnit::createObjectFromStepData( args[5], map );
	m_DefinedUnit = IfcUnit::createObjectFromStepData( args[6], map );
	m_CurveInterpolation = IfcCurveInterpolationEnum::createObjectFromStepData( args[7] );
}
void IfcPropertyTableValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSimpleProperty::getAttributes( vec_attributes );
	shared_ptr<IfcPPAttributeObjectVector> DefiningValues_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_DefiningValues.begin(), m_DefiningValues.end(), std::back_inserter( DefiningValues_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "DefiningValues", DefiningValues_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> DefinedValues_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_DefinedValues.begin(), m_DefinedValues.end(), std::back_inserter( DefinedValues_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "DefinedValues", DefinedValues_vec_object ) );
	vec_attributes.push_back( std::make_pair( "Expression", m_Expression ) );
	vec_attributes.push_back( std::make_pair( "DefiningUnit", m_DefiningUnit ) );
	vec_attributes.push_back( std::make_pair( "DefinedUnit", m_DefinedUnit ) );
	vec_attributes.push_back( std::make_pair( "CurveInterpolation", m_CurveInterpolation ) );
}
void IfcPropertyTableValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPropertyTableValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSimpleProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyTableValue::unlinkSelf()
{
	IfcSimpleProperty::unlinkSelf();
}
