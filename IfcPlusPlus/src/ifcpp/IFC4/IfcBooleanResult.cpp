/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
IfcBooleanResult::IfcBooleanResult() { m_entity_enum = IFCBOOLEANRESULT; }
IfcBooleanResult::IfcBooleanResult( int id ) { m_id = id; m_entity_enum = IFCBOOLEANRESULT; }
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
void IfcBooleanResult::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcBooleanResult, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
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
void IfcBooleanResult::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
