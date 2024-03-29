/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/IFC4X3/include/IfcShell.h"

// TYPE IfcShell = SELECT	(IfcClosedShell	,IfcOpenShell);
shared_ptr<IFC4X3::IfcShell> IFC4X3::IfcShell::createObjectFromSTEP( const std::string& arg, const std::map<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream )
{
	if( arg.empty() ){ return shared_ptr<IfcShell>(); }
	if( arg.compare("$")==0 )
	{
		return shared_ptr<IfcShell>();
	}
	if( arg.compare("*")==0 )
	{
		return shared_ptr<IfcShell>();
	}
	shared_ptr<IfcShell> result_object;
	readSelectType( arg, result_object, map, errorStream );
	return result_object;
}
