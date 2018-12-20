/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcApplication.h"
#include "ifcpp/IFC4/include/IfcChangeActionEnum.h"
#include "ifcpp/IFC4/include/IfcOwnerHistory.h"
#include "ifcpp/IFC4/include/IfcPersonAndOrganization.h"
#include "ifcpp/IFC4/include/IfcStateEnum.h"
#include "ifcpp/IFC4/include/IfcTimeStamp.h"

// ENTITY IfcOwnerHistory 
IfcOwnerHistory::IfcOwnerHistory() = default;
IfcOwnerHistory::IfcOwnerHistory( int id ) { m_entity_id = id; }
IfcOwnerHistory::~IfcOwnerHistory() = default;
shared_ptr<BuildingObject> IfcOwnerHistory::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcOwnerHistory> copy_self( new IfcOwnerHistory() );
	if( m_OwningUser ) { copy_self->m_OwningUser = dynamic_pointer_cast<IfcPersonAndOrganization>( m_OwningUser->getDeepCopy(options) ); }
	if( m_OwningApplication ) { copy_self->m_OwningApplication = dynamic_pointer_cast<IfcApplication>( m_OwningApplication->getDeepCopy(options) ); }
	if( m_State ) { copy_self->m_State = dynamic_pointer_cast<IfcStateEnum>( m_State->getDeepCopy(options) ); }
	if( m_ChangeAction ) { copy_self->m_ChangeAction = dynamic_pointer_cast<IfcChangeActionEnum>( m_ChangeAction->getDeepCopy(options) ); }
	if( m_LastModifiedDate ) { copy_self->m_LastModifiedDate = dynamic_pointer_cast<IfcTimeStamp>( m_LastModifiedDate->getDeepCopy(options) ); }
	if( m_LastModifyingUser ) { copy_self->m_LastModifyingUser = dynamic_pointer_cast<IfcPersonAndOrganization>( m_LastModifyingUser->getDeepCopy(options) ); }
	if( m_LastModifyingApplication ) { copy_self->m_LastModifyingApplication = dynamic_pointer_cast<IfcApplication>( m_LastModifyingApplication->getDeepCopy(options) ); }
	if( m_CreationDate ) { copy_self->m_CreationDate = dynamic_pointer_cast<IfcTimeStamp>( m_CreationDate->getDeepCopy(options) ); }
	return copy_self;
}
void IfcOwnerHistory::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCOWNERHISTORY" << "(";
	if( m_OwningUser ) { stream << "#" << m_OwningUser->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_OwningApplication ) { stream << "#" << m_OwningApplication->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_State ) { m_State->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ChangeAction ) { m_ChangeAction->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LastModifiedDate ) { m_LastModifiedDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LastModifyingUser ) { stream << "#" << m_LastModifyingUser->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_LastModifyingApplication ) { stream << "#" << m_LastModifyingApplication->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_CreationDate ) { m_CreationDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcOwnerHistory::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcOwnerHistory::toString() const { return L"IfcOwnerHistory"; }
void IfcOwnerHistory::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 8 ){ std::stringstream err; err << "Wrong parameter count for entity IfcOwnerHistory, expecting 8, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	readEntityReference( args[0], m_OwningUser, map );
	readEntityReference( args[1], m_OwningApplication, map );
	m_State = IfcStateEnum::createObjectFromSTEP( args[2], map );
	m_ChangeAction = IfcChangeActionEnum::createObjectFromSTEP( args[3], map );
	m_LastModifiedDate = IfcTimeStamp::createObjectFromSTEP( args[4], map );
	readEntityReference( args[5], m_LastModifyingUser, map );
	readEntityReference( args[6], m_LastModifyingApplication, map );
	m_CreationDate = IfcTimeStamp::createObjectFromSTEP( args[7], map );
}
void IfcOwnerHistory::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	vec_attributes.emplace_back( "OwningUser", m_OwningUser );
	vec_attributes.emplace_back( "OwningApplication", m_OwningApplication );
	vec_attributes.emplace_back( "State", m_State );
	vec_attributes.emplace_back( "ChangeAction", m_ChangeAction );
	vec_attributes.emplace_back( "LastModifiedDate", m_LastModifiedDate );
	vec_attributes.emplace_back( "LastModifyingUser", m_LastModifyingUser );
	vec_attributes.emplace_back( "LastModifyingApplication", m_LastModifyingApplication );
	vec_attributes.emplace_back( "CreationDate", m_CreationDate );
}
void IfcOwnerHistory::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
}
void IfcOwnerHistory::setInverseCounterparts( shared_ptr<BuildingEntity>  /*ptr_self*/)
{
}
void IfcOwnerHistory::unlinkFromInverseCounterparts()
{
}
