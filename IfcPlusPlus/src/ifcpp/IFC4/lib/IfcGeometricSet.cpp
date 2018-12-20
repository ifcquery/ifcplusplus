/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcGeometricSet.h"
#include "ifcpp/IFC4/include/IfcGeometricSetSelect.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcGeometricSet 
IfcGeometricSet::IfcGeometricSet() = default;
IfcGeometricSet::IfcGeometricSet( int id ) { m_entity_id = id; }
IfcGeometricSet::~IfcGeometricSet() = default;
shared_ptr<BuildingObject> IfcGeometricSet::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcGeometricSet> copy_self( new IfcGeometricSet() );
	for(auto item_ii : m_Elements)
	{
			if( item_ii )
		{
			copy_self->m_Elements.push_back( dynamic_pointer_cast<IfcGeometricSetSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcGeometricSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCGEOMETRICSET" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_Elements.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcGeometricSetSelect>& type_object = m_Elements[ii];
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
void IfcGeometricSet::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcGeometricSet::toString() const { return L"IfcGeometricSet"; }
void IfcGeometricSet::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcGeometricSet, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	readSelectList( args[0], m_Elements, map );
}
void IfcGeometricSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	if( !m_Elements.empty() )
	{
		shared_ptr<AttributeObjectVector> Elements_vec_object( new AttributeObjectVector() );
		std::copy( m_Elements.begin(), m_Elements.end(), std::back_inserter( Elements_vec_object->m_vec ) );
		vec_attributes.emplace_back( "Elements", Elements_vec_object );
	}
}
void IfcGeometricSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcGeometricSet::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcGeometricSet::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
