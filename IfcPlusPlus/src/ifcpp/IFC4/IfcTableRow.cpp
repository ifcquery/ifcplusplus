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
#include "include/IfcBoolean.h"
#include "include/IfcTableRow.h"
#include "include/IfcValue.h"

// ENTITY IfcTableRow 
IfcTableRow::IfcTableRow() { m_entity_enum = IFCTABLEROW; }
IfcTableRow::IfcTableRow( int id ) { m_id = id; m_entity_enum = IFCTABLEROW; }
IfcTableRow::~IfcTableRow() {}
shared_ptr<IfcPPObject> IfcTableRow::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTableRow> copy_self( new IfcTableRow() );
	for( size_t ii=0; ii<m_RowCells.size(); ++ii )
	{
		auto item_ii = m_RowCells[ii];
		if( item_ii )
		{
			copy_self->m_RowCells.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_IsHeading ) { copy_self->m_IsHeading = dynamic_pointer_cast<IfcBoolean>( m_IsHeading->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTableRow::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTABLEROW" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_RowCells.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcValue>& type_object = m_RowCells[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_IsHeading ) { m_IsHeading->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTableRow::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTableRow::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTableRow, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readSelectList( args[0], m_RowCells, map );
	m_IsHeading = IfcBoolean::createObjectFromSTEP( args[1] );
}
void IfcTableRow::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_RowCells.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RowCells_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_RowCells.begin(), m_RowCells.end(), std::back_inserter( RowCells_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "RowCells", RowCells_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "IsHeading", m_IsHeading ) );
}
void IfcTableRow::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTableRow::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcTableRow::unlinkFromInverseCounterparts()
{
}
