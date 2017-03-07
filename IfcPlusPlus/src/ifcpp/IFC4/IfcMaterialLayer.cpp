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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcInteger.h"
#include "include/IfcLabel.h"
#include "include/IfcLogical.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialLayer.h"
#include "include/IfcMaterialLayerSet.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialLayer 
IfcMaterialLayer::IfcMaterialLayer() { m_entity_enum = IFCMATERIALLAYER; }
IfcMaterialLayer::IfcMaterialLayer( int id ) { m_id = id; m_entity_enum = IFCMATERIALLAYER; }
IfcMaterialLayer::~IfcMaterialLayer() {}
shared_ptr<IfcPPObject> IfcMaterialLayer::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialLayer> copy_self( new IfcMaterialLayer() );
	if( m_Material ) { copy_self->m_Material = dynamic_pointer_cast<IfcMaterial>( m_Material->getDeepCopy(options) ); }
	if( m_LayerThickness ) { copy_self->m_LayerThickness = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_LayerThickness->getDeepCopy(options) ); }
	if( m_IsVentilated ) { copy_self->m_IsVentilated = dynamic_pointer_cast<IfcLogical>( m_IsVentilated->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Category ) { copy_self->m_Category = dynamic_pointer_cast<IfcLabel>( m_Category->getDeepCopy(options) ); }
	if( m_Priority ) { copy_self->m_Priority = dynamic_pointer_cast<IfcInteger>( m_Priority->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMaterialLayer::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALLAYER" << "(";
	if( m_Material ) { stream << "#" << m_Material->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_LayerThickness ) { m_LayerThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_IsVentilated ) { m_IsVentilated->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Priority ) { m_Priority->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialLayer::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialLayer::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialLayer, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Material, map );
	m_LayerThickness = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[1] );
	m_IsVentilated = IfcLogical::createObjectFromSTEP( args[2] );
	m_Name = IfcLabel::createObjectFromSTEP( args[3] );
	m_Description = IfcText::createObjectFromSTEP( args[4] );
	m_Category = IfcLabel::createObjectFromSTEP( args[5] );
	m_Priority = IfcInteger::createObjectFromSTEP( args[6] );
}
void IfcMaterialLayer::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Material", m_Material ) );
	vec_attributes.push_back( std::make_pair( "LayerThickness", m_LayerThickness ) );
	vec_attributes.push_back( std::make_pair( "IsVentilated", m_IsVentilated ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "Category", m_Category ) );
	vec_attributes.push_back( std::make_pair( "Priority", m_Priority ) );
}
void IfcMaterialLayer::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcMaterialDefinition::getAttributesInverse( vec_attributes_inverse );
	vec_attributes_inverse.push_back( std::make_pair( "ToMaterialLayerSet_inverse", shared_ptr<IfcPPEntity>( m_ToMaterialLayerSet_inverse ) ) );
}
void IfcMaterialLayer::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialLayer::unlinkFromInverseCounterparts()
{
	IfcMaterialDefinition::unlinkFromInverseCounterparts();
}
