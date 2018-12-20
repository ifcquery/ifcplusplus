/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcDirection.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"
#include "ifcpp/IFC4/include/IfcVector.h"

// ENTITY IfcVector 
IfcVector::IfcVector() = default;
IfcVector::IfcVector( int id ) { m_entity_id = id; }
IfcVector::~IfcVector() = default;
shared_ptr<BuildingObject> IfcVector::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcVector> copy_self( new IfcVector() );
	if( m_Orientation ) { copy_self->m_Orientation = dynamic_pointer_cast<IfcDirection>( m_Orientation->getDeepCopy(options) ); }
	if( m_Magnitude ) { copy_self->m_Magnitude = dynamic_pointer_cast<IfcLengthMeasure>( m_Magnitude->getDeepCopy(options) ); }
	return copy_self;
}
void IfcVector::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCVECTOR" << "(";
	if( m_Orientation ) { stream << "#" << m_Orientation->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Magnitude ) { m_Magnitude->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcVector::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcVector::toString() const { return L"IfcVector"; }
void IfcVector::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcVector, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	readEntityReference( args[0], m_Orientation, map );
	m_Magnitude = IfcLengthMeasure::createObjectFromSTEP( args[1], map );
}
void IfcVector::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "Orientation", m_Orientation );
	vec_attributes.emplace_back( "Magnitude", m_Magnitude );
}
void IfcVector::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcVector::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcVector::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
