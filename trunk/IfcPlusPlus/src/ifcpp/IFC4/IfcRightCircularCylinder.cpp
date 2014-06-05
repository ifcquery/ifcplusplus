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
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRightCircularCylinder.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcRightCircularCylinder 
IfcRightCircularCylinder::IfcRightCircularCylinder() {}
IfcRightCircularCylinder::IfcRightCircularCylinder( int id ) { m_id = id; }
IfcRightCircularCylinder::~IfcRightCircularCylinder() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRightCircularCylinder::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRightCircularCylinder> other = dynamic_pointer_cast<IfcRightCircularCylinder>(other_entity);
	if( !other) { return; }
	m_Position = other->m_Position;
	m_Height = other->m_Height;
	m_Radius = other->m_Radius;
}
void IfcRightCircularCylinder::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRIGHTCIRCULARCYLINDER" << "(";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Height ) { m_Height->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Radius ) { m_Radius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRightCircularCylinder::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRightCircularCylinder::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRightCircularCylinder, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcRightCircularCylinder, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Position, map );
	m_Height = IfcPositiveLengthMeasure::createObjectFromStepData( args[1] );
	m_Radius = IfcPositiveLengthMeasure::createObjectFromStepData( args[2] );
}
void IfcRightCircularCylinder::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCsgPrimitive3D::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Height", m_Height ) );
	vec_attributes.push_back( std::make_pair( "Radius", m_Radius ) );
}
void IfcRightCircularCylinder::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRightCircularCylinder::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCsgPrimitive3D::setInverseCounterparts( ptr_self_entity );
}
void IfcRightCircularCylinder::unlinkSelf()
{
	IfcCsgPrimitive3D::unlinkSelf();
}
