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
#include "include/IfcPresentableText.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTextLiteral.h"
#include "include/IfcTextPath.h"

// ENTITY IfcTextLiteral 
IfcTextLiteral::IfcTextLiteral() {}
IfcTextLiteral::IfcTextLiteral( int id ) { m_id = id; }
IfcTextLiteral::~IfcTextLiteral() {}
shared_ptr<IfcPPObject> IfcTextLiteral::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextLiteral> copy_self( new IfcTextLiteral() );
	if( m_Literal ) { copy_self->m_Literal = dynamic_pointer_cast<IfcPresentableText>( m_Literal->getDeepCopy(options) ); }
	if( m_Placement ) { copy_self->m_Placement = dynamic_pointer_cast<IfcAxis2Placement>( m_Placement->getDeepCopy(options) ); }
	if( m_Path ) { copy_self->m_Path = dynamic_pointer_cast<IfcTextPath>( m_Path->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTextLiteral::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTLITERAL" << "(";
	if( m_Literal ) { m_Literal->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Placement ) { m_Placement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_Path ) { m_Path->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTextLiteral::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextLiteral::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextLiteral, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Literal = IfcPresentableText::createObjectFromSTEP( args[0] );
	m_Placement = IfcAxis2Placement::createObjectFromSTEP( args[1], map );
	m_Path = IfcTextPath::createObjectFromSTEP( args[2] );
}
void IfcTextLiteral::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Literal", m_Literal ) );
	vec_attributes.push_back( std::make_pair( "Placement", m_Placement ) );
	vec_attributes.push_back( std::make_pair( "Path", m_Path ) );
}
void IfcTextLiteral::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextLiteral::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTextLiteral::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
