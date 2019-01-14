/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcElectricGeneratorTypeEnum = ENUMERATION OF	(CHP	,ENGINEGENERATOR	,STANDALONE	,USERDEFINED	,NOTDEFINED);
class IFCQUERY_EXPORT IfcElectricGeneratorTypeEnum : virtual public BuildingObject
{
public:
	enum IfcElectricGeneratorTypeEnumEnum
	{
		ENUM_CHP,
		ENUM_ENGINEGENERATOR,
		ENUM_STANDALONE,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcElectricGeneratorTypeEnum() = default;
	IfcElectricGeneratorTypeEnum( IfcElectricGeneratorTypeEnumEnum e ) { m_enum = e; }
	~IfcElectricGeneratorTypeEnum();
	virtual const char* className() const { return "IfcElectricGeneratorTypeEnum"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcElectricGeneratorTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	IfcElectricGeneratorTypeEnumEnum m_enum;
};

