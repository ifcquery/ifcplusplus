/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4X3/include/IfcPavementTypeEnum.h"

// TYPE IfcPavementTypeEnum = ENUMERATION OF	(FLEXIBLE	,RIGID	,USERDEFINED	,NOTDEFINED);
void IFC4X3::IfcPavementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const
{
	if( is_select_type ) { stream << "IFCPAVEMENTTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_FLEXIBLE:	stream << ".FLEXIBLE."; break;
		case ENUM_RIGID:	stream << ".RIGID."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IFC4X3::IfcPavementTypeEnum> IFC4X3::IfcPavementTypeEnum::createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	if( arg.size() == 0 ) { return shared_ptr<IfcPavementTypeEnum>(); }
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPavementTypeEnum>(); }
	if( arg.compare( "*" ) == 0 ) { return shared_ptr<IfcPavementTypeEnum>(); }
	shared_ptr<IfcPavementTypeEnum> type_object( new IfcPavementTypeEnum() );
	if( std_iequal( arg, ".FLEXIBLE." ) )
	{
		type_object->m_enum = IfcPavementTypeEnum::ENUM_FLEXIBLE;
	}
	else if( std_iequal( arg, ".RIGID." ) )
	{
		type_object->m_enum = IfcPavementTypeEnum::ENUM_RIGID;
	}
	else if( std_iequal( arg, ".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPavementTypeEnum::ENUM_USERDEFINED;
	}
	else if( std_iequal( arg, ".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPavementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
