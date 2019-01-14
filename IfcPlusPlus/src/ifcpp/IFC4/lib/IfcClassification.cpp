/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcClassification.h"
#include "ifcpp/IFC4/include/IfcClassificationReference.h"
#include "ifcpp/IFC4/include/IfcDate.h"
#include "ifcpp/IFC4/include/IfcIdentifier.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcRelAssociatesClassification.h"
#include "ifcpp/IFC4/include/IfcText.h"
#include "ifcpp/IFC4/include/IfcURIReference.h"

// ENTITY IfcClassification 
IfcClassification::IfcClassification( int id ) { m_entity_id = id; }
IfcClassification::~IfcClassification() {}
shared_ptr<BuildingObject> IfcClassification::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcClassification> copy_self( new IfcClassification() );
	if( m_Source ) { copy_self->m_Source = dynamic_pointer_cast<IfcLabel>( m_Source->getDeepCopy(options) ); }
	if( m_Edition ) { copy_self->m_Edition = dynamic_pointer_cast<IfcLabel>( m_Edition->getDeepCopy(options) ); }
	if( m_EditionDate ) { copy_self->m_EditionDate = dynamic_pointer_cast<IfcDate>( m_EditionDate->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Location ) { copy_self->m_Location = dynamic_pointer_cast<IfcURIReference>( m_Location->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ReferenceTokens.size(); ++ii )
	{
		auto item_ii = m_ReferenceTokens[ii];
		if( item_ii )
		{
			copy_self->m_ReferenceTokens.emplace_back( dynamic_pointer_cast<IfcIdentifier>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcClassification::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCCLASSIFICATION" << "(";
	if( m_Source ) { m_Source->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Edition ) { m_Edition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EditionDate ) { m_EditionDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_ReferenceTokens.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcIdentifier>& type_object = m_ReferenceTokens[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ");";
}
void IfcClassification::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcClassification::toString() const { return L"IfcClassification"; }
void IfcClassification::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcClassification, expecting 7, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	m_Source = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Edition = IfcLabel::createObjectFromSTEP( args[1], map );
	m_EditionDate = IfcDate::createObjectFromSTEP( args[2], map );
	m_Name = IfcLabel::createObjectFromSTEP( args[3], map );
	m_Description = IfcText::createObjectFromSTEP( args[4], map );
	m_Location = IfcURIReference::createObjectFromSTEP( args[5], map );
	readTypeOfStringList( args[6], m_ReferenceTokens );
}
void IfcClassification::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcExternalInformation::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "Source", m_Source ) );
	vec_attributes.emplace_back( std::make_pair( "Edition", m_Edition ) );
	vec_attributes.emplace_back( std::make_pair( "EditionDate", m_EditionDate ) );
	vec_attributes.emplace_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.emplace_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.emplace_back( std::make_pair( "Location", m_Location ) );
	if( !m_ReferenceTokens.empty() )
	{
		shared_ptr<AttributeObjectVector> ReferenceTokens_vec_object( new AttributeObjectVector() );
		std::copy( m_ReferenceTokens.begin(), m_ReferenceTokens.end(), std::back_inserter( ReferenceTokens_vec_object->m_vec ) );
		vec_attributes.emplace_back( std::make_pair( "ReferenceTokens", ReferenceTokens_vec_object ) );
	}
}
void IfcClassification::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcExternalInformation::getAttributesInverse( vec_attributes_inverse );
	if( !m_ClassificationForObjects_inverse.empty() )
	{
		shared_ptr<AttributeObjectVector> ClassificationForObjects_inverse_vec_obj( new AttributeObjectVector() );
		for( size_t i=0; i<m_ClassificationForObjects_inverse.size(); ++i )
		{
			if( !m_ClassificationForObjects_inverse[i].expired() )
			{
				ClassificationForObjects_inverse_vec_obj->m_vec.emplace_back( shared_ptr<IfcRelAssociatesClassification>( m_ClassificationForObjects_inverse[i] ) );
			}
		}
		vec_attributes_inverse.emplace_back( std::make_pair( "ClassificationForObjects_inverse", ClassificationForObjects_inverse_vec_obj ) );
	}
	if( !m_HasReferences_inverse.empty() )
	{
		shared_ptr<AttributeObjectVector> HasReferences_inverse_vec_obj( new AttributeObjectVector() );
		for( size_t i=0; i<m_HasReferences_inverse.size(); ++i )
		{
			if( !m_HasReferences_inverse[i].expired() )
			{
				HasReferences_inverse_vec_obj->m_vec.emplace_back( shared_ptr<IfcClassificationReference>( m_HasReferences_inverse[i] ) );
			}
		}
		vec_attributes_inverse.emplace_back( std::make_pair( "HasReferences_inverse", HasReferences_inverse_vec_obj ) );
	}
}
void IfcClassification::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcExternalInformation::setInverseCounterparts( ptr_self_entity );
}
void IfcClassification::unlinkFromInverseCounterparts()
{
	IfcExternalInformation::unlinkFromInverseCounterparts();
}
