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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcPropertyEnumeration.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyEnumeration 
IfcPropertyEnumeration::IfcPropertyEnumeration() {}
IfcPropertyEnumeration::IfcPropertyEnumeration( int id ) { m_id = id; }
IfcPropertyEnumeration::~IfcPropertyEnumeration() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPropertyEnumeration::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPropertyEnumeration> other = dynamic_pointer_cast<IfcPropertyEnumeration>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_EnumerationValues = other->m_EnumerationValues;
	m_Unit = other->m_Unit;
}
void IfcPropertyEnumeration::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTYENUMERATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_EnumerationValues, true );
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertyEnumeration::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyEnumeration::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertyEnumeration, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcPropertyEnumeration, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	readSelectList( args[1], m_EnumerationValues, map );
	m_Unit = IfcUnit::createObjectFromStepData( args[2], map );
}
void IfcPropertyEnumeration::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyAbstraction::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	shared_ptr<IfcPPAttributeObjectVector> EnumerationValues_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_EnumerationValues.begin(), m_EnumerationValues.end(), std::back_inserter( EnumerationValues_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "EnumerationValues", EnumerationValues_vec_object ) );
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
}
void IfcPropertyEnumeration::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPropertyEnumeration::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyAbstraction::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyEnumeration::unlinkSelf()
{
	IfcPropertyAbstraction::unlinkSelf();
}
