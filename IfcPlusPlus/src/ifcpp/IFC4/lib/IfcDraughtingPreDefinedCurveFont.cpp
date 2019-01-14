/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcDraughtingPreDefinedCurveFont.h"
#include "ifcpp/IFC4/include/IfcLabel.h"

// ENTITY IfcDraughtingPreDefinedCurveFont 
IfcDraughtingPreDefinedCurveFont::IfcDraughtingPreDefinedCurveFont( int id ) { m_entity_id = id; }
IfcDraughtingPreDefinedCurveFont::~IfcDraughtingPreDefinedCurveFont() {}
shared_ptr<BuildingObject> IfcDraughtingPreDefinedCurveFont::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcDraughtingPreDefinedCurveFont> copy_self( new IfcDraughtingPreDefinedCurveFont() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcDraughtingPreDefinedCurveFont::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCDRAUGHTINGPREDEFINEDCURVEFONT" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDraughtingPreDefinedCurveFont::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcDraughtingPreDefinedCurveFont::toString() const { return L"IfcDraughtingPreDefinedCurveFont"; }
void IfcDraughtingPreDefinedCurveFont::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcDraughtingPreDefinedCurveFont, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
}
void IfcDraughtingPreDefinedCurveFont::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcPreDefinedCurveFont::getAttributes( vec_attributes );
}
void IfcDraughtingPreDefinedCurveFont::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcPreDefinedCurveFont::getAttributesInverse( vec_attributes_inverse );
}
void IfcDraughtingPreDefinedCurveFont::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcPreDefinedCurveFont::setInverseCounterparts( ptr_self_entity );
}
void IfcDraughtingPreDefinedCurveFont::unlinkFromInverseCounterparts()
{
	IfcPreDefinedCurveFont::unlinkFromInverseCounterparts();
}
