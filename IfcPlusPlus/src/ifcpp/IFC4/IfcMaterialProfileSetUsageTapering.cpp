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
#include "include/IfcCardinalPointReference.h"
#include "include/IfcMaterialProfileSet.h"
#include "include/IfcMaterialProfileSetUsageTapering.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcRelAssociatesMaterial.h"

// ENTITY IfcMaterialProfileSetUsageTapering 
IfcMaterialProfileSetUsageTapering::IfcMaterialProfileSetUsageTapering() { m_entity_enum = IFCMATERIALPROFILESETUSAGETAPERING; }
IfcMaterialProfileSetUsageTapering::IfcMaterialProfileSetUsageTapering( int id ) { m_id = id; m_entity_enum = IFCMATERIALPROFILESETUSAGETAPERING; }
IfcMaterialProfileSetUsageTapering::~IfcMaterialProfileSetUsageTapering() {}
shared_ptr<IfcPPObject> IfcMaterialProfileSetUsageTapering::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialProfileSetUsageTapering> copy_self( new IfcMaterialProfileSetUsageTapering() );
	if( m_ForProfileSet ) { copy_self->m_ForProfileSet = dynamic_pointer_cast<IfcMaterialProfileSet>( m_ForProfileSet->getDeepCopy(options) ); }
	if( m_CardinalPoint ) { copy_self->m_CardinalPoint = dynamic_pointer_cast<IfcCardinalPointReference>( m_CardinalPoint->getDeepCopy(options) ); }
	if( m_ReferenceExtent ) { copy_self->m_ReferenceExtent = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_ReferenceExtent->getDeepCopy(options) ); }
	if( m_ForProfileEndSet ) { copy_self->m_ForProfileEndSet = dynamic_pointer_cast<IfcMaterialProfileSet>( m_ForProfileEndSet->getDeepCopy(options) ); }
	if( m_CardinalEndPoint ) { copy_self->m_CardinalEndPoint = dynamic_pointer_cast<IfcCardinalPointReference>( m_CardinalEndPoint->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMaterialProfileSetUsageTapering::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALPROFILESETUSAGETAPERING" << "(";
	if( m_ForProfileSet ) { stream << "#" << m_ForProfileSet->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_CardinalPoint ) { m_CardinalPoint->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ReferenceExtent ) { m_ReferenceExtent->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ForProfileEndSet ) { stream << "#" << m_ForProfileEndSet->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_CardinalEndPoint ) { m_CardinalEndPoint->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialProfileSetUsageTapering::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialProfileSetUsageTapering::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialProfileSetUsageTapering, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_ForProfileSet, map );
	m_CardinalPoint = IfcCardinalPointReference::createObjectFromSTEP( args[1] );
	m_ReferenceExtent = IfcPositiveLengthMeasure::createObjectFromSTEP( args[2] );
	readEntityReference( args[3], m_ForProfileEndSet, map );
	m_CardinalEndPoint = IfcCardinalPointReference::createObjectFromSTEP( args[4] );
}
void IfcMaterialProfileSetUsageTapering::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialProfileSetUsage::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ForProfileEndSet", m_ForProfileEndSet ) );
	vec_attributes.push_back( std::make_pair( "CardinalEndPoint", m_CardinalEndPoint ) );
}
void IfcMaterialProfileSetUsageTapering::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcMaterialProfileSetUsage::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialProfileSetUsageTapering::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialProfileSetUsage::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialProfileSetUsageTapering::unlinkFromInverseCounterparts()
{
	IfcMaterialProfileSetUsage::unlinkFromInverseCounterparts();
}
