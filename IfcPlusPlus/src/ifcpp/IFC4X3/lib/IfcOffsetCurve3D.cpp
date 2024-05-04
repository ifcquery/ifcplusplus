/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcCurve.h"
#include "ifcpp/IFC4X3/include/IfcDirection.h"
#include "ifcpp/IFC4X3/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4X3/include/IfcLogical.h"
#include "ifcpp/IFC4X3/include/IfcOffsetCurve3D.h"
#include "ifcpp/IFC4X3/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4X3/include/IfcStyledItem.h"

// ENTITY IfcOffsetCurve3D 
IFC4X3::IfcOffsetCurve3D::IfcOffsetCurve3D( int tag ) { m_tag = tag; }
void IFC4X3::IfcOffsetCurve3D::getStepLine( std::stringstream& stream, size_t precision ) const
{
	stream << "#" << m_tag << "=IFCOFFSETCURVE3D" << "(";
	if( m_BasisCurve ) { stream << "#" << m_BasisCurve->m_tag; } else { stream << "$"; }
	stream << ",";
	if( m_Distance ) { m_Distance->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_RefDirection ) { stream << "#" << m_RefDirection->m_tag; } else { stream << "$"; }
	stream << ");";
}
void IFC4X3::IfcOffsetCurve3D::getStepParameter( std::stringstream& stream, bool /*is_select_type*/, size_t /*precision*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcOffsetCurve3D::readStepArguments( const std::vector<std::string>& args, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){readEntityReference( args[0], m_BasisCurve, map, errorStream, entityIdNotFound );}
	if( num_args > 1 ){m_Distance = IfcLengthMeasure::createObjectFromSTEP( args[1], map, errorStream, entityIdNotFound );}
	if( num_args > 2 ){m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[2], map, errorStream, entityIdNotFound );}
	if( num_args > 3 ){readEntityReference( args[3], m_RefDirection, map, errorStream, entityIdNotFound );}
	if( num_args != 4 ){ errorStream << "Wrong parameter count for entity IfcOffsetCurve3D, expecting 4, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcOffsetCurve3D::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcOffsetCurve::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "Distance", m_Distance ) );
	vec_attributes.emplace_back( std::make_pair( "SelfIntersect", m_SelfIntersect ) );
	vec_attributes.emplace_back( std::make_pair( "RefDirection", m_RefDirection ) );
}
void IFC4X3::IfcOffsetCurve3D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcOffsetCurve::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcOffsetCurve3D::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcOffsetCurve::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcOffsetCurve3D::unlinkFromInverseCounterparts()
{
	IfcOffsetCurve::unlinkFromInverseCounterparts();
}
