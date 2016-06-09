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
#include "include/IfcBSplineCurve.h"
#include "include/IfcBSplineCurveForm.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcInteger.h"
#include "include/IfcLogical.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBSplineCurve 
IfcBSplineCurve::IfcBSplineCurve() { m_entity_enum = IFCBSPLINECURVE; }
IfcBSplineCurve::IfcBSplineCurve( int id ) { m_id = id; m_entity_enum = IFCBSPLINECURVE; }
IfcBSplineCurve::~IfcBSplineCurve() {}
shared_ptr<IfcPPObject> IfcBSplineCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBSplineCurve> copy_self( new IfcBSplineCurve() );
	if( m_Degree ) { copy_self->m_Degree = dynamic_pointer_cast<IfcInteger>( m_Degree->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ControlPointsList.size(); ++ii )
	{
		auto item_ii = m_ControlPointsList[ii];
		if( item_ii )
		{
			copy_self->m_ControlPointsList.push_back( dynamic_pointer_cast<IfcCartesianPoint>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_CurveForm ) { copy_self->m_CurveForm = dynamic_pointer_cast<IfcBSplineCurveForm>( m_CurveForm->getDeepCopy(options) ); }
	if( m_ClosedCurve ) { copy_self->m_ClosedCurve = dynamic_pointer_cast<IfcLogical>( m_ClosedCurve->getDeepCopy(options) ); }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBSplineCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBSPLINECURVE" << "(";
	if( m_Degree ) { m_Degree->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_ControlPointsList );
	stream << ",";
	if( m_CurveForm ) { m_CurveForm->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ClosedCurve ) { m_ClosedCurve->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcBSplineCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBSplineCurve::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcBSplineCurve, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Degree = IfcInteger::createObjectFromSTEP( args[0] );
	readEntityReferenceList( args[1], m_ControlPointsList, map );
	m_CurveForm = IfcBSplineCurveForm::createObjectFromSTEP( args[2] );
	m_ClosedCurve = IfcLogical::createObjectFromSTEP( args[3] );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[4] );
}
void IfcBSplineCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Degree", m_Degree ) );
	if( m_ControlPointsList.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ControlPointsList_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ControlPointsList.begin(), m_ControlPointsList.end(), std::back_inserter( ControlPointsList_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ControlPointsList", ControlPointsList_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "CurveForm", m_CurveForm ) );
	vec_attributes.push_back( std::make_pair( "ClosedCurve", m_ClosedCurve ) );
	vec_attributes.push_back( std::make_pair( "SelfIntersect", m_SelfIntersect ) );
}
void IfcBSplineCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcBSplineCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineCurve::unlinkFromInverseCounterparts()
{
	IfcBoundedCurve::unlinkFromInverseCounterparts();
}
