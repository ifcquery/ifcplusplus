/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcActor.h"
#include "include/IfcActorSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToActor.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcActor 
IfcActor::IfcActor() { m_entity_enum = IFCACTOR; }
IfcActor::IfcActor( int id ) { m_id = id; m_entity_enum = IFCACTOR; }
IfcActor::~IfcActor() {}
shared_ptr<IfcPPObject> IfcActor::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcActor> copy_self( new IfcActor() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy(options) ); }
	if( m_TheActor ) { copy_self->m_TheActor = dynamic_pointer_cast<IfcActorSelect>( m_TheActor->getDeepCopy(options) ); }
	return copy_self;
}
void IfcActor::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCACTOR" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TheActor ) { m_TheActor->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcActor::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcActor::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcActor, expecting 6, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	m_TheActor = IfcActorSelect::createObjectFromSTEP( args[5], map );
}
void IfcActor::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObject::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TheActor", m_TheActor ) );
}
void IfcActor::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcObject::getAttributesInverse( vec_attributes_inverse );
	if( m_IsActingUpon_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> IsActingUpon_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_IsActingUpon_inverse.size(); ++i )
		{
			if( !m_IsActingUpon_inverse[i].expired() )
			{
				IsActingUpon_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssignsToActor>( m_IsActingUpon_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "IsActingUpon_inverse", IsActingUpon_inverse_vec_obj ) );
	}
}
void IfcActor::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObject::setInverseCounterparts( ptr_self_entity );
}
void IfcActor::unlinkFromInverseCounterparts()
{
	IfcObject::unlinkFromInverseCounterparts();
}
