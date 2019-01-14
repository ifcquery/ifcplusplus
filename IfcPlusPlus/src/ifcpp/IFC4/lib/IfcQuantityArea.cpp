/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcAreaMeasure.h"
#include "ifcpp/IFC4/include/IfcExternalReferenceRelationship.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcNamedUnit.h"
#include "ifcpp/IFC4/include/IfcPhysicalComplexQuantity.h"
#include "ifcpp/IFC4/include/IfcQuantityArea.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcQuantityArea 
IfcQuantityArea::IfcQuantityArea( int id ) { m_entity_id = id; }
IfcQuantityArea::~IfcQuantityArea() {}
shared_ptr<BuildingObject> IfcQuantityArea::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcQuantityArea> copy_self( new IfcQuantityArea() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcNamedUnit>( m_Unit->getDeepCopy(options) ); }
	if( m_AreaValue ) { copy_self->m_AreaValue = dynamic_pointer_cast<IfcAreaMeasure>( m_AreaValue->getDeepCopy(options) ); }
	if( m_Formula ) { copy_self->m_Formula = dynamic_pointer_cast<IfcLabel>( m_Formula->getDeepCopy(options) ); }
	return copy_self;
}
void IfcQuantityArea::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCQUANTITYAREA" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Unit ) { stream << "#" << m_Unit->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_AreaValue ) { m_AreaValue->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Formula ) { m_Formula->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcQuantityArea::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcQuantityArea::toString() const { return L"IfcQuantityArea"; }
void IfcQuantityArea::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcQuantityArea, expecting 5, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	readEntityReference( args[2], m_Unit, map );
	m_AreaValue = IfcAreaMeasure::createObjectFromSTEP( args[3], map );
	m_Formula = IfcLabel::createObjectFromSTEP( args[4], map );
}
void IfcQuantityArea::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcPhysicalSimpleQuantity::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "AreaValue", m_AreaValue ) );
	vec_attributes.emplace_back( std::make_pair( "Formula", m_Formula ) );
}
void IfcQuantityArea::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcPhysicalSimpleQuantity::getAttributesInverse( vec_attributes_inverse );
}
void IfcQuantityArea::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcPhysicalSimpleQuantity::setInverseCounterparts( ptr_self_entity );
}
void IfcQuantityArea::unlinkFromInverseCounterparts()
{
	IfcPhysicalSimpleQuantity::unlinkFromInverseCounterparts();
}
