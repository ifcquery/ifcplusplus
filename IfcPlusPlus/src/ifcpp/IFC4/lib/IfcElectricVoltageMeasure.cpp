/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcDerivedMeasureValue.h"
#include "ifcpp/IFC4/include/IfcElectricVoltageMeasure.h"

// TYPE IfcElectricVoltageMeasure = REAL;
IfcElectricVoltageMeasure::IfcElectricVoltageMeasure() = default;
IfcElectricVoltageMeasure::IfcElectricVoltageMeasure( double value ) { m_value = value; }
IfcElectricVoltageMeasure::~IfcElectricVoltageMeasure() = default;
shared_ptr<BuildingObject> IfcElectricVoltageMeasure::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcElectricVoltageMeasure> copy_self( new IfcElectricVoltageMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcElectricVoltageMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICVOLTAGEMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcElectricVoltageMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcElectricVoltageMeasure> IfcElectricVoltageMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElectricVoltageMeasure>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElectricVoltageMeasure>(); }
	shared_ptr<IfcElectricVoltageMeasure> type_object( new IfcElectricVoltageMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
