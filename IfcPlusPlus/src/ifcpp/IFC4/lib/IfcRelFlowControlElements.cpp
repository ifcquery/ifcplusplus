/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcDistributionControlElement.h"
#include "ifcpp/IFC4/include/IfcDistributionFlowElement.h"
#include "ifcpp/IFC4/include/IfcGloballyUniqueId.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcOwnerHistory.h"
#include "ifcpp/IFC4/include/IfcRelFlowControlElements.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcRelFlowControlElements 
IfcRelFlowControlElements::IfcRelFlowControlElements( int id ) { m_entity_id = id; }
IfcRelFlowControlElements::~IfcRelFlowControlElements() {}
shared_ptr<BuildingObject> IfcRelFlowControlElements::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcRelFlowControlElements> copy_self( new IfcRelFlowControlElements() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = make_shared<IfcGloballyUniqueId>( createBase64Uuid<wchar_t>().data() ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedControlElements.size(); ++ii )
	{
		auto item_ii = m_RelatedControlElements[ii];
		if( item_ii )
		{
			copy_self->m_RelatedControlElements.emplace_back( dynamic_pointer_cast<IfcDistributionControlElement>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatingFlowElement ) { copy_self->m_RelatingFlowElement = dynamic_pointer_cast<IfcDistributionFlowElement>( m_RelatingFlowElement->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelFlowControlElements::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCRELFLOWCONTROLELEMENTS" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedControlElements );
	stream << ",";
	if( m_RelatingFlowElement ) { stream << "#" << m_RelatingFlowElement->m_entity_id; } else { stream << "$"; }
	stream << ");";
}
void IfcRelFlowControlElements::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcRelFlowControlElements::toString() const { return L"IfcRelFlowControlElements"; }
void IfcRelFlowControlElements::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRelFlowControlElements, expecting 6, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
	m_Description = IfcText::createObjectFromSTEP( args[3], map );
	readEntityReferenceList( args[4], m_RelatedControlElements, map );
	readEntityReference( args[5], m_RelatingFlowElement, map );
}
void IfcRelFlowControlElements::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcRelConnects::getAttributes( vec_attributes );
	if( !m_RelatedControlElements.empty() )
	{
		shared_ptr<AttributeObjectVector> RelatedControlElements_vec_object( new AttributeObjectVector() );
		std::copy( m_RelatedControlElements.begin(), m_RelatedControlElements.end(), std::back_inserter( RelatedControlElements_vec_object->m_vec ) );
		vec_attributes.emplace_back( std::make_pair( "RelatedControlElements", RelatedControlElements_vec_object ) );
	}
	vec_attributes.emplace_back( std::make_pair( "RelatingFlowElement", m_RelatingFlowElement ) );
}
void IfcRelFlowControlElements::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcRelConnects::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelFlowControlElements::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelFlowControlElements> ptr_self = dynamic_pointer_cast<IfcRelFlowControlElements>( ptr_self_entity );
	if( !ptr_self ) { throw BuildingException( "IfcRelFlowControlElements::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_RelatedControlElements.size(); ++i )
	{
		if( m_RelatedControlElements[i] )
		{
			m_RelatedControlElements[i]->m_AssignedToFlowElement_inverse.emplace_back( ptr_self );
		}
	}
	if( m_RelatingFlowElement )
	{
		m_RelatingFlowElement->m_HasControlElements_inverse.emplace_back( ptr_self );
	}
}
void IfcRelFlowControlElements::unlinkFromInverseCounterparts()
{
	IfcRelConnects::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_RelatedControlElements.size(); ++i )
	{
		if( m_RelatedControlElements[i] )
		{
			std::vector<weak_ptr<IfcRelFlowControlElements> >& AssignedToFlowElement_inverse = m_RelatedControlElements[i]->m_AssignedToFlowElement_inverse;
			for( auto it_AssignedToFlowElement_inverse = AssignedToFlowElement_inverse.begin(); it_AssignedToFlowElement_inverse != AssignedToFlowElement_inverse.end(); )
			{
				weak_ptr<IfcRelFlowControlElements> self_candidate_weak = *it_AssignedToFlowElement_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_AssignedToFlowElement_inverse;
					continue;
				}
				shared_ptr<IfcRelFlowControlElements> self_candidate( *it_AssignedToFlowElement_inverse );
				if( self_candidate.get() == this )
				{
					it_AssignedToFlowElement_inverse= AssignedToFlowElement_inverse.erase( it_AssignedToFlowElement_inverse );
				}
				else
				{
					++it_AssignedToFlowElement_inverse;
				}
			}
		}
	}
	if( m_RelatingFlowElement )
	{
		std::vector<weak_ptr<IfcRelFlowControlElements> >& HasControlElements_inverse = m_RelatingFlowElement->m_HasControlElements_inverse;
		for( auto it_HasControlElements_inverse = HasControlElements_inverse.begin(); it_HasControlElements_inverse != HasControlElements_inverse.end(); )
		{
			weak_ptr<IfcRelFlowControlElements> self_candidate_weak = *it_HasControlElements_inverse;
			if( self_candidate_weak.expired() )
			{
				++it_HasControlElements_inverse;
				continue;
			}
			shared_ptr<IfcRelFlowControlElements> self_candidate( *it_HasControlElements_inverse );
			if( self_candidate.get() == this )
			{
				it_HasControlElements_inverse= HasControlElements_inverse.erase( it_HasControlElements_inverse );
			}
			else
			{
				++it_HasControlElements_inverse;
			}
		}
	}
}
