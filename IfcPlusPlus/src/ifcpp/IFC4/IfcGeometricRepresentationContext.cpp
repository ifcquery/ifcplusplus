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
#include "include/IfcAxis2Placement.h"
#include "include/IfcDimensionCount.h"
#include "include/IfcDirection.h"
#include "include/IfcGeometricRepresentationContext.h"
#include "include/IfcGeometricRepresentationSubContext.h"
#include "include/IfcLabel.h"
#include "include/IfcRepresentation.h"

// ENTITY IfcGeometricRepresentationContext 
IfcGeometricRepresentationContext::IfcGeometricRepresentationContext() {}
IfcGeometricRepresentationContext::IfcGeometricRepresentationContext( int id ) { m_id = id; }
IfcGeometricRepresentationContext::~IfcGeometricRepresentationContext() {}

// method setEntity takes over all attributes from another instance of the class
void IfcGeometricRepresentationContext::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcGeometricRepresentationContext> other = dynamic_pointer_cast<IfcGeometricRepresentationContext>(other_entity);
	if( !other) { return; }
	m_ContextIdentifier = other->m_ContextIdentifier;
	m_ContextType = other->m_ContextType;
	m_CoordinateSpaceDimension = other->m_CoordinateSpaceDimension;
	m_Precision = other->m_Precision;
	m_WorldCoordinateSystem = other->m_WorldCoordinateSystem;
	m_TrueNorth = other->m_TrueNorth;
}
void IfcGeometricRepresentationContext::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCGEOMETRICREPRESENTATIONCONTEXT" << "(";
	if( m_ContextIdentifier ) { m_ContextIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ContextType ) { m_ContextType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CoordinateSpaceDimension ) { m_CoordinateSpaceDimension->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Precision == m_Precision ){ stream << m_Precision; }
	else { stream << "$"; }
	stream << ",";
	if( m_WorldCoordinateSystem ) { m_WorldCoordinateSystem->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TrueNorth ) { stream << "#" << m_TrueNorth->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcGeometricRepresentationContext::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGeometricRepresentationContext::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcGeometricRepresentationContext, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcGeometricRepresentationContext, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ContextIdentifier = IfcLabel::createObjectFromStepData( args[0] );
	m_ContextType = IfcLabel::createObjectFromStepData( args[1] );
	m_CoordinateSpaceDimension = IfcDimensionCount::createObjectFromStepData( args[2] );
	readRealValue( args[3], m_Precision );
	m_WorldCoordinateSystem = IfcAxis2Placement::createObjectFromStepData( args[4], map );
	readEntityReference( args[5], m_TrueNorth, map );
}
void IfcGeometricRepresentationContext::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRepresentationContext::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "CoordinateSpaceDimension", m_CoordinateSpaceDimension ) );
	vec_attributes.push_back( std::make_pair( "Precision", shared_ptr<IfcPPReal>( new IfcPPReal( m_Precision ) ) ) );
	vec_attributes.push_back( std::make_pair( "WorldCoordinateSystem", m_WorldCoordinateSystem ) );
	vec_attributes.push_back( std::make_pair( "TrueNorth", m_TrueNorth ) );
}
void IfcGeometricRepresentationContext::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasSubContexts_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasSubContexts_inverse.size(); ++i ) { HasSubContexts_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcGeometricRepresentationSubContext>( m_HasSubContexts_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasSubContexts_inverse", HasSubContexts_inverse_vec_obj ) );
}
void IfcGeometricRepresentationContext::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRepresentationContext::setInverseCounterparts( ptr_self_entity );
}
void IfcGeometricRepresentationContext::unlinkSelf()
{
	IfcRepresentationContext::unlinkSelf();
}
