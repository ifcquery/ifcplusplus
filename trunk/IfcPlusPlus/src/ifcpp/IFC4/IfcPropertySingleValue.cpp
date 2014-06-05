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
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcPropertySingleValue.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertySingleValue 
IfcPropertySingleValue::IfcPropertySingleValue() {}
IfcPropertySingleValue::IfcPropertySingleValue( int id ) { m_id = id; }
IfcPropertySingleValue::~IfcPropertySingleValue() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertySingleValue::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertySingleValue> other = dynamic_pointer_cast<IfcPropertySingleValue>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_NominalValue = other->m_NominalValue;
	m_Unit = other->m_Unit;
}
void IfcPropertySingleValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYSINGLEVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_NominalValue ) { m_NominalValue->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertySingleValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertySingleValue::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertySingleValue, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcPropertySingleValue, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_NominalValue = IfcValue::createObjectFromStepData( args[2], map );
	m_Unit = IfcUnit::createObjectFromStepData( args[3], map );
}
void IfcPropertySingleValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSimpleProperty::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "NominalValue", m_NominalValue ) );
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
}
void IfcPropertySingleValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPropertySingleValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSimpleProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertySingleValue::unlinkSelf()
{
	IfcSimpleProperty::unlinkSelf();
}
