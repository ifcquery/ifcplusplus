/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4X3/include/IfcTrackElementTypeEnum.h"

// TYPE IfcTrackElementTypeEnum = ENUMERATION OF	(BLOCKINGDEVICE	,DERAILER	,FROG	,HALF_SET_OF_BLADES	,SLEEPER	,SPEEDREGULATOR	,TRACKENDOFALIGNMENT	,VEHICLESTOP	,USERDEFINED	,NOTDEFINED);
void IFC4X3::IfcTrackElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const
{
	if( is_select_type ) { stream << "IFCTRACKELEMENTTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_BLOCKINGDEVICE:	stream << ".BLOCKINGDEVICE."; break;
		case ENUM_DERAILER:	stream << ".DERAILER."; break;
		case ENUM_FROG:	stream << ".FROG."; break;
		case ENUM_HALF_SET_OF_BLADES:	stream << ".HALF_SET_OF_BLADES."; break;
		case ENUM_SLEEPER:	stream << ".SLEEPER."; break;
		case ENUM_SPEEDREGULATOR:	stream << ".SPEEDREGULATOR."; break;
		case ENUM_TRACKENDOFALIGNMENT:	stream << ".TRACKENDOFALIGNMENT."; break;
		case ENUM_VEHICLESTOP:	stream << ".VEHICLESTOP."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IFC4X3::IfcTrackElementTypeEnum> IFC4X3::IfcTrackElementTypeEnum::createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	if( arg.size() == 0 ) { return shared_ptr<IfcTrackElementTypeEnum>(); }
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcTrackElementTypeEnum>(); }
	if( arg.compare( "*" ) == 0 ) { return shared_ptr<IfcTrackElementTypeEnum>(); }
	shared_ptr<IfcTrackElementTypeEnum> type_object( new IfcTrackElementTypeEnum() );
	if( std_iequal( arg, ".BLOCKINGDEVICE." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_BLOCKINGDEVICE;
	}
	else if( std_iequal( arg, ".DERAILER." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_DERAILER;
	}
	else if( std_iequal( arg, ".FROG." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_FROG;
	}
	else if( std_iequal( arg, ".HALF_SET_OF_BLADES." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_HALF_SET_OF_BLADES;
	}
	else if( std_iequal( arg, ".SLEEPER." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_SLEEPER;
	}
	else if( std_iequal( arg, ".SPEEDREGULATOR." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_SPEEDREGULATOR;
	}
	else if( std_iequal( arg, ".TRACKENDOFALIGNMENT." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_TRACKENDOFALIGNMENT;
	}
	else if( std_iequal( arg, ".VEHICLESTOP." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_VEHICLESTOP;
	}
	else if( std_iequal( arg, ".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( std_iequal( arg, ".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTrackElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
