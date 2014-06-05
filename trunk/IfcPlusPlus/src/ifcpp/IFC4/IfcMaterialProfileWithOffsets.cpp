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
#include "include/IfcLengthMeasure.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialProfileSet.h"
#include "include/IfcMaterialProfileWithOffsets.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcProfileDef.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialProfileWithOffsets 
IfcMaterialProfileWithOffsets::IfcMaterialProfileWithOffsets() {}
IfcMaterialProfileWithOffsets::IfcMaterialProfileWithOffsets( int id ) { m_id = id; }
IfcMaterialProfileWithOffsets::~IfcMaterialProfileWithOffsets() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialProfileWithOffsets::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialProfileWithOffsets> other = dynamic_pointer_cast<IfcMaterialProfileWithOffsets>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Material = other->m_Material;
	m_Profile = other->m_Profile;
	m_Priority = other->m_Priority;
	m_Category = other->m_Category;
	m_OffsetValues = other->m_OffsetValues;
}
void IfcMaterialProfileWithOffsets::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALPROFILEWITHOFFSETS" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Material ) { stream << "#" << m_Material->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Profile ) { stream << "#" << m_Profile->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Priority ) { m_Priority->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeOfRealList( stream, m_OffsetValues );
	stream << ");";
}
void IfcMaterialProfileWithOffsets::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialProfileWithOffsets::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialProfileWithOffsets, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcMaterialProfileWithOffsets, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Material, map );
	readEntityReference( args[3], m_Profile, map );
	m_Priority = IfcNormalisedRatioMeasure::createObjectFromStepData( args[4] );
	m_Category = IfcLabel::createObjectFromStepData( args[5] );
	readTypeOfRealList( args[6], m_OffsetValues );
}
void IfcMaterialProfileWithOffsets::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialProfile::getAttributes( vec_attributes );
	shared_ptr<IfcPPAttributeObjectVector> OffsetValues_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_OffsetValues.begin(), m_OffsetValues.end(), std::back_inserter( OffsetValues_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "OffsetValues", OffsetValues_vec_object ) );
}
void IfcMaterialProfileWithOffsets::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialProfileWithOffsets::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialProfile::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialProfileWithOffsets::unlinkSelf()
{
	IfcMaterialProfile::unlinkSelf();
}
