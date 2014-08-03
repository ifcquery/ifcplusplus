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
shared_ptr<IfcPPObject> IfcProductRepresentation::getDeepCopy()
{
	shared_ptr<IfcProductRepresentation> copy_self( new IfcProductRepresentation() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	for( size_t ii=0; ii<m_Representations.size(); ++ii )
	{
		auto item_ii = m_Representations[ii];
		if( item_ii )
		{
			copy_self->m_Representations.push_back( dynamic_pointer_cast<IfcRepresentation>(item_ii->getDeepCopy() ) );
		}
	}
	return copy_self;
}
void IfcProductRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPRODUCTREPRESENTATION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Representations );
	stream << ");";
}
void IfcProductRepresentation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProductRepresentation::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( m_Representations.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Representations_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Representations.begin(), m_Representations.end(), std::back_inserter( Representations_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Representations", Representations_vec_object ) );
	}
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
