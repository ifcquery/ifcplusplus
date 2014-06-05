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
#include "include/IfcBoundedCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBoundedCurve 
IfcBoundedCurve::IfcBoundedCurve() {}
IfcBoundedCurve::IfcBoundedCurve( int id ) { m_id = id; }
IfcBoundedCurve::~IfcBoundedCurve() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBoundedCurve::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBoundedCurve> other = dynamic_pointer_cast<IfcBoundedCurve>(other_entity);
	if( !other) { return; }
}
void IfcBoundedCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBOUNDEDCURVE" << "(";
	stream << ");";
}
void IfcBoundedCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundedCurve::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcBoundedCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCurve::getAttributes( vec_attributes );
}
void IfcBoundedCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBoundedCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundedCurve::unlinkSelf()
{
	IfcCurve::unlinkSelf();
}
