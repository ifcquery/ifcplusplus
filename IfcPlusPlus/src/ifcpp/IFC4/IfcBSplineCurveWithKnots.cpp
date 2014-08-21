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
#include "include/IfcBSplineCurveForm.h"
#include "include/IfcBSplineCurveWithKnots.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcKnotType.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBSplineCurveWithKnots 
IfcBSplineCurveWithKnots::IfcBSplineCurveWithKnots() {}
IfcBSplineCurveWithKnots::IfcBSplineCurveWithKnots( int id ) { m_id = id; }
IfcBSplineCurveWithKnots::~IfcBSplineCurveWithKnots() {}
shared_ptr<IfcPPObject> IfcBSplineCurveWithKnots::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBSplineCurveWithKnots> copy_self( new IfcBSplineCurveWithKnots() );
	if( m_Degree ) { copy_self->m_Degree = m_Degree; }
	for( size_t ii=0; ii<m_ControlPointsList.size(); ++ii )
	{
		auto item_ii = m_ControlPointsList[ii];
		if( item_ii )
		{
			copy_self->m_ControlPointsList.push_back( dynamic_pointer_cast<IfcCartesianPoint>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_CurveForm ) { copy_self->m_CurveForm = dynamic_pointer_cast<IfcBSplineCurveForm>( m_CurveForm->getDeepCopy(options) ); }
	if( m_ClosedCurve ) { copy_self->m_ClosedCurve = m_ClosedCurve; }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = m_SelfIntersect; }
	if( m_KnotMultiplicities.size() > 0 ) { std::copy( m_KnotMultiplicities.begin(), m_KnotMultiplicities.end(), std::back_inserter( copy_self->m_KnotMultiplicities ) ); }
	for( size_t ii=0; ii<m_Knots.size(); ++ii )
	{
		auto item_ii = m_Knots[ii];
		if( item_ii )
		{
			copy_self->m_Knots.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_KnotSpec ) { copy_self->m_KnotSpec = dynamic_pointer_cast<IfcKnotType>( m_KnotSpec->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBSplineCurveWithKnots::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBSPLINECURVEWITHKNOTS" << "(";
	if( m_Degree == m_Degree ){ stream << m_Degree; } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_ControlPointsList );
	stream << ",";
	if( m_CurveForm ) { m_CurveForm->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ClosedCurve == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_ClosedCurve == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ",";
	if( m_SelfIntersect == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_SelfIntersect == LOGICAL_TRUE ) { stream << ".T."; }
	else { stream << ".U."; } // LOGICAL_UNKNOWN
	stream << ",";
	writeIntList( stream, m_KnotMultiplicities );
	stream << ",";
	writeTypeOfRealList( stream, m_Knots );
	stream << ",";
	if( m_KnotSpec ) { m_KnotSpec->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcBSplineCurveWithKnots::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBSplineCurveWithKnots::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBSplineCurveWithKnots, expecting 8, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readIntValue( args[0], m_Degree );
	readEntityReferenceList( args[1], m_ControlPointsList, map );
	m_CurveForm = IfcBSplineCurveForm::createObjectFromSTEP( args[2] );
	if( boost::iequals( args[3], L".F." ) ) { m_ClosedCurve = LOGICAL_FALSE; }
	else if( boost::iequals( args[3], L".T." ) ) { m_ClosedCurve = LOGICAL_TRUE; }
	else if( boost::iequals( args[3], L".U." ) ) { m_ClosedCurve = LOGICAL_UNKNOWN; }
	if( boost::iequals( args[4], L".F." ) ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( boost::iequals( args[4], L".T." ) ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( boost::iequals( args[4], L".U." ) ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
	readIntList(  args[5], m_KnotMultiplicities );
	readTypeOfRealList( args[6], m_Knots );
	m_KnotSpec = IfcKnotType::createObjectFromSTEP( args[7] );
}
void IfcBSplineCurveWithKnots::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBSplineCurve::getAttributes( vec_attributes );
	if( m_KnotMultiplicities.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> KnotMultiplicities_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_KnotMultiplicities.size(); ++i )
		{
			KnotMultiplicities_vec_obj->m_vec.push_back( shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute(m_KnotMultiplicities[i] ) ) );
		}
		vec_attributes.push_back( std::make_pair( "KnotMultiplicities", KnotMultiplicities_vec_obj ) );
	}
	if( m_Knots.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Knots_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Knots.begin(), m_Knots.end(), std::back_inserter( Knots_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Knots", Knots_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "KnotSpec", m_KnotSpec ) );
}
void IfcBSplineCurveWithKnots::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBSplineCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcBSplineCurveWithKnots::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBSplineCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineCurveWithKnots::unlinkSelf()
{
	IfcBSplineCurve::unlinkSelf();
}
