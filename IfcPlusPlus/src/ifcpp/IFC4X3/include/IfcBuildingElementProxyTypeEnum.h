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
	// TYPE IfcBuildingElementProxyTypeEnum = ENUMERATION OF	(COMPLEX	,ELEMENT	,PARTIAL	,PROVISIONFORSPACE	,PROVISIONFORVOID	,USERDEFINED	,NOTDEFINED);
	class IFCQUERY_EXPORT IfcBuildingElementProxyTypeEnum : virtual public BuildingObject
	{
	public:
		enum IfcBuildingElementProxyTypeEnumEnum
		{
			ENUM_COMPLEX,
			ENUM_ELEMENT,
			ENUM_PARTIAL,
			ENUM_PROVISIONFORSPACE,
			ENUM_PROVISIONFORVOID,
			ENUM_USERDEFINED,
			ENUM_NOTDEFINED
		};

		IfcBuildingElementProxyTypeEnum() = default;
		IfcBuildingElementProxyTypeEnum( IfcBuildingElementProxyTypeEnumEnum e ) { m_enum = e; }
		virtual uint32_t classID() const { return 1913101020; }
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const;
		static shared_ptr<IfcBuildingElementProxyTypeEnum> createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
		IfcBuildingElementProxyTypeEnumEnum m_enum;
	};
}
