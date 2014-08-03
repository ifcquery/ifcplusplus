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
shared_ptr<IfcPPObject> IfcLightIntensityDistribution::getDeepCopy()
{
	shared_ptr<IfcLightIntensityDistribution> copy_self( new IfcLightIntensityDistribution() );
	if( m_LightDistributionCurve ) { copy_self->m_LightDistributionCurve = dynamic_pointer_cast<IfcLightDistributionCurveEnum>( m_LightDistributionCurve->getDeepCopy() ); }
	for( size_t ii=0; ii<m_DistributionData.size(); ++ii )
	{
		auto item_ii = m_DistributionData[ii];
		if( item_ii )
		{
			copy_self->m_DistributionData.push_back( dynamic_pointer_cast<IfcLightDistributionData>(item_ii->getDeepCopy() ) );
		}
	}
	return copy_self;
}
void IfcLightIntensityDistribution::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCLIGHTINTENSITYDISTRIBUTION" << "(";
	if( m_LightDistributionCurve ) { m_LightDistributionCurve->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_DistributionData );
	stream << ");";
}
void IfcLightIntensityDistribution::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightIntensityDistribution::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( m_DistributionData.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DistributionData_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_DistributionData.begin(), m_DistributionData.end(), std::back_inserter( DistributionData_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "DistributionData", DistributionData_vec_object ) );
	}
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
