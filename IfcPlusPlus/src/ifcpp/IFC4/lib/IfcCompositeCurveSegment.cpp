/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcBoolean.h"
#include "ifcpp/IFC4/include/IfcCompositeCurve.h"
#include "ifcpp/IFC4/include/IfcCompositeCurveSegment.h"
#include "ifcpp/IFC4/include/IfcCurve.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"
#include "ifcpp/IFC4/include/IfcTransitionCode.h"

// ENTITY IfcCompositeCurveSegment 
IfcCompositeCurveSegment::IfcCompositeCurveSegment( int id ) { m_entity_id = id; }
IfcCompositeCurveSegment::~IfcCompositeCurveSegment() {}
shared_ptr<BuildingObject> IfcCompositeCurveSegment::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcCompositeCurveSegment> copy_self( new IfcCompositeCurveSegment() );
	if( m_Transition ) { copy_self->m_Transition = dynamic_pointer_cast<IfcTransitionCode>( m_Transition->getDeepCopy(options) ); }
	if( m_SameSense ) { copy_self->m_SameSense = dynamic_pointer_cast<IfcBoolean>( m_SameSense->getDeepCopy(options) ); }
	if( m_ParentCurve ) { copy_self->m_ParentCurve = dynamic_pointer_cast<IfcCurve>( m_ParentCurve->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCompositeCurveSegment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCCOMPOSITECURVESEGMENT" << "(";
	if( m_Transition ) { m_Transition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SameSense ) { m_SameSense->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ParentCurve ) { stream << "#" << m_ParentCurve->m_entity_id; } else { stream << "$"; }
	stream << ");";
}
void IfcCompositeCurveSegment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcCompositeCurveSegment::toString() const { return L"IfcCompositeCurveSegment"; }
void IfcCompositeCurveSegment::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCompositeCurveSegment, expecting 3, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	m_Transition = IfcTransitionCode::createObjectFromSTEP( args[0], map );
	m_SameSense = IfcBoolean::createObjectFromSTEP( args[1], map );
	readEntityReference( args[2], m_ParentCurve, map );
}
void IfcCompositeCurveSegment::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "Transition", m_Transition ) );
	vec_attributes.emplace_back( std::make_pair( "SameSense", m_SameSense ) );
	vec_attributes.emplace_back( std::make_pair( "ParentCurve", m_ParentCurve ) );
}
void IfcCompositeCurveSegment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
	if( !m_UsingCurves_inverse.empty() )
	{
		shared_ptr<AttributeObjectVector> UsingCurves_inverse_vec_obj( new AttributeObjectVector() );
		for( size_t i=0; i<m_UsingCurves_inverse.size(); ++i )
		{
			if( !m_UsingCurves_inverse[i].expired() )
			{
				UsingCurves_inverse_vec_obj->m_vec.emplace_back( shared_ptr<IfcCompositeCurve>( m_UsingCurves_inverse[i] ) );
			}
		}
		vec_attributes_inverse.emplace_back( std::make_pair( "UsingCurves_inverse", UsingCurves_inverse_vec_obj ) );
	}
}
void IfcCompositeCurveSegment::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcCompositeCurveSegment::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
