/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcDerivedMeasureValue.h"
#include "ifcpp/IFC4/include/IfcVaporPermeabilityMeasure.h"

// TYPE IfcVaporPermeabilityMeasure = REAL;
IfcVaporPermeabilityMeasure::IfcVaporPermeabilityMeasure( double value ) { m_value = value; }
IfcVaporPermeabilityMeasure::~IfcVaporPermeabilityMeasure() {}
shared_ptr<BuildingObject> IfcVaporPermeabilityMeasure::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcVaporPermeabilityMeasure> copy_self( new IfcVaporPermeabilityMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcVaporPermeabilityMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCVAPORPERMEABILITYMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcVaporPermeabilityMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcVaporPermeabilityMeasure> IfcVaporPermeabilityMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcVaporPermeabilityMeasure>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcVaporPermeabilityMeasure>(); }
	shared_ptr<IfcVaporPermeabilityMeasure> type_object( new IfcVaporPermeabilityMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
