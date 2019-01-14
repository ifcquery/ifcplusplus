/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcObjectTypeEnum = ENUMERATION OF	(PRODUCT	,PROCESS	,CONTROL	,RESOURCE	,ACTOR	,GROUP	,PROJECT	,NOTDEFINED);
class IFCQUERY_EXPORT IfcObjectTypeEnum : virtual public BuildingObject
{
public:
	enum IfcObjectTypeEnumEnum
	{
		ENUM_PRODUCT,
		ENUM_PROCESS,
		ENUM_CONTROL,
		ENUM_RESOURCE,
		ENUM_ACTOR,
		ENUM_GROUP,
		ENUM_PROJECT,
		ENUM_NOTDEFINED
	};

	IfcObjectTypeEnum() = default;
	IfcObjectTypeEnum( IfcObjectTypeEnumEnum e ) { m_enum = e; }
	~IfcObjectTypeEnum();
	virtual const char* className() const { return "IfcObjectTypeEnum"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcObjectTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	IfcObjectTypeEnumEnum m_enum;
};

