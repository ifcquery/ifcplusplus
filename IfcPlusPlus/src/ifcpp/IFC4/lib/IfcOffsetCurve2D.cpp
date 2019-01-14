/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcCurve.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcLogical.h"
#include "ifcpp/IFC4/include/IfcOffsetCurve2D.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcOffsetCurve2D 
IfcOffsetCurve2D::IfcOffsetCurve2D( int id ) { m_entity_id = id; }
IfcOffsetCurve2D::~IfcOffsetCurve2D() {}
shared_ptr<BuildingObject> IfcOffsetCurve2D::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcOffsetCurve2D> copy_self( new IfcOffsetCurve2D() );
	if( m_BasisCurve ) { copy_self->m_BasisCurve = dynamic_pointer_cast<IfcCurve>( m_BasisCurve->getDeepCopy(options) ); }
	if( m_Distance ) { copy_self->m_Distance = dynamic_pointer_cast<IfcLengthMeasure>( m_Distance->getDeepCopy(options) ); }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	return copy_self;
}
void IfcOffsetCurve2D::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCOFFSETCURVE2D" << "(";
	if( m_BasisCurve ) { stream << "#" << m_BasisCurve->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Distance ) { m_Distance->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcOffsetCurve2D::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcOffsetCurve2D::toString() const { return L"IfcOffsetCurve2D"; }
void IfcOffsetCurve2D::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcOffsetCurve2D, expecting 3, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	readEntityReference( args[0], m_BasisCurve, map );
	m_Distance = IfcLengthMeasure::createObjectFromSTEP( args[1], map );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[2], map );
}
void IfcOffsetCurve2D::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcCurve::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "BasisCurve", m_BasisCurve ) );
	vec_attributes.emplace_back( std::make_pair( "Distance", m_Distance ) );
	vec_attributes.emplace_back( std::make_pair( "SelfIntersect", m_SelfIntersect ) );
}
void IfcOffsetCurve2D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcOffsetCurve2D::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcOffsetCurve2D::unlinkFromInverseCounterparts()
{
	IfcCurve::unlinkFromInverseCounterparts();
}
