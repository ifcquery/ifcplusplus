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
#include "include/IfcBSplineSurface.h"
#include "include/IfcBSplineSurfaceForm.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBSplineSurface 
IfcBSplineSurface::IfcBSplineSurface() {}
IfcBSplineSurface::IfcBSplineSurface( int id ) { m_id = id; }
IfcBSplineSurface::~IfcBSplineSurface() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBSplineSurface::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBSplineSurface> other = dynamic_pointer_cast<IfcBSplineSurface>(other_entity);
	if( !other) { return; }
	m_UDegree = other->m_UDegree;
	m_VDegree = other->m_VDegree;
	m_ControlPointsList = other->m_ControlPointsList;
	m_SurfaceForm = other->m_SurfaceForm;
	m_UClosed = other->m_UClosed;
	m_VClosed = other->m_VClosed;
	m_SelfIntersect = other->m_SelfIntersect;
}
void IfcBSplineSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBSPLINESURFACE" << "(";
	if( m_UDegree == m_UDegree ){ stream << m_UDegree; }
	else { stream << "$"; }
	stream << ",";
	if( m_VDegree == m_VDegree ){ stream << m_VDegree; }
	else { stream << "$"; }
	stream << ",";
	writeEntityList2D( stream, m_ControlPointsList );
	stream << ",";
	if( m_SurfaceForm ) { m_SurfaceForm->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UClosed == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_UClosed == LOGICAL_TRUE ) { stream << ".T."; }
	else if( m_UClosed == LOGICAL_UNKNOWN ) { stream << ".U."; }
	stream << ",";
	if( m_VClosed == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_VClosed == LOGICAL_TRUE ) { stream << ".T."; }
	else if( m_VClosed == LOGICAL_UNKNOWN ) { stream << ".U."; }
	stream << ",";
	if( m_SelfIntersect == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_SelfIntersect == LOGICAL_TRUE ) { stream << ".T."; }
	else if( m_SelfIntersect == LOGICAL_UNKNOWN ) { stream << ".U."; }
	stream << ");";
}
void IfcBSplineSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBSplineSurface::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBSplineSurface, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcBSplineSurface, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readIntValue( args[0], m_UDegree );
	readIntValue( args[1], m_VDegree );
	readEntityReferenceList2D( args[2], m_ControlPointsList, map );
	m_SurfaceForm = IfcBSplineSurfaceForm::createObjectFromStepData( args[3] );
	if( _stricmp( args[4].c_str(), ".F." ) == 0 ) { m_UClosed = LOGICAL_FALSE; }
	else if( _stricmp( args[4].c_str(), ".T." ) == 0 ) { m_UClosed = LOGICAL_TRUE; }
	else if( _stricmp( args[4].c_str(), ".U." ) == 0 ) { m_UClosed = LOGICAL_UNKNOWN; }
	if( _stricmp( args[5].c_str(), ".F." ) == 0 ) { m_VClosed = LOGICAL_FALSE; }
	else if( _stricmp( args[5].c_str(), ".T." ) == 0 ) { m_VClosed = LOGICAL_TRUE; }
	else if( _stricmp( args[5].c_str(), ".U." ) == 0 ) { m_VClosed = LOGICAL_UNKNOWN; }
	if( _stricmp( args[6].c_str(), ".F." ) == 0 ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( _stricmp( args[6].c_str(), ".T." ) == 0 ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( _stricmp( args[6].c_str(), ".U." ) == 0 ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
}
void IfcBSplineSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UDegree", shared_ptr<IfcPPInt>( new IfcPPInt( m_UDegree ) ) ) );
	vec_attributes.push_back( std::make_pair( "VDegree", shared_ptr<IfcPPInt>( new IfcPPInt( m_VDegree ) ) ) );
	vec_attributes.push_back( std::make_pair( "SurfaceForm", m_SurfaceForm ) );
	vec_attributes.push_back( std::make_pair( "UClosed", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_UClosed ) ) ) );
	vec_attributes.push_back( std::make_pair( "VClosed", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_VClosed ) ) ) );
	vec_attributes.push_back( std::make_pair( "SelfIntersect", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_SelfIntersect ) ) ) );
}
void IfcBSplineSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBSplineSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineSurface::unlinkSelf()
{
	IfcBoundedSurface::unlinkSelf();
}
