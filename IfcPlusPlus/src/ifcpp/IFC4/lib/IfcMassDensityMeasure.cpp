/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcDerivedMeasureValue.h"
#include "ifcpp/IFC4/include/IfcMassDensityMeasure.h"

// TYPE IfcMassDensityMeasure = REAL;
IfcMassDensityMeasure::IfcMassDensityMeasure() = default;
IfcMassDensityMeasure::IfcMassDensityMeasure( double value ) { m_value = value; }
IfcMassDensityMeasure::~IfcMassDensityMeasure() = default;
shared_ptr<BuildingObject> IfcMassDensityMeasure::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcMassDensityMeasure> copy_self( new IfcMassDensityMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcMassDensityMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCMASSDENSITYMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcMassDensityMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcMassDensityMeasure> IfcMassDensityMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcMassDensityMeasure>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcMassDensityMeasure>(); }
	shared_ptr<IfcMassDensityMeasure> type_object( new IfcMassDensityMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
