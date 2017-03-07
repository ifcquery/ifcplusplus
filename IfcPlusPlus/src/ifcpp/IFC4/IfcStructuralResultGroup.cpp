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
#include "include/IfcAnalysisTheoryTypeEnum.h"
#include "include/IfcBoolean.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToGroup.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcStructuralAnalysisModel.h"
#include "include/IfcStructuralLoadGroup.h"
#include "include/IfcStructuralResultGroup.h"
#include "include/IfcText.h"

// ENTITY IfcStructuralResultGroup 
IfcStructuralResultGroup::IfcStructuralResultGroup() { m_entity_enum = IFCSTRUCTURALRESULTGROUP; }
IfcStructuralResultGroup::IfcStructuralResultGroup( int id ) { m_id = id; m_entity_enum = IFCSTRUCTURALRESULTGROUP; }
IfcStructuralResultGroup::~IfcStructuralResultGroup() {}
shared_ptr<IfcPPObject> IfcStructuralResultGroup::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralResultGroup> copy_self( new IfcStructuralResultGroup() );
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
	if( m_TheoryType ) { copy_self->m_TheoryType = dynamic_pointer_cast<IfcAnalysisTheoryTypeEnum>( m_TheoryType->getDeepCopy(options) ); }
	if( m_ResultForLoadGroup ) { copy_self->m_ResultForLoadGroup = dynamic_pointer_cast<IfcStructuralLoadGroup>( m_ResultForLoadGroup->getDeepCopy(options) ); }
	if( m_IsLinear ) { copy_self->m_IsLinear = dynamic_pointer_cast<IfcBoolean>( m_IsLinear->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStructuralResultGroup::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALRESULTGROUP" << "(";
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
	if( m_TheoryType ) { m_TheoryType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ResultForLoadGroup ) { stream << "#" << m_ResultForLoadGroup->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_IsLinear ) { m_IsLinear->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralResultGroup::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralResultGroup::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream err; err << "Wrong parameter count for entity IfcStructuralResultGroup, expecting 8, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	m_TheoryType = IfcAnalysisTheoryTypeEnum::createObjectFromSTEP( args[5] );
	readEntityReference( args[6], m_ResultForLoadGroup, map );
	m_IsLinear = IfcBoolean::createObjectFromSTEP( args[7] );
}
void IfcStructuralResultGroup::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGroup::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TheoryType", m_TheoryType ) );
	vec_attributes.push_back( std::make_pair( "ResultForLoadGroup", m_ResultForLoadGroup ) );
	vec_attributes.push_back( std::make_pair( "IsLinear", m_IsLinear ) );
}
void IfcStructuralResultGroup::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGroup::getAttributesInverse( vec_attributes_inverse );
	if( m_ResultGroupFor_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ResultGroupFor_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_ResultGroupFor_inverse.size(); ++i )
		{
			if( !m_ResultGroupFor_inverse[i].expired() )
			{
				ResultGroupFor_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcStructuralAnalysisModel>( m_ResultGroupFor_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "ResultGroupFor_inverse", ResultGroupFor_inverse_vec_obj ) );
	}
}
void IfcStructuralResultGroup::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGroup::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcStructuralResultGroup> ptr_self = dynamic_pointer_cast<IfcStructuralResultGroup>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcStructuralResultGroup::setInverseCounterparts: type mismatch" ); }
	if( m_ResultForLoadGroup )
	{
		m_ResultForLoadGroup->m_SourceOfResultGroup_inverse.push_back( ptr_self );
	}
}
void IfcStructuralResultGroup::unlinkFromInverseCounterparts()
{
	IfcGroup::unlinkFromInverseCounterparts();
	if( m_ResultForLoadGroup )
	{
		std::vector<weak_ptr<IfcStructuralResultGroup> >& SourceOfResultGroup_inverse = m_ResultForLoadGroup->m_SourceOfResultGroup_inverse;
		for( auto it_SourceOfResultGroup_inverse = SourceOfResultGroup_inverse.begin(); it_SourceOfResultGroup_inverse != SourceOfResultGroup_inverse.end(); )
		{
			shared_ptr<IfcStructuralResultGroup> self_candidate( *it_SourceOfResultGroup_inverse );
			if( self_candidate.get() == this )
			{
				it_SourceOfResultGroup_inverse= SourceOfResultGroup_inverse.erase( it_SourceOfResultGroup_inverse );
			}
			else
			{
				++it_SourceOfResultGroup_inverse;
			}
		}
	}
}
