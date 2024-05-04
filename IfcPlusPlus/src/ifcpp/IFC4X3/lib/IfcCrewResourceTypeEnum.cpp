/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4X3/include/IfcCrewResourceTypeEnum.h"

// TYPE IfcCrewResourceTypeEnum = ENUMERATION OF	(OFFICE	,SITE	,USERDEFINED	,NOTDEFINED);
void IFC4X3::IfcCrewResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const
{
	if( is_select_type ) { stream << "IFCCREWRESOURCETYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_OFFICE:	stream << ".OFFICE."; break;
		case ENUM_SITE:	stream << ".SITE."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IFC4X3::IfcCrewResourceTypeEnum> IFC4X3::IfcCrewResourceTypeEnum::createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	if( arg.size() == 0 ) { return shared_ptr<IfcCrewResourceTypeEnum>(); }
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcCrewResourceTypeEnum>(); }
	if( arg.compare( "*" ) == 0 ) { return shared_ptr<IfcCrewResourceTypeEnum>(); }
	shared_ptr<IfcCrewResourceTypeEnum> type_object( new IfcCrewResourceTypeEnum() );
	if( std_iequal( arg, ".OFFICE." ) )
	{
		type_object->m_enum = IfcCrewResourceTypeEnum::ENUM_OFFICE;
	}
	else if( std_iequal( arg, ".SITE." ) )
	{
		type_object->m_enum = IfcCrewResourceTypeEnum::ENUM_SITE;
	}
	else if( std_iequal( arg, ".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCrewResourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( std_iequal( arg, ".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCrewResourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
