/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcFace.h"
#include "ifcpp/IFC4/include/IfcOpenShell.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcOpenShell 
IfcOpenShell::IfcOpenShell( int id ) { m_entity_id = id; }
IfcOpenShell::~IfcOpenShell() {}
shared_ptr<BuildingObject> IfcOpenShell::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcOpenShell> copy_self( new IfcOpenShell() );
	for( size_t ii=0; ii<m_CfsFaces.size(); ++ii )
	{
		auto item_ii = m_CfsFaces[ii];
		if( item_ii )
		{
			copy_self->m_CfsFaces.emplace_back( dynamic_pointer_cast<IfcFace>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcOpenShell::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCOPENSHELL" << "(";
	writeEntityList( stream, m_CfsFaces );
	stream << ");";
}
void IfcOpenShell::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcOpenShell::toString() const { return L"IfcOpenShell"; }
void IfcOpenShell::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcOpenShell, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_CfsFaces, map );
}
void IfcOpenShell::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcConnectedFaceSet::getAttributes( vec_attributes );
}
void IfcOpenShell::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcConnectedFaceSet::getAttributesInverse( vec_attributes_inverse );
}
void IfcOpenShell::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcConnectedFaceSet::setInverseCounterparts( ptr_self_entity );
}
void IfcOpenShell::unlinkFromInverseCounterparts()
{
	IfcConnectedFaceSet::unlinkFromInverseCounterparts();
}
