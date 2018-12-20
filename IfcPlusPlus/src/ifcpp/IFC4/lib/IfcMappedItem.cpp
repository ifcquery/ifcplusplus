/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcCartesianTransformationOperator.h"
#include "ifcpp/IFC4/include/IfcMappedItem.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcRepresentationMap.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcMappedItem 
IfcMappedItem::IfcMappedItem() = default;
IfcMappedItem::IfcMappedItem( int id ) { m_entity_id = id; }
IfcMappedItem::~IfcMappedItem() = default;
shared_ptr<BuildingObject> IfcMappedItem::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcMappedItem> copy_self( new IfcMappedItem() );
	if( m_MappingSource ) { copy_self->m_MappingSource = dynamic_pointer_cast<IfcRepresentationMap>( m_MappingSource->getDeepCopy(options) ); }
	if( m_MappingTarget ) { copy_self->m_MappingTarget = dynamic_pointer_cast<IfcCartesianTransformationOperator>( m_MappingTarget->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMappedItem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCMAPPEDITEM" << "(";
	if( m_MappingSource ) { stream << "#" << m_MappingSource->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_MappingTarget ) { stream << "#" << m_MappingTarget->m_entity_id; } else { stream << "$"; }
	stream << ");";
}
void IfcMappedItem::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcMappedItem::toString() const { return L"IfcMappedItem"; }
void IfcMappedItem::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMappedItem, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	readEntityReference( args[0], m_MappingSource, map );
	readEntityReference( args[1], m_MappingTarget, map );
}
void IfcMappedItem::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "MappingSource", m_MappingSource );
	vec_attributes.emplace_back( "MappingTarget", m_MappingTarget );
}
void IfcMappedItem::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcMappedItem::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcRepresentationItem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMappedItem> ptr_self = dynamic_pointer_cast<IfcMappedItem>( ptr_self_entity );
	if( !ptr_self ) { throw BuildingException( "IfcMappedItem::setInverseCounterparts: type mismatch" ); }
	if( m_MappingSource )
	{
		m_MappingSource->m_MapUsage_inverse.push_back( ptr_self );
	}
}
void IfcMappedItem::unlinkFromInverseCounterparts()
{
	IfcRepresentationItem::unlinkFromInverseCounterparts();
	if( m_MappingSource )
	{
		std::vector<weak_ptr<IfcMappedItem> >& MapUsage_inverse = m_MappingSource->m_MapUsage_inverse;
		for( auto it_MapUsage_inverse = MapUsage_inverse.begin(); it_MapUsage_inverse != MapUsage_inverse.end(); )
		{
			weak_ptr<IfcMappedItem> self_candidate_weak = *it_MapUsage_inverse;
			if( self_candidate_weak.expired() )
			{
				++it_MapUsage_inverse;
				continue;
			}
			shared_ptr<IfcMappedItem> self_candidate( *it_MapUsage_inverse );
			if( self_candidate.get() == this )
			{
				it_MapUsage_inverse= MapUsage_inverse.erase( it_MapUsage_inverse );
			}
			else
			{
				++it_MapUsage_inverse;
			}
		}
	}
}
