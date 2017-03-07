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
#include "include/IfcLightDistributionData.h"
#include "include/IfcLuminousIntensityDistributionMeasure.h"
#include "include/IfcPlaneAngleMeasure.h"

// ENTITY IfcLightDistributionData 
IfcLightDistributionData::IfcLightDistributionData() { m_entity_enum = IFCLIGHTDISTRIBUTIONDATA; }
IfcLightDistributionData::IfcLightDistributionData( int id ) { m_id = id; m_entity_enum = IFCLIGHTDISTRIBUTIONDATA; }
IfcLightDistributionData::~IfcLightDistributionData() {}
shared_ptr<IfcPPObject> IfcLightDistributionData::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLightDistributionData> copy_self( new IfcLightDistributionData() );
	if( m_MainPlaneAngle ) { copy_self->m_MainPlaneAngle = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_MainPlaneAngle->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_SecondaryPlaneAngle.size(); ++ii )
	{
		auto item_ii = m_SecondaryPlaneAngle[ii];
		if( item_ii )
		{
			copy_self->m_SecondaryPlaneAngle.push_back( dynamic_pointer_cast<IfcPlaneAngleMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_LuminousIntensity.size(); ++ii )
	{
		auto item_ii = m_LuminousIntensity[ii];
		if( item_ii )
		{
			copy_self->m_LuminousIntensity.push_back( dynamic_pointer_cast<IfcLuminousIntensityDistributionMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcLightDistributionData::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCLIGHTDISTRIBUTIONDATA" << "(";
	if( m_MainPlaneAngle ) { m_MainPlaneAngle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeNumericTypeList( stream, m_SecondaryPlaneAngle );
	stream << ",";
	writeNumericTypeList( stream, m_LuminousIntensity );
	stream << ");";
}
void IfcLightDistributionData::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightDistributionData::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcLightDistributionData, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_MainPlaneAngle = IfcPlaneAngleMeasure::createObjectFromSTEP( args[0] );
	readTypeOfRealList( args[1], m_SecondaryPlaneAngle );
	readTypeOfRealList( args[2], m_LuminousIntensity );
}
void IfcLightDistributionData::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "MainPlaneAngle", m_MainPlaneAngle ) );
	if( m_SecondaryPlaneAngle.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> SecondaryPlaneAngle_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_SecondaryPlaneAngle.begin(), m_SecondaryPlaneAngle.end(), std::back_inserter( SecondaryPlaneAngle_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "SecondaryPlaneAngle", SecondaryPlaneAngle_vec_object ) );
	}
	if( m_LuminousIntensity.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LuminousIntensity_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_LuminousIntensity.begin(), m_LuminousIntensity.end(), std::back_inserter( LuminousIntensity_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "LuminousIntensity", LuminousIntensity_vec_object ) );
	}
}
void IfcLightDistributionData::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLightDistributionData::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcLightDistributionData::unlinkFromInverseCounterparts()
{
}
