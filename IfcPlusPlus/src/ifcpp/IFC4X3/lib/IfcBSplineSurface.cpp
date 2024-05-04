/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcBSplineSurface.h"
#include "ifcpp/IFC4X3/include/IfcBSplineSurfaceForm.h"
#include "ifcpp/IFC4X3/include/IfcCartesianPoint.h"
#include "ifcpp/IFC4X3/include/IfcInteger.h"
#include "ifcpp/IFC4X3/include/IfcLogical.h"
#include "ifcpp/IFC4X3/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4X3/include/IfcStyledItem.h"

// ENTITY IfcBSplineSurface 
IFC4X3::IfcBSplineSurface::IfcBSplineSurface( int tag ) { m_tag = tag; }
void IFC4X3::IfcBSplineSurface::getStepLine( std::stringstream& stream, size_t precision ) const
{
	stream << "#" << m_tag << "=IFCBSPLINESURFACE" << "(";
	if( m_UDegree ) { m_UDegree->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_VDegree ) { m_VDegree->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	writeEntityList2D( stream, m_ControlPointsList );
	stream << ",";
	if( m_SurfaceForm ) { m_SurfaceForm->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_UClosed ) { m_UClosed->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_VClosed ) { m_VClosed->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ");";
}
void IFC4X3::IfcBSplineSurface::getStepParameter( std::stringstream& stream, bool /*is_select_type*/, size_t /*precision*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcBSplineSurface::readStepArguments( const std::vector<std::string>& args, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){m_UDegree = IfcInteger::createObjectFromSTEP( args[0], map, errorStream, entityIdNotFound );}
	if( num_args > 1 ){m_VDegree = IfcInteger::createObjectFromSTEP( args[1], map, errorStream, entityIdNotFound );}
	if( num_args > 2 ){readEntityReferenceList2D( args[2], m_ControlPointsList, map, errorStream, entityIdNotFound );}
	if( num_args > 3 ){m_SurfaceForm = IfcBSplineSurfaceForm::createObjectFromSTEP( args[3], map, errorStream, entityIdNotFound );}
	if( num_args > 4 ){m_UClosed = IfcLogical::createObjectFromSTEP( args[4], map, errorStream, entityIdNotFound );}
	if( num_args > 5 ){m_VClosed = IfcLogical::createObjectFromSTEP( args[5], map, errorStream, entityIdNotFound );}
	if( num_args > 6 ){m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[6], map, errorStream, entityIdNotFound );}
	if( num_args != 7 ){ errorStream << "Wrong parameter count for entity IfcBSplineSurface, expecting 7, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcBSplineSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "UDegree", m_UDegree ) );
	vec_attributes.emplace_back( std::make_pair( "VDegree", m_VDegree ) );
	shared_ptr<AttributeObjectVector> ControlPointsList_vector( new AttributeObjectVector() );
	vec_attributes.emplace_back( std::make_pair( "ControlPointsList", ControlPointsList_vector ) );
	for( size_t ii=0; ii<m_ControlPointsList.size(); ++ii )
	{
		const std::vector<shared_ptr<IfcCartesianPoint> >& vec_ii = m_ControlPointsList[ii];
		shared_ptr<AttributeObjectVector> inner_vector( new AttributeObjectVector() );
		ControlPointsList_vector->m_vec.push_back( inner_vector );
		std::copy(vec_ii.begin(), vec_ii.end(), std::back_inserter(inner_vector->m_vec));
	}
	vec_attributes.emplace_back( std::make_pair( "SurfaceForm", m_SurfaceForm ) );
	vec_attributes.emplace_back( std::make_pair( "UClosed", m_UClosed ) );
	vec_attributes.emplace_back( std::make_pair( "VClosed", m_VClosed ) );
	vec_attributes.emplace_back( std::make_pair( "SelfIntersect", m_SelfIntersect ) );
}
void IFC4X3::IfcBSplineSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcBoundedSurface::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcBSplineSurface::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcBSplineSurface::unlinkFromInverseCounterparts()
{
	IfcBoundedSurface::unlinkFromInverseCounterparts();
}
