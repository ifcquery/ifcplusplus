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
#include "include/IfcActorSelect.h"
#include "include/IfcApproval.h"
#include "include/IfcApprovalRelationship.h"
#include "include/IfcDateTime.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcRelAssociatesApproval.h"
#include "include/IfcResourceApprovalRelationship.h"
#include "include/IfcText.h"

// ENTITY IfcApproval 
IfcApproval::IfcApproval() {}
IfcApproval::IfcApproval( int id ) { m_id = id; }
IfcApproval::~IfcApproval() {}

// method setEntity takes over all attributes from another instance of the class
void IfcApproval::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcApproval> other = dynamic_pointer_cast<IfcApproval>(other_entity);
	if( !other) { return; }
	m_Identifier = other->m_Identifier;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_TimeOfApproval = other->m_TimeOfApproval;
	m_Status = other->m_Status;
	m_Level = other->m_Level;
	m_Qualifier = other->m_Qualifier;
	m_RequestingApproval = other->m_RequestingApproval;
	m_GivingApproval = other->m_GivingApproval;
}
void IfcApproval::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCAPPROVAL" << "(";
	if( m_Identifier ) { m_Identifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TimeOfApproval ) { m_TimeOfApproval->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Status ) { m_Status->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Level ) { m_Level->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Qualifier ) { m_Qualifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RequestingApproval ) { m_RequestingApproval->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_GivingApproval ) { m_GivingApproval->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcApproval::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcApproval::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcApproval, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcApproval, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Identifier = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Name = IfcLabel::createObjectFromStepData( args[1] );
	m_Description = IfcText::createObjectFromStepData( args[2] );
	m_TimeOfApproval = IfcDateTime::createObjectFromStepData( args[3] );
	m_Status = IfcLabel::createObjectFromStepData( args[4] );
	m_Level = IfcLabel::createObjectFromStepData( args[5] );
	m_Qualifier = IfcText::createObjectFromStepData( args[6] );
	m_RequestingApproval = IfcActorSelect::createObjectFromStepData( args[7], map );
	m_GivingApproval = IfcActorSelect::createObjectFromStepData( args[8], map );
}
void IfcApproval::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Identifier", m_Identifier ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "TimeOfApproval", m_TimeOfApproval ) );
	vec_attributes.push_back( std::make_pair( "Status", m_Status ) );
	vec_attributes.push_back( std::make_pair( "Level", m_Level ) );
	vec_attributes.push_back( std::make_pair( "Qualifier", m_Qualifier ) );
	vec_attributes.push_back( std::make_pair( "RequestingApproval", m_RequestingApproval ) );
	vec_attributes.push_back( std::make_pair( "GivingApproval", m_GivingApproval ) );
}
void IfcApproval::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasExternalReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasExternalReferences_inverse.size(); ++i ) { HasExternalReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReferences_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasExternalReferences_inverse", HasExternalReferences_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ApprovedObjects_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ApprovedObjects_inverse.size(); ++i ) { ApprovedObjects_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesApproval>( m_ApprovedObjects_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ApprovedObjects_inverse", ApprovedObjects_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> ApprovedResources_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ApprovedResources_inverse.size(); ++i ) { ApprovedResources_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcResourceApprovalRelationship>( m_ApprovedResources_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ApprovedResources_inverse", ApprovedResources_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> IsRelatedWith_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsRelatedWith_inverse.size(); ++i ) { IsRelatedWith_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcApprovalRelationship>( m_IsRelatedWith_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsRelatedWith_inverse", IsRelatedWith_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> Relates_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_Relates_inverse.size(); ++i ) { Relates_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcApprovalRelationship>( m_Relates_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "Relates_inverse", Relates_inverse_vec_obj ) );
}
void IfcApproval::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcApproval::unlinkSelf()
{
}
