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
#include "include/IfcDerivedUnit.h"
#include "include/IfcDerivedUnitElement.h"
#include "include/IfcDerivedUnitEnum.h"
#include "include/IfcLabel.h"

// ENTITY IfcDerivedUnit 
IfcDerivedUnit::IfcDerivedUnit() {}
IfcDerivedUnit::IfcDerivedUnit( int id ) { m_id = id; }
IfcDerivedUnit::~IfcDerivedUnit() {}

// method setEntity takes over all attributes from another instance of the class
void IfcDerivedUnit::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcDerivedUnit> other = dynamic_pointer_cast<IfcDerivedUnit>(other_entity);
	if( !other) { return; }
	m_Elements = other->m_Elements;
	m_UnitType = other->m_UnitType;
	m_UserDefinedType = other->m_UserDefinedType;
}
void IfcDerivedUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCDERIVEDUNIT" << "(";
	writeEntityList( stream, m_Elements );
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedType ) { m_UserDefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDerivedUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDerivedUnit::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDerivedUnit, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcDerivedUnit, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Elements, map );
	m_UnitType = IfcDerivedUnitEnum::createObjectFromStepData( args[1] );
	m_UserDefinedType = IfcLabel::createObjectFromStepData( args[2] );
}
void IfcDerivedUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "UnitType", m_UnitType ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedType", m_UserDefinedType ) );
}
void IfcDerivedUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcDerivedUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcDerivedUnit::unlinkSelf()
{
}
