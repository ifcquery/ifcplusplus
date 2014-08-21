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
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcReference.h"
#include "include/IfcTableColumn.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"

// ENTITY IfcTableColumn 
IfcTableColumn::IfcTableColumn() {}
IfcTableColumn::IfcTableColumn( int id ) { m_id = id; }
IfcTableColumn::~IfcTableColumn() {}
shared_ptr<IfcPPObject> IfcTableColumn::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTableColumn> copy_self( new IfcTableColumn() );
	if( m_Identifier ) { copy_self->m_Identifier = dynamic_pointer_cast<IfcIdentifier>( m_Identifier->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcUnit>( m_Unit->getDeepCopy(options) ); }
	if( m_ReferencePath ) { copy_self->m_ReferencePath = dynamic_pointer_cast<IfcReference>( m_ReferencePath->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTableColumn::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTABLECOLUMN" << "(";
	if( m_Identifier ) { m_Identifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ReferencePath ) { stream << "#" << m_ReferencePath->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcTableColumn::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTableColumn::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTableColumn, expecting 5, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Identifier = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Name = IfcLabel::createObjectFromSTEP( args[1] );
	m_Description = IfcText::createObjectFromSTEP( args[2] );
	m_Unit = IfcUnit::createObjectFromSTEP( args[3], map );
	readEntityReference( args[4], m_ReferencePath, map );
}
void IfcTableColumn::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Identifier", m_Identifier ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
	vec_attributes.push_back( std::make_pair( "ReferencePath", m_ReferencePath ) );
}
void IfcTableColumn::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTableColumn::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcTableColumn::unlinkSelf()
{
}
