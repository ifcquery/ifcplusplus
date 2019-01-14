/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcFanTypeEnum.h"

// TYPE IfcFanTypeEnum = ENUMERATION OF	(CENTRIFUGALFORWARDCURVED	,CENTRIFUGALRADIAL	,CENTRIFUGALBACKWARDINCLINEDCURVED	,CENTRIFUGALAIRFOIL	,TUBEAXIAL	,VANEAXIAL	,PROPELLORAXIAL	,USERDEFINED	,NOTDEFINED);
IfcFanTypeEnum::~IfcFanTypeEnum() {}
shared_ptr<BuildingObject> IfcFanTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcFanTypeEnum> copy_self( new IfcFanTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFanTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFANTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_CENTRIFUGALFORWARDCURVED:	stream << ".CENTRIFUGALFORWARDCURVED."; break;
		case ENUM_CENTRIFUGALRADIAL:	stream << ".CENTRIFUGALRADIAL."; break;
		case ENUM_CENTRIFUGALBACKWARDINCLINEDCURVED:	stream << ".CENTRIFUGALBACKWARDINCLINEDCURVED."; break;
		case ENUM_CENTRIFUGALAIRFOIL:	stream << ".CENTRIFUGALAIRFOIL."; break;
		case ENUM_TUBEAXIAL:	stream << ".TUBEAXIAL."; break;
		case ENUM_VANEAXIAL:	stream << ".VANEAXIAL."; break;
		case ENUM_PROPELLORAXIAL:	stream << ".PROPELLORAXIAL."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcFanTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_CENTRIFUGALFORWARDCURVED:	return L"CENTRIFUGALFORWARDCURVED";
		case ENUM_CENTRIFUGALRADIAL:	return L"CENTRIFUGALRADIAL";
		case ENUM_CENTRIFUGALBACKWARDINCLINEDCURVED:	return L"CENTRIFUGALBACKWARDINCLINEDCURVED";
		case ENUM_CENTRIFUGALAIRFOIL:	return L"CENTRIFUGALAIRFOIL";
		case ENUM_TUBEAXIAL:	return L"TUBEAXIAL";
		case ENUM_VANEAXIAL:	return L"VANEAXIAL";
		case ENUM_PROPELLORAXIAL:	return L"PROPELLORAXIAL";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcFanTypeEnum> IfcFanTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFanTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFanTypeEnum>(); }
	shared_ptr<IfcFanTypeEnum> type_object( new IfcFanTypeEnum() );
	if( boost::iequals( arg, L".CENTRIFUGALFORWARDCURVED." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_CENTRIFUGALFORWARDCURVED;
	}
	else if( boost::iequals( arg, L".CENTRIFUGALRADIAL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_CENTRIFUGALRADIAL;
	}
	else if( boost::iequals( arg, L".CENTRIFUGALBACKWARDINCLINEDCURVED." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_CENTRIFUGALBACKWARDINCLINEDCURVED;
	}
	else if( boost::iequals( arg, L".CENTRIFUGALAIRFOIL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_CENTRIFUGALAIRFOIL;
	}
	else if( boost::iequals( arg, L".TUBEAXIAL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_TUBEAXIAL;
	}
	else if( boost::iequals( arg, L".VANEAXIAL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_VANEAXIAL;
	}
	else if( boost::iequals( arg, L".PROPELLORAXIAL." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_PROPELLORAXIAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFanTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
