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
#include "include/IfcColourRgb.h"
#include "include/IfcLabel.h"
#include "include/IfcNormalisedRatioMeasure.h"

// ENTITY IfcColourRgb 
IfcColourRgb::IfcColourRgb() {}
IfcColourRgb::IfcColourRgb( int id ) { m_id = id; }
IfcColourRgb::~IfcColourRgb() {}

// method setEntity takes over all attributes from another instance of the class
void IfcColourRgb::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcColourRgb> other = dynamic_pointer_cast<IfcColourRgb>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Red = other->m_Red;
	m_Green = other->m_Green;
	m_Blue = other->m_Blue;
}
void IfcColourRgb::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCOLOURRGB" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Red ) { m_Red->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Green ) { m_Green->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Blue ) { m_Blue->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcColourRgb::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcColourRgb::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcColourRgb, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcColourRgb, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Red = IfcNormalisedRatioMeasure::createObjectFromStepData( args[1] );
	m_Green = IfcNormalisedRatioMeasure::createObjectFromStepData( args[2] );
	m_Blue = IfcNormalisedRatioMeasure::createObjectFromStepData( args[3] );
}
void IfcColourRgb::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcColourSpecification::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Red", m_Red ) );
	vec_attributes.push_back( std::make_pair( "Green", m_Green ) );
	vec_attributes.push_back( std::make_pair( "Blue", m_Blue ) );
}
void IfcColourRgb::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcColourRgb::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcColourSpecification::setInverseCounterparts( ptr_self_entity );
}
void IfcColourRgb::unlinkSelf()
{
	IfcColourSpecification::unlinkSelf();
}
