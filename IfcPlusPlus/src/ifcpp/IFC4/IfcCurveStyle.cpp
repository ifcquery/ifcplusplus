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
#include "include/IfcColour.h"
#include "include/IfcCurveFontOrScaledCurveFontSelect.h"
#include "include/IfcCurveStyle.h"
#include "include/IfcLabel.h"
#include "include/IfcSizeSelect.h"

// ENTITY IfcCurveStyle 
IfcCurveStyle::IfcCurveStyle() {}
IfcCurveStyle::IfcCurveStyle( int id ) { m_id = id; }
IfcCurveStyle::~IfcCurveStyle() {}
shared_ptr<IfcPPObject> IfcCurveStyle::getDeepCopy()
{
	shared_ptr<IfcCurveStyle> copy_self( new IfcCurveStyle() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_CurveFont ) { copy_self->m_CurveFont = dynamic_pointer_cast<IfcCurveFontOrScaledCurveFontSelect>( m_CurveFont->getDeepCopy() ); }
	if( m_CurveWidth ) { copy_self->m_CurveWidth = dynamic_pointer_cast<IfcSizeSelect>( m_CurveWidth->getDeepCopy() ); }
	if( m_CurveColour ) { copy_self->m_CurveColour = dynamic_pointer_cast<IfcColour>( m_CurveColour->getDeepCopy() ); }
	if( m_ModelOrDraughting ) { copy_self->m_ModelOrDraughting = m_ModelOrDraughting; }
	return copy_self;
}
void IfcCurveStyle::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCURVESTYLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_CurveFont ) { m_CurveFont->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_CurveWidth ) { m_CurveWidth->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_CurveColour ) { m_CurveColour->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ModelOrDraughting == false ) { stream << ".F."; }
	else if( m_ModelOrDraughting == true ) { stream << ".T."; }
	stream << ");";
}
void IfcCurveStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurveStyle::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCurveStyle, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcCurveStyle, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_CurveFont = IfcCurveFontOrScaledCurveFontSelect::createObjectFromStepData( args[1], map );
	m_CurveWidth = IfcSizeSelect::createObjectFromStepData( args[2], map );
	m_CurveColour = IfcColour::createObjectFromStepData( args[3], map );
	if( boost::iequals( args[4], L".F." ) ) { m_ModelOrDraughting = false; }
	else if( boost::iequals( args[4], L".T." ) ) { m_ModelOrDraughting = true; }
}
void IfcCurveStyle::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationStyle::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "CurveFont", m_CurveFont ) );
	vec_attributes.push_back( std::make_pair( "CurveWidth", m_CurveWidth ) );
	vec_attributes.push_back( std::make_pair( "CurveColour", m_CurveColour ) );
	vec_attributes.push_back( std::make_pair( "ModelOrDraughting", shared_ptr<IfcPPBool>( new IfcPPBool( m_ModelOrDraughting ) ) ) );
}
void IfcCurveStyle::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationStyle::getAttributesInverse( vec_attributes_inverse );
}
void IfcCurveStyle::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationStyle::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveStyle::unlinkSelf()
{
	IfcPresentationStyle::unlinkSelf();
}
