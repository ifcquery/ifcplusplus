/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcExternalReferenceRelationship.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcMaterialConstituent.h"
#include "ifcpp/IFC4/include/IfcMaterialConstituentSet.h"
#include "ifcpp/IFC4/include/IfcMaterialProperties.h"
#include "ifcpp/IFC4/include/IfcRelAssociatesMaterial.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcMaterialConstituentSet 
IfcMaterialConstituentSet::IfcMaterialConstituentSet() = default;
IfcMaterialConstituentSet::IfcMaterialConstituentSet( int id ) { m_entity_id = id; }
IfcMaterialConstituentSet::~IfcMaterialConstituentSet() = default;
shared_ptr<BuildingObject> IfcMaterialConstituentSet::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcMaterialConstituentSet> copy_self( new IfcMaterialConstituentSet() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for(auto item_ii : m_MaterialConstituents)
	{
			if( item_ii )
		{
			copy_self->m_MaterialConstituents.push_back( dynamic_pointer_cast<IfcMaterialConstituent>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcMaterialConstituentSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCMATERIALCONSTITUENTSET" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_MaterialConstituents );
	stream << ");";
}
void IfcMaterialConstituentSet::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcMaterialConstituentSet::toString() const { return L"IfcMaterialConstituentSet"; }
void IfcMaterialConstituentSet::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialConstituentSet, expecting 3, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	readEntityReferenceList( args[2], m_MaterialConstituents, map );
}
void IfcMaterialConstituentSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcMaterialDefinition::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "Name", m_Name );
	vec_attributes.emplace_back( "Description", m_Description );
	if( !m_MaterialConstituents.empty() )
	{
		shared_ptr<AttributeObjectVector> MaterialConstituents_vec_object( new AttributeObjectVector() );
		std::copy( m_MaterialConstituents.begin(), m_MaterialConstituents.end(), std::back_inserter( MaterialConstituents_vec_object->m_vec ) );
		vec_attributes.emplace_back( "MaterialConstituents", MaterialConstituents_vec_object );
	}
}
void IfcMaterialConstituentSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcMaterialDefinition::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialConstituentSet::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMaterialConstituentSet> ptr_self = dynamic_pointer_cast<IfcMaterialConstituentSet>( ptr_self_entity );
	if( !ptr_self ) { throw BuildingException( "IfcMaterialConstituentSet::setInverseCounterparts: type mismatch" ); }
	for(auto & m_MaterialConstituent : m_MaterialConstituents)
	{
		if( m_MaterialConstituent )
		{
			m_MaterialConstituent->m_ToMaterialConstituentSet_inverse = ptr_self;
		}
	}
}
void IfcMaterialConstituentSet::unlinkFromInverseCounterparts()
{
	IfcMaterialDefinition::unlinkFromInverseCounterparts();
	for(auto & m_MaterialConstituent : m_MaterialConstituents)
	{
		if( m_MaterialConstituent )
		{
			if( !m_MaterialConstituent->m_ToMaterialConstituentSet_inverse.expired() )
			{
				shared_ptr<IfcMaterialConstituentSet> self_candidate( m_MaterialConstituent->m_ToMaterialConstituentSet_inverse );
				if( self_candidate.get() == this )
				{
					weak_ptr<IfcMaterialConstituentSet>& self_candidate_weak = m_MaterialConstituent->m_ToMaterialConstituentSet_inverse;
					self_candidate_weak.reset();
				}
			}
		}
	}
}
