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
#include "include/IfcCompositeCurve.h"
#include "include/IfcCurve.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcReparametrisedCompositeCurveSegment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTransitionCode.h"

// ENTITY IfcReparametrisedCompositeCurveSegment 
IfcReparametrisedCompositeCurveSegment::IfcReparametrisedCompositeCurveSegment() {}
IfcReparametrisedCompositeCurveSegment::IfcReparametrisedCompositeCurveSegment( int id ) { m_id = id; }
IfcReparametrisedCompositeCurveSegment::~IfcReparametrisedCompositeCurveSegment() {}

// method setEntity takes over all attributes from another instance of the class
void IfcReparametrisedCompositeCurveSegment::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcReparametrisedCompositeCurveSegment> other = dynamic_pointer_cast<IfcReparametrisedCompositeCurveSegment>(other_entity);
	if( !other) { return; }
	m_Transition = other->m_Transition;
	m_SameSense = other->m_SameSense;
	m_ParentCurve = other->m_ParentCurve;
	m_ParamLength = other->m_ParamLength;
}
void IfcReparametrisedCompositeCurveSegment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCREPARAMETRISEDCOMPOSITECURVESEGMENT" << "(";
	if( m_Transition ) { m_Transition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SameSense == false ) { stream << ".F."; }
	else if( m_SameSense == true ) { stream << ".T."; }
	stream << ",";
	if( m_ParentCurve ) { stream << "#" << m_ParentCurve->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_ParamLength ) { m_ParamLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcReparametrisedCompositeCurveSegment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcReparametrisedCompositeCurveSegment::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcReparametrisedCompositeCurveSegment, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcReparametrisedCompositeCurveSegment, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Transition = IfcTransitionCode::createObjectFromStepData( args[0] );
	if( _stricmp( args[1].c_str(), ".F." ) == 0 ) { m_SameSense = false; }
	else if( _stricmp( args[1].c_str(), ".T." ) == 0 ) { m_SameSense = true; }
	readEntityReference( args[2], m_ParentCurve, map );
	m_ParamLength = IfcParameterValue::createObjectFromStepData( args[3] );
}
void IfcReparametrisedCompositeCurveSegment::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCompositeCurveSegment::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ParamLength", m_ParamLength ) );
}
void IfcReparametrisedCompositeCurveSegment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcReparametrisedCompositeCurveSegment::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCompositeCurveSegment::setInverseCounterparts( ptr_self_entity );
}
void IfcReparametrisedCompositeCurveSegment::unlinkSelf()
{
	IfcCompositeCurveSegment::unlinkSelf();
}
