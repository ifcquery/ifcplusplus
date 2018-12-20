/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcDerivedMeasureValue.h"
#include "ifcpp/IFC4/include/IfcSoundPowerLevelMeasure.h"

// TYPE IfcSoundPowerLevelMeasure = REAL;
IfcSoundPowerLevelMeasure::IfcSoundPowerLevelMeasure() = default;
IfcSoundPowerLevelMeasure::IfcSoundPowerLevelMeasure( double value ) { m_value = value; }
IfcSoundPowerLevelMeasure::~IfcSoundPowerLevelMeasure() = default;
shared_ptr<BuildingObject> IfcSoundPowerLevelMeasure::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcSoundPowerLevelMeasure> copy_self( new IfcSoundPowerLevelMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcSoundPowerLevelMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSOUNDPOWERLEVELMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcSoundPowerLevelMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcSoundPowerLevelMeasure> IfcSoundPowerLevelMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSoundPowerLevelMeasure>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSoundPowerLevelMeasure>(); }
	shared_ptr<IfcSoundPowerLevelMeasure> type_object( new IfcSoundPowerLevelMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
