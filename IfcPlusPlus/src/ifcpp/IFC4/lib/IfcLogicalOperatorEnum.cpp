/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcLogicalOperatorEnum.h"

// TYPE IfcLogicalOperatorEnum = ENUMERATION OF	(LOGICALAND	,LOGICALOR	,LOGICALXOR	,LOGICALNOTAND	,LOGICALNOTOR);
IfcLogicalOperatorEnum::~IfcLogicalOperatorEnum() {}
shared_ptr<BuildingObject> IfcLogicalOperatorEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcLogicalOperatorEnum> copy_self( new IfcLogicalOperatorEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcLogicalOperatorEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLOGICALOPERATORENUM("; }
	switch( m_enum )
	{
		case ENUM_LOGICALAND:	stream << ".LOGICALAND."; break;
		case ENUM_LOGICALOR:	stream << ".LOGICALOR."; break;
		case ENUM_LOGICALXOR:	stream << ".LOGICALXOR."; break;
		case ENUM_LOGICALNOTAND:	stream << ".LOGICALNOTAND."; break;
		case ENUM_LOGICALNOTOR:	stream << ".LOGICALNOTOR."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcLogicalOperatorEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_LOGICALAND:	return L"LOGICALAND";
		case ENUM_LOGICALOR:	return L"LOGICALOR";
		case ENUM_LOGICALXOR:	return L"LOGICALXOR";
		case ENUM_LOGICALNOTAND:	return L"LOGICALNOTAND";
		case ENUM_LOGICALNOTOR:	return L"LOGICALNOTOR";
	}
	return L"";
}
shared_ptr<IfcLogicalOperatorEnum> IfcLogicalOperatorEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcLogicalOperatorEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcLogicalOperatorEnum>(); }
	shared_ptr<IfcLogicalOperatorEnum> type_object( new IfcLogicalOperatorEnum() );
	if( boost::iequals( arg, L".LOGICALAND." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALAND;
	}
	else if( boost::iequals( arg, L".LOGICALOR." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALOR;
	}
	else if( boost::iequals( arg, L".LOGICALXOR." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALXOR;
	}
	else if( boost::iequals( arg, L".LOGICALNOTAND." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALNOTAND;
	}
	else if( boost::iequals( arg, L".LOGICALNOTOR." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALNOTOR;
	}
	return type_object;
}
