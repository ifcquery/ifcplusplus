/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
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
IfcLightSourceGoniometric::IfcLightSourceGoniometric() { m_entity_enum = IFCLIGHTSOURCEGONIOMETRIC; }
IfcLightSourceGoniometric::IfcLightSourceGoniometric( int id ) { m_id = id; m_entity_enum = IFCLIGHTSOURCEGONIOMETRIC; }
IfcLightSourceGoniometric::~IfcLightSourceGoniometric() {}
shared_ptr<IfcPPObject> IfcLightSourceGoniometric::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLightSourceGoniometric> copy_self( new IfcLightSourceGoniometric() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_LightColour ) { copy_self->m_LightColour = dynamic_pointer_cast<IfcColourRgb>( m_LightColour->getDeepCopy(options) ); }
	if( m_AmbientIntensity ) { copy_self->m_AmbientIntensity = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_AmbientIntensity->getDeepCopy(options) ); }
	if( m_Intensity ) { copy_self->m_Intensity = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Intensity->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_ColourAppearance ) { copy_self->m_ColourAppearance = dynamic_pointer_cast<IfcColourRgb>( m_ColourAppearance->getDeepCopy(options) ); }
	if( m_ColourTemperature ) { copy_self->m_ColourTemperature = dynamic_pointer_cast<IfcThermodynamicTemperatureMeasure>( m_ColourTemperature->getDeepCopy(options) ); }
	if( m_LuminousFlux ) { copy_self->m_LuminousFlux = dynamic_pointer_cast<IfcLuminousFluxMeasure>( m_LuminousFlux->getDeepCopy(options) ); }
	if( m_LightEmissionSource ) { copy_self->m_LightEmissionSource = dynamic_pointer_cast<IfcLightEmissionSourceEnum>( m_LightEmissionSource->getDeepCopy(options) ); }
	if( m_LightDistributionDataSource ) { copy_self->m_LightDistributionDataSource = dynamic_pointer_cast<IfcLightDistributionDataSourceSelect>( m_LightDistributionDataSource->getDeepCopy(options) ); }
	return copy_self;
}
void IfcLightSourceGoniometric::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCLIGHTSOURCEGONIOMETRIC" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LightColour ) { stream << "#" << m_LightColour->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_AmbientIntensity ) { m_AmbientIntensity->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Intensity ) { m_Intensity->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_ColourAppearance ) { stream << "#" << m_ColourAppearance->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_ColourTemperature ) { m_ColourTemperature->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LuminousFlux ) { m_LuminousFlux->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LightEmissionSource ) { m_LightEmissionSource->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LightDistributionDataSource ) { m_LightDistributionDataSource->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcLightSourceGoniometric::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightSourceGoniometric::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 10 ){ std::stringstream err; err << "Wrong parameter count for entity IfcLightSourceGoniometric, expecting 10, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_LightColour, map );
	m_AmbientIntensity = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[2] );
	m_Intensity = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[3] );
	readEntityReference( args[4], m_Position, map );
	readEntityReference( args[5], m_ColourAppearance, map );
	m_ColourTemperature = IfcThermodynamicTemperatureMeasure::createObjectFromSTEP( args[6] );
	m_LuminousFlux = IfcLuminousFluxMeasure::createObjectFromSTEP( args[7] );
	m_LightEmissionSource = IfcLightEmissionSourceEnum::createObjectFromSTEP( args[8] );
	m_LightDistributionDataSource = IfcLightDistributionDataSourceSelect::createObjectFromSTEP( args[9], map );
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
	IfcLightSource::getAttributesInverse( vec_attributes_inverse );
}
void IfcLightSourceGoniometric::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLightSource::setInverseCounterparts( ptr_self_entity );
}
void IfcLightSourceGoniometric::unlinkFromInverseCounterparts()
{
	IfcLightSource::unlinkFromInverseCounterparts();
}
