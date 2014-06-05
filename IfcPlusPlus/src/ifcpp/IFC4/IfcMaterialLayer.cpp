/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcLogical.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialLayer.h"
#include "include/IfcMaterialLayerSet.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialLayer 
IfcMaterialLayer::IfcMaterialLayer() {}
IfcMaterialLayer::IfcMaterialLayer( int id ) { m_id = id; }
IfcMaterialLayer::~IfcMaterialLayer() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialLayer::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialLayer> other = dynamic_pointer_cast<IfcMaterialLayer>(other_entity);
	if( !other) { return; }
	m_Material = other->m_Material;
	m_LayerThickness = other->m_LayerThickness;
	m_IsVentilated = other->m_IsVentilated;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Category = other->m_Category;
	m_Priority = other->m_Priority;
}
void IfcMaterialLayer::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALLAYER" << "(";
	if( m_Material ) { stream << "#" << m_Material->getId(); } else { stream << "$"; }
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
void IfcMaterialLayer::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialLayer, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcMaterialLayer, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Material, map );
	m_LayerThickness = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[1] );
	m_IsVentilated = IfcLogical::createObjectFromStepData( args[2] );
	m_Name = IfcLabel::createObjectFromStepData( args[3] );
	m_Description = IfcText::createObjectFromStepData( args[4] );
	m_Category = IfcLabel::createObjectFromStepData( args[5] );
	m_Priority = IfcNormalisedRatioMeasure::createObjectFromStepData( args[6] );
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
	vec_attributes_inverse.push_back( std::make_pair( "ToMaterialLayerSet_inverse", shared_ptr<IfcPPEntity>( m_ToMaterialLayerSet_inverse ) ) );
}
void IfcMaterialLayer::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialLayer::unlinkSelf()
{
	IfcMaterialDefinition::unlinkSelf();
}
