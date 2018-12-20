/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcCompositeCurveSegment.h"
#include "ifcpp/IFC4/include/IfcLogical.h"
#include "ifcpp/IFC4/include/IfcOuterBoundaryCurve.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcOuterBoundaryCurve 
IfcOuterBoundaryCurve::IfcOuterBoundaryCurve() = default;
IfcOuterBoundaryCurve::IfcOuterBoundaryCurve( int id ) { m_entity_id = id; }
IfcOuterBoundaryCurve::~IfcOuterBoundaryCurve() = default;
shared_ptr<BuildingObject> IfcOuterBoundaryCurve::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcOuterBoundaryCurve> copy_self( new IfcOuterBoundaryCurve() );
	for(auto item_ii : m_Segments)
	{
			if( item_ii )
		{
			copy_self->m_Segments.push_back( dynamic_pointer_cast<IfcCompositeCurveSegment>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	return copy_self;
}
void IfcOuterBoundaryCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCOUTERBOUNDARYCURVE" << "(";
	writeEntityList( stream, m_Segments );
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcOuterBoundaryCurve::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcOuterBoundaryCurve::toString() const { return L"IfcOuterBoundaryCurve"; }
void IfcOuterBoundaryCurve::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcOuterBoundaryCurve, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	readEntityReferenceList( args[0], m_Segments, map );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[1], map );
}
void IfcOuterBoundaryCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcBoundaryCurve::getAttributes( vec_attributes );
}
void IfcOuterBoundaryCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcBoundaryCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcOuterBoundaryCurve::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcBoundaryCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcOuterBoundaryCurve::unlinkFromInverseCounterparts()
{
	IfcBoundaryCurve::unlinkFromInverseCounterparts();
}
