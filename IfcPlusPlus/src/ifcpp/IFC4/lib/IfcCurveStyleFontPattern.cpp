/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcCurveStyleFontPattern.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcPositiveLengthMeasure.h"

// ENTITY IfcCurveStyleFontPattern 
IfcCurveStyleFontPattern::IfcCurveStyleFontPattern() = default;
IfcCurveStyleFontPattern::IfcCurveStyleFontPattern( int id ) { m_entity_id = id; }
IfcCurveStyleFontPattern::~IfcCurveStyleFontPattern() = default;
shared_ptr<BuildingObject> IfcCurveStyleFontPattern::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcCurveStyleFontPattern> copy_self( new IfcCurveStyleFontPattern() );
	if( m_VisibleSegmentLength ) { copy_self->m_VisibleSegmentLength = dynamic_pointer_cast<IfcLengthMeasure>( m_VisibleSegmentLength->getDeepCopy(options) ); }
	if( m_InvisibleSegmentLength ) { copy_self->m_InvisibleSegmentLength = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_InvisibleSegmentLength->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCurveStyleFontPattern::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCCURVESTYLEFONTPATTERN" << "(";
	if( m_VisibleSegmentLength ) { m_VisibleSegmentLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InvisibleSegmentLength ) { m_InvisibleSegmentLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCurveStyleFontPattern::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcCurveStyleFontPattern::toString() const { return L"IfcCurveStyleFontPattern"; }
void IfcCurveStyleFontPattern::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCurveStyleFontPattern, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_VisibleSegmentLength = IfcLengthMeasure::createObjectFromSTEP( args[0], map );
	m_InvisibleSegmentLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[1], map );
}
void IfcCurveStyleFontPattern::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "VisibleSegmentLength", m_VisibleSegmentLength );
	vec_attributes.emplace_back( "InvisibleSegmentLength", m_InvisibleSegmentLength );
}
void IfcCurveStyleFontPattern::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcCurveStyleFontPattern::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveStyleFontPattern::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}
