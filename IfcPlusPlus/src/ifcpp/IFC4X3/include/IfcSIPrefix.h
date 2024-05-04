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
	// TYPE IfcSIPrefix = ENUMERATION OF	(ATTO	,CENTI	,DECA	,DECI	,EXA	,FEMTO	,GIGA	,HECTO	,KILO	,MEGA	,MICRO	,MILLI	,NANO	,PETA	,PICO	,TERA);
	class IFCQUERY_EXPORT IfcSIPrefix : virtual public BuildingObject
	{
	public:
		enum IfcSIPrefixEnum
		{
			ENUM_ATTO,
			ENUM_CENTI,
			ENUM_DECA,
			ENUM_DECI,
			ENUM_EXA,
			ENUM_FEMTO,
			ENUM_GIGA,
			ENUM_HECTO,
			ENUM_KILO,
			ENUM_MEGA,
			ENUM_MICRO,
			ENUM_MILLI,
			ENUM_NANO,
			ENUM_PETA,
			ENUM_PICO,
			ENUM_TERA
		};

		IfcSIPrefix() = default;
		IfcSIPrefix( IfcSIPrefixEnum e ) { m_enum = e; }
		virtual uint32_t classID() const { return 1787361927; }
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const;
		static shared_ptr<IfcSIPrefix> createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
		IfcSIPrefixEnum m_enum;
	};
}
