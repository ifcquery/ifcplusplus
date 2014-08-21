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
#include "include/IfcTime.h"
#include "include/IfcTimePeriod.h"

// ENTITY IfcTimePeriod 
IfcTimePeriod::IfcTimePeriod() {}
IfcTimePeriod::IfcTimePeriod( int id ) { m_id = id; }
IfcTimePeriod::~IfcTimePeriod() {}
shared_ptr<IfcPPObject> IfcTimePeriod::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTimePeriod> copy_self( new IfcTimePeriod() );
	if( m_StartTime ) { copy_self->m_StartTime = dynamic_pointer_cast<IfcTime>( m_StartTime->getDeepCopy(options) ); }
	if( m_EndTime ) { copy_self->m_EndTime = dynamic_pointer_cast<IfcTime>( m_EndTime->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTimePeriod::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTIMEPERIOD" << "(";
	if( m_StartTime ) { m_StartTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EndTime ) { m_EndTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTimePeriod::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTimePeriod::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTimePeriod, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_StartTime = IfcTime::createObjectFromSTEP( args[0] );
	m_EndTime = IfcTime::createObjectFromSTEP( args[1] );
}
void IfcTimePeriod::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "StartTime", m_StartTime ) );
	vec_attributes.push_back( std::make_pair( "EndTime", m_EndTime ) );
}
void IfcTimePeriod::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTimePeriod::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcTimePeriod::unlinkSelf()
{
}
