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
#include "include/IfcPropertyListValue.h"
#include "include/IfcPropertySet.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyListValue 
IfcPropertyListValue::IfcPropertyListValue() {}
IfcPropertyListValue::IfcPropertyListValue( int id ) { m_id = id; }
IfcPropertyListValue::~IfcPropertyListValue() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertyListValue::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertyListValue> other = dynamic_pointer_cast<IfcPropertyListValue>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ListValues = other->m_ListValues;
	m_Unit = other->m_Unit;
}
void IfcPropertyListValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYLISTVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_ListValues, true );
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertyListValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyListValue::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertyListValue, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcPropertyListValue, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readSelectList( args[2], m_ListValues, map );
	m_Unit = IfcUnit::createObjectFromStepData( args[3], map );
}
void IfcPropertyListValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSimpleProperty::getAttributes( vec_attributes );
	shared_ptr<IfcPPAttributeObjectVector> ListValues_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_ListValues.begin(), m_ListValues.end(), std::back_inserter( ListValues_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "ListValues", ListValues_vec_object ) );
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
}
void IfcPropertyListValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPropertyListValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSimpleProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyListValue::unlinkSelf()
{
	IfcSimpleProperty::unlinkSelf();
}
