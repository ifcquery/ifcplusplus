/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcBoolean.h"
#include "ifcpp/IFC4/include/IfcFaceOuterBound.h"
#include "ifcpp/IFC4/include/IfcLoop.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcFaceOuterBound 
IfcFaceOuterBound::IfcFaceOuterBound( int id ) { m_entity_id = id; }
IfcFaceOuterBound::~IfcFaceOuterBound() {}
shared_ptr<BuildingObject> IfcFaceOuterBound::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcFaceOuterBound> copy_self( new IfcFaceOuterBound() );
	if( m_Bound ) { copy_self->m_Bound = dynamic_pointer_cast<IfcLoop>( m_Bound->getDeepCopy(options) ); }
	if( m_Orientation ) { copy_self->m_Orientation = dynamic_pointer_cast<IfcBoolean>( m_Orientation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcFaceOuterBound::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCFACEOUTERBOUND" << "(";
	if( m_Bound ) { stream << "#" << m_Bound->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Orientation ) { m_Orientation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcFaceOuterBound::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcFaceOuterBound::toString() const { return L"IfcFaceOuterBound"; }
void IfcFaceOuterBound::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcFaceOuterBound, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	readEntityReference( args[0], m_Bound, map );
	m_Orientation = IfcBoolean::createObjectFromSTEP( args[1], map );
}
void IfcFaceOuterBound::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcFaceBound::getAttributes( vec_attributes );
}
void IfcFaceOuterBound::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcFaceBound::getAttributesInverse( vec_attributes_inverse );
}
void IfcFaceOuterBound::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcFaceBound::setInverseCounterparts( ptr_self_entity );
}
void IfcFaceOuterBound::unlinkFromInverseCounterparts()
{
	IfcFaceBound::unlinkFromInverseCounterparts();
}
