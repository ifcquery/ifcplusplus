/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcDerivedMeasureValue.h"
#include "ifcpp/IFC4/include/IfcShearModulusMeasure.h"

// TYPE IfcShearModulusMeasure = REAL;
IfcShearModulusMeasure::IfcShearModulusMeasure() = default;
IfcShearModulusMeasure::IfcShearModulusMeasure( double value ) { m_value = value; }
IfcShearModulusMeasure::~IfcShearModulusMeasure() = default;
shared_ptr<BuildingObject> IfcShearModulusMeasure::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcShearModulusMeasure> copy_self( new IfcShearModulusMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcShearModulusMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSHEARMODULUSMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcShearModulusMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcShearModulusMeasure> IfcShearModulusMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcShearModulusMeasure>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcShearModulusMeasure>(); }
	shared_ptr<IfcShearModulusMeasure> type_object( new IfcShearModulusMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
