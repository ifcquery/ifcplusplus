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
shared_ptr<IfcPPObject> IfcDerivedUnit::getDeepCopy()
{
	shared_ptr<IfcDerivedUnit> copy_self( new IfcDerivedUnit() );
	for( size_t ii=0; ii<m_Elements.size(); ++ii )
	{
		auto item_ii = m_Elements[ii];
		if( item_ii )
		{
			copy_self->m_Elements.push_back( dynamic_pointer_cast<IfcDerivedUnitElement>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_UnitType ) { copy_self->m_UnitType = dynamic_pointer_cast<IfcDerivedUnitEnum>( m_UnitType->getDeepCopy() ); }
	if( m_UserDefinedType ) { copy_self->m_UserDefinedType = dynamic_pointer_cast<IfcLabel>( m_UserDefinedType->getDeepCopy() ); }
	return copy_self;
}
void IfcDerivedUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDERIVEDUNIT" << "(";
	writeEntityList( stream, m_Elements );
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedType ) { m_UserDefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDerivedUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDerivedUnit::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( m_Elements.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Elements_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Elements.begin(), m_Elements.end(), std::back_inserter( Elements_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Elements", Elements_vec_object ) );
	}
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
