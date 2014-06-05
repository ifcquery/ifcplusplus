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
#include "include/IfcLightDistributionData.h"
#include "include/IfcLuminousIntensityDistributionMeasure.h"
#include "include/IfcPlaneAngleMeasure.h"

// ENTITY IfcLightDistributionData 
IfcLightDistributionData::IfcLightDistributionData() {}
IfcLightDistributionData::IfcLightDistributionData( int id ) { m_id = id; }
IfcLightDistributionData::~IfcLightDistributionData() {}

// method setEntity takes over all attributes from another instance of the class
void IfcLightDistributionData::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcLightDistributionData> other = dynamic_pointer_cast<IfcLightDistributionData>(other_entity);
	if( !other) { return; }
	m_MainPlaneAngle = other->m_MainPlaneAngle;
	m_SecondaryPlaneAngle = other->m_SecondaryPlaneAngle;
	m_LuminousIntensity = other->m_LuminousIntensity;
}
void IfcLightDistributionData::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCLIGHTDISTRIBUTIONDATA" << "(";
	if( m_MainPlaneAngle ) { m_MainPlaneAngle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeOfRealList( stream, m_SecondaryPlaneAngle );
	stream << ",";
	writeTypeOfRealList( stream, m_LuminousIntensity );
	stream << ");";
}
void IfcLightDistributionData::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightDistributionData::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLightDistributionData, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcLightDistributionData, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_MainPlaneAngle = IfcPlaneAngleMeasure::createObjectFromStepData( args[0] );
	readTypeOfRealList( args[1], m_SecondaryPlaneAngle );
	readTypeOfRealList( args[2], m_LuminousIntensity );
}
void IfcLightDistributionData::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "MainPlaneAngle", m_MainPlaneAngle ) );
	shared_ptr<IfcPPAttributeObjectVector> SecondaryPlaneAngle_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_SecondaryPlaneAngle.begin(), m_SecondaryPlaneAngle.end(), std::back_inserter( SecondaryPlaneAngle_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "SecondaryPlaneAngle", SecondaryPlaneAngle_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> LuminousIntensity_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_LuminousIntensity.begin(), m_LuminousIntensity.end(), std::back_inserter( LuminousIntensity_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "LuminousIntensity", LuminousIntensity_vec_object ) );
}
void IfcLightDistributionData::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLightDistributionData::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcLightDistributionData::unlinkSelf()
{
}
