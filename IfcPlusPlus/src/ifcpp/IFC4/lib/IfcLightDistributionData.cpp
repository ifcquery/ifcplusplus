/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcLightDistributionData.h"
#include "ifcpp/IFC4/include/IfcLuminousIntensityDistributionMeasure.h"
#include "ifcpp/IFC4/include/IfcPlaneAngleMeasure.h"

// ENTITY IfcLightDistributionData 
IfcLightDistributionData::IfcLightDistributionData() = default;
IfcLightDistributionData::IfcLightDistributionData( int id ) { m_entity_id = id; }
IfcLightDistributionData::~IfcLightDistributionData() = default;
shared_ptr<BuildingObject> IfcLightDistributionData::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcLightDistributionData> copy_self( new IfcLightDistributionData() );
	if( m_MainPlaneAngle ) { copy_self->m_MainPlaneAngle = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_MainPlaneAngle->getDeepCopy(options) ); }
	for(auto item_ii : m_SecondaryPlaneAngle)
	{
			if( item_ii )
		{
			copy_self->m_SecondaryPlaneAngle.push_back( dynamic_pointer_cast<IfcPlaneAngleMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	for(auto item_ii : m_LuminousIntensity)
	{
			if( item_ii )
		{
			copy_self->m_LuminousIntensity.push_back( dynamic_pointer_cast<IfcLuminousIntensityDistributionMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcLightDistributionData::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCLIGHTDISTRIBUTIONDATA" << "(";
	if( m_MainPlaneAngle ) { m_MainPlaneAngle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeNumericTypeList( stream, m_SecondaryPlaneAngle );
	stream << ",";
	writeNumericTypeList( stream, m_LuminousIntensity );
	stream << ");";
}
void IfcLightDistributionData::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcLightDistributionData::toString() const { return L"IfcLightDistributionData"; }
void IfcLightDistributionData::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcLightDistributionData, expecting 3, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_MainPlaneAngle = IfcPlaneAngleMeasure::createObjectFromSTEP( args[0], map );
	readTypeOfRealList( args[1], m_SecondaryPlaneAngle );
	readTypeOfRealList( args[2], m_LuminousIntensity );
}
void IfcLightDistributionData::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	vec_attributes.emplace_back( "MainPlaneAngle", m_MainPlaneAngle );
	if( !m_SecondaryPlaneAngle.empty() )
	{
		shared_ptr<AttributeObjectVector> SecondaryPlaneAngle_vec_object( new AttributeObjectVector() );
		std::copy( m_SecondaryPlaneAngle.begin(), m_SecondaryPlaneAngle.end(), std::back_inserter( SecondaryPlaneAngle_vec_object->m_vec ) );
		vec_attributes.emplace_back( "SecondaryPlaneAngle", SecondaryPlaneAngle_vec_object );
	}
	if( !m_LuminousIntensity.empty() )
	{
		shared_ptr<AttributeObjectVector> LuminousIntensity_vec_object( new AttributeObjectVector() );
		std::copy( m_LuminousIntensity.begin(), m_LuminousIntensity.end(), std::back_inserter( LuminousIntensity_vec_object->m_vec ) );
		vec_attributes.emplace_back( "LuminousIntensity", LuminousIntensity_vec_object );
	}
}
void IfcLightDistributionData::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
}
void IfcLightDistributionData::setInverseCounterparts( shared_ptr<BuildingEntity>  /*ptr_self*/)
{
}
void IfcLightDistributionData::unlinkFromInverseCounterparts()
{
}
