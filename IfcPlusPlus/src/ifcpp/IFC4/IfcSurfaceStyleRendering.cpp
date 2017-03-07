/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcColourOrFactor.h"
#include "include/IfcColourRgb.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcReflectanceMethodEnum.h"
#include "include/IfcSpecularHighlightSelect.h"
#include "include/IfcSurfaceStyleRendering.h"

// ENTITY IfcSurfaceStyleRendering 
IfcSurfaceStyleRendering::IfcSurfaceStyleRendering() { m_entity_enum = IFCSURFACESTYLERENDERING; }
IfcSurfaceStyleRendering::IfcSurfaceStyleRendering( int id ) { m_id = id; m_entity_enum = IFCSURFACESTYLERENDERING; }
IfcSurfaceStyleRendering::~IfcSurfaceStyleRendering() {}
shared_ptr<IfcPPObject> IfcSurfaceStyleRendering::getDeepCopy( IfcPPCopyOptions& options )
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
	stream << "#" << m_id << "= IFCSURFACESTYLERENDERING" << "(";
	if( m_SurfaceColour ) { stream << "#" << m_SurfaceColour->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Transparency ) { m_Transparency->getStepParameter( stream ); } else { stream << "*"; }
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
void IfcSurfaceStyleRendering::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyleRendering::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSurfaceStyleRendering, expecting 9, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_SurfaceColour, map );
	m_Transparency = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[1] );
	m_DiffuseColour = IfcColourOrFactor::createObjectFromSTEP( args[2], map );
	m_TransmissionColour = IfcColourOrFactor::createObjectFromSTEP( args[3], map );
	m_DiffuseTransmissionColour = IfcColourOrFactor::createObjectFromSTEP( args[4], map );
	m_ReflectionColour = IfcColourOrFactor::createObjectFromSTEP( args[5], map );
	m_SpecularColour = IfcColourOrFactor::createObjectFromSTEP( args[6], map );
	m_SpecularHighlight = IfcSpecularHighlightSelect::createObjectFromSTEP( args[7], map );
	m_ReflectanceMethod = IfcReflectanceMethodEnum::createObjectFromSTEP( args[8] );
}
void IfcSurfaceStyleRendering::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSurfaceStyleShading::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "DiffuseColour", m_DiffuseColour ) );
	vec_attributes.push_back( std::make_pair( "TransmissionColour", m_TransmissionColour ) );
	vec_attributes.push_back( std::make_pair( "DiffuseTransmissionColour", m_DiffuseTransmissionColour ) );
	vec_attributes.push_back( std::make_pair( "ReflectionColour", m_ReflectionColour ) );
	vec_attributes.push_back( std::make_pair( "SpecularColour", m_SpecularColour ) );
	vec_attributes.push_back( std::make_pair( "SpecularHighlight", m_SpecularHighlight ) );
	vec_attributes.push_back( std::make_pair( "ReflectanceMethod", m_ReflectanceMethod ) );
}
void IfcSurfaceStyleRendering::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSurfaceStyleShading::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceStyleRendering::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSurfaceStyleShading::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyleRendering::unlinkFromInverseCounterparts()
{
	IfcSurfaceStyleShading::unlinkFromInverseCounterparts();
}
