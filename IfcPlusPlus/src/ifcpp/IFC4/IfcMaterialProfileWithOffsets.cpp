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
#include "include/IfcInteger.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcMaterial.h"
#include "include/IfcMaterialProfileSet.h"
#include "include/IfcMaterialProfileWithOffsets.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcProfileDef.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialProfileWithOffsets 
IfcMaterialProfileWithOffsets::IfcMaterialProfileWithOffsets() { m_entity_enum = IFCMATERIALPROFILEWITHOFFSETS; }
IfcMaterialProfileWithOffsets::IfcMaterialProfileWithOffsets( int id ) { m_id = id; m_entity_enum = IFCMATERIALPROFILEWITHOFFSETS; }
IfcMaterialProfileWithOffsets::~IfcMaterialProfileWithOffsets() {}
shared_ptr<IfcPPObject> IfcMaterialProfileWithOffsets::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialProfileWithOffsets> copy_self( new IfcMaterialProfileWithOffsets() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_Material ) { copy_self->m_Material = dynamic_pointer_cast<IfcMaterial>( m_Material->getDeepCopy(options) ); }
	if( m_Profile )
	{
		if( options.shallow_copy_IfcProfileDef ) { copy_self->m_Profile = m_Profile; }
		else { copy_self->m_Profile = dynamic_pointer_cast<IfcProfileDef>( m_Profile->getDeepCopy(options) ); }
	}
	if( m_Priority ) { copy_self->m_Priority = dynamic_pointer_cast<IfcInteger>( m_Priority->getDeepCopy(options) ); }
	if( m_Category ) { copy_self->m_Category = dynamic_pointer_cast<IfcLabel>( m_Category->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_OffsetValues.size(); ++ii )
	{
		auto item_ii = m_OffsetValues[ii];
		if( item_ii )
		{
			copy_self->m_OffsetValues.push_back( dynamic_pointer_cast<IfcLengthMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcMaterialProfileWithOffsets::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALPROFILEWITHOFFSETS" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Material ) { stream << "#" << m_Material->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Profile ) { stream << "#" << m_Profile->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Priority ) { m_Priority->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Category ) { m_Category->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeNumericTypeList( stream, m_OffsetValues );
	stream << ");";
}
void IfcMaterialProfileWithOffsets::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialProfileWithOffsets::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialProfileWithOffsets, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Material, map );
	readEntityReference( args[3], m_Profile, map );
	m_Priority = IfcInteger::createObjectFromSTEP( args[4] );
	m_Category = IfcLabel::createObjectFromSTEP( args[5] );
	readTypeOfRealList( args[6], m_OffsetValues );
}
void IfcMaterialProfileWithOffsets::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialProfile::getAttributes( vec_attributes );
	if( m_OffsetValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> OffsetValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_OffsetValues.begin(), m_OffsetValues.end(), std::back_inserter( OffsetValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "OffsetValues", OffsetValues_vec_object ) );
	}
}
void IfcMaterialProfileWithOffsets::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcMaterialProfile::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialProfileWithOffsets::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialProfile::setInverseCounterparts( ptr_self_entity );
}
void IfcMaterialProfileWithOffsets::unlinkFromInverseCounterparts()
{
	IfcMaterialProfile::unlinkFromInverseCounterparts();
}
