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
#include "include/IfcClassificationSelect.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialClassificationRelationship.h"

// ENTITY IfcMaterialClassificationRelationship 
IfcMaterialClassificationRelationship::IfcMaterialClassificationRelationship() {}
IfcMaterialClassificationRelationship::IfcMaterialClassificationRelationship( int id ) { m_id = id; }
IfcMaterialClassificationRelationship::~IfcMaterialClassificationRelationship() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialClassificationRelationship::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialClassificationRelationship> other = dynamic_pointer_cast<IfcMaterialClassificationRelationship>(other_entity);
	if( !other) { return; }
	m_MaterialClassifications = other->m_MaterialClassifications;
	m_ClassifiedMaterial = other->m_ClassifiedMaterial;
}
void IfcMaterialClassificationRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALCLASSIFICATIONRELATIONSHIP" << "(";
	writeTypeList( stream, m_MaterialClassifications, true );
	stream << ",";
	if( m_ClassifiedMaterial ) { stream << "#" << m_ClassifiedMaterial->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialClassificationRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialClassificationRelationship::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialClassificationRelationship, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcMaterialClassificationRelationship, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readSelectList( args[0], m_MaterialClassifications, map );
	readEntityReference( args[1], m_ClassifiedMaterial, map );
}
void IfcMaterialClassificationRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	shared_ptr<IfcPPAttributeObjectVector> MaterialClassifications_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_MaterialClassifications.begin(), m_MaterialClassifications.end(), std::back_inserter( MaterialClassifications_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "MaterialClassifications", MaterialClassifications_vec_object ) );
	vec_attributes.push_back( std::make_pair( "ClassifiedMaterial", m_ClassifiedMaterial ) );
}
void IfcMaterialClassificationRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialClassificationRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMaterialClassificationRelationship::unlinkSelf()
{
}
