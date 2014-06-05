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
#include "include/IfcElement.h"
#include "include/IfcFeatureElementAddition.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelProjectsElement.h"
#include "include/IfcText.h"

// ENTITY IfcRelProjectsElement 
IfcRelProjectsElement::IfcRelProjectsElement() {}
IfcRelProjectsElement::IfcRelProjectsElement( int id ) { m_id = id; }
IfcRelProjectsElement::~IfcRelProjectsElement() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelProjectsElement::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelProjectsElement> other = dynamic_pointer_cast<IfcRelProjectsElement>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingElement = other->m_RelatingElement;
	m_RelatedFeatureElement = other->m_RelatedFeatureElement;
}
void IfcRelProjectsElement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELPROJECTSELEMENT" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingElement ) { stream << "#" << m_RelatingElement->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RelatedFeatureElement ) { stream << "#" << m_RelatedFeatureElement->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelProjectsElement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelProjectsElement::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelProjectsElement, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelProjectsElement, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_RelatingElement, map );
	readEntityReference( args[5], m_RelatedFeatureElement, map );
}
void IfcRelProjectsElement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelDecomposes::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingElement", m_RelatingElement ) );
	vec_attributes.push_back( std::make_pair( "RelatedFeatureElement", m_RelatedFeatureElement ) );
}
void IfcRelProjectsElement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelProjectsElement::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelDecomposes::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelProjectsElement> ptr_self = dynamic_pointer_cast<IfcRelProjectsElement>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelProjectsElement::setInverseCounterparts: type mismatch" ); }
	if( m_RelatedFeatureElement )
	{
		m_RelatedFeatureElement->m_ProjectsElements_inverse = ptr_self;
	}
	if( m_RelatingElement )
	{
		m_RelatingElement->m_HasProjections_inverse.push_back( ptr_self );
	}
}
void IfcRelProjectsElement::unlinkSelf()
{
	IfcRelDecomposes::unlinkSelf();
	if( m_RelatedFeatureElement )
	{
		shared_ptr<IfcRelProjectsElement> self_candidate( m_RelatedFeatureElement->m_ProjectsElements_inverse );
		if( self_candidate->getId() == this->getId() )
		{
			weak_ptr<IfcRelProjectsElement>& self_candidate_weak = m_RelatedFeatureElement->m_ProjectsElements_inverse;
			self_candidate_weak.reset();
		}
	}
	if( m_RelatingElement )
	{
		std::vector<weak_ptr<IfcRelProjectsElement> >& HasProjections_inverse = m_RelatingElement->m_HasProjections_inverse;
		std::vector<weak_ptr<IfcRelProjectsElement> >::iterator it_HasProjections_inverse;
		for( it_HasProjections_inverse = HasProjections_inverse.begin(); it_HasProjections_inverse != HasProjections_inverse.end(); ++it_HasProjections_inverse)
		{
			shared_ptr<IfcRelProjectsElement> self_candidate( *it_HasProjections_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasProjections_inverse.erase( it_HasProjections_inverse );
				break;
			}
		}
	}
}
