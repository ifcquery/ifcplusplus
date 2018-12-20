/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcConnectionCurveGeometry.h"
#include "ifcpp/IFC4/include/IfcCurveOrEdgeCurve.h"

// ENTITY IfcConnectionCurveGeometry 
IfcConnectionCurveGeometry::IfcConnectionCurveGeometry() = default;
IfcConnectionCurveGeometry::IfcConnectionCurveGeometry( int id ) { m_entity_id = id; }
IfcConnectionCurveGeometry::~IfcConnectionCurveGeometry() = default;
shared_ptr<BuildingObject> IfcConnectionCurveGeometry::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcConnectionCurveGeometry> copy_self( new IfcConnectionCurveGeometry() );
	if( m_CurveOnRelatingElement ) { copy_self->m_CurveOnRelatingElement = dynamic_pointer_cast<IfcCurveOrEdgeCurve>( m_CurveOnRelatingElement->getDeepCopy(options) ); }
	if( m_CurveOnRelatedElement ) { copy_self->m_CurveOnRelatedElement = dynamic_pointer_cast<IfcCurveOrEdgeCurve>( m_CurveOnRelatedElement->getDeepCopy(options) ); }
	return copy_self;
}
void IfcConnectionCurveGeometry::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCCONNECTIONCURVEGEOMETRY" << "(";
	if( m_CurveOnRelatingElement ) { m_CurveOnRelatingElement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_CurveOnRelatedElement ) { m_CurveOnRelatedElement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcConnectionCurveGeometry::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcConnectionCurveGeometry::toString() const { return L"IfcConnectionCurveGeometry"; }
void IfcConnectionCurveGeometry::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcConnectionCurveGeometry, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_CurveOnRelatingElement = IfcCurveOrEdgeCurve::createObjectFromSTEP( args[0], map );
	m_CurveOnRelatedElement = IfcCurveOrEdgeCurve::createObjectFromSTEP( args[1], map );
}
void IfcConnectionCurveGeometry::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcConnectionGeometry::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "CurveOnRelatingElement", m_CurveOnRelatingElement );
	vec_attributes.emplace_back( "CurveOnRelatedElement", m_CurveOnRelatedElement );
}
void IfcConnectionCurveGeometry::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcConnectionGeometry::getAttributesInverse( vec_attributes_inverse );
}
void IfcConnectionCurveGeometry::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcConnectionGeometry::setInverseCounterparts( ptr_self_entity );
}
void IfcConnectionCurveGeometry::unlinkFromInverseCounterparts()
{
	IfcConnectionGeometry::unlinkFromInverseCounterparts();
}
