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
#include "include/IfcAxis2Placement2D.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"
#include "include/IfcRectangleHollowProfileDef.h"

// ENTITY IfcRectangleHollowProfileDef 
IfcRectangleHollowProfileDef::IfcRectangleHollowProfileDef() {}
IfcRectangleHollowProfileDef::IfcRectangleHollowProfileDef( int id ) { m_id = id; }
IfcRectangleHollowProfileDef::~IfcRectangleHollowProfileDef() {}
shared_ptr<IfcPPObject> IfcRectangleHollowProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRectangleHollowProfileDef> copy_self( new IfcRectangleHollowProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement2D>( m_Position->getDeepCopy(options) ); }
	if( m_XDim ) { copy_self->m_XDim = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_XDim->getDeepCopy(options) ); }
	if( m_YDim ) { copy_self->m_YDim = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_YDim->getDeepCopy(options) ); }
	if( m_WallThickness ) { copy_self->m_WallThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_WallThickness->getDeepCopy(options) ); }
	if( m_InnerFilletRadius ) { copy_self->m_InnerFilletRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_InnerFilletRadius->getDeepCopy(options) ); }
	if( m_OuterFilletRadius ) { copy_self->m_OuterFilletRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_OuterFilletRadius->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRectangleHollowProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRECTANGLEHOLLOWPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_XDim ) { m_XDim->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_YDim ) { m_YDim->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_WallThickness ) { m_WallThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InnerFilletRadius ) { m_InnerFilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OuterFilletRadius ) { m_OuterFilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRectangleHollowProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRectangleHollowProfileDef::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRectangleHollowProfileDef, expecting 8, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_XDim = IfcPositiveLengthMeasure::createObjectFromSTEP( args[3] );
	m_YDim = IfcPositiveLengthMeasure::createObjectFromSTEP( args[4] );
	m_WallThickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[5] );
	m_InnerFilletRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[6] );
	m_OuterFilletRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[7] );
}
void IfcRectangleHollowProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRectangleProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "WallThickness", m_WallThickness ) );
	vec_attributes.push_back( std::make_pair( "InnerFilletRadius", m_InnerFilletRadius ) );
	vec_attributes.push_back( std::make_pair( "OuterFilletRadius", m_OuterFilletRadius ) );
}
void IfcRectangleHollowProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRectangleProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcRectangleHollowProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRectangleProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcRectangleHollowProfileDef::unlinkSelf()
{
	IfcRectangleProfileDef::unlinkSelf();
}
