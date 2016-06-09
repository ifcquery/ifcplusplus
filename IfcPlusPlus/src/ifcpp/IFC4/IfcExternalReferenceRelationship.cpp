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
#include "include/IfcActorRole.h"
#include "include/IfcAppliedValue.h"
#include "include/IfcApproval.h"
#include "include/IfcConstraint.h"
#include "include/IfcContextDependentUnit.h"
#include "include/IfcConversionBasedUnit.h"
#include "include/IfcExternalReference.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcMaterialDefinition.h"
#include "include/IfcPhysicalQuantity.h"
#include "include/IfcProfileDef.h"
#include "include/IfcPropertyAbstraction.h"
#include "include/IfcResourceObjectSelect.h"
#include "include/IfcText.h"
#include "include/IfcTimeSeries.h"

// ENTITY IfcExternalReferenceRelationship 
IfcExternalReferenceRelationship::IfcExternalReferenceRelationship() { m_entity_enum = IFCEXTERNALREFERENCERELATIONSHIP; }
IfcExternalReferenceRelationship::IfcExternalReferenceRelationship( int id ) { m_id = id; m_entity_enum = IFCEXTERNALREFERENCERELATIONSHIP; }
IfcExternalReferenceRelationship::~IfcExternalReferenceRelationship() {}
shared_ptr<IfcPPObject> IfcExternalReferenceRelationship::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcExternalReferenceRelationship> copy_self( new IfcExternalReferenceRelationship() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_RelatingReference ) { copy_self->m_RelatingReference = dynamic_pointer_cast<IfcExternalReference>( m_RelatingReference->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedResourceObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedResourceObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedResourceObjects.push_back( dynamic_pointer_cast<IfcResourceObjectSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcExternalReferenceRelationship::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCEXTERNALREFERENCERELATIONSHIP" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingReference ) { stream << "#" << m_RelatingReference->m_id; } else { stream << "$"; }
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
void IfcExternalReferenceRelationship::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcExternalReferenceRelationship::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcExternalReferenceRelationship, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_RelatingReference, map );
	readSelectList( args[3], m_RelatedResourceObjects, map );
}
void IfcExternalReferenceRelationship::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcResourceLevelRelationship::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingReference", m_RelatingReference ) );
	if( m_RelatedResourceObjects.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RelatedResourceObjects_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RelatedResourceObjects.begin(), m_RelatedResourceObjects.end(), std::back_inserter( RelatedResourceObjects_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RelatedResourceObjects", RelatedResourceObjects_vec_object ) );
	}
}
void IfcExternalReferenceRelationship::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcResourceLevelRelationship::getAttributesInverse( vec_attributes_inverse );
}
void IfcExternalReferenceRelationship::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcResourceLevelRelationship::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcExternalReferenceRelationship> ptr_self = dynamic_pointer_cast<IfcExternalReferenceRelationship>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcExternalReferenceRelationship::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedResourceObjects.size(); ++i )
	{
		shared_ptr<IfcActorRole>  RelatedResourceObjects_IfcActorRole = dynamic_pointer_cast<IfcActorRole>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcActorRole )
		{
			RelatedResourceObjects_IfcActorRole->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcAppliedValue>  RelatedResourceObjects_IfcAppliedValue = dynamic_pointer_cast<IfcAppliedValue>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcAppliedValue )
		{
			RelatedResourceObjects_IfcAppliedValue->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcApproval>  RelatedResourceObjects_IfcApproval = dynamic_pointer_cast<IfcApproval>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcApproval )
		{
			RelatedResourceObjects_IfcApproval->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcConstraint>  RelatedResourceObjects_IfcConstraint = dynamic_pointer_cast<IfcConstraint>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcConstraint )
		{
			RelatedResourceObjects_IfcConstraint->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcContextDependentUnit>  RelatedResourceObjects_IfcContextDependentUnit = dynamic_pointer_cast<IfcContextDependentUnit>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcContextDependentUnit )
		{
			RelatedResourceObjects_IfcContextDependentUnit->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcConversionBasedUnit>  RelatedResourceObjects_IfcConversionBasedUnit = dynamic_pointer_cast<IfcConversionBasedUnit>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcConversionBasedUnit )
		{
			RelatedResourceObjects_IfcConversionBasedUnit->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcMaterialDefinition>  RelatedResourceObjects_IfcMaterialDefinition = dynamic_pointer_cast<IfcMaterialDefinition>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcMaterialDefinition )
		{
			RelatedResourceObjects_IfcMaterialDefinition->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcPhysicalQuantity>  RelatedResourceObjects_IfcPhysicalQuantity = dynamic_pointer_cast<IfcPhysicalQuantity>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcPhysicalQuantity )
		{
			RelatedResourceObjects_IfcPhysicalQuantity->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcProfileDef>  RelatedResourceObjects_IfcProfileDef = dynamic_pointer_cast<IfcProfileDef>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcProfileDef )
		{
			RelatedResourceObjects_IfcProfileDef->m_HasExternalReference_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcPropertyAbstraction>  RelatedResourceObjects_IfcPropertyAbstraction = dynamic_pointer_cast<IfcPropertyAbstraction>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcPropertyAbstraction )
		{
			RelatedResourceObjects_IfcPropertyAbstraction->m_HasExternalReferences_inverse.push_back( ptr_self );
		}
		shared_ptr<IfcTimeSeries>  RelatedResourceObjects_IfcTimeSeries = dynamic_pointer_cast<IfcTimeSeries>( m_RelatedResourceObjects[i] );
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
	for( size_t i=0; i<m_RelatedResourceObjects.size(); ++i )
	{
		shared_ptr<IfcActorRole>  RelatedResourceObjects_IfcActorRole = dynamic_pointer_cast<IfcActorRole>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcActorRole )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcActorRole->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
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
		shared_ptr<IfcAppliedValue>  RelatedResourceObjects_IfcAppliedValue = dynamic_pointer_cast<IfcAppliedValue>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcAppliedValue )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcAppliedValue->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
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
		shared_ptr<IfcApproval>  RelatedResourceObjects_IfcApproval = dynamic_pointer_cast<IfcApproval>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcApproval )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcApproval->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
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
		shared_ptr<IfcConstraint>  RelatedResourceObjects_IfcConstraint = dynamic_pointer_cast<IfcConstraint>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcConstraint )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcConstraint->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
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
		shared_ptr<IfcContextDependentUnit>  RelatedResourceObjects_IfcContextDependentUnit = dynamic_pointer_cast<IfcContextDependentUnit>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcContextDependentUnit )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcContextDependentUnit->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
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
		shared_ptr<IfcConversionBasedUnit>  RelatedResourceObjects_IfcConversionBasedUnit = dynamic_pointer_cast<IfcConversionBasedUnit>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcConversionBasedUnit )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcConversionBasedUnit->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
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
		shared_ptr<IfcMaterialDefinition>  RelatedResourceObjects_IfcMaterialDefinition = dynamic_pointer_cast<IfcMaterialDefinition>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcMaterialDefinition )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcMaterialDefinition->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
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
		shared_ptr<IfcPhysicalQuantity>  RelatedResourceObjects_IfcPhysicalQuantity = dynamic_pointer_cast<IfcPhysicalQuantity>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcPhysicalQuantity )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcPhysicalQuantity->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
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
		shared_ptr<IfcProfileDef>  RelatedResourceObjects_IfcProfileDef = dynamic_pointer_cast<IfcProfileDef>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcProfileDef )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcProfileDef->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
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
		shared_ptr<IfcPropertyAbstraction>  RelatedResourceObjects_IfcPropertyAbstraction = dynamic_pointer_cast<IfcPropertyAbstraction>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcPropertyAbstraction )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReferences_inverse = RelatedResourceObjects_IfcPropertyAbstraction->m_HasExternalReferences_inverse;
			for( auto it_HasExternalReferences_inverse = HasExternalReferences_inverse.begin(); it_HasExternalReferences_inverse != HasExternalReferences_inverse.end(); )
			{
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
		shared_ptr<IfcTimeSeries>  RelatedResourceObjects_IfcTimeSeries = dynamic_pointer_cast<IfcTimeSeries>( m_RelatedResourceObjects[i] );
		if( RelatedResourceObjects_IfcTimeSeries )
		{
			std::vector<weak_ptr<IfcExternalReferenceRelationship> >& HasExternalReference_inverse = RelatedResourceObjects_IfcTimeSeries->m_HasExternalReference_inverse;
			for( auto it_HasExternalReference_inverse = HasExternalReference_inverse.begin(); it_HasExternalReference_inverse != HasExternalReference_inverse.end(); )
			{
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
