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
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcSurface 
IfcSurface::IfcSurface() {}
IfcSurface::IfcSurface( int id ) { m_id = id; }
IfcSurface::~IfcSurface() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSurface::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSurface> other = dynamic_pointer_cast<IfcSurface>(other_entity);
	if( !other) { return; }
}
void IfcSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSURFACE" << "(";
	stream << ");";
}
void IfcSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurface::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
}
void IfcSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcSurface::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
