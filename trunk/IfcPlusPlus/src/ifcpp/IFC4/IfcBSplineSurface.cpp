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
#include "ifcpp/model/IfcPPGuid.h"
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
shared_ptr<IfcPPObject> IfcBSplineSurface::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBSplineSurface> copy_self( new IfcBSplineSurface() );
	if( m_UDegree ) { copy_self->m_UDegree = m_UDegree; }
	if( m_VDegree ) { copy_self->m_VDegree = m_VDegree; }
	copy_self->m_ControlPointsList.resize( m_ControlPointsList.size() );
	for( size_t ii=0; ii<m_ControlPointsList.size(); ++ii )
	{
		std::vector<shared_ptr<IfcCartesianPoint> >& vec_ii = m_ControlPointsList[ii];
		std::vector<shared_ptr<IfcCartesianPoint> >& vec_ii_target = copy_self->m_ControlPointsList[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcCartesianPoint>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcCartesianPoint>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	if( m_SurfaceForm ) { copy_self->m_SurfaceForm = dynamic_pointer_cast<IfcBSplineSurfaceForm>( m_SurfaceForm->getDeepCopy(options) ); }
	if( m_UClosed ) { copy_self->m_UClosed = m_UClosed; }
	if( m_VClosed ) { copy_self->m_VClosed = m_VClosed; }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = m_SelfIntersect; }
	return copy_self;
}
void IfcBSplineSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBSPLINESURFACE" << "(";
	if( m_UDegree == m_UDegree ){ stream << m_UDegree; } else { stream << "$"; }
	stream << ",";
	if( m_VDegree == m_VDegree ){ stream << m_VDegree; } else { stream << "$"; }
	stream << ",";
	writeEntityList2D( stream, m_ControlPointsList );
	stream << ",";
	if( m_SurfaceForm ) { m_SurfaceForm->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UClosed == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_UClosed == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ",";
	if( m_VClosed == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_VClosed == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ",";
	if( m_SelfIntersect == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_SelfIntersect == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ");";
}
void IfcBSplineSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBSplineSurface::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBSplineSurface, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readIntValue( args[0], m_UDegree );
	readIntValue( args[1], m_VDegree );
	readEntityReferenceList2D( args[2], m_ControlPointsList, map );
	m_SurfaceForm = IfcBSplineSurfaceForm::createObjectFromSTEP( args[3] );
	if( boost::iequals( args[4], L".F." ) ) { m_UClosed = LOGICAL_FALSE; }
	else if( boost::iequals( args[4], L".T." ) ) { m_UClosed = LOGICAL_TRUE; }
	else if( boost::iequals( args[4], L".U." ) ) { m_UClosed = LOGICAL_UNKNOWN; }
	if( boost::iequals( args[5], L".F." ) ) { m_VClosed = LOGICAL_FALSE; }
	else if( boost::iequals( args[5], L".T." ) ) { m_VClosed = LOGICAL_TRUE; }
	else if( boost::iequals( args[5], L".U." ) ) { m_VClosed = LOGICAL_UNKNOWN; }
	if( boost::iequals( args[6], L".F." ) ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( boost::iequals( args[6], L".T." ) ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( boost::iequals( args[6], L".U." ) ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
}
void IfcBSplineSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UDegree", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_UDegree ) ) ) );
	vec_attributes.push_back( std::make_pair( "VDegree", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_VDegree ) ) ) );
	vec_attributes.push_back( std::make_pair( "SurfaceForm", m_SurfaceForm ) );
	vec_attributes.push_back( std::make_pair( "UClosed", shared_ptr<IfcPPLogicalAttribute>( new IfcPPLogicalAttribute( m_UClosed ) ) ) );
	vec_attributes.push_back( std::make_pair( "VClosed", shared_ptr<IfcPPLogicalAttribute>( new IfcPPLogicalAttribute( m_VClosed ) ) ) );
	vec_attributes.push_back( std::make_pair( "SelfIntersect", shared_ptr<IfcPPLogicalAttribute>( new IfcPPLogicalAttribute( m_SelfIntersect ) ) ) );
}
void IfcBSplineSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcBSplineSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineSurface::unlinkSelf()
{
	IfcBoundedSurface::unlinkSelf();
}
