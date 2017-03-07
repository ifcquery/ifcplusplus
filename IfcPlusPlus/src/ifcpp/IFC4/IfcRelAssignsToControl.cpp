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
#include "include/IfcControl.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcObjectTypeEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssignsToControl.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssignsToControl 
IfcRelAssignsToControl::IfcRelAssignsToControl() { m_entity_enum = IFCRELASSIGNSTOCONTROL; }
IfcRelAssignsToControl::IfcRelAssignsToControl( int id ) { m_id = id; m_entity_enum = IFCRELASSIGNSTOCONTROL; }
IfcRelAssignsToControl::~IfcRelAssignsToControl() {}
shared_ptr<IfcPPObject> IfcRelAssignsToControl::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelAssignsToControl> copy_self( new IfcRelAssignsToControl() );
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
	for( size_t ii=0; ii<m_RelatedObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedObjects.push_back( dynamic_pointer_cast<IfcObjectDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatedObjectsType ) { copy_self->m_RelatedObjectsType = dynamic_pointer_cast<IfcObjectTypeEnum>( m_RelatedObjectsType->getDeepCopy(options) ); }
	if( m_RelatingControl ) { copy_self->m_RelatingControl = dynamic_pointer_cast<IfcControl>( m_RelatingControl->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelAssignsToControl::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELASSIGNSTOCONTROL" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_RelatedObjects );
	stream << ",";
	if( m_RelatedObjectsType ) { m_RelatedObjectsType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingControl ) { stream << "#" << m_RelatingControl->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssignsToControl::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssignsToControl::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRelAssignsToControl, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatedObjectsType = IfcObjectTypeEnum::createObjectFromSTEP( args[5] );
	readEntityReference( args[6], m_RelatingControl, map );
}
void IfcRelAssignsToControl::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssigns::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingControl", m_RelatingControl ) );
}
void IfcRelAssignsToControl::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelAssigns::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelAssignsToControl::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssigns::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssignsToControl> ptr_self = dynamic_pointer_cast<IfcRelAssignsToControl>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssignsToControl::setInverseCounterparts: type mismatch" ); }
	if( m_RelatingControl )
	{
		m_RelatingControl->m_Controls_inverse.push_back( ptr_self );
	}
}
void IfcRelAssignsToControl::unlinkFromInverseCounterparts()
{
	IfcRelAssigns::unlinkFromInverseCounterparts();
	if( m_RelatingControl )
	{
		std::vector<weak_ptr<IfcRelAssignsToControl> >& Controls_inverse = m_RelatingControl->m_Controls_inverse;
		for( auto it_Controls_inverse = Controls_inverse.begin(); it_Controls_inverse != Controls_inverse.end(); )
		{
			shared_ptr<IfcRelAssignsToControl> self_candidate( *it_Controls_inverse );
			if( self_candidate.get() == this )
			{
				it_Controls_inverse= Controls_inverse.erase( it_Controls_inverse );
			}
			else
			{
				++it_Controls_inverse;
			}
		}
	}
}
