/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcBoolean.h"
#include "ifcpp/IFC4X3/include/IfcParameterValue.h"
#include "ifcpp/IFC4X3/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4X3/include/IfcRectangularTrimmedSurface.h"
#include "ifcpp/IFC4X3/include/IfcStyledItem.h"
#include "ifcpp/IFC4X3/include/IfcSurface.h"

// ENTITY IfcRectangularTrimmedSurface 
IFC4X3::IfcRectangularTrimmedSurface::IfcRectangularTrimmedSurface( int tag ) { m_tag = tag; }
void IFC4X3::IfcRectangularTrimmedSurface::getStepLine( std::stringstream& stream, size_t precision ) const
{
	stream << "#" << m_tag << "=IFCRECTANGULARTRIMMEDSURFACE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->m_tag; } else { stream << "$"; }
	stream << ",";
	if( m_U1 ) { m_U1->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_V1 ) { m_V1->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_U2 ) { m_U2->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_V2 ) { m_V2->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_Usense ) { m_Usense->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_Vsense ) { m_Vsense->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ");";
}
void IFC4X3::IfcRectangularTrimmedSurface::getStepParameter( std::stringstream& stream, bool /*is_select_type*/, size_t /*precision*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcRectangularTrimmedSurface::readStepArguments( const std::vector<std::string>& args, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){readEntityReference( args[0], m_BasisSurface, map, errorStream, entityIdNotFound );}
	if( num_args > 1 ){m_U1 = IfcParameterValue::createObjectFromSTEP( args[1], map, errorStream, entityIdNotFound );}
	if( num_args > 2 ){m_V1 = IfcParameterValue::createObjectFromSTEP( args[2], map, errorStream, entityIdNotFound );}
	if( num_args > 3 ){m_U2 = IfcParameterValue::createObjectFromSTEP( args[3], map, errorStream, entityIdNotFound );}
	if( num_args > 4 ){m_V2 = IfcParameterValue::createObjectFromSTEP( args[4], map, errorStream, entityIdNotFound );}
	if( num_args > 5 ){m_Usense = IfcBoolean::createObjectFromSTEP( args[5], map, errorStream, entityIdNotFound );}
	if( num_args > 6 ){m_Vsense = IfcBoolean::createObjectFromSTEP( args[6], map, errorStream, entityIdNotFound );}
	if( num_args != 7 ){ errorStream << "Wrong parameter count for entity IfcRectangularTrimmedSurface, expecting 7, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcRectangularTrimmedSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	vec_attributes.emplace_back( std::make_pair( "U1", m_U1 ) );
	vec_attributes.emplace_back( std::make_pair( "V1", m_V1 ) );
	vec_attributes.emplace_back( std::make_pair( "U2", m_U2 ) );
	vec_attributes.emplace_back( std::make_pair( "V2", m_V2 ) );
	vec_attributes.emplace_back( std::make_pair( "Usense", m_Usense ) );
	vec_attributes.emplace_back( std::make_pair( "Vsense", m_Vsense ) );
}
void IFC4X3::IfcRectangularTrimmedSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcBoundedSurface::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcRectangularTrimmedSurface::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcRectangularTrimmedSurface::unlinkFromInverseCounterparts()
{
	IfcBoundedSurface::unlinkFromInverseCounterparts();
}
