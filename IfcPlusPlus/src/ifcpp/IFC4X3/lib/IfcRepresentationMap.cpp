/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcAxis2Placement.h"
#include "ifcpp/IFC4X3/include/IfcMappedItem.h"
#include "ifcpp/IFC4X3/include/IfcRepresentation.h"
#include "ifcpp/IFC4X3/include/IfcRepresentationMap.h"
#include "ifcpp/IFC4X3/include/IfcShapeAspect.h"

// ENTITY IfcRepresentationMap 
IFC4X3::IfcRepresentationMap::IfcRepresentationMap( int tag ) { m_tag = tag; }
void IFC4X3::IfcRepresentationMap::getStepLine( std::stringstream& stream, size_t precision ) const
{
	stream << "#" << m_tag << "=IFCREPRESENTATIONMAP" << "(";
	if( m_MappingOrigin ) { m_MappingOrigin->getStepParameter( stream, true, precision ); } else { stream << "$" ; }
	stream << ",";
	if( m_MappedRepresentation ) { stream << "#" << m_MappedRepresentation->m_tag; } else { stream << "$"; }
	stream << ");";
}
void IFC4X3::IfcRepresentationMap::getStepParameter( std::stringstream& stream, bool /*is_select_type*/, size_t /*precision*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcRepresentationMap::readStepArguments( const std::vector<std::string>& args, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){m_MappingOrigin = IfcAxis2Placement::createObjectFromSTEP( args[0], map, errorStream, entityIdNotFound );}
	if( num_args > 1 ){readEntityReference( args[1], m_MappedRepresentation, map, errorStream, entityIdNotFound );}
	if( num_args != 2 ){ errorStream << "Wrong parameter count for entity IfcRepresentationMap, expecting 2, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcRepresentationMap::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	vec_attributes.emplace_back( std::make_pair( "MappingOrigin", m_MappingOrigin ) );
	vec_attributes.emplace_back( std::make_pair( "MappedRepresentation", m_MappedRepresentation ) );
}
void IFC4X3::IfcRepresentationMap::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	shared_ptr<AttributeObjectVector> HasShapeAspects_inverse_vec_obj( new AttributeObjectVector() );
	for( size_t i=0; i<m_HasShapeAspects_inverse.size(); ++i )
	{
		if( !m_HasShapeAspects_inverse[i].expired() )
		{
			HasShapeAspects_inverse_vec_obj->m_vec.emplace_back( shared_ptr<IfcShapeAspect>( m_HasShapeAspects_inverse[i] ) );
		}
	}
	vec_attributes_inverse.emplace_back( std::make_pair( "HasShapeAspects_inverse", HasShapeAspects_inverse_vec_obj ) );
	shared_ptr<AttributeObjectVector> MapUsage_inverse_vec_obj( new AttributeObjectVector() );
	for( size_t i=0; i<m_MapUsage_inverse.size(); ++i )
	{
		if( !m_MapUsage_inverse[i].expired() )
		{
			MapUsage_inverse_vec_obj->m_vec.emplace_back( shared_ptr<IfcMappedItem>( m_MapUsage_inverse[i] ) );
		}
	}
	vec_attributes_inverse.emplace_back( std::make_pair( "MapUsage_inverse", MapUsage_inverse_vec_obj ) );
}
void IFC4X3::IfcRepresentationMap::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	shared_ptr<IfcRepresentationMap> ptr_self = dynamic_pointer_cast<IfcRepresentationMap>( ptr_self_entity );
	if( !ptr_self ) { throw BuildingException( "IfcRepresentationMap::setInverseCounterparts: type mismatch" ); }
	if( m_MappedRepresentation )
	{
		m_MappedRepresentation->m_RepresentationMap_inverse.emplace_back( ptr_self );
	}
}
void IFC4X3::IfcRepresentationMap::unlinkFromInverseCounterparts()
{
	if( m_MappedRepresentation )
	{
		std::vector<weak_ptr<IfcRepresentationMap> >& RepresentationMap_inverse = m_MappedRepresentation->m_RepresentationMap_inverse;
		for( auto it_RepresentationMap_inverse = RepresentationMap_inverse.begin(); it_RepresentationMap_inverse != RepresentationMap_inverse.end(); )
		{
			weak_ptr<IfcRepresentationMap> self_candidate_weak = *it_RepresentationMap_inverse;
			if( self_candidate_weak.expired() )
			{
				++it_RepresentationMap_inverse;
				continue;
			}
			shared_ptr<IfcRepresentationMap> self_candidate( *it_RepresentationMap_inverse );
			if( self_candidate.get() == this )
			{
				it_RepresentationMap_inverse= RepresentationMap_inverse.erase( it_RepresentationMap_inverse );
			}
			else
			{
				++it_RepresentationMap_inverse;
			}
		}
	}
}
