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
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProfileDef.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurfaceOfRevolution.h"

// ENTITY IfcSurfaceOfRevolution 
IfcSurfaceOfRevolution::IfcSurfaceOfRevolution() {}
IfcSurfaceOfRevolution::IfcSurfaceOfRevolution( int id ) { m_id = id; }
IfcSurfaceOfRevolution::~IfcSurfaceOfRevolution() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSurfaceOfRevolution::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSurfaceOfRevolution> other = dynamic_pointer_cast<IfcSurfaceOfRevolution>(other_entity);
	if( !other) { return; }
	m_SweptCurve = other->m_SweptCurve;
	m_Position = other->m_Position;
	m_AxisPosition = other->m_AxisPosition;
}
void IfcSurfaceOfRevolution::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSURFACEOFREVOLUTION" << "(";
	if( m_SweptCurve ) { stream << "#" << m_SweptCurve->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AxisPosition ) { stream << "#" << m_AxisPosition->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceOfRevolution::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceOfRevolution::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSurfaceOfRevolution, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcSurfaceOfRevolution, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_SweptCurve, map );
	readEntityReference( args[1], m_Position, map );
	readEntityReference( args[2], m_AxisPosition, map );
}
void IfcSurfaceOfRevolution::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSweptSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "AxisPosition", m_AxisPosition ) );
}
void IfcSurfaceOfRevolution::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSurfaceOfRevolution::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSweptSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceOfRevolution::unlinkSelf()
{
	IfcSweptSurface::unlinkSelf();
}
