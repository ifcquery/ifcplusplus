/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcDerivedMeasureValue.h"
#include "ifcpp/IFC4/include/IfcMagneticFluxDensityMeasure.h"

// TYPE IfcMagneticFluxDensityMeasure = REAL;
IfcMagneticFluxDensityMeasure::IfcMagneticFluxDensityMeasure() = default;
IfcMagneticFluxDensityMeasure::IfcMagneticFluxDensityMeasure( double value ) { m_value = value; }
IfcMagneticFluxDensityMeasure::~IfcMagneticFluxDensityMeasure() = default;
shared_ptr<BuildingObject> IfcMagneticFluxDensityMeasure::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcMagneticFluxDensityMeasure> copy_self( new IfcMagneticFluxDensityMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcMagneticFluxDensityMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCMAGNETICFLUXDENSITYMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcMagneticFluxDensityMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcMagneticFluxDensityMeasure> IfcMagneticFluxDensityMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcMagneticFluxDensityMeasure>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcMagneticFluxDensityMeasure>(); }
	shared_ptr<IfcMagneticFluxDensityMeasure> type_object( new IfcMagneticFluxDensityMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
