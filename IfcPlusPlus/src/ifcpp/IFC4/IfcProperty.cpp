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
#include "include/IfcProperty.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcText.h"

// ENTITY IfcProperty 
IfcProperty::IfcProperty() {}
IfcProperty::IfcProperty( int id ) { m_id = id; }
IfcProperty::~IfcProperty() {}

// method setEntity takes over all attributes from another instance of the class
void IfcProperty::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcProperty> other = dynamic_pointer_cast<IfcProperty>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
}
void IfcProperty::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROPERTY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcProperty::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProperty::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcProperty, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcProperty, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
}
void IfcProperty::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyAbstraction::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcProperty::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> PartOfPset_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_PartOfPset_inverse.size(); ++i ) { PartOfPset_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPropertySet>( m_PartOfPset_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "PartOfPset_inverse", PartOfPset_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> PropertyForDependance_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_PropertyForDependance_inverse.size(); ++i ) { PropertyForDependance_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPropertyDependencyRelationship>( m_PropertyForDependance_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "PropertyForDependance_inverse", PropertyForDependance_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> PropertyDependsOn_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_PropertyDependsOn_inverse.size(); ++i ) { PropertyDependsOn_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcPropertyDependencyRelationship>( m_PropertyDependsOn_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "PropertyDependsOn_inverse", PropertyDependsOn_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> PartOfComplex_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_PartOfComplex_inverse.size(); ++i ) { PartOfComplex_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcComplexProperty>( m_PartOfComplex_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "PartOfComplex_inverse", PartOfComplex_inverse_vec_obj ) );
}
void IfcProperty::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyAbstraction::setInverseCounterparts( ptr_self_entity );
}
void IfcProperty::unlinkSelf()
{
	IfcPropertyAbstraction::unlinkSelf();
}
