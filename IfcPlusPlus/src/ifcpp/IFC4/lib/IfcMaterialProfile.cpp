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
#include "ifcpp/IFC4/include/IfcMaterial.h"
#include "ifcpp/IFC4/include/IfcMaterialProfile.h"
#include "ifcpp/IFC4/include/IfcMaterialProfileSet.h"
#include "ifcpp/IFC4/include/IfcMaterialProperties.h"
#include "ifcpp/IFC4/include/IfcProfileDef.h"
#include "ifcpp/IFC4/include/IfcRelAssociatesMaterial.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcMaterialProfile 
IfcMaterialProfile::IfcMaterialProfile() = default;
IfcMaterialProfile::IfcMaterialProfile( int id ) { m_entity_id = id; }
IfcMaterialProfile::~IfcMaterialProfile() = default;
shared_ptr<BuildingObject> IfcMaterialProfile::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcMaterialProfile> copy_self( new IfcMaterialProfile() );
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
	return copy_self;
}
void IfcMaterialProfile::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCMATERIALPROFILE" << "(";
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
	stream << ");";
}
void IfcMaterialProfile::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcMaterialProfile::toString() const { return L"IfcMaterialProfile"; }
void IfcMaterialProfile::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialProfile, expecting 6, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	readEntityReference( args[2], m_Material, map );
	readEntityReference( args[3], m_Profile, map );
	m_Priority = IfcInteger::createObjectFromSTEP( args[4], map );
	m_Category = IfcLabel::createObjectFromSTEP( args[5], map );
}
void IfcMaterialProfile::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcMaterialDefinition::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "Name", m_Name );
	vec_attributes.emplace_back( "Description", m_Description );
	vec_attributes.emplace_back( "Material", m_Material );
	vec_attributes.emplace_back( "Profile", m_Profile );
	vec_attributes.emplace_back( "Priority", m_Priority );
	vec_attributes.emplace_back( "Category", m_Category );
}
void IfcMaterialProfile::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcMaterialDefinition::getAttributesInverse( vec_attributes_inverse );
	vec_attributes_inverse.emplace_back( "ToMaterialProfileSet_inverse", shared_ptr<BuildingEntity>( m_ToMaterialProfileSet_inverse ) );
}
void IfcMaterialProfile::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialProfile::unlinkFromInverseCounterparts()
{
	IfcMaterialDefinition::unlinkFromInverseCounterparts();
}
