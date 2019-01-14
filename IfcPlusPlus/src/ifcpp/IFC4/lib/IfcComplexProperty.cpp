/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcComplexProperty.h"
#include "ifcpp/IFC4/include/IfcExternalReferenceRelationship.h"
#include "ifcpp/IFC4/include/IfcIdentifier.h"
#include "ifcpp/IFC4/include/IfcProperty.h"
#include "ifcpp/IFC4/include/IfcPropertyDependencyRelationship.h"
#include "ifcpp/IFC4/include/IfcPropertySet.h"
#include "ifcpp/IFC4/include/IfcResourceApprovalRelationship.h"
#include "ifcpp/IFC4/include/IfcResourceConstraintRelationship.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcComplexProperty 
IfcComplexProperty::IfcComplexProperty( int id ) { m_entity_id = id; }
IfcComplexProperty::~IfcComplexProperty() {}
shared_ptr<BuildingObject> IfcComplexProperty::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcComplexProperty> copy_self( new IfcComplexProperty() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_UsageName ) { copy_self->m_UsageName = dynamic_pointer_cast<IfcIdentifier>( m_UsageName->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_HasProperties.size(); ++ii )
	{
		auto item_ii = m_HasProperties[ii];
		if( item_ii )
		{
			copy_self->m_HasProperties.emplace_back( dynamic_pointer_cast<IfcProperty>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcComplexProperty::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCCOMPLEXPROPERTY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UsageName ) { m_UsageName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasProperties );
	stream << ");";
}
void IfcComplexProperty::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcComplexProperty::toString() const { return L"IfcComplexProperty"; }
void IfcComplexProperty::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcComplexProperty, expecting 4, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	m_UsageName = IfcIdentifier::createObjectFromSTEP( args[2], map );
	readEntityReferenceList( args[3], m_HasProperties, map );
}
void IfcComplexProperty::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcProperty::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "UsageName", m_UsageName ) );
	if( !m_HasProperties.empty() )
	{
		shared_ptr<AttributeObjectVector> HasProperties_vec_object( new AttributeObjectVector() );
		std::copy( m_HasProperties.begin(), m_HasProperties.end(), std::back_inserter( HasProperties_vec_object->m_vec ) );
		vec_attributes.emplace_back( std::make_pair( "HasProperties", HasProperties_vec_object ) );
	}
}
void IfcComplexProperty::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcProperty::getAttributesInverse( vec_attributes_inverse );
}
void IfcComplexProperty::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcProperty::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcComplexProperty> ptr_self = dynamic_pointer_cast<IfcComplexProperty>( ptr_self_entity );
	if( !ptr_self ) { throw BuildingException( "IfcComplexProperty::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_HasProperties.size(); ++i )
	{
		if( m_HasProperties[i] )
		{
			m_HasProperties[i]->m_PartOfComplex_inverse.emplace_back( ptr_self );
		}
	}
}
void IfcComplexProperty::unlinkFromInverseCounterparts()
{
	IfcProperty::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_HasProperties.size(); ++i )
	{
		if( m_HasProperties[i] )
		{
			std::vector<weak_ptr<IfcComplexProperty> >& PartOfComplex_inverse = m_HasProperties[i]->m_PartOfComplex_inverse;
			for( auto it_PartOfComplex_inverse = PartOfComplex_inverse.begin(); it_PartOfComplex_inverse != PartOfComplex_inverse.end(); )
			{
				weak_ptr<IfcComplexProperty> self_candidate_weak = *it_PartOfComplex_inverse;
				if( self_candidate_weak.expired() )
				{
					++it_PartOfComplex_inverse;
					continue;
				}
				shared_ptr<IfcComplexProperty> self_candidate( *it_PartOfComplex_inverse );
				if( self_candidate.get() == this )
				{
					it_PartOfComplex_inverse= PartOfComplex_inverse.erase( it_PartOfComplex_inverse );
				}
				else
				{
					++it_PartOfComplex_inverse;
				}
			}
		}
	}
}
