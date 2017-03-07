/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcLabel.h"
#include "include/IfcStructuralLoadTemperature.h"
#include "include/IfcThermodynamicTemperatureMeasure.h"

// ENTITY IfcStructuralLoadTemperature 
IfcStructuralLoadTemperature::IfcStructuralLoadTemperature() { m_entity_enum = IFCSTRUCTURALLOADTEMPERATURE; }
IfcStructuralLoadTemperature::IfcStructuralLoadTemperature( int id ) { m_id = id; m_entity_enum = IFCSTRUCTURALLOADTEMPERATURE; }
IfcStructuralLoadTemperature::~IfcStructuralLoadTemperature() {}
shared_ptr<IfcPPObject> IfcStructuralLoadTemperature::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralLoadTemperature> copy_self( new IfcStructuralLoadTemperature() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_DeltaTConstant ) { copy_self->m_DeltaTConstant = dynamic_pointer_cast<IfcThermodynamicTemperatureMeasure>( m_DeltaTConstant->getDeepCopy(options) ); }
	if( m_DeltaTY ) { copy_self->m_DeltaTY = dynamic_pointer_cast<IfcThermodynamicTemperatureMeasure>( m_DeltaTY->getDeepCopy(options) ); }
	if( m_DeltaTZ ) { copy_self->m_DeltaTZ = dynamic_pointer_cast<IfcThermodynamicTemperatureMeasure>( m_DeltaTZ->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStructuralLoadTemperature::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALLOADTEMPERATURE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DeltaTConstant ) { m_DeltaTConstant->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DeltaTY ) { m_DeltaTY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DeltaTZ ) { m_DeltaTZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralLoadTemperature::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadTemperature::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcStructuralLoadTemperature, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_DeltaTConstant = IfcThermodynamicTemperatureMeasure::createObjectFromSTEP( args[1] );
	m_DeltaTY = IfcThermodynamicTemperatureMeasure::createObjectFromSTEP( args[2] );
	m_DeltaTZ = IfcThermodynamicTemperatureMeasure::createObjectFromSTEP( args[3] );
}
void IfcStructuralLoadTemperature::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoadStatic::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "DeltaTConstant", m_DeltaTConstant ) );
	vec_attributes.push_back( std::make_pair( "DeltaTY", m_DeltaTY ) );
	vec_attributes.push_back( std::make_pair( "DeltaTZ", m_DeltaTZ ) );
}
void IfcStructuralLoadTemperature::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralLoadStatic::getAttributesInverse( vec_attributes_inverse );
}
void IfcStructuralLoadTemperature::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoadStatic::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadTemperature::unlinkFromInverseCounterparts()
{
	IfcStructuralLoadStatic::unlinkFromInverseCounterparts();
}
