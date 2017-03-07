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
#include "include/IfcLabel.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialRelationship.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialRelationship 
IfcMaterialRelationship::IfcMaterialRelationship() { m_entity_enum = IFCMATERIALRELATIONSHIP; }
IfcMaterialRelationship::IfcMaterialRelationship( int id ) { m_id = id; m_entity_enum = IFCMATERIALRELATIONSHIP; }
IfcMaterialRelationship::~IfcMaterialRelationship() {}
shared_ptr<IfcPPObject> IfcMaterialRelationship::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialRelationship> copy_self( new IfcMaterialRelationship() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_RelatingMaterial ) { copy_self->m_RelatingMaterial = dynamic_pointer_cast<IfcMaterial>( m_RelatingMaterial->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedMaterials.size(); ++ii )
	{
		auto item_ii = m_RelatedMaterials[ii];
		if( item_ii )
		{
			copy_self->m_RelatedMaterials.push_back( dynamic_pointer_cast<IfcMaterial>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Expression ) { copy_self->m_Expression = dynamic_pointer_cast<IfcLabel>( m_Expression->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMaterialRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALRELATIONSHIP" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingMaterial ) { stream << "#" << m_RelatingMaterial->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedMaterials );
	stream << ",";
	if( m_Expression ) { m_Expression->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialRelationship::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialRelationship, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_RelatingMaterial, map );
	readEntityReferenceList( args[3], m_RelatedMaterials, map );
	m_Expression = IfcLabel::createObjectFromSTEP( args[4] );
}
void IfcMaterialRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcResourceLevelRelationship::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingMaterial", m_RelatingMaterial ) );
	if( m_RelatedMaterials.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedMaterials_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedMaterials.begin(), m_RelatedMaterials.end(), std::back_inserter( RelatedMaterials_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedMaterials", RelatedMaterials_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "Expression", m_Expression ) );
}
void IfcMaterialRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcResourceLevelRelationship::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcResourceLevelRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMaterialRelationship> ptr_self = dynamic_pointer_cast<IfcMaterialRelationship>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcMaterialRelationship::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedMaterials.size(); ++i )
	{
		if( m_RelatedMaterials[i] )
		{
			m_RelatedMaterials[i]->m_IsRelatedWith_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingMaterial )
	{
		m_RelatingMaterial->m_RelatesTo_inverse.push_back( ptr_self );
	}
}
void IfcMaterialRelationship::unlinkFromInverseCounterparts()
{
	IfcResourceLevelRelationship::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_RelatedMaterials.size(); ++i )
	{
		if( m_RelatedMaterials[i] )
		{
			std::vector<weak_ptr<IfcMaterialRelationship> >& IsRelatedWith_inverse = m_RelatedMaterials[i]->m_IsRelatedWith_inverse;
			for( auto it_IsRelatedWith_inverse = IsRelatedWith_inverse.begin(); it_IsRelatedWith_inverse != IsRelatedWith_inverse.end(); )
			{
				shared_ptr<IfcMaterialRelationship> self_candidate( *it_IsRelatedWith_inverse );
				if( self_candidate.get() == this )
				{
					it_IsRelatedWith_inverse= IsRelatedWith_inverse.erase( it_IsRelatedWith_inverse );
				}
				else
				{
					++it_IsRelatedWith_inverse;
				}
			}
		}
	}
	if( m_RelatingMaterial )
	{
		std::vector<weak_ptr<IfcMaterialRelationship> >& RelatesTo_inverse = m_RelatingMaterial->m_RelatesTo_inverse;
		for( auto it_RelatesTo_inverse = RelatesTo_inverse.begin(); it_RelatesTo_inverse != RelatesTo_inverse.end(); )
		{
			shared_ptr<IfcMaterialRelationship> self_candidate( *it_RelatesTo_inverse );
			if( self_candidate.get() == this )
			{
				it_RelatesTo_inverse= RelatesTo_inverse.erase( it_RelatesTo_inverse );
			}
			else
			{
				++it_RelatesTo_inverse;
			}
		}
	}
}
