/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcPresentableText.h"

// TYPE IfcPresentableText = STRING;
IfcPresentableText::IfcPresentableText( std::wstring value ) { m_value = value; }
IfcPresentableText::~IfcPresentableText() {}
shared_ptr<BuildingObject> IfcPresentableText::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcPresentableText> copy_self( new IfcPresentableText() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcPresentableText::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPRESENTABLETEXT("; }
	stream << "'" << encodeStepString( m_value ) << "'";
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcPresentableText::toString() const
{
	return m_value;
}
shared_ptr<IfcPresentableText> IfcPresentableText::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPresentableText>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPresentableText>(); }
	shared_ptr<IfcPresentableText> type_object( new IfcPresentableText() );
	readString( arg, type_object->m_value );
	return type_object;
}
