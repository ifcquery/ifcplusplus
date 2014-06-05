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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcDirection.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcAxis2Placement3D 
IfcAxis2Placement3D::IfcAxis2Placement3D() {}
IfcAxis2Placement3D::IfcAxis2Placement3D( int id ) { m_id = id; }
IfcAxis2Placement3D::~IfcAxis2Placement3D() {}

// method setEntity takes over all attributes from another instance of the class
void IfcAxis2Placement3D::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcAxis2Placement3D> other = dynamic_pointer_cast<IfcAxis2Placement3D>(other_entity);
	if( !other) { return; }
	m_Location = other->m_Location;
	m_Axis = other->m_Axis;
	m_RefDirection = other->m_RefDirection;
}
void IfcAxis2Placement3D::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCAXIS2PLACEMENT3D" << "(";
	if( m_Location ) { stream << "#" << m_Location->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Axis ) { stream << "#" << m_Axis->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RefDirection ) { stream << "#" << m_RefDirection->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcAxis2Placement3D::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAxis2Placement3D::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcAxis2Placement3D, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcAxis2Placement3D, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Location, map );
	readEntityReference( args[1], m_Axis, map );
	readEntityReference( args[2], m_RefDirection, map );
}
void IfcAxis2Placement3D::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPlacement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Axis", m_Axis ) );
	vec_attributes.push_back( std::make_pair( "RefDirection", m_RefDirection ) );
}
void IfcAxis2Placement3D::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcAxis2Placement3D::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPlacement::setInverseCounterparts( ptr_self_entity );
}
void IfcAxis2Placement3D::unlinkSelf()
{
	IfcPlacement::unlinkSelf();
}
