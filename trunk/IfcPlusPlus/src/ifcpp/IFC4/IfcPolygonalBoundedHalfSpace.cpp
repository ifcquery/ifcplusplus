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
#include "include/IfcBoundedCurve.h"
#include "include/IfcPolygonalBoundedHalfSpace.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcPolygonalBoundedHalfSpace 
IfcPolygonalBoundedHalfSpace::IfcPolygonalBoundedHalfSpace() {}
IfcPolygonalBoundedHalfSpace::IfcPolygonalBoundedHalfSpace( int id ) { m_id = id; }
IfcPolygonalBoundedHalfSpace::~IfcPolygonalBoundedHalfSpace() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPolygonalBoundedHalfSpace::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPolygonalBoundedHalfSpace> other = dynamic_pointer_cast<IfcPolygonalBoundedHalfSpace>(other_entity);
	if( !other) { return; }
	m_BaseSurface = other->m_BaseSurface;
	m_AgreementFlag = other->m_AgreementFlag;
	m_Position = other->m_Position;
	m_PolygonalBoundary = other->m_PolygonalBoundary;
}
void IfcPolygonalBoundedHalfSpace::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPOLYGONALBOUNDEDHALFSPACE" << "(";
	if( m_BaseSurface ) { stream << "#" << m_BaseSurface->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AgreementFlag == false ) { stream << ".F."; }
	else if( m_AgreementFlag == true ) { stream << ".T."; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PolygonalBoundary ) { stream << "#" << m_PolygonalBoundary->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcPolygonalBoundedHalfSpace::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPolygonalBoundedHalfSpace::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPolygonalBoundedHalfSpace, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcPolygonalBoundedHalfSpace, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BaseSurface, map );
	if( _stricmp( args[1].c_str(), ".F." ) == 0 ) { m_AgreementFlag = false; }
	else if( _stricmp( args[1].c_str(), ".T." ) == 0 ) { m_AgreementFlag = true; }
	readEntityReference( args[2], m_Position, map );
	readEntityReference( args[3], m_PolygonalBoundary, map );
}
void IfcPolygonalBoundedHalfSpace::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcHalfSpaceSolid::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Position", m_Position ) );
	vec_attributes.push_back( std::make_pair( "PolygonalBoundary", m_PolygonalBoundary ) );
}
void IfcPolygonalBoundedHalfSpace::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPolygonalBoundedHalfSpace::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcHalfSpaceSolid::setInverseCounterparts( ptr_self_entity );
}
void IfcPolygonalBoundedHalfSpace::unlinkSelf()
{
	IfcHalfSpaceSolid::unlinkSelf();
}
