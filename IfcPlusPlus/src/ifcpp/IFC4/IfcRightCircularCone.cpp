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
#include "include/IfcRightCircularCone.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcRightCircularCone 
IfcRightCircularCone::IfcRightCircularCone() {}
IfcRightCircularCone::IfcRightCircularCone( int id ) { m_id = id; }
IfcRightCircularCone::~IfcRightCircularCone() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRightCircularCone::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRightCircularCone> other = dynamic_pointer_cast<IfcRightCircularCone>(other_entity);
	if( !other) { return; }
	m_Position = other->m_Position;
	m_Height = other->m_Height;
	m_BottomRadius = other->m_BottomRadius;
}
void IfcRightCircularCone::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRIGHTCIRCULARCONE" << "(";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Height ) { m_Height->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomRadius ) { m_BottomRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRightCircularCone::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRightCircularCone::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRightCircularCone, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcRightCircularCone, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Position, map );
	m_Height = IfcPositiveLengthMeasure::createObjectFromStepData( args[1] );
	m_BottomRadius = IfcPositiveLengthMeasure::createObjectFromStepData( args[2] );
}
void IfcRightCircularCone::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCsgPrimitive3D::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Height", m_Height ) );
	vec_attributes.push_back( std::make_pair( "BottomRadius", m_BottomRadius ) );
}
void IfcRightCircularCone::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRightCircularCone::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCsgPrimitive3D::setInverseCounterparts( ptr_self_entity );
}
void IfcRightCircularCone::unlinkSelf()
{
	IfcCsgPrimitive3D::unlinkSelf();
}
