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
#include "include/IfcLightDistributionCurveEnum.h"
#include "include/IfcLightDistributionData.h"
#include "include/IfcLightIntensityDistribution.h"

// ENTITY IfcLightIntensityDistribution 
IfcLightIntensityDistribution::IfcLightIntensityDistribution() { m_entity_enum = IFCLIGHTINTENSITYDISTRIBUTION; }
IfcLightIntensityDistribution::IfcLightIntensityDistribution( int id ) { m_id = id; m_entity_enum = IFCLIGHTINTENSITYDISTRIBUTION; }
IfcLightIntensityDistribution::~IfcLightIntensityDistribution() {}
shared_ptr<IfcPPObject> IfcLightIntensityDistribution::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLightIntensityDistribution> copy_self( new IfcLightIntensityDistribution() );
	if( m_LightDistributionCurve ) { copy_self->m_LightDistributionCurve = dynamic_pointer_cast<IfcLightDistributionCurveEnum>( m_LightDistributionCurve->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_DistributionData.size(); ++ii )
	{
		auto item_ii = m_DistributionData[ii];
		if( item_ii )
		{
			copy_self->m_DistributionData.push_back( dynamic_pointer_cast<IfcLightDistributionData>(item_ii->getDeepCopy(options) ) );
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
void IfcLightIntensityDistribution::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcLightIntensityDistribution, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_LightDistributionCurve = IfcLightDistributionCurveEnum::createObjectFromSTEP( args[0] );
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
void IfcLightIntensityDistribution::unlinkFromInverseCounterparts()
{
}
