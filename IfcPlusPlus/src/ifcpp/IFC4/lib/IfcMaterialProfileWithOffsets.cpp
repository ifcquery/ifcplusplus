/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcExternalReferenceRelationship.h"
#include "ifcpp/IFC4/include/IfcInteger.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcMaterial.h"
#include "ifcpp/IFC4/include/IfcMaterialProfileSet.h"
#include "ifcpp/IFC4/include/IfcMaterialProfileWithOffsets.h"
#include "ifcpp/IFC4/include/IfcMaterialProperties.h"
#include "ifcpp/IFC4/include/IfcProfileDef.h"
#include "ifcpp/IFC4/include/IfcRelAssociatesMaterial.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcMaterialProfileWithOffsets 
IfcMaterialProfileWithOffsets::IfcMaterialProfileWithOffsets() = default;
IfcMaterialProfileWithOffsets::IfcMaterialProfileWithOffsets( int id ) { m_entity_id = id; }
IfcMaterialProfileWithOffsets::~IfcMaterialProfileWithOffsets() = default;
shared_ptr<BuildingObject> IfcMaterialProfileWithOffsets::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcMaterialProfileWithOffsets> copy_self( new IfcMaterialProfileWithOffsets() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Material ) { copy_self->m_Material = dynamic_pointer_cast<IfcMaterial>( m_Material->getDeepCopy(options) ); }
	if( m_Profile )
	{
		if( options.shallow_copy_IfcProfileDef ) { copy_self->m_Profile = m_Profile; }
		else { copy_self->m_Profile = dynamic_pointer_cast<IfcProfileDef>( m_Profile->getDeepCopy(options) ); }
	}
	if( m_Priority ) { copy_self->m_Priority = dynamic_pointer_cast<IfcInteger>( m_Priority->getDeepCopy(options) ); }
	if( m_Category ) { copy_self->m_Category = dynamic_pointer_cast<IfcLabel>( m_Category->getDeepCopy(options) ); }
	for(auto item_ii : m_OffsetValues)
	{
			if( item_ii )
		{
			copy_self->m_OffsetValues.push_back( dynamic_pointer_cast<IfcLengthMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcMaterialProfileWithOffsets::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCMATERIALPROFILEWITHOFFSETS" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Material ) { stream << "#" << m_Material->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Profile ) { stream << "#" << m_Profile->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Priority ) { m_Priority->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeNumericTypeList( stream, m_OffsetValues );
	stream << ");";
}
void IfcMaterialProfileWithOffsets::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcMaterialProfileWithOffsets::toString() const { return L"IfcMaterialProfileWithOffsets"; }
void IfcMaterialProfileWithOffsets::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialProfileWithOffsets, expecting 7, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	readEntityReference( args[2], m_Material, map );
	readEntityReference( args[3], m_Profile, map );
	m_Priority = IfcInteger::createObjectFromSTEP( args[4], map );
	m_Category = IfcLabel::createObjectFromSTEP( args[5], map );
	readTypeOfRealList( args[6], m_OffsetValues );
}
void IfcMaterialProfileWithOffsets::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcMaterialProfile::getAttributes( vec_attributes );
	if( !m_OffsetValues.empty() )
	{
		shared_ptr<AttributeObjectVector> OffsetValues_vec_object( new AttributeObjectVector() );
		std::copy( m_OffsetValues.begin(), m_OffsetValues.end(), std::back_inserter( OffsetValues_vec_object->m_vec ) );
		vec_attributes.emplace_back( "OffsetValues", OffsetValues_vec_object );
	}
}
void IfcMaterialProfileWithOffsets::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcMaterialProfile::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialProfileWithOffsets::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcMaterialProfile::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialProfileWithOffsets::unlinkFromInverseCounterparts()
{
	IfcMaterialProfile::unlinkFromInverseCounterparts();
}
