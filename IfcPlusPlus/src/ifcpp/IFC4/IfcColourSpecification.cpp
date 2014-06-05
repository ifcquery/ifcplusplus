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
#include "include/IfcColourSpecification.h"
#include "include/IfcLabel.h"

// ENTITY IfcColourSpecification 
IfcColourSpecification::IfcColourSpecification() {}
IfcColourSpecification::IfcColourSpecification( int id ) { m_id = id; }
IfcColourSpecification::~IfcColourSpecification() {}

// method setEntity takes over all attributes from another instance of the class
void IfcColourSpecification::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcColourSpecification> other = dynamic_pointer_cast<IfcColourSpecification>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
}
void IfcColourSpecification::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCOLOURSPECIFICATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcColourSpecification::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcColourSpecification::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcColourSpecification, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcColourSpecification, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
}
void IfcColourSpecification::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
}
void IfcColourSpecification::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcColourSpecification::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcColourSpecification::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
