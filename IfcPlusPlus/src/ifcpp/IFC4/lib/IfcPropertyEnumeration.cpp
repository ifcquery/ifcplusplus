/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcExternalReferenceRelationship.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcPropertyEnumeration.h"
#include "ifcpp/IFC4/include/IfcUnit.h"
#include "ifcpp/IFC4/include/IfcValue.h"

// ENTITY IfcPropertyEnumeration 
IfcPropertyEnumeration::IfcPropertyEnumeration() = default;
IfcPropertyEnumeration::IfcPropertyEnumeration( int id ) { m_entity_id = id; }
IfcPropertyEnumeration::~IfcPropertyEnumeration() = default;
shared_ptr<BuildingObject> IfcPropertyEnumeration::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcPropertyEnumeration> copy_self( new IfcPropertyEnumeration() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	for(auto item_ii : m_EnumerationValues)
	{
			if( item_ii )
		{
			copy_self->m_EnumerationValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcUnit>( m_Unit->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPropertyEnumeration::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCPROPERTYENUMERATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_EnumerationValues.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcValue>& type_object = m_EnumerationValues[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcPropertyEnumeration::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcPropertyEnumeration::toString() const { return L"IfcPropertyEnumeration"; }
void IfcPropertyEnumeration::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPropertyEnumeration, expecting 3, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	readSelectList( args[1], m_EnumerationValues, map );
	m_Unit = IfcUnit::createObjectFromSTEP( args[2], map );
}
void IfcPropertyEnumeration::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcPropertyAbstraction::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "Name", m_Name );
	if( !m_EnumerationValues.empty() )
	{
		shared_ptr<AttributeObjectVector> EnumerationValues_vec_object( new AttributeObjectVector() );
		std::copy( m_EnumerationValues.begin(), m_EnumerationValues.end(), std::back_inserter( EnumerationValues_vec_object->m_vec ) );
		vec_attributes.emplace_back( "EnumerationValues", EnumerationValues_vec_object );
	}
	vec_attributes.emplace_back( "Unit", m_Unit );
}
void IfcPropertyEnumeration::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcPropertyAbstraction::getAttributesInverse( vec_attributes_inverse );
}
void IfcPropertyEnumeration::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcPropertyAbstraction::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyEnumeration::unlinkFromInverseCounterparts()
{
	IfcPropertyAbstraction::unlinkFromInverseCounterparts();
}
