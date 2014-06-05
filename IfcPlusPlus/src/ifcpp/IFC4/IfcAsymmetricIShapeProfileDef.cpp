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
#include "include/IfcAsymmetricIShapeProfileDef.h"
#include "include/IfcAxis2Placement2D.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcAsymmetricIShapeProfileDef 
IfcAsymmetricIShapeProfileDef::IfcAsymmetricIShapeProfileDef() {}
IfcAsymmetricIShapeProfileDef::IfcAsymmetricIShapeProfileDef( int id ) { m_id = id; }
IfcAsymmetricIShapeProfileDef::~IfcAsymmetricIShapeProfileDef() {}

// method setEntity takes over all attributes from another instance of the class
void IfcAsymmetricIShapeProfileDef::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcAsymmetricIShapeProfileDef> other = dynamic_pointer_cast<IfcAsymmetricIShapeProfileDef>(other_entity);
	if( !other) { return; }
	m_ProfileType = other->m_ProfileType;
	m_ProfileName = other->m_ProfileName;
	m_Position = other->m_Position;
	m_BottomFlangeWidth = other->m_BottomFlangeWidth;
	m_OverallDepth = other->m_OverallDepth;
	m_WebThickness = other->m_WebThickness;
	m_BottomFlangeThickness = other->m_BottomFlangeThickness;
	m_BottomFlangeFilletRadius = other->m_BottomFlangeFilletRadius;
	m_TopFlangeWidth = other->m_TopFlangeWidth;
	m_TopFlangeThickness = other->m_TopFlangeThickness;
	m_TopFlangeFilletRadius = other->m_TopFlangeFilletRadius;
	m_BottomFlangeEdgeRadius = other->m_BottomFlangeEdgeRadius;
	m_BottomFlangeSlope = other->m_BottomFlangeSlope;
	m_TopFlangeEdgeRadius = other->m_TopFlangeEdgeRadius;
	m_TopFlangeSlope = other->m_TopFlangeSlope;
}
void IfcAsymmetricIShapeProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCASYMMETRICISHAPEPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeWidth ) { m_BottomFlangeWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OverallDepth ) { m_OverallDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_WebThickness ) { m_WebThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeThickness ) { m_BottomFlangeThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeFilletRadius ) { m_BottomFlangeFilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeWidth ) { m_TopFlangeWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeThickness ) { m_TopFlangeThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeFilletRadius ) { m_TopFlangeFilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeEdgeRadius ) { m_BottomFlangeEdgeRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeSlope ) { m_BottomFlangeSlope->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeEdgeRadius ) { m_TopFlangeEdgeRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeSlope ) { m_TopFlangeSlope->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcAsymmetricIShapeProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAsymmetricIShapeProfileDef::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<15 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcAsymmetricIShapeProfileDef, expecting 15, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>15 ){ std::cout << "Wrong parameter count for entity IfcAsymmetricIShapeProfileDef, expecting 15, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ProfileType = IfcProfileTypeEnum::createObjectFromStepData( args[0] );
	m_ProfileName = IfcLabel::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_BottomFlangeWidth = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
	m_OverallDepth = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
	m_WebThickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[5] );
	m_BottomFlangeThickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[6] );
	m_BottomFlangeFilletRadius = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[7] );
	m_TopFlangeWidth = IfcPositiveLengthMeasure::createObjectFromStepData( args[8] );
	m_TopFlangeThickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[9] );
	m_TopFlangeFilletRadius = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[10] );
	m_BottomFlangeEdgeRadius = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[11] );
	m_BottomFlangeSlope = IfcPlaneAngleMeasure::createObjectFromStepData( args[12] );
	m_TopFlangeEdgeRadius = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[13] );
	m_TopFlangeSlope = IfcPlaneAngleMeasure::createObjectFromStepData( args[14] );
}
void IfcAsymmetricIShapeProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BottomFlangeWidth", m_BottomFlangeWidth ) );
	vec_attributes.push_back( std::make_pair( "OverallDepth", m_OverallDepth ) );
	vec_attributes.push_back( std::make_pair( "WebThickness", m_WebThickness ) );
	vec_attributes.push_back( std::make_pair( "BottomFlangeThickness", m_BottomFlangeThickness ) );
	vec_attributes.push_back( std::make_pair( "BottomFlangeFilletRadius", m_BottomFlangeFilletRadius ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeWidth", m_TopFlangeWidth ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeThickness", m_TopFlangeThickness ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeFilletRadius", m_TopFlangeFilletRadius ) );
	vec_attributes.push_back( std::make_pair( "BottomFlangeEdgeRadius", m_BottomFlangeEdgeRadius ) );
	vec_attributes.push_back( std::make_pair( "BottomFlangeSlope", m_BottomFlangeSlope ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeEdgeRadius", m_TopFlangeEdgeRadius ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeSlope", m_TopFlangeSlope ) );
}
void IfcAsymmetricIShapeProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcAsymmetricIShapeProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcAsymmetricIShapeProfileDef::unlinkSelf()
{
	IfcParameterizedProfileDef::unlinkSelf();
}
