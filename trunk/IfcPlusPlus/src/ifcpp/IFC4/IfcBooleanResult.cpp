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
#include "include/IfcBooleanOperand.h"
#include "include/IfcBooleanOperator.h"
#include "include/IfcBooleanResult.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBooleanResult 
IfcBooleanResult::IfcBooleanResult() {}
IfcBooleanResult::IfcBooleanResult( int id ) { m_id = id; }
IfcBooleanResult::~IfcBooleanResult() {}
shared_ptr<IfcPPObject> IfcBooleanResult::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBooleanResult> copy_self( new IfcBooleanResult() );
	if( m_Operator ) { copy_self->m_Operator = dynamic_pointer_cast<IfcBooleanOperator>( m_Operator->getDeepCopy(options) ); }
	if( m_FirstOperand ) { copy_self->m_FirstOperand = dynamic_pointer_cast<IfcBooleanOperand>( m_FirstOperand->getDeepCopy(options) ); }
	if( m_SecondOperand ) { copy_self->m_SecondOperand = dynamic_pointer_cast<IfcBooleanOperand>( m_SecondOperand->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBooleanResult::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBOOLEANRESULT" << "(";
	if( m_Operator ) { m_Operator->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FirstOperand ) { m_FirstOperand->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_SecondOperand ) { m_SecondOperand->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcBooleanResult::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBooleanResult::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBooleanResult, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Operator = IfcBooleanOperator::createObjectFromSTEP( args[0] );
	m_FirstOperand = IfcBooleanOperand::createObjectFromSTEP( args[1], map );
	m_SecondOperand = IfcBooleanOperand::createObjectFromSTEP( args[2], map );
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
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcBooleanResult::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcBooleanResult::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
