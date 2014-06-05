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
#include "include/IfcBSplineCurve.h"
#include "include/IfcBSplineCurveForm.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBSplineCurve 
IfcBSplineCurve::IfcBSplineCurve() {}
IfcBSplineCurve::IfcBSplineCurve( int id ) { m_id = id; }
IfcBSplineCurve::~IfcBSplineCurve() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBSplineCurve::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBSplineCurve> other = dynamic_pointer_cast<IfcBSplineCurve>(other_entity);
	if( !other) { return; }
	m_Degree = other->m_Degree;
	m_ControlPointsList = other->m_ControlPointsList;
	m_CurveForm = other->m_CurveForm;
	m_ClosedCurve = other->m_ClosedCurve;
	m_SelfIntersect = other->m_SelfIntersect;
}
void IfcBSplineCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBSPLINECURVE" << "(";
	if( m_Degree == m_Degree ){ stream << m_Degree; }
	else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_ControlPointsList );
	stream << ",";
	if( m_CurveForm ) { m_CurveForm->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ClosedCurve == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_ClosedCurve == LOGICAL_TRUE ) { stream << ".T."; }
	else if( m_ClosedCurve == LOGICAL_UNKNOWN ) { stream << ".U."; }
	stream << ",";
	if( m_SelfIntersect == LOGICAL_FALSE ) { stream << ".F."; }
	else if( m_SelfIntersect == LOGICAL_TRUE ) { stream << ".T."; }
	else if( m_SelfIntersect == LOGICAL_UNKNOWN ) { stream << ".U."; }
	stream << ");";
}
void IfcBSplineCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBSplineCurve::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBSplineCurve, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcBSplineCurve, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readIntValue( args[0], m_Degree );
	readEntityReferenceList( args[1], m_ControlPointsList, map );
	m_CurveForm = IfcBSplineCurveForm::createObjectFromStepData( args[2] );
	if( _stricmp( args[3].c_str(), ".F." ) == 0 ) { m_ClosedCurve = LOGICAL_FALSE; }
	else if( _stricmp( args[3].c_str(), ".T." ) == 0 ) { m_ClosedCurve = LOGICAL_TRUE; }
	else if( _stricmp( args[3].c_str(), ".U." ) == 0 ) { m_ClosedCurve = LOGICAL_UNKNOWN; }
	if( _stricmp( args[4].c_str(), ".F." ) == 0 ) { m_SelfIntersect = LOGICAL_FALSE; }
	else if( _stricmp( args[4].c_str(), ".T." ) == 0 ) { m_SelfIntersect = LOGICAL_TRUE; }
	else if( _stricmp( args[4].c_str(), ".U." ) == 0 ) { m_SelfIntersect = LOGICAL_UNKNOWN; }
}
void IfcBSplineCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Degree", shared_ptr<IfcPPInt>( new IfcPPInt( m_Degree ) ) ) );
	vec_attributes.push_back( std::make_pair( "CurveForm", m_CurveForm ) );
	vec_attributes.push_back( std::make_pair( "ClosedCurve", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_ClosedCurve ) ) ) );
	vec_attributes.push_back( std::make_pair( "SelfIntersect", shared_ptr<IfcPPLogical>( new IfcPPLogical( m_SelfIntersect ) ) ) );
}
void IfcBSplineCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBSplineCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineCurve::unlinkSelf()
{
	IfcBoundedCurve::unlinkSelf();
}
