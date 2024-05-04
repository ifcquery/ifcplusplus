/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcParameterValue.h"
#include "ifcpp/IFC4X3/include/IfcTextureVertex.h"

// ENTITY IfcTextureVertex 
IFC4X3::IfcTextureVertex::IfcTextureVertex( int tag ) { m_tag = tag; }
void IFC4X3::IfcTextureVertex::getStepLine( std::stringstream& stream, size_t precision ) const
{
	stream << "#" << m_tag << "=IFCTEXTUREVERTEX" << "(";
	writeTypeOfRealList( stream, m_Coordinates, false, precision );
	stream << ");";
}
void IFC4X3::IfcTextureVertex::getStepParameter( std::stringstream& stream, bool /*is_select_type*/, size_t /*precision*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcTextureVertex::readStepArguments( const std::vector<std::string>& args, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){readTypeOfRealList( args[0], m_Coordinates );}
	if( num_args != 1 ){ errorStream << "Wrong parameter count for entity IfcTextureVertex, expecting 1, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcTextureVertex::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcPresentationItem::getAttributes( vec_attributes );
	shared_ptr<AttributeObjectVector> Coordinates_vec_object( new AttributeObjectVector() );
	std::copy( m_Coordinates.begin(), m_Coordinates.end(), std::back_inserter( Coordinates_vec_object->m_vec ) );
	vec_attributes.emplace_back( std::make_pair( "Coordinates", Coordinates_vec_object ) );
}
void IFC4X3::IfcTextureVertex::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcTextureVertex::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcTextureVertex::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}
