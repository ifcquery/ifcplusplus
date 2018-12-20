/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcAxis2Placement.h"
#include "ifcpp/IFC4/include/IfcConic.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcConic 
IfcConic::IfcConic() = default;
IfcConic::IfcConic( int id ) { m_entity_id = id; }
IfcConic::~IfcConic() = default;
shared_ptr<BuildingObject> IfcConic::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcConic> copy_self( new IfcConic() );
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement>( m_Position->getDeepCopy(options) ); }
	return copy_self;
}
void IfcConic::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCCONIC" << "(";
	if( m_Position ) { m_Position->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcConic::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcConic::toString() const { return L"IfcConic"; }
void IfcConic::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcConic, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Position = IfcAxis2Placement::createObjectFromSTEP( args[0], map );
}
void IfcConic::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcCurve::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "Position", m_Position );
}
void IfcConic::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcConic::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcConic::unlinkFromInverseCounterparts()
{
	IfcCurve::unlinkFromInverseCounterparts();
}
