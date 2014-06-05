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
#include "include/IfcBooleanClippingResult.h"
#include "include/IfcBooleanOperand.h"
#include "include/IfcBooleanOperator.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBooleanClippingResult 
IfcBooleanClippingResult::IfcBooleanClippingResult() {}
IfcBooleanClippingResult::IfcBooleanClippingResult( int id ) { m_id = id; }
IfcBooleanClippingResult::~IfcBooleanClippingResult() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBooleanClippingResult::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBooleanClippingResult> other = dynamic_pointer_cast<IfcBooleanClippingResult>(other_entity);
	if( !other) { return; }
	m_Operator = other->m_Operator;
	m_FirstOperand = other->m_FirstOperand;
	m_SecondOperand = other->m_SecondOperand;
}
void IfcBooleanClippingResult::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBOOLEANCLIPPINGRESULT" << "(";
	if( m_Operator ) { m_Operator->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FirstOperand ) { m_FirstOperand->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_SecondOperand ) { m_SecondOperand->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcBooleanClippingResult::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBooleanClippingResult::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBooleanClippingResult, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcBooleanClippingResult, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Operator = IfcBooleanOperator::createObjectFromStepData( args[0] );
	m_FirstOperand = IfcBooleanOperand::createObjectFromStepData( args[1], map );
	m_SecondOperand = IfcBooleanOperand::createObjectFromStepData( args[2], map );
}
void IfcBooleanClippingResult::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBooleanResult::getAttributes( vec_attributes );
}
void IfcBooleanClippingResult::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBooleanClippingResult::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBooleanResult::setInverseCounterparts( ptr_self_entity );
}
void IfcBooleanClippingResult::unlinkSelf()
{
	IfcBooleanResult::unlinkSelf();
}
