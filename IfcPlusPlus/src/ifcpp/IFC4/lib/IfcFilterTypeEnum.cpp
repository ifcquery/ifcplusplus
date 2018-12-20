/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcFilterTypeEnum.h"

// TYPE IfcFilterTypeEnum = ENUMERATION OF	(AIRPARTICLEFILTER	,COMPRESSEDAIRFILTER	,ODORFILTER	,OILFILTER	,STRAINER	,WATERFILTER	,USERDEFINED	,NOTDEFINED);
IfcFilterTypeEnum::IfcFilterTypeEnum() = default;
IfcFilterTypeEnum::~IfcFilterTypeEnum() = default;
shared_ptr<BuildingObject> IfcFilterTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcFilterTypeEnum> copy_self( new IfcFilterTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFilterTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFILTERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_AIRPARTICLEFILTER:	stream << ".AIRPARTICLEFILTER."; break;
		case ENUM_COMPRESSEDAIRFILTER:	stream << ".COMPRESSEDAIRFILTER."; break;
		case ENUM_ODORFILTER:	stream << ".ODORFILTER."; break;
		case ENUM_OILFILTER:	stream << ".OILFILTER."; break;
		case ENUM_STRAINER:	stream << ".STRAINER."; break;
		case ENUM_WATERFILTER:	stream << ".WATERFILTER."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcFilterTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_AIRPARTICLEFILTER:	return L"AIRPARTICLEFILTER";
		case ENUM_COMPRESSEDAIRFILTER:	return L"COMPRESSEDAIRFILTER";
		case ENUM_ODORFILTER:	return L"ODORFILTER";
		case ENUM_OILFILTER:	return L"OILFILTER";
		case ENUM_STRAINER:	return L"STRAINER";
		case ENUM_WATERFILTER:	return L"WATERFILTER";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcFilterTypeEnum> IfcFilterTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFilterTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFilterTypeEnum>(); }
	shared_ptr<IfcFilterTypeEnum> type_object( new IfcFilterTypeEnum() );
	if( boost::iequals( arg, L".AIRPARTICLEFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_AIRPARTICLEFILTER;
	}
	else if( boost::iequals( arg, L".COMPRESSEDAIRFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_COMPRESSEDAIRFILTER;
	}
	else if( boost::iequals( arg, L".ODORFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_ODORFILTER;
	}
	else if( boost::iequals( arg, L".OILFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_OILFILTER;
	}
	else if( boost::iequals( arg, L".STRAINER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_STRAINER;
	}
	else if( boost::iequals( arg, L".WATERFILTER." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_WATERFILTER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFilterTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
