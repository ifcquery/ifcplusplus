/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcMeasureValue.h"
#include "ifcpp/IFC4/include/IfcThermodynamicTemperatureMeasure.h"

// TYPE IfcThermodynamicTemperatureMeasure = REAL;
IfcThermodynamicTemperatureMeasure::IfcThermodynamicTemperatureMeasure() = default;
IfcThermodynamicTemperatureMeasure::IfcThermodynamicTemperatureMeasure( double value ) { m_value = value; }
IfcThermodynamicTemperatureMeasure::~IfcThermodynamicTemperatureMeasure() = default;
shared_ptr<BuildingObject> IfcThermodynamicTemperatureMeasure::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcThermodynamicTemperatureMeasure> copy_self( new IfcThermodynamicTemperatureMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcThermodynamicTemperatureMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTHERMODYNAMICTEMPERATUREMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcThermodynamicTemperatureMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcThermodynamicTemperatureMeasure> IfcThermodynamicTemperatureMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcThermodynamicTemperatureMeasure>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcThermodynamicTemperatureMeasure>(); }
	shared_ptr<IfcThermodynamicTemperatureMeasure> type_object( new IfcThermodynamicTemperatureMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
