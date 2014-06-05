
#pragma once
#include <string>
#include <vector>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"

//class IfcPPAttributeObject : public IfcPPObject
//{
//public:
//	IfcPPAttributeObject()
//	{
//	}
//	~IfcPPAttributeObject()
//	{
//	}
//};


class IfcPPAttributeObjectVector : public IfcPPObject
{
public:
	IfcPPAttributeObjectVector();
	IfcPPAttributeObjectVector( std::vector<shared_ptr<IfcPPObject> >& vec );
	~IfcPPAttributeObjectVector();
	std::vector<shared_ptr<IfcPPObject> > m_vec;
};
