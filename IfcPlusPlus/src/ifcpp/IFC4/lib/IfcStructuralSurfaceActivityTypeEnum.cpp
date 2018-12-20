/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcStructuralSurfaceActivityTypeEnum.h"

// TYPE IfcStructuralSurfaceActivityTypeEnum = ENUMERATION OF	(CONST	,BILINEAR	,DISCRETE	,ISOCONTOUR	,USERDEFINED	,NOTDEFINED);
IfcStructuralSurfaceActivityTypeEnum::IfcStructuralSurfaceActivityTypeEnum() = default;
IfcStructuralSurfaceActivityTypeEnum::~IfcStructuralSurfaceActivityTypeEnum() = default;
shared_ptr<BuildingObject> IfcStructuralSurfaceActivityTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcStructuralSurfaceActivityTypeEnum> copy_self( new IfcStructuralSurfaceActivityTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcStructuralSurfaceActivityTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTRUCTURALSURFACEACTIVITYTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_CONST:	stream << ".CONST."; break;
		case ENUM_BILINEAR:	stream << ".BILINEAR."; break;
		case ENUM_DISCRETE:	stream << ".DISCRETE."; break;
		case ENUM_ISOCONTOUR:	stream << ".ISOCONTOUR."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcStructuralSurfaceActivityTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_CONST:	return L"CONST";
		case ENUM_BILINEAR:	return L"BILINEAR";
		case ENUM_DISCRETE:	return L"DISCRETE";
		case ENUM_ISOCONTOUR:	return L"ISOCONTOUR";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcStructuralSurfaceActivityTypeEnum> IfcStructuralSurfaceActivityTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcStructuralSurfaceActivityTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcStructuralSurfaceActivityTypeEnum>(); }
	shared_ptr<IfcStructuralSurfaceActivityTypeEnum> type_object( new IfcStructuralSurfaceActivityTypeEnum() );
	if( boost::iequals( arg, L".CONST." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceActivityTypeEnum::ENUM_CONST;
	}
	else if( boost::iequals( arg, L".BILINEAR." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceActivityTypeEnum::ENUM_BILINEAR;
	}
	else if( boost::iequals( arg, L".DISCRETE." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceActivityTypeEnum::ENUM_DISCRETE;
	}
	else if( boost::iequals( arg, L".ISOCONTOUR." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceActivityTypeEnum::ENUM_ISOCONTOUR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceActivityTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceActivityTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
