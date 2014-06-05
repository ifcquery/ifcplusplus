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
#include "include/IfcAxis2Placement2D.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLShapeProfileDef.h"
#include "include/IfcLabel.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcLShapeProfileDef 
IfcLShapeProfileDef::IfcLShapeProfileDef() {}
IfcLShapeProfileDef::IfcLShapeProfileDef( int id ) { m_id = id; }
IfcLShapeProfileDef::~IfcLShapeProfileDef() {}

// method setEntity takes over all attributes from another instance of the class
void IfcLShapeProfileDef::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcLShapeProfileDef> other = dynamic_pointer_cast<IfcLShapeProfileDef>(other_entity);
	if( !other) { return; }
	m_ProfileType = other->m_ProfileType;
	m_ProfileName = other->m_ProfileName;
	m_Position = other->m_Position;
	m_Depth = other->m_Depth;
	m_Width = other->m_Width;
	m_Thickness = other->m_Thickness;
	m_FilletRadius = other->m_FilletRadius;
	m_EdgeRadius = other->m_EdgeRadius;
	m_LegSlope = other->m_LegSlope;
}
void IfcLShapeProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCLSHAPEPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Depth ) { m_Depth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Width ) { m_Width->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Thickness ) { m_Thickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FilletRadius ) { m_FilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EdgeRadius ) { m_EdgeRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LegSlope ) { m_LegSlope->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcLShapeProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLShapeProfileDef::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLShapeProfileDef, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcLShapeProfileDef, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ProfileType = IfcProfileTypeEnum::createObjectFromStepData( args[0] );
	m_ProfileName = IfcLabel::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_Depth = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
	m_Width = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
	m_Thickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[5] );
	m_FilletRadius = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[6] );
	m_EdgeRadius = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[7] );
	m_LegSlope = IfcPlaneAngleMeasure::createObjectFromStepData( args[8] );
}
void IfcLShapeProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Depth", m_Depth ) );
	vec_attributes.push_back( std::make_pair( "Width", m_Width ) );
	vec_attributes.push_back( std::make_pair( "Thickness", m_Thickness ) );
	vec_attributes.push_back( std::make_pair( "FilletRadius", m_FilletRadius ) );
	vec_attributes.push_back( std::make_pair( "EdgeRadius", m_EdgeRadius ) );
	vec_attributes.push_back( std::make_pair( "LegSlope", m_LegSlope ) );
}
void IfcLShapeProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLShapeProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcLShapeProfileDef::unlinkSelf()
{
	IfcParameterizedProfileDef::unlinkSelf();
}
