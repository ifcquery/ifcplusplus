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
#include "include/IfcConnectionSurfaceGeometry.h"
#include "include/IfcSurfaceOrFaceSurface.h"

// ENTITY IfcConnectionSurfaceGeometry 
IfcConnectionSurfaceGeometry::IfcConnectionSurfaceGeometry() { m_entity_enum = IFCCONNECTIONSURFACEGEOMETRY; }
IfcConnectionSurfaceGeometry::IfcConnectionSurfaceGeometry( int id ) { m_id = id; m_entity_enum = IFCCONNECTIONSURFACEGEOMETRY; }
IfcConnectionSurfaceGeometry::~IfcConnectionSurfaceGeometry() {}
shared_ptr<IfcPPObject> IfcConnectionSurfaceGeometry::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcConnectionSurfaceGeometry> copy_self( new IfcConnectionSurfaceGeometry() );
	if( m_SurfaceOnRelatingElement ) { copy_self->m_SurfaceOnRelatingElement = dynamic_pointer_cast<IfcSurfaceOrFaceSurface>( m_SurfaceOnRelatingElement->getDeepCopy(options) ); }
	if( m_SurfaceOnRelatedElement ) { copy_self->m_SurfaceOnRelatedElement = dynamic_pointer_cast<IfcSurfaceOrFaceSurface>( m_SurfaceOnRelatedElement->getDeepCopy(options) ); }
	return copy_self;
}
void IfcConnectionSurfaceGeometry::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCONNECTIONSURFACEGEOMETRY" << "(";
	if( m_SurfaceOnRelatingElement ) { m_SurfaceOnRelatingElement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_SurfaceOnRelatedElement ) { m_SurfaceOnRelatedElement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcConnectionSurfaceGeometry::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConnectionSurfaceGeometry::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcConnectionSurfaceGeometry, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_SurfaceOnRelatingElement = IfcSurfaceOrFaceSurface::createObjectFromSTEP( args[0], map );
	m_SurfaceOnRelatedElement = IfcSurfaceOrFaceSurface::createObjectFromSTEP( args[1], map );
}
void IfcConnectionSurfaceGeometry::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConnectionGeometry::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SurfaceOnRelatingElement", m_SurfaceOnRelatingElement ) );
	vec_attributes.push_back( std::make_pair( "SurfaceOnRelatedElement", m_SurfaceOnRelatedElement ) );
}
void IfcConnectionSurfaceGeometry::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConnectionGeometry::getAttributesInverse( vec_attributes_inverse );
}
void IfcConnectionSurfaceGeometry::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConnectionGeometry::setInverseCounterparts( ptr_self_entity );
}
void IfcConnectionSurfaceGeometry::unlinkFromInverseCounterparts()
{
	IfcConnectionGeometry::unlinkFromInverseCounterparts();
}
