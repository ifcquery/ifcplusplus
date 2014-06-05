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
#include "include/IfcTable.h"
#include "include/IfcTableColumn.h"
#include "include/IfcTableRow.h"

// ENTITY IfcTable 
IfcTable::IfcTable() {}
IfcTable::IfcTable( int id ) { m_id = id; }
IfcTable::~IfcTable() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTable::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTable> other = dynamic_pointer_cast<IfcTable>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Rows = other->m_Rows;
	m_Columns = other->m_Columns;
}
void IfcTable::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTABLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Rows );
	stream << ",";
	writeEntityList( stream, m_Columns );
	stream << ");";
}
void IfcTable::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTable::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTable, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcTable, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	readEntityReferenceList( args[1], m_Rows, map );
	readEntityReferenceList( args[2], m_Columns, map );
}
void IfcTable::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
}
void IfcTable::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTable::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcTable> ptr_self = dynamic_pointer_cast<IfcTable>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcTable::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_Rows.size(); ++i )
	{
		if( m_Rows[i] )
		{
			m_Rows[i]->m_OfTable_inverse = ptr_self;
		}
	}
}
void IfcTable::unlinkSelf()
{
	for( int i=0; i<m_Rows.size(); ++i )
	{
		if( m_Rows[i] )
		{
			shared_ptr<IfcTable> self_candidate( m_Rows[i]->m_OfTable_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				weak_ptr<IfcTable>& self_candidate_weak = m_Rows[i]->m_OfTable_inverse;
				self_candidate_weak.reset();
			}
		}
	}
}
