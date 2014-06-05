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
#include "include/IfcProfileDef.h"
#include "include/IfcSectionProperties.h"
#include "include/IfcSectionTypeEnum.h"

// ENTITY IfcSectionProperties 
IfcSectionProperties::IfcSectionProperties() {}
IfcSectionProperties::IfcSectionProperties( int id ) { m_id = id; }
IfcSectionProperties::~IfcSectionProperties() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSectionProperties::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSectionProperties> other = dynamic_pointer_cast<IfcSectionProperties>(other_entity);
	if( !other) { return; }
	m_SectionType = other->m_SectionType;
	m_StartProfile = other->m_StartProfile;
	m_EndProfile = other->m_EndProfile;
}
void IfcSectionProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSECTIONPROPERTIES" << "(";
	if( m_SectionType ) { m_SectionType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_StartProfile ) { stream << "#" << m_StartProfile->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_EndProfile ) { stream << "#" << m_EndProfile->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcSectionProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSectionProperties::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSectionProperties, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcSectionProperties, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_SectionType = IfcSectionTypeEnum::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_StartProfile, map );
	readEntityReference( args[2], m_EndProfile, map );
}
void IfcSectionProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedProperties::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SectionType", m_SectionType ) );
	vec_attributes.push_back( std::make_pair( "StartProfile", m_StartProfile ) );
	vec_attributes.push_back( std::make_pair( "EndProfile", m_EndProfile ) );
}
void IfcSectionProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSectionProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedProperties::setInverseCounterparts( ptr_self_entity );
}
void IfcSectionProperties::unlinkSelf()
{
	IfcPreDefinedProperties::unlinkSelf();
}
