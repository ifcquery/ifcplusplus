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
#include "include/IfcDataOriginEnum.h"
#include "include/IfcLabel.h"
#include "include/IfcSchedulingTime.h"

// ENTITY IfcSchedulingTime 
IfcSchedulingTime::IfcSchedulingTime() {}
IfcSchedulingTime::IfcSchedulingTime( int id ) { m_id = id; }
IfcSchedulingTime::~IfcSchedulingTime() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSchedulingTime::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSchedulingTime> other = dynamic_pointer_cast<IfcSchedulingTime>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_DataOrigin = other->m_DataOrigin;
	m_UserDefinedDataOrigin = other->m_UserDefinedDataOrigin;
}
void IfcSchedulingTime::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSCHEDULINGTIME" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSchedulingTime::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSchedulingTime::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSchedulingTime, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcSchedulingTime, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromStepData( args[1] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromStepData( args[2] );
}
void IfcSchedulingTime::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "DataOrigin", m_DataOrigin ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedDataOrigin", m_UserDefinedDataOrigin ) );
}
void IfcSchedulingTime::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSchedulingTime::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcSchedulingTime::unlinkSelf()
{
}
