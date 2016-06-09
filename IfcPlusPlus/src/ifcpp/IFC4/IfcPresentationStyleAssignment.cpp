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
#include "include/IfcPresentationStyleAssignment.h"
#include "include/IfcPresentationStyleSelect.h"

// ENTITY IfcPresentationStyleAssignment 
IfcPresentationStyleAssignment::IfcPresentationStyleAssignment() { m_entity_enum = IFCPRESENTATIONSTYLEASSIGNMENT; }
IfcPresentationStyleAssignment::IfcPresentationStyleAssignment( int id ) { m_id = id; m_entity_enum = IFCPRESENTATIONSTYLEASSIGNMENT; }
IfcPresentationStyleAssignment::~IfcPresentationStyleAssignment() {}
shared_ptr<IfcPPObject> IfcPresentationStyleAssignment::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPresentationStyleAssignment> copy_self( new IfcPresentationStyleAssignment() );
	for( size_t ii=0; ii<m_Styles.size(); ++ii )
	{
		auto item_ii = m_Styles[ii];
		if( item_ii )
		{
			copy_self->m_Styles.push_back( dynamic_pointer_cast<IfcPresentationStyleSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcPresentationStyleAssignment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPRESENTATIONSTYLEASSIGNMENT" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_Styles.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcPresentationStyleSelect>& type_object = m_Styles[ii];
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
	stream << ");";
}
void IfcPresentationStyleAssignment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPresentationStyleAssignment::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPresentationStyleAssignment, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readSelectList( args[0], m_Styles, map );
}
void IfcPresentationStyleAssignment::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_Styles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Styles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Styles.begin(), m_Styles.end(), std::back_inserter( Styles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Styles", Styles_vec_object ) );
	}
}
void IfcPresentationStyleAssignment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPresentationStyleAssignment::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcPresentationStyleAssignment::unlinkFromInverseCounterparts()
{
}
