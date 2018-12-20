/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcProductRepresentation.h"
#include "ifcpp/IFC4/include/IfcRepresentation.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcProductRepresentation 
IfcProductRepresentation::IfcProductRepresentation() = default;
IfcProductRepresentation::IfcProductRepresentation( int id ) { m_entity_id = id; }
IfcProductRepresentation::~IfcProductRepresentation() = default;
shared_ptr<BuildingObject> IfcProductRepresentation::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcProductRepresentation> copy_self( new IfcProductRepresentation() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for(auto item_ii : m_Representations)
	{
			if( item_ii )
		{
			copy_self->m_Representations.push_back( dynamic_pointer_cast<IfcRepresentation>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcProductRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCPRODUCTREPRESENTATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Representations );
	stream << ");";
}
void IfcProductRepresentation::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcProductRepresentation::toString() const { return L"IfcProductRepresentation"; }
void IfcProductRepresentation::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcProductRepresentation, expecting 3, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	readEntityReferenceList( args[2], m_Representations, map );
}
void IfcProductRepresentation::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	vec_attributes.emplace_back( "Name", m_Name );
	vec_attributes.emplace_back( "Description", m_Description );
	if( !m_Representations.empty() )
	{
		shared_ptr<AttributeObjectVector> Representations_vec_object( new AttributeObjectVector() );
		std::copy( m_Representations.begin(), m_Representations.end(), std::back_inserter( Representations_vec_object->m_vec ) );
		vec_attributes.emplace_back( "Representations", Representations_vec_object );
	}
}
void IfcProductRepresentation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
}
void IfcProductRepresentation::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	shared_ptr<IfcProductRepresentation> ptr_self = dynamic_pointer_cast<IfcProductRepresentation>( ptr_self_entity );
	if( !ptr_self ) { throw BuildingException( "IfcProductRepresentation::setInverseCounterparts: type mismatch" ); }
	for(auto & m_Representation : m_Representations)
	{
		if( m_Representation )
		{
			m_Representation->m_OfProductRepresentation_inverse.push_back( ptr_self );
		}
	}
}
void IfcProductRepresentation::unlinkFromInverseCounterparts()
{
	for(auto & m_Representation : m_Representations)
	{
		if( m_Representation )
		{
			std::vector<weak_ptr<IfcProductRepresentation> >& OfProductRepresentation_inverse = m_Representation->m_OfProductRepresentation_inverse;
			for( auto it_OfProductRepresentation_inverse = OfProductRepresentation_inverse.begin(); it_OfProductRepresentation_inverse != OfProductRepresentation_inverse.end(); )
			{
				weak_ptr<IfcProductRepresentation> self_candidate_weak = *it_OfProductRepresentation_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_OfProductRepresentation_inverse;
					continue;
				}
				shared_ptr<IfcProductRepresentation> self_candidate( *it_OfProductRepresentation_inverse );
				if( self_candidate.get() == this )
				{
					it_OfProductRepresentation_inverse= OfProductRepresentation_inverse.erase( it_OfProductRepresentation_inverse );
				}
				else
				{
					++it_OfProductRepresentation_inverse;
				}
			}
		}
	}
}
