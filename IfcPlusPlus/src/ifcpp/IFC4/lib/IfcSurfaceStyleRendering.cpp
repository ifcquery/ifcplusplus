/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcColourOrFactor.h"
#include "ifcpp/IFC4/include/IfcColourRgb.h"
#include "ifcpp/IFC4/include/IfcNormalisedRatioMeasure.h"
#include "ifcpp/IFC4/include/IfcReflectanceMethodEnum.h"
#include "ifcpp/IFC4/include/IfcSpecularHighlightSelect.h"
#include "ifcpp/IFC4/include/IfcSurfaceStyleRendering.h"

// ENTITY IfcSurfaceStyleRendering 
IfcSurfaceStyleRendering::IfcSurfaceStyleRendering() = default;
IfcSurfaceStyleRendering::IfcSurfaceStyleRendering( int id ) { m_entity_id = id; }
IfcSurfaceStyleRendering::~IfcSurfaceStyleRendering() = default;
shared_ptr<BuildingObject> IfcSurfaceStyleRendering::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcSurfaceStyleRendering> copy_self( new IfcSurfaceStyleRendering() );
	if( m_SurfaceColour ) { copy_self->m_SurfaceColour = dynamic_pointer_cast<IfcColourRgb>( m_SurfaceColour->getDeepCopy(options) ); }
	if( m_Transparency ) { copy_self->m_Transparency = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Transparency->getDeepCopy(options) ); }
	if( m_DiffuseColour ) { copy_self->m_DiffuseColour = dynamic_pointer_cast<IfcColourOrFactor>( m_DiffuseColour->getDeepCopy(options) ); }
	if( m_TransmissionColour ) { copy_self->m_TransmissionColour = dynamic_pointer_cast<IfcColourOrFactor>( m_TransmissionColour->getDeepCopy(options) ); }
	if( m_DiffuseTransmissionColour ) { copy_self->m_DiffuseTransmissionColour = dynamic_pointer_cast<IfcColourOrFactor>( m_DiffuseTransmissionColour->getDeepCopy(options) ); }
	if( m_ReflectionColour ) { copy_self->m_ReflectionColour = dynamic_pointer_cast<IfcColourOrFactor>( m_ReflectionColour->getDeepCopy(options) ); }
	if( m_SpecularColour ) { copy_self->m_SpecularColour = dynamic_pointer_cast<IfcColourOrFactor>( m_SpecularColour->getDeepCopy(options) ); }
	if( m_SpecularHighlight ) { copy_self->m_SpecularHighlight = dynamic_pointer_cast<IfcSpecularHighlightSelect>( m_SpecularHighlight->getDeepCopy(options) ); }
	if( m_ReflectanceMethod ) { copy_self->m_ReflectanceMethod = dynamic_pointer_cast<IfcReflectanceMethodEnum>( m_ReflectanceMethod->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSurfaceStyleRendering::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCSURFACESTYLERENDERING" << "(";
	if( m_SurfaceColour ) { stream << "#" << m_SurfaceColour->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Transparency ) { m_Transparency->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DiffuseColour ) { m_DiffuseColour->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TransmissionColour ) { m_TransmissionColour->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_DiffuseTransmissionColour ) { m_DiffuseTransmissionColour->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ReflectionColour ) { m_ReflectionColour->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_SpecularColour ) { m_SpecularColour->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_SpecularHighlight ) { m_SpecularHighlight->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ReflectanceMethod ) { m_ReflectanceMethod->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceStyleRendering::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcSurfaceStyleRendering::toString() const { return L"IfcSurfaceStyleRendering"; }
void IfcSurfaceStyleRendering::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSurfaceStyleRendering, expecting 9, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	readEntityReference( args[0], m_SurfaceColour, map );
	m_Transparency = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[1], map );
	m_DiffuseColour = IfcColourOrFactor::createObjectFromSTEP( args[2], map );
	m_TransmissionColour = IfcColourOrFactor::createObjectFromSTEP( args[3], map );
	m_DiffuseTransmissionColour = IfcColourOrFactor::createObjectFromSTEP( args[4], map );
	m_ReflectionColour = IfcColourOrFactor::createObjectFromSTEP( args[5], map );
	m_SpecularColour = IfcColourOrFactor::createObjectFromSTEP( args[6], map );
	m_SpecularHighlight = IfcSpecularHighlightSelect::createObjectFromSTEP( args[7], map );
	m_ReflectanceMethod = IfcReflectanceMethodEnum::createObjectFromSTEP( args[8], map );
}
void IfcSurfaceStyleRendering::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcSurfaceStyleShading::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "DiffuseColour", m_DiffuseColour );
	vec_attributes.emplace_back( "TransmissionColour", m_TransmissionColour );
	vec_attributes.emplace_back( "DiffuseTransmissionColour", m_DiffuseTransmissionColour );
	vec_attributes.emplace_back( "ReflectionColour", m_ReflectionColour );
	vec_attributes.emplace_back( "SpecularColour", m_SpecularColour );
	vec_attributes.emplace_back( "SpecularHighlight", m_SpecularHighlight );
	vec_attributes.emplace_back( "ReflectanceMethod", m_ReflectanceMethod );
}
void IfcSurfaceStyleRendering::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcSurfaceStyleShading::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceStyleRendering::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcSurfaceStyleShading::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyleRendering::unlinkFromInverseCounterparts()
{
	IfcSurfaceStyleShading::unlinkFromInverseCounterparts();
}
