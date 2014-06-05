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

// method setEntity takes over all attributes from another instance of the class
void IfcTextLiteral::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTextLiteral> other = dynamic_pointer_cast<IfcTextLiteral>(other_entity);
	if( !other) { return; }
	m_Literal = other->m_Literal;
	m_Placement = other->m_Placement;
	m_Path = other->m_Path;
}
void IfcTextLiteral::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTEXTLITERAL" << "(";
	if( m_Literal ) { m_Literal->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Placement ) { m_Placement->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_Path ) { m_Path->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTextLiteral::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextLiteral::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextLiteral, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcTextLiteral, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Literal = IfcPresentableText::createObjectFromStepData( args[0] );
	m_Placement = IfcAxis2Placement::createObjectFromStepData( args[1], map );
	m_Path = IfcTextPath::createObjectFromStepData( args[2] );
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
}
void IfcTextLiteral::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTextLiteral::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
