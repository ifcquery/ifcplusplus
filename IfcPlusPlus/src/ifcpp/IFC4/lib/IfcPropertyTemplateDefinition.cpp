/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <memory>
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcGloballyUniqueId.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcOwnerHistory.h"
#include "ifcpp/IFC4/include/IfcPropertyTemplateDefinition.h"
#include "ifcpp/IFC4/include/IfcRelAssociates.h"
#include "ifcpp/IFC4/include/IfcRelDeclares.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcPropertyTemplateDefinition 
IfcPropertyTemplateDefinition::IfcPropertyTemplateDefinition() = default;
IfcPropertyTemplateDefinition::IfcPropertyTemplateDefinition( int id ) { m_entity_id = id; }
IfcPropertyTemplateDefinition::~IfcPropertyTemplateDefinition() = default;
shared_ptr<BuildingObject> IfcPropertyTemplateDefinition::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcPropertyTemplateDefinition> copy_self( new IfcPropertyTemplateDefinition() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = std::make_shared<IfcGloballyUniqueId>( createBase64Uuid<wchar_t>() ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPropertyTemplateDefinition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCPROPERTYTEMPLATEDEFINITION" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPropertyTemplateDefinition::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcPropertyTemplateDefinition::toString() const { return L"IfcPropertyTemplateDefinition"; }
void IfcPropertyTemplateDefinition::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPropertyTemplateDefinition, expecting 4, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
	m_Description = IfcText::createObjectFromSTEP( args[3], map );
}
void IfcPropertyTemplateDefinition::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcPropertyDefinition::getAttributes( vec_attributes );
}
void IfcPropertyTemplateDefinition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcPropertyDefinition::getAttributesInverse( vec_attributes_inverse );
}
void IfcPropertyTemplateDefinition::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcPropertyDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyTemplateDefinition::unlinkFromInverseCounterparts()
{
	IfcPropertyDefinition::unlinkFromInverseCounterparts();
}
