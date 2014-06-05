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
#include "include/IfcParameterValue.h"
#include "include/IfcPointOnSurface.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcPointOnSurface 
IfcPointOnSurface::IfcPointOnSurface() {}
IfcPointOnSurface::IfcPointOnSurface( int id ) { m_id = id; }
IfcPointOnSurface::~IfcPointOnSurface() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPointOnSurface::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPointOnSurface> other = dynamic_pointer_cast<IfcPointOnSurface>(other_entity);
	if( !other) { return; }
	m_BasisSurface = other->m_BasisSurface;
	m_PointParameterU = other->m_PointParameterU;
	m_PointParameterV = other->m_PointParameterV;
}
void IfcPointOnSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPOINTONSURFACE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PointParameterU ) { m_PointParameterU->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PointParameterV ) { m_PointParameterV->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPointOnSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPointOnSurface::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPointOnSurface, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcPointOnSurface, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BasisSurface, map );
	m_PointParameterU = IfcParameterValue::createObjectFromStepData( args[1] );
	m_PointParameterV = IfcParameterValue::createObjectFromStepData( args[2] );
}
void IfcPointOnSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPoint::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	vec_attributes.push_back( std::make_pair( "PointParameterU", m_PointParameterU ) );
	vec_attributes.push_back( std::make_pair( "PointParameterV", m_PointParameterV ) );
}
void IfcPointOnSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPointOnSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPoint::setInverseCounterparts( ptr_self_entity );
}
void IfcPointOnSurface::unlinkSelf()
{
	IfcPoint::unlinkSelf();
}
