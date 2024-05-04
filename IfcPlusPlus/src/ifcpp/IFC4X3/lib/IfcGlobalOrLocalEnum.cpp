/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4X3/include/IfcGlobalOrLocalEnum.h"

// TYPE IfcGlobalOrLocalEnum = ENUMERATION OF	(GLOBAL_COORDS	,LOCAL_COORDS);
void IFC4X3::IfcGlobalOrLocalEnum::getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const
{
	if( is_select_type ) { stream << "IFCGLOBALORLOCALENUM("; }
	switch( m_enum )
	{
		case ENUM_GLOBAL_COORDS:	stream << ".GLOBAL_COORDS."; break;
		case ENUM_LOCAL_COORDS:	stream << ".LOCAL_COORDS."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IFC4X3::IfcGlobalOrLocalEnum> IFC4X3::IfcGlobalOrLocalEnum::createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	if( arg.size() == 0 ) { return shared_ptr<IfcGlobalOrLocalEnum>(); }
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcGlobalOrLocalEnum>(); }
	if( arg.compare( "*" ) == 0 ) { return shared_ptr<IfcGlobalOrLocalEnum>(); }
	shared_ptr<IfcGlobalOrLocalEnum> type_object( new IfcGlobalOrLocalEnum() );
	if( std_iequal( arg, ".GLOBAL_COORDS." ) )
	{
		type_object->m_enum = IfcGlobalOrLocalEnum::ENUM_GLOBAL_COORDS;
	}
	else if( std_iequal( arg, ".LOCAL_COORDS." ) )
	{
		type_object->m_enum = IfcGlobalOrLocalEnum::ENUM_LOCAL_COORDS;
	}
	return type_object;
}
