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
#include "include/IfcCartesianPoint.h"
#include "include/IfcColourRgb.h"
#include "include/IfcLabel.h"
#include "include/IfcLightSourcePositional.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcReal.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcLightSourcePositional 
IfcLightSourcePositional::IfcLightSourcePositional() {}
IfcLightSourcePositional::IfcLightSourcePositional( int id ) { m_id = id; }
IfcLightSourcePositional::~IfcLightSourcePositional() {}

// method setEntity takes over all attributes from another instance of the class
void IfcLightSourcePositional::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcLightSourcePositional> other = dynamic_pointer_cast<IfcLightSourcePositional>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_LightColour = other->m_LightColour;
	m_AmbientIntensity = other->m_AmbientIntensity;
	m_Intensity = other->m_Intensity;
	m_Position = other->m_Position;
	m_Radius = other->m_Radius;
	m_ConstantAttenuation = other->m_ConstantAttenuation;
	m_DistanceAttenuation = other->m_DistanceAttenuation;
	m_QuadricAttenuation = other->m_QuadricAttenuation;
}
void IfcLightSourcePositional::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCLIGHTSOURCEPOSITIONAL" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LightColour ) { stream << "#" << m_LightColour->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AmbientIntensity ) { m_AmbientIntensity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Intensity ) { m_Intensity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Radius ) { m_Radius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ConstantAttenuation ) { m_ConstantAttenuation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DistanceAttenuation ) { m_DistanceAttenuation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_QuadricAttenuation ) { m_QuadricAttenuation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcLightSourcePositional::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightSourcePositional::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLightSourcePositional, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcLightSourcePositional, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_LightColour, map );
	m_AmbientIntensity = IfcNormalisedRatioMeasure::createObjectFromStepData( args[2] );
	m_Intensity = IfcNormalisedRatioMeasure::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_Position, map );
	m_Radius = IfcPositiveLengthMeasure::createObjectFromStepData( args[5] );
	m_ConstantAttenuation = IfcReal::createObjectFromStepData( args[6] );
	m_DistanceAttenuation = IfcReal::createObjectFromStepData( args[7] );
	m_QuadricAttenuation = IfcReal::createObjectFromStepData( args[8] );
}
void IfcLightSourcePositional::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcLightSource::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Position", m_Position ) );
	vec_attributes.push_back( std::make_pair( "Radius", m_Radius ) );
	vec_attributes.push_back( std::make_pair( "ConstantAttenuation", m_ConstantAttenuation ) );
	vec_attributes.push_back( std::make_pair( "DistanceAttenuation", m_DistanceAttenuation ) );
	vec_attributes.push_back( std::make_pair( "QuadricAttenuation", m_QuadricAttenuation ) );
}
void IfcLightSourcePositional::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLightSourcePositional::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLightSource::setInverseCounterparts( ptr_self_entity );
}
void IfcLightSourcePositional::unlinkSelf()
{
	IfcLightSource::unlinkSelf();
}
