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
#include "include/IfcCurveStyleFontAndScaling.h"
#include "include/IfcCurveStyleFontSelect.h"
#include "include/IfcLabel.h"
#include "include/IfcPositiveRatioMeasure.h"

// ENTITY IfcCurveStyleFontAndScaling 
IfcCurveStyleFontAndScaling::IfcCurveStyleFontAndScaling() {}
IfcCurveStyleFontAndScaling::IfcCurveStyleFontAndScaling( int id ) { m_id = id; }
IfcCurveStyleFontAndScaling::~IfcCurveStyleFontAndScaling() {}

// method setEntity takes over all attributes from another instance of the class
void IfcCurveStyleFontAndScaling::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcCurveStyleFontAndScaling> other = dynamic_pointer_cast<IfcCurveStyleFontAndScaling>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_CurveFont = other->m_CurveFont;
	m_CurveFontScaling = other->m_CurveFontScaling;
}
void IfcCurveStyleFontAndScaling::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCCURVESTYLEFONTANDSCALING" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CurveFont ) { m_CurveFont->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_CurveFontScaling ) { m_CurveFontScaling->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCurveStyleFontAndScaling::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurveStyleFontAndScaling::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCurveStyleFontAndScaling, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcCurveStyleFontAndScaling, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_CurveFont = IfcCurveStyleFontSelect::createObjectFromStepData( args[1], map );
	m_CurveFontScaling = IfcPositiveRatioMeasure::createObjectFromStepData( args[2] );
}
void IfcCurveStyleFontAndScaling::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "CurveFont", m_CurveFont ) );
	vec_attributes.push_back( std::make_pair( "CurveFontScaling", m_CurveFontScaling ) );
}
void IfcCurveStyleFontAndScaling::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcCurveStyleFontAndScaling::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveStyleFontAndScaling::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
