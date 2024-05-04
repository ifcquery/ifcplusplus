/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <iostream>
#include <sstream>
#include <map>
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

namespace IFC4X3
{
	// TYPE IfcModulusOfTranslationalSubgradeReactionSelect = SELECT	(IfcBoolean	,IfcModulusOfLinearSubgradeReactionMeasure);
	class IFCQUERY_EXPORT IfcModulusOfTranslationalSubgradeReactionSelect : virtual public BuildingObject
	{
	public:
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const = 0;
		static shared_ptr<IfcModulusOfTranslationalSubgradeReactionSelect> createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
	};
}

