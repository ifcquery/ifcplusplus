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
#include "include/IfcBSplineSurfaceForm.h"
#include "include/IfcBSplineSurfaceWithKnots.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcKnotType.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBSplineSurfaceWithKnots 
IfcBSplineSurfaceWithKnots::IfcBSplineSurfaceWithKnots() {}
IfcBSplineSurfaceWithKnots::IfcBSplineSurfaceWithKnots( int id ) { m_id = id; }
IfcBSplineSurfaceWithKnots::~IfcBSplineSurfaceWithKnots() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBSplineSurfaceWithKnots::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBSplineSurfaceWithKnots> other = dynamic_pointer_cast<IfcBSplineSurfaceWithKnots>(other_entity);
	if( !other) { return; }
	m_UDegree = other->m_UDegree;
	m_VDegree = other->m_VDegree;
	m_ControlPointsList = other->m_ControlPointsList;
	m_SurfaceForm = other->m_SurfaceForm;
	m_UClosed = other->m_UClosed;
	m_VClosed = other->m_VClosed;
	m_SelfIntersect = other->m_SelfIntersect;
	m_UMultiplicities = other->m_UMultiplicities;
	m_VMultiplicities = other->m_VMultiplicities;
	m_UKnots = other->m_UKnots;
	m_VKnots = other->m_VKnots;
	m_KnotSpec = other->m_KnotSpec;
}
void IfcBSplineSurfaceWithKnots::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBSPLINESURFACEWITHKNOTS" << "(";
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
	stream << ",";
	writeIntList( stream, m_UMultiplicities );
	stream << ",";
	writeIntList( stream, m_VMultiplicities );
	stream << ",";
	writeTypeOfRealList( stream, m_UKnots );
	stream << ",";
	writeTypeOfRealList( stream, m_VKnots );
	stream << ",";
	if( m_KnotSpec ) { m_KnotSpec->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcBSplineSurfaceWithKnots::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBSplineSurfaceWithKnots::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<12 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBSplineSurfaceWithKnots, expecting 12, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>12 ){ std::cout << "Wrong parameter count for entity IfcBSplineSurfaceWithKnots, expecting 12, having " << num_args << ". Object id: " << getId() << std::endl; }
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
	readIntList(  args[7], m_UMultiplicities );
	readIntList(  args[8], m_VMultiplicities );
	readTypeOfRealList( args[9], m_UKnots );
	readTypeOfRealList( args[10], m_VKnots );
	m_KnotSpec = IfcKnotType::createObjectFromStepData( args[11] );
}
void IfcBSplineSurfaceWithKnots::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBSplineSurface::getAttributes( vec_attributes );
	shared_ptr<IfcPPAttributeObjectVector> UKnots_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_UKnots.begin(), m_UKnots.end(), std::back_inserter( UKnots_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "UKnots", UKnots_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> VKnots_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_VKnots.begin(), m_VKnots.end(), std::back_inserter( VKnots_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "VKnots", VKnots_vec_object ) );
	vec_attributes.push_back( std::make_pair( "KnotSpec", m_KnotSpec ) );
}
void IfcBSplineSurfaceWithKnots::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBSplineSurfaceWithKnots::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBSplineSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineSurfaceWithKnots::unlinkSelf()
{
	IfcBSplineSurface::unlinkSelf();
}
