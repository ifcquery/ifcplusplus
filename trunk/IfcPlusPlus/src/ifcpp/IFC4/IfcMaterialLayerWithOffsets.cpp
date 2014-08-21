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
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcLayerSetDirectionEnum.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcLogical.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialLayerSet.h"
#include "include/IfcMaterialLayerWithOffsets.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialLayerWithOffsets 
IfcMaterialLayerWithOffsets::IfcMaterialLayerWithOffsets() {}
IfcMaterialLayerWithOffsets::IfcMaterialLayerWithOffsets( int id ) { m_id = id; }
IfcMaterialLayerWithOffsets::~IfcMaterialLayerWithOffsets() {}
shared_ptr<IfcPPObject> IfcMaterialLayerWithOffsets::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialLayerWithOffsets> copy_self( new IfcMaterialLayerWithOffsets() );
	if( m_Material ) { copy_self->m_Material = dynamic_pointer_cast<IfcMaterial>( m_Material->getDeepCopy(options) ); }
	if( m_LayerThickness ) { copy_self->m_LayerThickness = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_LayerThickness->getDeepCopy(options) ); }
	if( m_IsVentilated ) { copy_self->m_IsVentilated = dynamic_pointer_cast<IfcLogical>( m_IsVentilated->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Category ) { copy_self->m_Category = dynamic_pointer_cast<IfcLabel>( m_Category->getDeepCopy(options) ); }
	if( m_Priority ) { copy_self->m_Priority = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Priority->getDeepCopy(options) ); }
	if( m_OffsetDirection ) { copy_self->m_OffsetDirection = dynamic_pointer_cast<IfcLayerSetDirectionEnum>( m_OffsetDirection->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_OffsetValues.size(); ++ii )
	{
		auto item_ii = m_OffsetValues[ii];
		if( item_ii )
		{
			copy_self->m_OffsetValues.push_back( dynamic_pointer_cast<IfcLengthMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcMaterialLayerWithOffsets::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALLAYERWITHOFFSETS" << "(";
	if( m_Material ) { stream << "#" << m_Material->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_LayerThickness ) { m_LayerThickness->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_IsVentilated ) { m_IsVentilated->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Priority ) { m_Priority->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OffsetDirection ) { m_OffsetDirection->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeOfRealList( stream, m_OffsetValues );
	stream << ");";
}
void IfcMaterialLayerWithOffsets::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialLayerWithOffsets::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialLayerWithOffsets, expecting 9, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_Material, map );
	m_LayerThickness = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[1] );
	m_IsVentilated = IfcLogical::createObjectFromSTEP( args[2] );
	m_Name = IfcLabel::createObjectFromSTEP( args[3] );
	m_Description = IfcText::createObjectFromSTEP( args[4] );
	m_Category = IfcLabel::createObjectFromSTEP( args[5] );
	m_Priority = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[6] );
	m_OffsetDirection = IfcLayerSetDirectionEnum::createObjectFromSTEP( args[7] );
	readTypeOfRealList( args[8], m_OffsetValues );
}
void IfcMaterialLayerWithOffsets::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialLayer::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "OffsetDirection", m_OffsetDirection ) );
	if( m_OffsetValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> OffsetValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_OffsetValues.begin(), m_OffsetValues.end(), std::back_inserter( OffsetValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "OffsetValues", OffsetValues_vec_object ) );
	}
}
void IfcMaterialLayerWithOffsets::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcMaterialLayer::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialLayerWithOffsets::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialLayer::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialLayerWithOffsets::unlinkSelf()
{
	IfcMaterialLayer::unlinkSelf();
}
