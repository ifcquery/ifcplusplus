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
#include "include/IfcApplication.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOrganization.h"

// ENTITY IfcApplication 
IfcApplication::IfcApplication() {}
IfcApplication::IfcApplication( int id ) { m_id = id; }
IfcApplication::~IfcApplication() {}

// method setEntity takes over all attributes from another instance of the class
void IfcApplication::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcApplication> other = dynamic_pointer_cast<IfcApplication>(other_entity);
	if( !other) { return; }
	m_ApplicationDeveloper = other->m_ApplicationDeveloper;
	m_Version = other->m_Version;
	m_ApplicationFullName = other->m_ApplicationFullName;
	m_ApplicationIdentifier = other->m_ApplicationIdentifier;
}
void IfcApplication::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCAPPLICATION" << "(";
	if( m_ApplicationDeveloper ) { stream << "#" << m_ApplicationDeveloper->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Version ) { m_Version->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ApplicationFullName ) { m_ApplicationFullName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ApplicationIdentifier ) { m_ApplicationIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcApplication::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcApplication::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcApplication, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcApplication, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_ApplicationDeveloper, map );
	m_Version = IfcLabel::createObjectFromStepData( args[1] );
	m_ApplicationFullName = IfcLabel::createObjectFromStepData( args[2] );
	m_ApplicationIdentifier = IfcIdentifier::createObjectFromStepData( args[3] );
}
void IfcApplication::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "ApplicationDeveloper", m_ApplicationDeveloper ) );
	vec_attributes.push_back( std::make_pair( "Version", m_Version ) );
	vec_attributes.push_back( std::make_pair( "ApplicationFullName", m_ApplicationFullName ) );
	vec_attributes.push_back( std::make_pair( "ApplicationIdentifier", m_ApplicationIdentifier ) );
}
void IfcApplication::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcApplication::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcApplication::unlinkSelf()
{
}
