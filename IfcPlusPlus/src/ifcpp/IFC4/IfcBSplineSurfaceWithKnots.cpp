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
shared_ptr<IfcPPObject> IfcBSplineSurfaceWithKnots::getDeepCopy()
{
	shared_ptr<IfcBSplineSurfaceWithKnots> copy_self( new IfcBSplineSurfaceWithKnots() );
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
				vec_ii_target.push_back( dynamic_pointer_cast<IfcCartesianPoint>( item_jj->getDeepCopy() ) );
			}
		}
	}
	if( m_SurfaceForm ) { copy_self->m_SurfaceForm = dynamic_pointer_cast<IfcBSplineSurfaceForm>( m_SurfaceForm->getDeepCopy() ); }
	if( m_UClosed ) { copy_self->m_UClosed = m_UClosed; }
	if( m_VClosed ) { copy_self->m_VClosed = m_VClosed; }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = m_SelfIntersect; }
	for( size_t ii=0; ii<m_UKnots.size(); ++ii )
	{
		auto item_ii = m_UKnots[ii];
		if( item_ii )
		{
			copy_self->m_UKnots.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_VKnots.size(); ++ii )
	{
		auto item_ii = m_VKnots[ii];
		if( item_ii )
		{
			copy_self->m_VKnots.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_KnotSpec ) { copy_self->m_KnotSpec = dynamic_pointer_cast<IfcKnotType>( m_KnotSpec->getDeepCopy() ); }
	return copy_self;
}
void IfcBSplineSurfaceWithKnots::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBSPLINESURFACEWITHKNOTS" << "(";
	if( m_UDegree == m_UDegree ){ stream << m_UDegree; } else { stream << "*"; }
	stream << ",";
	if( m_VDegree == m_VDegree ){ stream << m_VDegree; } else { stream << "*"; }
	stream << ",";
	writeEntityList2D( stream, m_ControlPointsList );
	stream << ",";
	if( m_SurfaceForm ) { m_SurfaceForm->getStepParameter( stream ); } else { stream << "*"; }
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
void IfcBSplineSurfaceWithKnots::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( boost::iequals( args[4], L".F." ) ) { m_UClosed = LOGICAL_FALSE; }
	else if( boost::iequals( args[4], L".T." ) ) { m_UClosed = LOGICAL_TRUE; }
	else if( boost::iequals( args[4], L".U." ) ) { m_UClosed = LOGICAL_UNKNOWN; }
	if( boost::iequals( args[5], L".F." ) ) { m_VClosed = LOGICAL_FALSE; }
	else if( boost::iequals( args[5], L".T." ) ) { m_VClosed = LOGICAL_TRUE; }
	else if( boost::iequals( args[5], L".U." ) ) { m_VClosed = LOGICAL_UNKNOWN; }
	if( boost::iequals( args[6], L".F." ) ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( boost::iequals( args[6], L".T." ) ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( boost::iequals( args[6], L".U." ) ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
	readIntList(  args[7], m_UMultiplicities );
	readIntList(  args[8], m_VMultiplicities );
	readTypeOfRealList( args[9], m_UKnots );
	readTypeOfRealList( args[10], m_VKnots );
	m_KnotSpec = IfcKnotType::createObjectFromStepData( args[11] );
}
void IfcBSplineSurfaceWithKnots::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBSplineSurface::getAttributes( vec_attributes );
	if( m_UMultiplicities.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> UMultiplicities_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_UMultiplicities.size(); ++i )
		{
			UMultiplicities_vec_obj->m_vec.push_back( shared_ptr<IfcPPInt>( new IfcPPInt(m_UMultiplicities[i] ) ) );
		}
		vec_attributes.push_back( std::make_pair( "UMultiplicities", UMultiplicities_vec_obj ) );
	}
	if( m_VMultiplicities.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> VMultiplicities_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_VMultiplicities.size(); ++i )
		{
			VMultiplicities_vec_obj->m_vec.push_back( shared_ptr<IfcPPInt>( new IfcPPInt(m_VMultiplicities[i] ) ) );
		}
		vec_attributes.push_back( std::make_pair( "VMultiplicities", VMultiplicities_vec_obj ) );
	}
	if( m_UKnots.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> UKnots_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_UKnots.begin(), m_UKnots.end(), std::back_inserter( UKnots_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "UKnots", UKnots_vec_object ) );
	}
	if( m_VKnots.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> VKnots_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_VKnots.begin(), m_VKnots.end(), std::back_inserter( VKnots_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "VKnots", VKnots_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "KnotSpec", m_KnotSpec ) );
}
void IfcBSplineSurfaceWithKnots::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBSplineSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcBSplineSurfaceWithKnots::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBSplineSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineSurfaceWithKnots::unlinkSelf()
{
	IfcBSplineSurface::unlinkSelf();
}
