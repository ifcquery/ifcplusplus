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
#include "include/IfcConnectionSurfaceGeometry.h"
#include "include/IfcSurfaceOrFaceSurface.h"

// ENTITY IfcConnectionSurfaceGeometry 
IfcConnectionSurfaceGeometry::IfcConnectionSurfaceGeometry() {}
IfcConnectionSurfaceGeometry::IfcConnectionSurfaceGeometry( int id ) { m_id = id; }
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
void IfcConnectionSurfaceGeometry::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcConnectionSurfaceGeometry, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
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
void IfcConnectionSurfaceGeometry::unlinkSelf()
{
	IfcConnectionGeometry::unlinkSelf();
}
