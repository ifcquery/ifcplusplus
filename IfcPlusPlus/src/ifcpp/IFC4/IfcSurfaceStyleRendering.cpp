/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
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
IfcSurfaceStyleRendering::IfcSurfaceStyleRendering() {}
IfcSurfaceStyleRendering::IfcSurfaceStyleRendering( int id ) { m_id = id; }
IfcSurfaceStyleRendering::~IfcSurfaceStyleRendering() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSurfaceStyleRendering::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSurfaceStyleRendering> other = dynamic_pointer_cast<IfcSurfaceStyleRendering>(other_entity);
	if( !other) { return; }
	m_SurfaceColour = other->m_SurfaceColour;
	m_Transparency = other->m_Transparency;
	m_DiffuseColour = other->m_DiffuseColour;
	m_TransmissionColour = other->m_TransmissionColour;
	m_DiffuseTransmissionColour = other->m_DiffuseTransmissionColour;
	m_ReflectionColour = other->m_ReflectionColour;
	m_SpecularColour = other->m_SpecularColour;
	m_SpecularHighlight = other->m_SpecularHighlight;
	m_ReflectanceMethod = other->m_ReflectanceMethod;
}
void IfcSurfaceStyleRendering::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSURFACESTYLERENDERING" << "(";
	if( m_SurfaceColour ) { stream << "#" << m_SurfaceColour->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Transparency ) { m_Transparency->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DiffuseColour ) { m_DiffuseColour->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TransmissionColour ) { m_TransmissionColour->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_DiffuseTransmissionColour ) { m_DiffuseTransmissionColour->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_ReflectionColour ) { m_ReflectionColour->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_SpecularColour ) { m_SpecularColour->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_SpecularHighlight ) { m_SpecularHighlight->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_ReflectanceMethod ) { m_ReflectanceMethod->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceStyleRendering::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyleRendering::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSurfaceStyleRendering, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcSurfaceStyleRendering, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_SurfaceColour, map );
	m_Transparency = IfcNormalisedRatioMeasure::createObjectFromStepData( args[1] );
	m_DiffuseColour = IfcColourOrFactor::createObjectFromStepData( args[2], map );
	m_TransmissionColour = IfcColourOrFactor::createObjectFromStepData( args[3], map );
	m_DiffuseTransmissionColour = IfcColourOrFactor::createObjectFromStepData( args[4], map );
	m_ReflectionColour = IfcColourOrFactor::createObjectFromStepData( args[5], map );
	m_SpecularColour = IfcColourOrFactor::createObjectFromStepData( args[6], map );
	m_SpecularHighlight = IfcSpecularHighlightSelect::createObjectFromStepData( args[7], map );
	m_ReflectanceMethod = IfcReflectanceMethodEnum::createObjectFromStepData( args[8] );
}
void IfcSurfaceStyleRendering::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSurfaceStyleShading::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Transparency", m_Transparency ) );
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
}
void IfcSurfaceStyleRendering::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSurfaceStyleShading::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyleRendering::unlinkSelf()
{
	IfcSurfaceStyleShading::unlinkSelf();
}
