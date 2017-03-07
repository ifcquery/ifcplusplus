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
void IfcSurfaceStyleShading::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
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
