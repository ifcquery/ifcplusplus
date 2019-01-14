/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcDocumentStatusEnum.h"

// TYPE IfcDocumentStatusEnum = ENUMERATION OF	(DRAFT	,FINALDRAFT	,FINAL	,REVISION	,NOTDEFINED);
IfcDocumentStatusEnum::~IfcDocumentStatusEnum() {}
shared_ptr<BuildingObject> IfcDocumentStatusEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcDocumentStatusEnum> copy_self( new IfcDocumentStatusEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDocumentStatusEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDOCUMENTSTATUSENUM("; }
	switch( m_enum )
	{
		case ENUM_DRAFT:	stream << ".DRAFT."; break;
		case ENUM_FINALDRAFT:	stream << ".FINALDRAFT."; break;
		case ENUM_FINAL:	stream << ".FINAL."; break;
		case ENUM_REVISION:	stream << ".REVISION."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcDocumentStatusEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_DRAFT:	return L"DRAFT";
		case ENUM_FINALDRAFT:	return L"FINALDRAFT";
		case ENUM_FINAL:	return L"FINAL";
		case ENUM_REVISION:	return L"REVISION";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcDocumentStatusEnum> IfcDocumentStatusEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDocumentStatusEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDocumentStatusEnum>(); }
	shared_ptr<IfcDocumentStatusEnum> type_object( new IfcDocumentStatusEnum() );
	if( boost::iequals( arg, L".DRAFT." ) )
	{
		type_object->m_enum = IfcDocumentStatusEnum::ENUM_DRAFT;
	}
	else if( boost::iequals( arg, L".FINALDRAFT." ) )
	{
		type_object->m_enum = IfcDocumentStatusEnum::ENUM_FINALDRAFT;
	}
	else if( boost::iequals( arg, L".FINAL." ) )
	{
		type_object->m_enum = IfcDocumentStatusEnum::ENUM_FINAL;
	}
	else if( boost::iequals( arg, L".REVISION." ) )
	{
		type_object->m_enum = IfcDocumentStatusEnum::ENUM_REVISION;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDocumentStatusEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
