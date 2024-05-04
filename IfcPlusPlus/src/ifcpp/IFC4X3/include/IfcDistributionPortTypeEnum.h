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
	// TYPE IfcDistributionPortTypeEnum = ENUMERATION OF	(CABLE	,CABLECARRIER	,DUCT	,PIPE	,WIRELESS	,USERDEFINED	,NOTDEFINED);
	class IFCQUERY_EXPORT IfcDistributionPortTypeEnum : virtual public BuildingObject
	{
	public:
		enum IfcDistributionPortTypeEnumEnum
		{
			ENUM_CABLE,
			ENUM_CABLECARRIER,
			ENUM_DUCT,
			ENUM_PIPE,
			ENUM_WIRELESS,
			ENUM_USERDEFINED,
			ENUM_NOTDEFINED
		};

		IfcDistributionPortTypeEnum() = default;
		IfcDistributionPortTypeEnum( IfcDistributionPortTypeEnumEnum e ) { m_enum = e; }
		virtual uint32_t classID() const { return 860830233; }
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const;
		static shared_ptr<IfcDistributionPortTypeEnum> createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
		IfcDistributionPortTypeEnumEnum m_enum;
	};
}
