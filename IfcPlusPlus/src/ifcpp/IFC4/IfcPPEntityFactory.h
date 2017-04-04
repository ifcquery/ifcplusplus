#pragma once
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPObject.h"

class IFCPP_EXPORT IfcPPEntityFactory
{
public:
	static IfcPPEntity* createEntityObject( const std::string& class_name_upper );
};
