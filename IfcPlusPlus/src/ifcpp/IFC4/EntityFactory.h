/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

class IFCQUERY_EXPORT EntityFactory
{
public:
	static BuildingEntity* createEntityObject( const std::string& class_name_upper );
	static void emptyMapOfEntities();
};
