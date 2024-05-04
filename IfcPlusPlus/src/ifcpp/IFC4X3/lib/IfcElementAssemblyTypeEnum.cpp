/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4X3/include/IfcElementAssemblyTypeEnum.h"

// TYPE IfcElementAssemblyTypeEnum = ENUMERATION OF	(ABUTMENT	,ACCESSORY_ASSEMBLY	,ARCH	,BEAM_GRID	,BRACED_FRAME	,CROSS_BRACING	,DECK	,DILATATIONPANEL	,ENTRANCEWORKS	,GIRDER	,GRID	,MAST	,PIER	,PYLON	,RAIL_MECHANICAL_EQUIPMENT_ASSEMBLY	,REINFORCEMENT_UNIT	,RIGID_FRAME	,SHELTER	,SIGNALASSEMBLY	,SLAB_FIELD	,SUMPBUSTER	,SUPPORTINGASSEMBLY	,SUSPENSIONASSEMBLY	,TRACKPANEL	,TRACTION_SWITCHING_ASSEMBLY	,TRAFFIC_CALMING_DEVICE	,TRUSS	,TURNOUTPANEL	,USERDEFINED	,NOTDEFINED);
void IFC4X3::IfcElementAssemblyTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const
{
	if( is_select_type ) { stream << "IFCELEMENTASSEMBLYTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_ABUTMENT:	stream << ".ABUTMENT."; break;
		case ENUM_ACCESSORY_ASSEMBLY:	stream << ".ACCESSORY_ASSEMBLY."; break;
		case ENUM_ARCH:	stream << ".ARCH."; break;
		case ENUM_BEAM_GRID:	stream << ".BEAM_GRID."; break;
		case ENUM_BRACED_FRAME:	stream << ".BRACED_FRAME."; break;
		case ENUM_CROSS_BRACING:	stream << ".CROSS_BRACING."; break;
		case ENUM_DECK:	stream << ".DECK."; break;
		case ENUM_DILATATIONPANEL:	stream << ".DILATATIONPANEL."; break;
		case ENUM_ENTRANCEWORKS:	stream << ".ENTRANCEWORKS."; break;
		case ENUM_GIRDER:	stream << ".GIRDER."; break;
		case ENUM_GRID:	stream << ".GRID."; break;
		case ENUM_MAST:	stream << ".MAST."; break;
		case ENUM_PIER:	stream << ".PIER."; break;
		case ENUM_PYLON:	stream << ".PYLON."; break;
		case ENUM_RAIL_MECHANICAL_EQUIPMENT_ASSEMBLY:	stream << ".RAIL_MECHANICAL_EQUIPMENT_ASSEMBLY."; break;
		case ENUM_REINFORCEMENT_UNIT:	stream << ".REINFORCEMENT_UNIT."; break;
		case ENUM_RIGID_FRAME:	stream << ".RIGID_FRAME."; break;
		case ENUM_SHELTER:	stream << ".SHELTER."; break;
		case ENUM_SIGNALASSEMBLY:	stream << ".SIGNALASSEMBLY."; break;
		case ENUM_SLAB_FIELD:	stream << ".SLAB_FIELD."; break;
		case ENUM_SUMPBUSTER:	stream << ".SUMPBUSTER."; break;
		case ENUM_SUPPORTINGASSEMBLY:	stream << ".SUPPORTINGASSEMBLY."; break;
		case ENUM_SUSPENSIONASSEMBLY:	stream << ".SUSPENSIONASSEMBLY."; break;
		case ENUM_TRACKPANEL:	stream << ".TRACKPANEL."; break;
		case ENUM_TRACTION_SWITCHING_ASSEMBLY:	stream << ".TRACTION_SWITCHING_ASSEMBLY."; break;
		case ENUM_TRAFFIC_CALMING_DEVICE:	stream << ".TRAFFIC_CALMING_DEVICE."; break;
		case ENUM_TRUSS:	stream << ".TRUSS."; break;
		case ENUM_TURNOUTPANEL:	stream << ".TURNOUTPANEL."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IFC4X3::IfcElementAssemblyTypeEnum> IFC4X3::IfcElementAssemblyTypeEnum::createObjectFromSTEP( const std::string& arg, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	if( arg.size() == 0 ) { return shared_ptr<IfcElementAssemblyTypeEnum>(); }
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcElementAssemblyTypeEnum>(); }
	if( arg.compare( "*" ) == 0 ) { return shared_ptr<IfcElementAssemblyTypeEnum>(); }
	shared_ptr<IfcElementAssemblyTypeEnum> type_object( new IfcElementAssemblyTypeEnum() );
	if( std_iequal( arg, ".ABUTMENT." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_ABUTMENT;
	}
	else if( std_iequal( arg, ".ACCESSORY_ASSEMBLY." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_ACCESSORY_ASSEMBLY;
	}
	else if( std_iequal( arg, ".ARCH." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_ARCH;
	}
	else if( std_iequal( arg, ".BEAM_GRID." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_BEAM_GRID;
	}
	else if( std_iequal( arg, ".BRACED_FRAME." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_BRACED_FRAME;
	}
	else if( std_iequal( arg, ".CROSS_BRACING." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_CROSS_BRACING;
	}
	else if( std_iequal( arg, ".DECK." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_DECK;
	}
	else if( std_iequal( arg, ".DILATATIONPANEL." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_DILATATIONPANEL;
	}
	else if( std_iequal( arg, ".ENTRANCEWORKS." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_ENTRANCEWORKS;
	}
	else if( std_iequal( arg, ".GIRDER." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_GIRDER;
	}
	else if( std_iequal( arg, ".GRID." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_GRID;
	}
	else if( std_iequal( arg, ".MAST." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_MAST;
	}
	else if( std_iequal( arg, ".PIER." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_PIER;
	}
	else if( std_iequal( arg, ".PYLON." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_PYLON;
	}
	else if( std_iequal( arg, ".RAIL_MECHANICAL_EQUIPMENT_ASSEMBLY." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_RAIL_MECHANICAL_EQUIPMENT_ASSEMBLY;
	}
	else if( std_iequal( arg, ".REINFORCEMENT_UNIT." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_REINFORCEMENT_UNIT;
	}
	else if( std_iequal( arg, ".RIGID_FRAME." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_RIGID_FRAME;
	}
	else if( std_iequal( arg, ".SHELTER." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_SHELTER;
	}
	else if( std_iequal( arg, ".SIGNALASSEMBLY." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_SIGNALASSEMBLY;
	}
	else if( std_iequal( arg, ".SLAB_FIELD." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_SLAB_FIELD;
	}
	else if( std_iequal( arg, ".SUMPBUSTER." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_SUMPBUSTER;
	}
	else if( std_iequal( arg, ".SUPPORTINGASSEMBLY." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_SUPPORTINGASSEMBLY;
	}
	else if( std_iequal( arg, ".SUSPENSIONASSEMBLY." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_SUSPENSIONASSEMBLY;
	}
	else if( std_iequal( arg, ".TRACKPANEL." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_TRACKPANEL;
	}
	else if( std_iequal( arg, ".TRACTION_SWITCHING_ASSEMBLY." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_TRACTION_SWITCHING_ASSEMBLY;
	}
	else if( std_iequal( arg, ".TRAFFIC_CALMING_DEVICE." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_TRAFFIC_CALMING_DEVICE;
	}
	else if( std_iequal( arg, ".TRUSS." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_TRUSS;
	}
	else if( std_iequal( arg, ".TURNOUTPANEL." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_TURNOUTPANEL;
	}
	else if( std_iequal( arg, ".USERDEFINED." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_USERDEFINED;
	}
	else if( std_iequal( arg, ".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
