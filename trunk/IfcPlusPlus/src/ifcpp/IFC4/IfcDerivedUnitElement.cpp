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
#include "include/IfcDerivedUnitElement.h"
#include "include/IfcNamedUnit.h"

// ENTITY IfcDerivedUnitElement 
IfcDerivedUnitElement::IfcDerivedUnitElement() {}
IfcDerivedUnitElement::IfcDerivedUnitElement( int id ) { m_id = id; }
IfcDerivedUnitElement::~IfcDerivedUnitElement() {}
shared_ptr<IfcPPObject> IfcDerivedUnitElement::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDerivedUnitElement> copy_self( new IfcDerivedUnitElement() );
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcNamedUnit>( m_Unit->getDeepCopy(options) ); }
	if( m_Exponent ) { copy_self->m_Exponent = m_Exponent; }
	return copy_self;
}
void IfcDerivedUnitElement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDERIVEDUNITELEMENT" << "(";
	if( m_Unit ) { stream << "#" << m_Unit->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Exponent == m_Exponent ){ stream << m_Exponent; } else { stream << "$"; }
	stream << ");";
}
void IfcDerivedUnitElement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDerivedUnitElement::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDerivedUnitElement, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_Unit, map );
	readIntValue( args[1], m_Exponent );
}
void IfcDerivedUnitElement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
	vec_attributes.push_back( std::make_pair( "Exponent", shared_ptr<IfcPPIntAttribute>( new IfcPPIntAttribute( m_Exponent ) ) ) );
}
void IfcDerivedUnitElement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcDerivedUnitElement::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcDerivedUnitElement::unlinkSelf()
{
}
