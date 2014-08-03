
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
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const {}
	std::vector<shared_ptr<IfcPPObject> > m_vec;
};

template<typename T>
class IfcPPEnumObject : public IfcPPObject
{
public:
	IfcPPEnumObject(T en) : m_enum(en)
	{
	}
	~IfcPPEnumObject(){}
	T m_enum;
};