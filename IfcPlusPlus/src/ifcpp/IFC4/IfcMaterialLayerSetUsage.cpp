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
#include "include/IfcDirectionSenseEnum.h"
#include "include/IfcLayerSetDirectionEnum.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcMaterialLayerSet.h"
#include "include/IfcMaterialLayerSetUsage.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcRelAssociatesMaterial.h"

// ENTITY IfcMaterialLayerSetUsage 
IfcMaterialLayerSetUsage::IfcMaterialLayerSetUsage() {}
IfcMaterialLayerSetUsage::IfcMaterialLayerSetUsage( int id ) { m_id = id; }
IfcMaterialLayerSetUsage::~IfcMaterialLayerSetUsage() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialLayerSetUsage::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialLayerSetUsage> other = dynamic_pointer_cast<IfcMaterialLayerSetUsage>(other_entity);
	if( !other) { return; }
	m_ForLayerSet = other->m_ForLayerSet;
	m_LayerSetDirection = other->m_LayerSetDirection;
	m_DirectionSense = other->m_DirectionSense;
	m_OffsetFromReferenceLine = other->m_OffsetFromReferenceLine;
	m_ReferenceExtent = other->m_ReferenceExtent;
}
void IfcMaterialLayerSetUsage::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALLAYERSETUSAGE" << "(";
	if( m_ForLayerSet ) { stream << "#" << m_ForLayerSet->getId(); } else { stream << "$"; }
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
void IfcMaterialLayerSetUsage::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialLayerSetUsage, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcMaterialLayerSetUsage, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_ForLayerSet, map );
	m_LayerSetDirection = IfcLayerSetDirectionEnum::createObjectFromStepData( args[1] );
	m_DirectionSense = IfcDirectionSenseEnum::createObjectFromStepData( args[2] );
	m_OffsetFromReferenceLine = IfcLengthMeasure::createObjectFromStepData( args[3] );
	m_ReferenceExtent = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
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
}
void IfcMaterialLayerSetUsage::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialUsageDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialLayerSetUsage::unlinkSelf()
{
	IfcMaterialUsageDefinition::unlinkSelf();
}
