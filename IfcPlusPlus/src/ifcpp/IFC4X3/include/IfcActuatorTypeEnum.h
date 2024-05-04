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
	// TYPE IfcActuatorTypeEnum = ENUMERATION OF	(ELECTRICACTUATOR	,HANDOPERATEDACTUATOR	,HYDRAULICACTUATOR	,PNEUMATICACTUATOR	,THERMOSTATICACTUATOR	,USERDEFINED	,NOTDEFINED);
	class IFCQUERY_EXPORT IfcActuatorTypeEnum : virtual public BuildingObject
	{
	public:
		enum IfcActuatorTypeEnumEnum
		{
			ENUM_ELECTRICACTUATOR,
			ENUM_HANDOPERATEDACTUATOR,
			ENUM_HYDRAULICACTUATOR,
			ENUM_PNEUMATICACTUATOR,
			ENUM_THERMOSTATICACTUATOR,
			ENUM_USERDEFINED,
			ENUM_NOTDEFINED
		};

		IfcActuatorTypeEnum() = default;
		IfcActuatorTypeEnum( IfcActuatorTypeEnumEnum e ) { m_enum = e; }
		virtual uint32_t classID() const { return 815500815; }
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const;
		static shared_ptr<IfcActuatorTypeEnum> createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
		IfcActuatorTypeEnumEnum m_enum;
	};
}
