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
#include "include/IfcBooleanOperand.h"
#include "include/IfcBooleanOperator.h"
#include "include/IfcBooleanResult.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBooleanResult 
IfcBooleanResult::IfcBooleanResult() {}
IfcBooleanResult::IfcBooleanResult( int id ) { m_id = id; }
IfcBooleanResult::~IfcBooleanResult() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBooleanResult::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBooleanResult> other = dynamic_pointer_cast<IfcBooleanResult>(other_entity);
	if( !other) { return; }
	m_Operator = other->m_Operator;
	m_FirstOperand = other->m_FirstOperand;
	m_SecondOperand = other->m_SecondOperand;
}
void IfcBooleanResult::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBOOLEANRESULT" << "(";
	if( m_Operator ) { m_Operator->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FirstOperand ) { m_FirstOperand->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_SecondOperand ) { m_SecondOperand->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcBooleanResult::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBooleanResult::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBooleanResult, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcBooleanResult, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Operator = IfcBooleanOperator::createObjectFromStepData( args[0] );
	m_FirstOperand = IfcBooleanOperand::createObjectFromStepData( args[1], map );
	m_SecondOperand = IfcBooleanOperand::createObjectFromStepData( args[2], map );
}
void IfcBooleanResult::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Operator", m_Operator ) );
	vec_attributes.push_back( std::make_pair( "FirstOperand", m_FirstOperand ) );
	vec_attributes.push_back( std::make_pair( "SecondOperand", m_SecondOperand ) );
}
void IfcBooleanResult::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBooleanResult::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcBooleanResult::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
