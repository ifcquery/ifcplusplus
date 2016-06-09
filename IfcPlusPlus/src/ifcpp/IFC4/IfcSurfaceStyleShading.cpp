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
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcColourRgb.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcSurfaceStyleShading.h"

// ENTITY IfcSurfaceStyleShading 
IfcSurfaceStyleShading::IfcSurfaceStyleShading() { m_entity_enum = IFCSURFACESTYLESHADING; }
IfcSurfaceStyleShading::IfcSurfaceStyleShading( int id ) { m_id = id; m_entity_enum = IFCSURFACESTYLESHADING; }
IfcSurfaceStyleShading::~IfcSurfaceStyleShading() {}
shared_ptr<IfcPPObject> IfcSurfaceStyleShading::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceStyleShading> copy_self( new IfcSurfaceStyleShading() );
	if( m_SurfaceColour ) { copy_self->m_SurfaceColour = dynamic_pointer_cast<IfcColourRgb>( m_SurfaceColour->getDeepCopy(options) ); }
	if( m_Transparency ) { copy_self->m_Transparency = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Transparency->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSurfaceStyleShading::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSURFACESTYLESHADING" << "(";
	if( m_SurfaceColour ) { stream << "#" << m_SurfaceColour->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Transparency ) { m_Transparency->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceStyleShading::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyleShading::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSurfaceStyleShading, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_SurfaceColour, map );
	m_Transparency = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[1] );
}
void IfcSurfaceStyleShading::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SurfaceColour", m_SurfaceColour ) );
	vec_attributes.push_back( std::make_pair( "Transparency", m_Transparency ) );
}
void IfcSurfaceStyleShading::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceStyleShading::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyleShading::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}
