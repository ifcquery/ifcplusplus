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
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcCurveStyleFontPattern.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"

// ENTITY IfcCurveStyleFontPattern 
IfcCurveStyleFontPattern::IfcCurveStyleFontPattern() {}
IfcCurveStyleFontPattern::IfcCurveStyleFontPattern( int id ) { m_id = id; }
IfcCurveStyleFontPattern::~IfcCurveStyleFontPattern() {}
shared_ptr<IfcPPObject> IfcCurveStyleFontPattern::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCurveStyleFontPattern> copy_self( new IfcCurveStyleFontPattern() );
	if( m_VisibleSegmentLength ) { copy_self->m_VisibleSegmentLength = dynamic_pointer_cast<IfcLengthMeasure>( m_VisibleSegmentLength->getDeepCopy(options) ); }
	if( m_InvisibleSegmentLength ) { copy_self->m_InvisibleSegmentLength = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_InvisibleSegmentLength->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCurveStyleFontPattern::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCURVESTYLEFONTPATTERN" << "(";
	if( m_VisibleSegmentLength ) { m_VisibleSegmentLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InvisibleSegmentLength ) { m_InvisibleSegmentLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCurveStyleFontPattern::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurveStyleFontPattern::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcCurveStyleFontPattern, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_VisibleSegmentLength = IfcLengthMeasure::createObjectFromSTEP( args[0] );
	m_InvisibleSegmentLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[1] );
}
void IfcCurveStyleFontPattern::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "VisibleSegmentLength", m_VisibleSegmentLength ) );
	vec_attributes.push_back( std::make_pair( "InvisibleSegmentLength", m_InvisibleSegmentLength ) );
}
void IfcCurveStyleFontPattern::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcCurveStyleFontPattern::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveStyleFontPattern::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
