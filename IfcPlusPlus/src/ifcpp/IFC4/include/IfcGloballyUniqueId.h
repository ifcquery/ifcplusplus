/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcGloballyUniqueId = STRING(22) FIXED;
class IFCQUERY_EXPORT IfcGloballyUniqueId : public BuildingObject
{
public:
	IfcGloballyUniqueId() = default;
	IfcGloballyUniqueId( std::wstring value );
	~IfcGloballyUniqueId();
	virtual const char* className() const { return "IfcGloballyUniqueId"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcGloballyUniqueId> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	std::wstring m_value;
};

