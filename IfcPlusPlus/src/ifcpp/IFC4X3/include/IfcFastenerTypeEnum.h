/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

namespace IFC4X3
{
	// TYPE IfcFastenerTypeEnum = ENUMERATION OF	(GLUE	,MORTAR	,WELD	,USERDEFINED	,NOTDEFINED);
	class IFCQUERY_EXPORT IfcFastenerTypeEnum : virtual public BuildingObject
	{
	public:
		enum IfcFastenerTypeEnumEnum
		{
			ENUM_GLUE,
			ENUM_MORTAR,
			ENUM_WELD,
			ENUM_USERDEFINED,
			ENUM_NOTDEFINED
		};

		IfcFastenerTypeEnum() = default;
		IfcFastenerTypeEnum( IfcFastenerTypeEnumEnum e ) { m_enum = e; }
		virtual uint32_t classID() const { return 2244117335; }
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const;
		static shared_ptr<IfcFastenerTypeEnum> createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
		IfcFastenerTypeEnumEnum m_enum;
	};
}
