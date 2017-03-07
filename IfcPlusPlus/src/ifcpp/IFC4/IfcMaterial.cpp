/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
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
IfcMaterial::IfcMaterial() { m_entity_enum = IFCMATERIAL; }
IfcMaterial::IfcMaterial( int id ) { m_id = id; m_entity_enum = IFCMATERIAL; }
IfcMaterial::~IfcMaterial() {}
shared_ptr<IfcPPObject> IfcMaterial::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterial> copy_self( new IfcMaterial() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Category ) { copy_self->m_Category = dynamic_pointer_cast<IfcLabel>( m_Category->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMaterial::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIAL" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterial::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterial::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterial, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_Category = IfcLabel::createObjectFromSTEP( args[2] );
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
	IfcMaterialDefinition::getAttributesInverse( vec_attributes_inverse );
	if( m_HasRepresentation_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasRepresentation_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasRepresentation_inverse.size(); ++i )
		{
			if( !m_HasRepresentation_inverse[i].expired() )
			{
				HasRepresentation_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMaterialDefinitionRepresentation>( m_HasRepresentation_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasRepresentation_inverse", HasRepresentation_inverse_vec_obj ) );
	}
	if( m_IsRelatedWith_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> IsRelatedWith_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_IsRelatedWith_inverse.size(); ++i )
		{
			if( !m_IsRelatedWith_inverse[i].expired() )
			{
				IsRelatedWith_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMaterialRelationship>( m_IsRelatedWith_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "IsRelatedWith_inverse", IsRelatedWith_inverse_vec_obj ) );
	}
	if( m_RelatesTo_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatesTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_RelatesTo_inverse.size(); ++i )
		{
			if( !m_RelatesTo_inverse[i].expired() )
			{
				RelatesTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMaterialRelationship>( m_RelatesTo_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "RelatesTo_inverse", RelatesTo_inverse_vec_obj ) );
	}
}
void IfcMaterial::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterial::unlinkFromInverseCounterparts()
{
	IfcMaterialDefinition::unlinkFromInverseCounterparts();
}
