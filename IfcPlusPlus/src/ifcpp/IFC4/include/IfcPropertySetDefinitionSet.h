/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcPropertySetDefinitionSelect.h"
class IFCQUERY_EXPORT IfcPropertySetDefinition;

// TYPE IfcPropertySetDefinitionSet = SET [1:?] OF IfcPropertySetDefinition;
class IFCQUERY_EXPORT IfcPropertySetDefinitionSet : public IfcPropertySetDefinitionSelect
{
public:
	IfcPropertySetDefinitionSet() = default;
	~IfcPropertySetDefinitionSet();
	virtual const char* className() const { return "IfcPropertySetDefinitionSet"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcPropertySetDefinitionSet> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	std::vector<shared_ptr<IfcPropertySetDefinition> > m_vec;
};

