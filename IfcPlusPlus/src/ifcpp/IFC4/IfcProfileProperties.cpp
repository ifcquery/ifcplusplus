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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcProfileDef.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProperty.h"
#include "include/IfcText.h"

// ENTITY IfcProfileProperties 
IfcProfileProperties::IfcProfileProperties() { m_entity_enum = IFCPROFILEPROPERTIES; }
IfcProfileProperties::IfcProfileProperties( int id ) { m_id = id; m_entity_enum = IFCPROFILEPROPERTIES; }
IfcProfileProperties::~IfcProfileProperties() {}
shared_ptr<IfcPPObject> IfcProfileProperties::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcProfileProperties> copy_self( new IfcProfileProperties() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Properties.size(); ++ii )
	{
		auto item_ii = m_Properties[ii];
		if( item_ii )
		{
			copy_self->m_Properties.push_back( dynamic_pointer_cast<IfcProperty>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_ProfileDefinition )
	{
		if( options.shallow_copy_IfcProfileDef ) { copy_self->m_ProfileDefinition = m_ProfileDefinition; }
		else { copy_self->m_ProfileDefinition = dynamic_pointer_cast<IfcProfileDef>( m_ProfileDefinition->getDeepCopy(options) ); }
	}
	return copy_self;
}
void IfcProfileProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROFILEPROPERTIES" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Properties );
	stream << ",";
	if( m_ProfileDefinition ) { stream << "#" << m_ProfileDefinition->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcProfileProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProfileProperties::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcProfileProperties, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReferenceList( args[2], m_Properties, map );
	readEntityReference( args[3], m_ProfileDefinition, map );
}
void IfcProfileProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExtendedProperties::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ProfileDefinition", m_ProfileDefinition ) );
}
void IfcProfileProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcExtendedProperties::getAttributesInverse( vec_attributes_inverse );
}
void IfcProfileProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExtendedProperties::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcProfileProperties> ptr_self = dynamic_pointer_cast<IfcProfileProperties>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcProfileProperties::setInverseCounterparts: type mismatch" ); }
	if( m_ProfileDefinition )
	{
		m_ProfileDefinition->m_HasProperties_inverse.push_back( ptr_self );
	}
}
void IfcProfileProperties::unlinkFromInverseCounterparts()
{
	IfcExtendedProperties::unlinkFromInverseCounterparts();
	if( m_ProfileDefinition )
	{
		std::vector<weak_ptr<IfcProfileProperties> >& HasProperties_inverse = m_ProfileDefinition->m_HasProperties_inverse;
		for( auto it_HasProperties_inverse = HasProperties_inverse.begin(); it_HasProperties_inverse != HasProperties_inverse.end(); )
		{
			shared_ptr<IfcProfileProperties> self_candidate( *it_HasProperties_inverse );
			if( self_candidate.get() == this )
			{
				it_HasProperties_inverse= HasProperties_inverse.erase( it_HasProperties_inverse );
			}
			else
			{
				++it_HasProperties_inverse;
			}
		}
	}
}
