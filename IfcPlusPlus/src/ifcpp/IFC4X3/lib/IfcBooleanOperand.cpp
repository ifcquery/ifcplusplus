/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/IFC4X3/include/IfcBooleanOperand.h"

// TYPE IfcBooleanOperand = SELECT	(IfcBooleanResult	,IfcCsgPrimitive3D	,IfcHalfSpaceSolid	,IfcSolidModel	,IfcTessellatedFaceSet);
shared_ptr<IFC4X3::IfcBooleanOperand> IFC4X3::IfcBooleanOperand::createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	if( arg.empty() ){ return shared_ptr<IfcBooleanOperand>(); }
	if( arg.compare("$")==0 )
	{
		return shared_ptr<IfcBooleanOperand>();
	}
	if( arg.compare("*")==0 )
	{
		return shared_ptr<IfcBooleanOperand>();
	}
	shared_ptr<IfcBooleanOperand> result_object;
	readSelectType( arg, result_object, map, errorStream, entityIdNotFound );
	return result_object;
}
