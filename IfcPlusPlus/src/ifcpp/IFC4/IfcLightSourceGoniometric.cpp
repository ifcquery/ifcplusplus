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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcColourRgb.h"
#include "include/IfcLabel.h"
#include "include/IfcLightDistributionDataSourceSelect.h"
#include "include/IfcLightEmissionSourceEnum.h"
#include "include/IfcLightSourceGoniometric.h"
#include "include/IfcLuminousFluxMeasure.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcThermodynamicTemperatureMeasure.h"

// ENTITY IfcLightSourceGoniometric 
IfcLightSourceGoniometric::IfcLightSourceGoniometric() {}
IfcLightSourceGoniometric::IfcLightSourceGoniometric( int id ) { m_id = id; }
IfcLightSourceGoniometric::~IfcLightSourceGoniometric() {}

// method setEntity takes over all attributes from another instance of the class
void IfcLightSourceGoniometric::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcLightSourceGoniometric> other = dynamic_pointer_cast<IfcLightSourceGoniometric>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_LightColour = other->m_LightColour;
	m_AmbientIntensity = other->m_AmbientIntensity;
	m_Intensity = other->m_Intensity;
	m_Position = other->m_Position;
	m_ColourAppearance = other->m_ColourAppearance;
	m_ColourTemperature = other->m_ColourTemperature;
	m_LuminousFlux = other->m_LuminousFlux;
	m_LightEmissionSource = other->m_LightEmissionSource;
	m_LightDistributionDataSource = other->m_LightDistributionDataSource;
}
void IfcLightSourceGoniometric::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCLIGHTSOURCEGONIOMETRIC" << "(";
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
	if( m_ColourAppearance ) { stream << "#" << m_ColourAppearance->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_ColourTemperature ) { m_ColourTemperature->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LuminousFlux ) { m_LuminousFlux->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LightEmissionSource ) { m_LightEmissionSource->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LightDistributionDataSource ) { m_LightDistributionDataSource->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcLightSourceGoniometric::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightSourceGoniometric::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLightSourceGoniometric, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcLightSourceGoniometric, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_LightColour, map );
	m_AmbientIntensity = IfcNormalisedRatioMeasure::createObjectFromStepData( args[2] );
	m_Intensity = IfcNormalisedRatioMeasure::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_Position, map );
	readEntityReference( args[5], m_ColourAppearance, map );
	m_ColourTemperature = IfcThermodynamicTemperatureMeasure::createObjectFromStepData( args[6] );
	m_LuminousFlux = IfcLuminousFluxMeasure::createObjectFromStepData( args[7] );
	m_LightEmissionSource = IfcLightEmissionSourceEnum::createObjectFromStepData( args[8] );
	m_LightDistributionDataSource = IfcLightDistributionDataSourceSelect::createObjectFromStepData( args[9], map );
}
void IfcLightSourceGoniometric::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcLightSource::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Position", m_Position ) );
	vec_attributes.push_back( std::make_pair( "ColourAppearance", m_ColourAppearance ) );
	vec_attributes.push_back( std::make_pair( "ColourTemperature", m_ColourTemperature ) );
	vec_attributes.push_back( std::make_pair( "LuminousFlux", m_LuminousFlux ) );
	vec_attributes.push_back( std::make_pair( "LightEmissionSource", m_LightEmissionSource ) );
	vec_attributes.push_back( std::make_pair( "LightDistributionDataSource", m_LightDistributionDataSource ) );
}
void IfcLightSourceGoniometric::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLightSourceGoniometric::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLightSource::setInverseCounterparts( ptr_self_entity );
}
void IfcLightSourceGoniometric::unlinkSelf()
{
	IfcLightSource::unlinkSelf();
}
