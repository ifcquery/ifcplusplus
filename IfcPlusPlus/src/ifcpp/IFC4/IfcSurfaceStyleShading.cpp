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
#include "include/IfcColourRgb.h"
#include "include/IfcSurfaceStyleShading.h"

// ENTITY IfcSurfaceStyleShading 
IfcSurfaceStyleShading::IfcSurfaceStyleShading() {}
IfcSurfaceStyleShading::IfcSurfaceStyleShading( int id ) { m_id = id; }
IfcSurfaceStyleShading::~IfcSurfaceStyleShading() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSurfaceStyleShading::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSurfaceStyleShading> other = dynamic_pointer_cast<IfcSurfaceStyleShading>(other_entity);
	if( !other) { return; }
	m_SurfaceColour = other->m_SurfaceColour;
}
void IfcSurfaceStyleShading::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSURFACESTYLESHADING" << "(";
	if( m_SurfaceColour ) { stream << "#" << m_SurfaceColour->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceStyleShading::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyleShading::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSurfaceStyleShading, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcSurfaceStyleShading, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_SurfaceColour, map );
}
void IfcSurfaceStyleShading::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SurfaceColour", m_SurfaceColour ) );
}
void IfcSurfaceStyleShading::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSurfaceStyleShading::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyleShading::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
