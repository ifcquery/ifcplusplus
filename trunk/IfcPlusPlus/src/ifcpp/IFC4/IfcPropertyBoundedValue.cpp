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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcPropertyBoundedValue.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyBoundedValue 
IfcPropertyBoundedValue::IfcPropertyBoundedValue() {}
IfcPropertyBoundedValue::IfcPropertyBoundedValue( int id ) { m_id = id; }
IfcPropertyBoundedValue::~IfcPropertyBoundedValue() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertyBoundedValue::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertyBoundedValue> other = dynamic_pointer_cast<IfcPropertyBoundedValue>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_UpperBoundValue = other->m_UpperBoundValue;
	m_LowerBoundValue = other->m_LowerBoundValue;
	m_Unit = other->m_Unit;
	m_SetPointValue = other->m_SetPointValue;
}
void IfcPropertyBoundedValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYBOUNDEDVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UpperBoundValue ) { m_UpperBoundValue->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_LowerBoundValue ) { m_LowerBoundValue->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_SetPointValue ) { m_SetPointValue->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertyBoundedValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyBoundedValue::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertyBoundedValue, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcPropertyBoundedValue, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_UpperBoundValue = IfcValue::createObjectFromStepData( args[2], map );
	m_LowerBoundValue = IfcValue::createObjectFromStepData( args[3], map );
	m_Unit = IfcUnit::createObjectFromStepData( args[4], map );
	m_SetPointValue = IfcValue::createObjectFromStepData( args[5], map );
}
void IfcPropertyBoundedValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSimpleProperty::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UpperBoundValue", m_UpperBoundValue ) );
	vec_attributes.push_back( std::make_pair( "LowerBoundValue", m_LowerBoundValue ) );
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
	vec_attributes.push_back( std::make_pair( "SetPointValue", m_SetPointValue ) );
}
void IfcPropertyBoundedValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPropertyBoundedValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSimpleProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyBoundedValue::unlinkSelf()
{
	IfcSimpleProperty::unlinkSelf();
}
