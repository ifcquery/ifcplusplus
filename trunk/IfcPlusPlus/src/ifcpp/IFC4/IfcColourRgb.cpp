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
shared_ptr<IfcPPObject> IfcColourRgb::getDeepCopy()
{
	shared_ptr<IfcColourRgb> copy_self( new IfcColourRgb() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Red ) { copy_self->m_Red = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Red->getDeepCopy() ); }
	if( m_Green ) { copy_self->m_Green = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Green->getDeepCopy() ); }
	if( m_Blue ) { copy_self->m_Blue = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Blue->getDeepCopy() ); }
	return copy_self;
}
void IfcColourRgb::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOLOURRGB" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Red ) { m_Red->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Green ) { m_Green->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Blue ) { m_Blue->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcColourRgb::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcColourRgb::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	IfcColourSpecification::getAttributesInverse( vec_attributes_inverse );
}
void IfcColourRgb::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcColourSpecification::setInverseCounterparts( ptr_self_entity );
}
void IfcColourRgb::unlinkSelf()
{
	IfcColourSpecification::unlinkSelf();
}
