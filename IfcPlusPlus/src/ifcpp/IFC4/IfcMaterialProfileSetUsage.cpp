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
#include "include/IfcCardinalPointReference.h"
#include "include/IfcMaterialProfileSet.h"
#include "include/IfcMaterialProfileSetUsage.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcRelAssociatesMaterial.h"

// ENTITY IfcMaterialProfileSetUsage 
IfcMaterialProfileSetUsage::IfcMaterialProfileSetUsage() {}
IfcMaterialProfileSetUsage::IfcMaterialProfileSetUsage( int id ) { m_id = id; }
IfcMaterialProfileSetUsage::~IfcMaterialProfileSetUsage() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialProfileSetUsage::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialProfileSetUsage> other = dynamic_pointer_cast<IfcMaterialProfileSetUsage>(other_entity);
	if( !other) { return; }
	m_ForProfileSet = other->m_ForProfileSet;
	m_CardinalPoint = other->m_CardinalPoint;
	m_ReferenceExtent = other->m_ReferenceExtent;
}
void IfcMaterialProfileSetUsage::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALPROFILESETUSAGE" << "(";
	if( m_ForProfileSet ) { stream << "#" << m_ForProfileSet->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_CardinalPoint ) { m_CardinalPoint->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReferenceExtent ) { m_ReferenceExtent->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialProfileSetUsage::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialProfileSetUsage::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialProfileSetUsage, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcMaterialProfileSetUsage, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_ForProfileSet, map );
	m_CardinalPoint = IfcCardinalPointReference::createObjectFromStepData( args[1] );
	m_ReferenceExtent = IfcPositiveLengthMeasure::createObjectFromStepData( args[2] );
}
void IfcMaterialProfileSetUsage::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialUsageDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ForProfileSet", m_ForProfileSet ) );
	vec_attributes.push_back( std::make_pair( "CardinalPoint", m_CardinalPoint ) );
	vec_attributes.push_back( std::make_pair( "ReferenceExtent", m_ReferenceExtent ) );
}
void IfcMaterialProfileSetUsage::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialProfileSetUsage::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialUsageDefinition::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialProfileSetUsage::unlinkSelf()
{
	IfcMaterialUsageDefinition::unlinkSelf();
}
