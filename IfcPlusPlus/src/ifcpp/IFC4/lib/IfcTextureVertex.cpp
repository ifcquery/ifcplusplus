/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcParameterValue.h"
#include "ifcpp/IFC4/include/IfcTextureVertex.h"

// ENTITY IfcTextureVertex 
IfcTextureVertex::IfcTextureVertex() = default;
IfcTextureVertex::IfcTextureVertex( int id ) { m_entity_id = id; }
IfcTextureVertex::~IfcTextureVertex() = default;
shared_ptr<BuildingObject> IfcTextureVertex::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcTextureVertex> copy_self( new IfcTextureVertex() );
	for(auto item_ii : m_Coordinates)
	{
			if( item_ii )
		{
			copy_self->m_Coordinates.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTextureVertex::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCTEXTUREVERTEX" << "(";
	writeNumericTypeList( stream, m_Coordinates );
	stream << ");";
}
void IfcTextureVertex::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcTextureVertex::toString() const { return L"IfcTextureVertex"; }
void IfcTextureVertex::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTextureVertex, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	readTypeOfRealList( args[0], m_Coordinates );
}
void IfcTextureVertex::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcPresentationItem::getAttributes( vec_attributes );
	if( !m_Coordinates.empty() )
	{
		shared_ptr<AttributeObjectVector> Coordinates_vec_object( new AttributeObjectVector() );
		std::copy( m_Coordinates.begin(), m_Coordinates.end(), std::back_inserter( Coordinates_vec_object->m_vec ) );
		vec_attributes.emplace_back( "Coordinates", Coordinates_vec_object );
	}
}
void IfcTextureVertex::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextureVertex::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTextureVertex::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}
