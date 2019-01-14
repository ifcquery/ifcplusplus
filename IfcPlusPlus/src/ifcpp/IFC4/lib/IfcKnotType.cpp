/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcKnotType.h"

// TYPE IfcKnotType = ENUMERATION OF	(UNIFORM_KNOTS	,QUASI_UNIFORM_KNOTS	,PIECEWISE_BEZIER_KNOTS	,UNSPECIFIED);
IfcKnotType::~IfcKnotType() {}
shared_ptr<BuildingObject> IfcKnotType::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcKnotType> copy_self( new IfcKnotType() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcKnotType::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCKNOTTYPE("; }
	switch( m_enum )
	{
		case ENUM_UNIFORM_KNOTS:	stream << ".UNIFORM_KNOTS."; break;
		case ENUM_QUASI_UNIFORM_KNOTS:	stream << ".QUASI_UNIFORM_KNOTS."; break;
		case ENUM_PIECEWISE_BEZIER_KNOTS:	stream << ".PIECEWISE_BEZIER_KNOTS."; break;
		case ENUM_UNSPECIFIED:	stream << ".UNSPECIFIED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcKnotType::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_UNIFORM_KNOTS:	return L"UNIFORM_KNOTS";
		case ENUM_QUASI_UNIFORM_KNOTS:	return L"QUASI_UNIFORM_KNOTS";
		case ENUM_PIECEWISE_BEZIER_KNOTS:	return L"PIECEWISE_BEZIER_KNOTS";
		case ENUM_UNSPECIFIED:	return L"UNSPECIFIED";
	}
	return L"";
}
shared_ptr<IfcKnotType> IfcKnotType::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcKnotType>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcKnotType>(); }
	shared_ptr<IfcKnotType> type_object( new IfcKnotType() );
	if( boost::iequals( arg, L".UNIFORM_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_UNIFORM_KNOTS;
	}
	else if( boost::iequals( arg, L".QUASI_UNIFORM_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_QUASI_UNIFORM_KNOTS;
	}
	else if( boost::iequals( arg, L".PIECEWISE_BEZIER_KNOTS." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_PIECEWISE_BEZIER_KNOTS;
	}
	else if( boost::iequals( arg, L".UNSPECIFIED." ) )
	{
		type_object->m_enum = IfcKnotType::ENUM_UNSPECIFIED;
	}
	return type_object;
}
