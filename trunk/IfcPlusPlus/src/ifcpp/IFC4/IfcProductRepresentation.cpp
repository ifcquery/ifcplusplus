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
#include "include/IfcProductRepresentation.h"
#include "include/IfcRepresentation.h"
#include "include/IfcText.h"

// ENTITY IfcProductRepresentation 
IfcProductRepresentation::IfcProductRepresentation() {}
IfcProductRepresentation::IfcProductRepresentation( int id ) { m_id = id; }
IfcProductRepresentation::~IfcProductRepresentation() {}

// method setEntity takes over all attributes from another instance of the class
void IfcProductRepresentation::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcProductRepresentation> other = dynamic_pointer_cast<IfcProductRepresentation>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Representations = other->m_Representations;
}
void IfcProductRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPRODUCTREPRESENTATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Representations );
	stream << ");";
}
void IfcProductRepresentation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProductRepresentation::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcProductRepresentation, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcProductRepresentation, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReferenceList( args[2], m_Representations, map );
}
void IfcProductRepresentation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcProductRepresentation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcProductRepresentation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcProductRepresentation> ptr_self = dynamic_pointer_cast<IfcProductRepresentation>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcProductRepresentation::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_Representations.size(); ++i )
	{
		if( m_Representations[i] )
		{
			m_Representations[i]->m_OfProductRepresentation_inverse.push_back( ptr_self );
		}
	}
}
void IfcProductRepresentation::unlinkSelf()
{
	for( int i=0; i<m_Representations.size(); ++i )
	{
		if( m_Representations[i] )
		{
			std::vector<weak_ptr<IfcProductRepresentation> >& OfProductRepresentation_inverse = m_Representations[i]->m_OfProductRepresentation_inverse;
			std::vector<weak_ptr<IfcProductRepresentation> >::iterator it_OfProductRepresentation_inverse;
			for( it_OfProductRepresentation_inverse = OfProductRepresentation_inverse.begin(); it_OfProductRepresentation_inverse != OfProductRepresentation_inverse.end(); ++it_OfProductRepresentation_inverse)
			{
				shared_ptr<IfcProductRepresentation> self_candidate( *it_OfProductRepresentation_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					OfProductRepresentation_inverse.erase( it_OfProductRepresentation_inverse );
					break;
				}
			}
		}
	}
}
