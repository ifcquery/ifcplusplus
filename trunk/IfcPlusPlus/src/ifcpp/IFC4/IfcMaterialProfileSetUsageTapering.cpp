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
#include "include/IfcMaterialProfileSetUsageTapering.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcRelAssociatesMaterial.h"

// ENTITY IfcMaterialProfileSetUsageTapering 
IfcMaterialProfileSetUsageTapering::IfcMaterialProfileSetUsageTapering() {}
IfcMaterialProfileSetUsageTapering::IfcMaterialProfileSetUsageTapering( int id ) { m_id = id; }
IfcMaterialProfileSetUsageTapering::~IfcMaterialProfileSetUsageTapering() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialProfileSetUsageTapering::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialProfileSetUsageTapering> other = dynamic_pointer_cast<IfcMaterialProfileSetUsageTapering>(other_entity);
	if( !other) { return; }
	m_ForProfileSet = other->m_ForProfileSet;
	m_CardinalPoint = other->m_CardinalPoint;
	m_ReferenceExtent = other->m_ReferenceExtent;
	m_ForProfileEndSet = other->m_ForProfileEndSet;
	m_CardinalEndPoint = other->m_CardinalEndPoint;
}
void IfcMaterialProfileSetUsageTapering::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALPROFILESETUSAGETAPERING" << "(";
	if( m_ForProfileSet ) { stream << "#" << m_ForProfileSet->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_CardinalPoint ) { m_CardinalPoint->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReferenceExtent ) { m_ReferenceExtent->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ForProfileEndSet ) { stream << "#" << m_ForProfileEndSet->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_CardinalEndPoint ) { m_CardinalEndPoint->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialProfileSetUsageTapering::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialProfileSetUsageTapering::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialProfileSetUsageTapering, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcMaterialProfileSetUsageTapering, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_ForProfileSet, map );
	m_CardinalPoint = IfcCardinalPointReference::createObjectFromStepData( args[1] );
	m_ReferenceExtent = IfcPositiveLengthMeasure::createObjectFromStepData( args[2] );
	readEntityReference( args[3], m_ForProfileEndSet, map );
	m_CardinalEndPoint = IfcCardinalPointReference::createObjectFromStepData( args[4] );
}
void IfcMaterialProfileSetUsageTapering::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialProfileSetUsage::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ForProfileEndSet", m_ForProfileEndSet ) );
	vec_attributes.push_back( std::make_pair( "CardinalEndPoint", m_CardinalEndPoint ) );
}
void IfcMaterialProfileSetUsageTapering::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialProfileSetUsageTapering::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialProfileSetUsage::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialProfileSetUsageTapering::unlinkSelf()
{
	IfcMaterialProfileSetUsage::unlinkSelf();
}
