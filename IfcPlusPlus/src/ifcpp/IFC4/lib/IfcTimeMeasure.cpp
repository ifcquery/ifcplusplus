/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcMeasureValue.h"
#include "ifcpp/IFC4/include/IfcTimeMeasure.h"

// TYPE IfcTimeMeasure = REAL;
IfcTimeMeasure::IfcTimeMeasure() = default;
IfcTimeMeasure::IfcTimeMeasure( double value ) { m_value = value; }
IfcTimeMeasure::~IfcTimeMeasure() = default;
shared_ptr<BuildingObject> IfcTimeMeasure::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcTimeMeasure> copy_self( new IfcTimeMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcTimeMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTIMEMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcTimeMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcTimeMeasure> IfcTimeMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTimeMeasure>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTimeMeasure>(); }
	shared_ptr<IfcTimeMeasure> type_object( new IfcTimeMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
