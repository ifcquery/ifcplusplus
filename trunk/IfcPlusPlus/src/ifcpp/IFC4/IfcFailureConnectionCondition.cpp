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
#include "include/IfcFailureConnectionCondition.h"
#include "include/IfcForceMeasure.h"
#include "include/IfcLabel.h"

// ENTITY IfcFailureConnectionCondition 
IfcFailureConnectionCondition::IfcFailureConnectionCondition() {}
IfcFailureConnectionCondition::IfcFailureConnectionCondition( int id ) { m_id = id; }
IfcFailureConnectionCondition::~IfcFailureConnectionCondition() {}

// method setEntity takes over all attributes from another instance of the class
void IfcFailureConnectionCondition::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcFailureConnectionCondition> other = dynamic_pointer_cast<IfcFailureConnectionCondition>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_TensionFailureX = other->m_TensionFailureX;
	m_TensionFailureY = other->m_TensionFailureY;
	m_TensionFailureZ = other->m_TensionFailureZ;
	m_CompressionFailureX = other->m_CompressionFailureX;
	m_CompressionFailureY = other->m_CompressionFailureY;
	m_CompressionFailureZ = other->m_CompressionFailureZ;
}
void IfcFailureConnectionCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCFAILURECONNECTIONCONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TensionFailureX ) { m_TensionFailureX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TensionFailureY ) { m_TensionFailureY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TensionFailureZ ) { m_TensionFailureZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CompressionFailureX ) { m_CompressionFailureX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CompressionFailureY ) { m_CompressionFailureY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CompressionFailureZ ) { m_CompressionFailureZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcFailureConnectionCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFailureConnectionCondition::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFailureConnectionCondition, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcFailureConnectionCondition, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_TensionFailureX = IfcForceMeasure::createObjectFromStepData( args[1] );
	m_TensionFailureY = IfcForceMeasure::createObjectFromStepData( args[2] );
	m_TensionFailureZ = IfcForceMeasure::createObjectFromStepData( args[3] );
	m_CompressionFailureX = IfcForceMeasure::createObjectFromStepData( args[4] );
	m_CompressionFailureY = IfcForceMeasure::createObjectFromStepData( args[5] );
	m_CompressionFailureZ = IfcForceMeasure::createObjectFromStepData( args[6] );
}
void IfcFailureConnectionCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralConnectionCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TensionFailureX", m_TensionFailureX ) );
	vec_attributes.push_back( std::make_pair( "TensionFailureY", m_TensionFailureY ) );
	vec_attributes.push_back( std::make_pair( "TensionFailureZ", m_TensionFailureZ ) );
	vec_attributes.push_back( std::make_pair( "CompressionFailureX", m_CompressionFailureX ) );
	vec_attributes.push_back( std::make_pair( "CompressionFailureY", m_CompressionFailureY ) );
	vec_attributes.push_back( std::make_pair( "CompressionFailureZ", m_CompressionFailureZ ) );
}
void IfcFailureConnectionCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcFailureConnectionCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralConnectionCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcFailureConnectionCondition::unlinkSelf()
{
	IfcStructuralConnectionCondition::unlinkSelf();
}
