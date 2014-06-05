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
#include "include/IfcMaterial.h"
#include "include/IfcMaterialDefinitionRepresentation.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcMaterialRelationship.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterial 
IfcMaterial::IfcMaterial() {}
IfcMaterial::IfcMaterial( int id ) { m_id = id; }
IfcMaterial::~IfcMaterial() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterial::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterial> other = dynamic_pointer_cast<IfcMaterial>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Category = other->m_Category;
}
void IfcMaterial::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIAL" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterial::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterial::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterial, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcMaterial, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_Category = IfcLabel::createObjectFromStepData( args[2] );
}
void IfcMaterial::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "Category", m_Category ) );
}
void IfcMaterial::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasRepresentation_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasRepresentation_inverse.size(); ++i ) { HasRepresentation_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMaterialDefinitionRepresentation>( m_HasRepresentation_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasRepresentation_inverse", HasRepresentation_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsRelatedWith_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsRelatedWith_inverse.size(); ++i ) { IsRelatedWith_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMaterialRelationship>( m_IsRelatedWith_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsRelatedWith_inverse", IsRelatedWith_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> RelatesTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_RelatesTo_inverse.size(); ++i ) { RelatesTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMaterialRelationship>( m_RelatesTo_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "RelatesTo_inverse", RelatesTo_inverse_vec_obj ) );
}
void IfcMaterial::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterial::unlinkSelf()
{
	IfcMaterialDefinition::unlinkSelf();
}
