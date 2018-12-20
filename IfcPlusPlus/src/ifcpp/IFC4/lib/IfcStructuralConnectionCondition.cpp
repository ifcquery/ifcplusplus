/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcStructuralConnectionCondition.h"

// ENTITY IfcStructuralConnectionCondition 
IfcStructuralConnectionCondition::IfcStructuralConnectionCondition() = default;
IfcStructuralConnectionCondition::IfcStructuralConnectionCondition( int id ) { m_entity_id = id; }
IfcStructuralConnectionCondition::~IfcStructuralConnectionCondition() = default;
shared_ptr<BuildingObject> IfcStructuralConnectionCondition::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcStructuralConnectionCondition> copy_self( new IfcStructuralConnectionCondition() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStructuralConnectionCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCSTRUCTURALCONNECTIONCONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralConnectionCondition::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcStructuralConnectionCondition::toString() const { return L"IfcStructuralConnectionCondition"; }
void IfcStructuralConnectionCondition::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcStructuralConnectionCondition, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
}
void IfcStructuralConnectionCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	vec_attributes.emplace_back( "Name", m_Name );
}
void IfcStructuralConnectionCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
}
void IfcStructuralConnectionCondition::setInverseCounterparts( shared_ptr<BuildingEntity>  /*ptr_self*/)
{
}
void IfcStructuralConnectionCondition::unlinkFromInverseCounterparts()
{
}
