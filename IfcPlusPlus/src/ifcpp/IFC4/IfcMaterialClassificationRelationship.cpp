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
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcClassificationSelect.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialClassificationRelationship.h"

// ENTITY IfcMaterialClassificationRelationship 
IfcMaterialClassificationRelationship::IfcMaterialClassificationRelationship() { m_entity_enum = IFCMATERIALCLASSIFICATIONRELATIONSHIP; }
IfcMaterialClassificationRelationship::IfcMaterialClassificationRelationship( int id ) { m_id = id; m_entity_enum = IFCMATERIALCLASSIFICATIONRELATIONSHIP; }
IfcMaterialClassificationRelationship::~IfcMaterialClassificationRelationship() {}
shared_ptr<IfcPPObject> IfcMaterialClassificationRelationship::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialClassificationRelationship> copy_self( new IfcMaterialClassificationRelationship() );
	for( size_t ii=0; ii<m_MaterialClassifications.size(); ++ii )
	{
		auto item_ii = m_MaterialClassifications[ii];
		if( item_ii )
		{
			copy_self->m_MaterialClassifications.push_back( dynamic_pointer_cast<IfcClassificationSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_ClassifiedMaterial ) { copy_self->m_ClassifiedMaterial = dynamic_pointer_cast<IfcMaterial>( m_ClassifiedMaterial->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMaterialClassificationRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALCLASSIFICATIONRELATIONSHIP" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_MaterialClassifications.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcClassificationSelect>& type_object = m_MaterialClassifications[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_ClassifiedMaterial ) { stream << "#" << m_ClassifiedMaterial->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialClassificationRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialClassificationRelationship::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialClassificationRelationship, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readSelectList( args[0], m_MaterialClassifications, map );
	readEntityReference( args[1], m_ClassifiedMaterial, map );
}
void IfcMaterialClassificationRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_MaterialClassifications.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> MaterialClassifications_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_MaterialClassifications.begin(), m_MaterialClassifications.end(), std::back_inserter( MaterialClassifications_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "MaterialClassifications", MaterialClassifications_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "ClassifiedMaterial", m_ClassifiedMaterial ) );
}
void IfcMaterialClassificationRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialClassificationRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMaterialClassificationRelationship::unlinkFromInverseCounterparts()
{
}
