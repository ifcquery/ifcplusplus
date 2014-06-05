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
#include "include/IfcUnit.h"
#include "include/IfcUnitAssignment.h"

// ENTITY IfcUnitAssignment 
IfcUnitAssignment::IfcUnitAssignment() {}
IfcUnitAssignment::IfcUnitAssignment( int id ) { m_id = id; }
IfcUnitAssignment::~IfcUnitAssignment() {}

// method setEntity takes over all attributes from another instance of the class
void IfcUnitAssignment::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcUnitAssignment> other = dynamic_pointer_cast<IfcUnitAssignment>(other_entity);
	if( !other) { return; }
	m_Units = other->m_Units;
}
void IfcUnitAssignment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCUNITASSIGNMENT" << "(";
	writeTypeList( stream, m_Units, true );
	stream << ");";
}
void IfcUnitAssignment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcUnitAssignment::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcUnitAssignment, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcUnitAssignment, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readSelectList( args[0], m_Units, map );
}
void IfcUnitAssignment::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	shared_ptr<IfcPPAttributeObjectVector> Units_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Units.begin(), m_Units.end(), std::back_inserter( Units_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Units", Units_vec_object ) );
}
void IfcUnitAssignment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcUnitAssignment::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcUnitAssignment::unlinkSelf()
{
}
