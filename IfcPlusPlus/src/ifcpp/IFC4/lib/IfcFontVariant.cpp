/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcFontVariant.h"

// TYPE IfcFontVariant = STRING;
IfcFontVariant::IfcFontVariant( std::wstring value ) { m_value = value; }
IfcFontVariant::~IfcFontVariant() {}
shared_ptr<BuildingObject> IfcFontVariant::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcFontVariant> copy_self( new IfcFontVariant() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcFontVariant::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFONTVARIANT("; }
	stream << "'" << encodeStepString( m_value ) << "'";
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcFontVariant::toString() const
{
	return m_value;
}
shared_ptr<IfcFontVariant> IfcFontVariant::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFontVariant>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFontVariant>(); }
	shared_ptr<IfcFontVariant> type_object( new IfcFontVariant() );
	readString( arg, type_object->m_value );
	return type_object;
}
