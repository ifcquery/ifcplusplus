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
#include "include/IfcLightDistributionCurveEnum.h"
#include "include/IfcLightDistributionData.h"
#include "include/IfcLightIntensityDistribution.h"

// ENTITY IfcLightIntensityDistribution 
IfcLightIntensityDistribution::IfcLightIntensityDistribution() {}
IfcLightIntensityDistribution::IfcLightIntensityDistribution( int id ) { m_id = id; }
IfcLightIntensityDistribution::~IfcLightIntensityDistribution() {}

// method setEntity takes over all attributes from another instance of the class
void IfcLightIntensityDistribution::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcLightIntensityDistribution> other = dynamic_pointer_cast<IfcLightIntensityDistribution>(other_entity);
	if( !other) { return; }
	m_LightDistributionCurve = other->m_LightDistributionCurve;
	m_DistributionData = other->m_DistributionData;
}
void IfcLightIntensityDistribution::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCLIGHTINTENSITYDISTRIBUTION" << "(";
	if( m_LightDistributionCurve ) { m_LightDistributionCurve->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_DistributionData );
	stream << ");";
}
void IfcLightIntensityDistribution::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightIntensityDistribution::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLightIntensityDistribution, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcLightIntensityDistribution, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_LightDistributionCurve = IfcLightDistributionCurveEnum::createObjectFromStepData( args[0] );
	readEntityReferenceList( args[1], m_DistributionData, map );
}
void IfcLightIntensityDistribution::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "LightDistributionCurve", m_LightDistributionCurve ) );
}
void IfcLightIntensityDistribution::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLightIntensityDistribution::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcLightIntensityDistribution::unlinkSelf()
{
}
