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
#include "include/IfcAxis1Placement.h"
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProfileDef.h"
#include "include/IfcRevolvedAreaSolid.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcRevolvedAreaSolid 
IfcRevolvedAreaSolid::IfcRevolvedAreaSolid() {}
IfcRevolvedAreaSolid::IfcRevolvedAreaSolid( int id ) { m_id = id; }
IfcRevolvedAreaSolid::~IfcRevolvedAreaSolid() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRevolvedAreaSolid::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRevolvedAreaSolid> other = dynamic_pointer_cast<IfcRevolvedAreaSolid>(other_entity);
	if( !other) { return; }
	m_SweptArea = other->m_SweptArea;
	m_Position = other->m_Position;
	m_Axis = other->m_Axis;
	m_Angle = other->m_Angle;
}
void IfcRevolvedAreaSolid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCREVOLVEDAREASOLID" << "(";
	if( m_SweptArea ) { stream << "#" << m_SweptArea->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Axis ) { stream << "#" << m_Axis->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Angle ) { m_Angle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRevolvedAreaSolid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRevolvedAreaSolid::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRevolvedAreaSolid, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcRevolvedAreaSolid, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_SweptArea, map );
	readEntityReference( args[1], m_Position, map );
	readEntityReference( args[2], m_Axis, map );
	m_Angle = IfcPlaneAngleMeasure::createObjectFromStepData( args[3] );
}
void IfcRevolvedAreaSolid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSweptAreaSolid::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Axis", m_Axis ) );
	vec_attributes.push_back( std::make_pair( "Angle", m_Angle ) );
}
void IfcRevolvedAreaSolid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRevolvedAreaSolid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSweptAreaSolid::setInverseCounterparts( ptr_self_entity );
}
void IfcRevolvedAreaSolid::unlinkSelf()
{
	IfcSweptAreaSolid::unlinkSelf();
}
