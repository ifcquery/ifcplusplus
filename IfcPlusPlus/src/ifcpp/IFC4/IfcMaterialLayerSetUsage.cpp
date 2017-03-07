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
#include "include/IfcDirectionSenseEnum.h"
#include "include/IfcLayerSetDirectionEnum.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcMaterialLayerSet.h"
#include "include/IfcMaterialLayerSetUsage.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcRelAssociatesMaterial.h"

// ENTITY IfcMaterialLayerSetUsage 
IfcMaterialLayerSetUsage::IfcMaterialLayerSetUsage() { m_entity_enum = IFCMATERIALLAYERSETUSAGE; }
IfcMaterialLayerSetUsage::IfcMaterialLayerSetUsage( int id ) { m_id = id; m_entity_enum = IFCMATERIALLAYERSETUSAGE; }
IfcMaterialLayerSetUsage::~IfcMaterialLayerSetUsage() {}
shared_ptr<IfcPPObject> IfcMaterialLayerSetUsage::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialLayerSetUsage> copy_self( new IfcMaterialLayerSetUsage() );
	if( m_ForLayerSet ) { copy_self->m_ForLayerSet = dynamic_pointer_cast<IfcMaterialLayerSet>( m_ForLayerSet->getDeepCopy(options) ); }
	if( m_LayerSetDirection ) { copy_self->m_LayerSetDirection = dynamic_pointer_cast<IfcLayerSetDirectionEnum>( m_LayerSetDirection->getDeepCopy(options) ); }
	if( m_DirectionSense ) { copy_self->m_DirectionSense = dynamic_pointer_cast<IfcDirectionSenseEnum>( m_DirectionSense->getDeepCopy(options) ); }
	if( m_OffsetFromReferenceLine ) { copy_self->m_OffsetFromReferenceLine = dynamic_pointer_cast<IfcLengthMeasure>( m_OffsetFromReferenceLine->getDeepCopy(options) ); }
	if( m_ReferenceExtent ) { copy_self->m_ReferenceExtent = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_ReferenceExtent->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMaterialLayerSetUsage::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALLAYERSETUSAGE" << "(";
	if( m_ForLayerSet ) { stream << "#" << m_ForLayerSet->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_LayerSetDirection ) { m_LayerSetDirection->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DirectionSense ) { m_DirectionSense->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OffsetFromReferenceLine ) { m_OffsetFromReferenceLine->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReferenceExtent ) { m_ReferenceExtent->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialLayerSetUsage::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialLayerSetUsage::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialLayerSetUsage, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_ForLayerSet, map );
	m_LayerSetDirection = IfcLayerSetDirectionEnum::createObjectFromSTEP( args[1] );
	m_DirectionSense = IfcDirectionSenseEnum::createObjectFromSTEP( args[2] );
	m_OffsetFromReferenceLine = IfcLengthMeasure::createObjectFromSTEP( args[3] );
	m_ReferenceExtent = IfcPositiveLengthMeasure::createObjectFromSTEP( args[4] );
}
void IfcMaterialLayerSetUsage::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialUsageDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ForLayerSet", m_ForLayerSet ) );
	vec_attributes.push_back( std::make_pair( "LayerSetDirection", m_LayerSetDirection ) );
	vec_attributes.push_back( std::make_pair( "DirectionSense", m_DirectionSense ) );
	vec_attributes.push_back( std::make_pair( "OffsetFromReferenceLine", m_OffsetFromReferenceLine ) );
	vec_attributes.push_back( std::make_pair( "ReferenceExtent", m_ReferenceExtent ) );
}
void IfcMaterialLayerSetUsage::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcMaterialUsageDefinition::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialLayerSetUsage::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialUsageDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialLayerSetUsage::unlinkFromInverseCounterparts()
{
	IfcMaterialUsageDefinition::unlinkFromInverseCounterparts();
}
