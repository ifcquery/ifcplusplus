/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <memory>
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcActorSelect.h"
#include "ifcpp/IFC4/include/IfcGloballyUniqueId.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcOccupant.h"
#include "ifcpp/IFC4/include/IfcOccupantTypeEnum.h"
#include "ifcpp/IFC4/include/IfcOwnerHistory.h"
#include "ifcpp/IFC4/include/IfcRelAggregates.h"
#include "ifcpp/IFC4/include/IfcRelAssigns.h"
#include "ifcpp/IFC4/include/IfcRelAssignsToActor.h"
#include "ifcpp/IFC4/include/IfcRelAssociates.h"
#include "ifcpp/IFC4/include/IfcRelDeclares.h"
#include "ifcpp/IFC4/include/IfcRelDefinesByObject.h"
#include "ifcpp/IFC4/include/IfcRelDefinesByProperties.h"
#include "ifcpp/IFC4/include/IfcRelDefinesByType.h"
#include "ifcpp/IFC4/include/IfcRelNests.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcOccupant 
IfcOccupant::IfcOccupant() = default;
IfcOccupant::IfcOccupant( int id ) { m_entity_id = id; }
IfcOccupant::~IfcOccupant() = default;
shared_ptr<BuildingObject> IfcOccupant::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcOccupant> copy_self( new IfcOccupant() );
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
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy(options) ); }
	if( m_TheActor ) { copy_self->m_TheActor = dynamic_pointer_cast<IfcActorSelect>( m_TheActor->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcOccupantTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcOccupant::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCOCCUPANT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TheActor ) { m_TheActor->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcOccupant::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcOccupant::toString() const { return L"IfcOccupant"; }
void IfcOccupant::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcOccupant, expecting 7, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
	m_Description = IfcText::createObjectFromSTEP( args[3], map );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4], map );
	m_TheActor = IfcActorSelect::createObjectFromSTEP( args[5], map );
	m_PredefinedType = IfcOccupantTypeEnum::createObjectFromSTEP( args[6], map );
}
void IfcOccupant::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcActor::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "PredefinedType", m_PredefinedType );
}
void IfcOccupant::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcActor::getAttributesInverse( vec_attributes_inverse );
}
void IfcOccupant::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcActor::setInverseCounterparts( ptr_self_entity );
}
void IfcOccupant::unlinkFromInverseCounterparts()
{
	IfcActor::unlinkFromInverseCounterparts();
}
