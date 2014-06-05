#include "IfcPPAttributeObject.h"



IfcPPAttributeObjectVector::IfcPPAttributeObjectVector()
{
}

IfcPPAttributeObjectVector::IfcPPAttributeObjectVector( std::vector<shared_ptr<IfcPPObject> >& vec )
{
	m_vec = vec;
}

IfcPPAttributeObjectVector::~IfcPPAttributeObjectVector()
{
}
