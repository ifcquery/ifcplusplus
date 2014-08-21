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
#include "include/IfcAxis2Placement.h"
#include "include/IfcBoxAlignment.h"
#include "include/IfcPlanarExtent.h"
#include "include/IfcPresentableText.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTextLiteralWithExtent.h"
#include "include/IfcTextPath.h"

// ENTITY IfcTextLiteralWithExtent 
IfcTextLiteralWithExtent::IfcTextLiteralWithExtent() {}
IfcTextLiteralWithExtent::IfcTextLiteralWithExtent( int id ) { m_id = id; }
IfcTextLiteralWithExtent::~IfcTextLiteralWithExtent() {}
shared_ptr<IfcPPObject> IfcTextLiteralWithExtent::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextLiteralWithExtent> copy_self( new IfcTextLiteralWithExtent() );
	if( m_Literal ) { copy_self->m_Literal = dynamic_pointer_cast<IfcPresentableText>( m_Literal->getDeepCopy(options) ); }
	if( m_Placement ) { copy_self->m_Placement = dynamic_pointer_cast<IfcAxis2Placement>( m_Placement->getDeepCopy(options) ); }
	if( m_Path ) { copy_self->m_Path = dynamic_pointer_cast<IfcTextPath>( m_Path->getDeepCopy(options) ); }
	if( m_Extent ) { copy_self->m_Extent = dynamic_pointer_cast<IfcPlanarExtent>( m_Extent->getDeepCopy(options) ); }
	if( m_BoxAlignment ) { copy_self->m_BoxAlignment = dynamic_pointer_cast<IfcBoxAlignment>( m_BoxAlignment->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTextLiteralWithExtent::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTLITERALWITHEXTENT" << "(";
	if( m_Literal ) { m_Literal->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Placement ) { m_Placement->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_Path ) { m_Path->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Extent ) { stream << "#" << m_Extent->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_BoxAlignment ) { m_BoxAlignment->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTextLiteralWithExtent::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextLiteralWithExtent::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextLiteralWithExtent, expecting 5, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Literal = IfcPresentableText::createObjectFromSTEP( args[0] );
	m_Placement = IfcAxis2Placement::createObjectFromSTEP( args[1], map );
	m_Path = IfcTextPath::createObjectFromSTEP( args[2] );
	readEntityReference( args[3], m_Extent, map );
	m_BoxAlignment = IfcBoxAlignment::createObjectFromSTEP( args[4] );
}
void IfcTextLiteralWithExtent::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTextLiteral::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Extent", m_Extent ) );
	vec_attributes.push_back( std::make_pair( "BoxAlignment", m_BoxAlignment ) );
}
void IfcTextLiteralWithExtent::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTextLiteral::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextLiteralWithExtent::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTextLiteral::setInverseCounterparts( ptr_self_entity );
}
void IfcTextLiteralWithExtent::unlinkSelf()
{
	IfcTextLiteral::unlinkSelf();
}
