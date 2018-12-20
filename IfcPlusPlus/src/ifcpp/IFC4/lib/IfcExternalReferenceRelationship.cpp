/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcActorRole.h"
#include "ifcpp/IFC4/include/IfcAppliedValue.h"
#include "ifcpp/IFC4/include/IfcApproval.h"
#include "ifcpp/IFC4/include/IfcConstraint.h"
#include "ifcpp/IFC4/include/IfcContextDependentUnit.h"
#include "ifcpp/IFC4/include/IfcConversionBasedUnit.h"
#include "ifcpp/IFC4/include/IfcExternalReference.h"
#include "ifcpp/IFC4/include/IfcExternalReferenceRelationship.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcMaterialDefinition.h"
#include "ifcpp/IFC4/include/IfcPhysicalQuantity.h"
#include "ifcpp/IFC4/include/IfcProfileDef.h"
#include "ifcpp/IFC4/include/IfcPropertyAbstraction.h"
#include "ifcpp/IFC4/include/IfcResourceObjectSelect.h"
#include "ifcpp/IFC4/include/IfcText.h"
#include "ifcpp/IFC4/include/IfcTimeSeries.h"

// ENTITY IfcExternalReferenceRelationship 
IfcExternalReferenceRelationship::IfcExternalReferenceRelationship() = default;
IfcExternalReferenceRelationship::IfcExternalReferenceRelationship( int id ) { m_entity_id = id; }
IfcExternalReferenceRelationship::~IfcExternalReferenceRelationship() = default;
shared_ptr<BuildingObject> IfcExternalReferenceRelationship::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcExternalReferenceRelationship> copy_self( new IfcExternalReferenceRelationship() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_RelatingReference ) { copy_self->m_RelatingReference = dynamic_pointer_cast<IfcExternalReference>( m_RelatingReference->getDeepCopy(options) ); }
	for(auto item_ii : m_RelatedResourceObjects)
	{
			if( item_ii )
		{
			copy_self->m_RelatedResourceObjects.push_back( dynamic_pointer_cast<IfcResourceObjectSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcExternalReferenceRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCEXTERNALREFERENCERELATIONSHIP" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingReference ) { stream << "#" << m_RelatingReference->m_entity_id; } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_RelatedResourceObjects.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcResourceObjectSelect>& type_object = m_RelatedResourceObjects[ii];
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
	stream << ");";
}
void IfcExternalReferenceRelationship::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcExternalReferenceRelationship::toString() const { return L"IfcExternalReferenceRelationship"; }
void IfcExternalReferenceRelationship::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcExternalReferenceRelationship, expecting 4, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	readEntityReference( args[2], m_RelatingReference, map );
	readSelectList( args[3], m_RelatedResourceObjects, map );
}
void IfcExternalReferenceRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcResourceLevelRelationship::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "RelatingReference", m_RelatingReference );
	if( !m_RelatedResourceObjects.empty() )
	{
		shared_ptr<AttributeObjectVector> RelatedResourceObjects_vec_object( new AttributeObjectVector() );
		std::copy( m_RelatedResourceObjects.begin(), m_RelatedResourceObjects.end(), std::back_inserter( RelatedResourceObjects_vec_object->m_vec ) );
		vec_attributes.emplace_back( "RelatedResourceObjects", RelatedResourceObjects_vec_object );
	}
}
void IfcExternalReferenceRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcResourceLevelRelationship::getAttributesInverse( vec_attributes_inverse );
}
void IfcExternalReferenceRelationship::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcResourceLevelRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcExternalReferenceRelationship> ptr_self = dynamic_pointer_cast<IfcExternalReferenceRelationship>( ptr_self_entity );
	if( !ptr_self ) { throw BuildingException( "IfcExternalReferenceRelationship::setInverseCounterparts: type mismatch" ); }
	for(const auto & m_RelatedResourceObject : m_RelatedResourceObjects)
	{
		shared_ptr<IfcActorRole>  RelatedResourceObjects_IfcActorRole = dynamic_pointer_cast<IfcActorRole>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcActorRole )
		{
			RelatedResourceObjects_IfcActorRole->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcAppliedValue>  RelatedResourceObjects_IfcAppliedValue = dynamic_pointer_cast<IfcAppliedValue>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcAppliedValue )
		{
			RelatedResourceObjects_IfcAppliedValue->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcApproval>  RelatedResourceObjects_IfcApproval = dynamic_pointer_cast<IfcApproval>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcApproval )
		{
			RelatedResourceObjects_IfcApproval->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcConstraint>  RelatedResourceObjects_IfcConstraint = dynamic_pointer_cast<IfcConstraint>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcConstraint )
		{
			RelatedResourceObjects_IfcConstraint->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcContextDependentUnit>  RelatedResourceObjects_IfcContextDependentUnit = dynamic_pointer_cast<IfcContextDependentUnit>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcContextDependentUnit )
		{
			RelatedResourceObjects_IfcContextDependentUnit->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcConversionBasedUnit>  RelatedResourceObjects_IfcConversionBasedUnit = dynamic_pointer_cast<IfcConversionBasedUnit>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcConversionBasedUnit )
		{
			RelatedResourceObjects_IfcConversionBasedUnit->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcMaterialDefinition>  RelatedResourceObjects_IfcMaterialDefinition = dynamic_pointer_cast<IfcMaterialDefinition>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcMaterialDefinition )
		{
			RelatedResourceObjects_IfcMaterialDefinition->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcPhysicalQuantity>  RelatedResourceObjects_IfcPhysicalQuantity = dynamic_pointer_cast<IfcPhysicalQuantity>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcPhysicalQuantity )
		{
			RelatedResourceObjects_IfcPhysicalQuantity->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcProfileDef>  RelatedResourceObjects_IfcProfileDef = dynamic_pointer_cast<IfcProfileDef>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcProfileDef )
		{
			RelatedResourceObjects_IfcProfileDef->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcPropertyAbstraction>  RelatedResourceObjects_IfcPropertyAbstraction = dynamic_pointer_cast<IfcPropertyAbstraction>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcPropertyAbstraction )
		{
			RelatedResourceObjects_IfcPropertyAbstraction->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcTimeSeries>  RelatedResourceObjects_IfcTimeSeries = dynamic_pointer_cast<IfcTimeSeries>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcTimeSeries )
		{
			RelatedResourceObjects_IfcTimeSeries->m_HasExternalReference_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingReference )
	{
		m_RelatingReference->m_ExternalReferenceForResources_inverse.push_back( ptr_self );
	}
}
void IfcExternalReferenceRelationship::unlinkFromInverseCounterparts()
{
	IfcResourceLevelRelationship::unlinkFromInverseCounterparts();
	for(const auto & m_RelatedResourceObject : m_RelatedResourceObjects)
	{
		shared_ptr<IfcActorRole>  RelatedResourceObjects_IfcActorRole = dynamic_pointer_cast<IfcActorRole>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcActorRole )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcActorRole->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReference_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReference_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReference_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReference_inverse= HasExternalReference_inverse.erase( it_HasExternalReference_inverse );
				}
				else
				{
					++it_HasExternalReference_inverse;
				}
			}
		}
		shared_ptr<IfcAppliedValue>  RelatedResourceObjects_IfcAppliedValue = dynamic_pointer_cast<IfcAppliedValue>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcAppliedValue )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcAppliedValue->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReference_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReference_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReference_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReference_inverse= HasExternalReference_inverse.erase( it_HasExternalReference_inverse );
				}
				else
				{
					++it_HasExternalReference_inverse;
				}
			}
		}
		shared_ptr<IfcApproval>  RelatedResourceObjects_IfcApproval = dynamic_pointer_cast<IfcApproval>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcApproval )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcApproval->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReferences_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReferences_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReferences_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReferences_inverse= HasExternalReferences_inverse.erase( it_HasExternalReferences_inverse );
				}
				else
				{
					++it_HasExternalReferences_inverse;
				}
			}
		}
		shared_ptr<IfcConstraint>  RelatedResourceObjects_IfcConstraint = dynamic_pointer_cast<IfcConstraint>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcConstraint )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcConstraint->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReferences_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReferences_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReferences_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReferences_inverse= HasExternalReferences_inverse.erase( it_HasExternalReferences_inverse );
				}
				else
				{
					++it_HasExternalReferences_inverse;
				}
			}
		}
		shared_ptr<IfcContextDependentUnit>  RelatedResourceObjects_IfcContextDependentUnit = dynamic_pointer_cast<IfcContextDependentUnit>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcContextDependentUnit )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcContextDependentUnit->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReference_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReference_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReference_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReference_inverse= HasExternalReference_inverse.erase( it_HasExternalReference_inverse );
				}
				else
				{
					++it_HasExternalReference_inverse;
				}
			}
		}
		shared_ptr<IfcConversionBasedUnit>  RelatedResourceObjects_IfcConversionBasedUnit = dynamic_pointer_cast<IfcConversionBasedUnit>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcConversionBasedUnit )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcConversionBasedUnit->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReference_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReference_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReference_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReference_inverse= HasExternalReference_inverse.erase( it_HasExternalReference_inverse );
				}
				else
				{
					++it_HasExternalReference_inverse;
				}
			}
		}
		shared_ptr<IfcMaterialDefinition>  RelatedResourceObjects_IfcMaterialDefinition = dynamic_pointer_cast<IfcMaterialDefinition>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcMaterialDefinition )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcMaterialDefinition->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReferences_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReferences_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReferences_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReferences_inverse= HasExternalReferences_inverse.erase( it_HasExternalReferences_inverse );
				}
				else
				{
					++it_HasExternalReferences_inverse;
				}
			}
		}
		shared_ptr<IfcPhysicalQuantity>  RelatedResourceObjects_IfcPhysicalQuantity = dynamic_pointer_cast<IfcPhysicalQuantity>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcPhysicalQuantity )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcPhysicalQuantity->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReferences_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReferences_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReferences_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReferences_inverse= HasExternalReferences_inverse.erase( it_HasExternalReferences_inverse );
				}
				else
				{
					++it_HasExternalReferences_inverse;
				}
			}
		}
		shared_ptr<IfcProfileDef>  RelatedResourceObjects_IfcProfileDef = dynamic_pointer_cast<IfcProfileDef>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcProfileDef )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcProfileDef->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReference_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReference_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReference_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReference_inverse= HasExternalReference_inverse.erase( it_HasExternalReference_inverse );
				}
				else
				{
					++it_HasExternalReference_inverse;
				}
			}
		}
		shared_ptr<IfcPropertyAbstraction>  RelatedResourceObjects_IfcPropertyAbstraction = dynamic_pointer_cast<IfcPropertyAbstraction>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcPropertyAbstraction )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcPropertyAbstraction->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReferences_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReferences_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReferences_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReferences_inverse= HasExternalReferences_inverse.erase( it_HasExternalReferences_inverse );
				}
				else
				{
					++it_HasExternalReferences_inverse;
				}
			}
		}
		shared_ptr<IfcTimeSeries>  RelatedResourceObjects_IfcTimeSeries = dynamic_pointer_cast<IfcTimeSeries>( m_RelatedResourceObject );
		if( RelatedResourceObjects_IfcTimeSeries )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcTimeSeries->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
				weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_HasExternalReference_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_HasExternalReference_inverse;
					continue;
				}
				shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_HasExternalReference_inverse );
				if( self_candidate.get() == this )
				{
					it_HasExternalReference_inverse= HasExternalReference_inverse.erase( it_HasExternalReference_inverse );
				}
				else
				{
					++it_HasExternalReference_inverse;
				}
			}
		}
	}
	if( m_RelatingReference )
	{
		std::vector<weak_ptr<IfcExternalReferenceRelationship> >& ExternalReferenceForResources_inverse = m_RelatingReference->m_ExternalReferenceForResources_inverse;
		for( auto it_ExternalReferenceForResources_inverse = ExternalReferenceForResources_inverse.begin(); it_ExternalReferenceForResources_inverse != ExternalReferenceForResources_inverse.end(); )
		{
			weak_ptr<IfcExternalReferenceRelationship> self_candidate_weak = *it_ExternalReferenceForResources_inverse;
			if( self_candidate_weak.expired() )
			{
				++it_ExternalReferenceForResources_inverse;
				continue;
			}
			shared_ptr<IfcExternalReferenceRelationship> self_candidate( *it_ExternalReferenceForResources_inverse );
			if( self_candidate.get() == this )
			{
				it_ExternalReferenceForResources_inverse= ExternalReferenceForResources_inverse.erase( it_ExternalReferenceForResources_inverse );
			}
			else
			{
				++it_ExternalReferenceForResources_inverse;
			}
		}
	}
}
