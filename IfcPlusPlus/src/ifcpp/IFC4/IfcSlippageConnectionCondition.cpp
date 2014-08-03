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
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcSlippageConnectionCondition.h"

// ENTITY IfcSlippageConnectionCondition 
IfcSlippageConnectionCondition::IfcSlippageConnectionCondition() {}
IfcSlippageConnectionCondition::IfcSlippageConnectionCondition( int id ) { m_id = id; }
IfcSlippageConnectionCondition::~IfcSlippageConnectionCondition() {}
shared_ptr<IfcPPObject> IfcSlippageConnectionCondition::getDeepCopy()
{
	shared_ptr<IfcSlippageConnectionCondition> copy_self( new IfcSlippageConnectionCondition() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_SlippageX ) { copy_self->m_SlippageX = dynamic_pointer_cast<IfcLengthMeasure>( m_SlippageX->getDeepCopy() ); }
	if( m_SlippageY ) { copy_self->m_SlippageY = dynamic_pointer_cast<IfcLengthMeasure>( m_SlippageY->getDeepCopy() ); }
	if( m_SlippageZ ) { copy_self->m_SlippageZ = dynamic_pointer_cast<IfcLengthMeasure>( m_SlippageZ->getDeepCopy() ); }
	return copy_self;
}
void IfcSlippageConnectionCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSLIPPAGECONNECTIONCONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_SlippageX ) { m_SlippageX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SlippageY ) { m_SlippageY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SlippageZ ) { m_SlippageZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSlippageConnectionCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSlippageConnectionCondition::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSlippageConnectionCondition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcSlippageConnectionCondition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_SlippageX = IfcLengthMeasure::createObjectFromStepData( args[1] );
	m_SlippageY = IfcLengthMeasure::createObjectFromStepData( args[2] );
	m_SlippageZ = IfcLengthMeasure::createObjectFromStepData( args[3] );
}
void IfcSlippageConnectionCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralConnectionCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SlippageX", m_SlippageX ) );
	vec_attributes.push_back( std::make_pair( "SlippageY", m_SlippageY ) );
	vec_attributes.push_back( std::make_pair( "SlippageZ", m_SlippageZ ) );
}
void IfcSlippageConnectionCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralConnectionCondition::getAttributesInverse( vec_attributes_inverse );
}
void IfcSlippageConnectionCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralConnectionCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcSlippageConnectionCondition::unlinkSelf()
{
	IfcStructuralConnectionCondition::unlinkSelf();
}
