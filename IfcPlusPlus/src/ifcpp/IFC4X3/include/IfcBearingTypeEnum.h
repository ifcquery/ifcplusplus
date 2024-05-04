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
	// TYPE IfcBearingTypeEnum = ENUMERATION OF	(CYLINDRICAL	,DISK	,ELASTOMERIC	,GUIDE	,POT	,ROCKER	,ROLLER	,SPHERICAL	,USERDEFINED	,NOTDEFINED);
	class IFCQUERY_EXPORT IfcBearingTypeEnum : virtual public BuildingObject
	{
	public:
		enum IfcBearingTypeEnumEnum
		{
			ENUM_CYLINDRICAL,
			ENUM_DISK,
			ENUM_ELASTOMERIC,
			ENUM_GUIDE,
			ENUM_POT,
			ENUM_ROCKER,
			ENUM_ROLLER,
			ENUM_SPHERICAL,
			ENUM_USERDEFINED,
			ENUM_NOTDEFINED
		};

		IfcBearingTypeEnum() = default;
		IfcBearingTypeEnum( IfcBearingTypeEnumEnum e ) { m_enum = e; }
		virtual uint32_t classID() const { return 3124462625; }
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const;
		static shared_ptr<IfcBearingTypeEnum> createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
		IfcBearingTypeEnumEnum m_enum;
	};
}
