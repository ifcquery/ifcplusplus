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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcCompositeCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProfileDef.h"
#include "include/IfcSectionedSpine.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcSectionedSpine 
IfcSectionedSpine::IfcSectionedSpine() { m_entity_enum = IFCSECTIONEDSPINE; }
IfcSectionedSpine::IfcSectionedSpine( int id ) { m_id = id; m_entity_enum = IFCSECTIONEDSPINE; }
IfcSectionedSpine::~IfcSectionedSpine() {}
shared_ptr<IfcPPObject> IfcSectionedSpine::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSectionedSpine> copy_self( new IfcSectionedSpine() );
	if( m_SpineCurve ) { copy_self->m_SpineCurve = dynamic_pointer_cast<IfcCompositeCurve>( m_SpineCurve->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_CrossSections.size(); ++ii )
	{
		auto item_ii = m_CrossSections[ii];
		if( item_ii )
		{
			copy_self->m_CrossSections.push_back( dynamic_pointer_cast<IfcProfileDef>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_CrossSectionPositions.size(); ++ii )
	{
		auto item_ii = m_CrossSectionPositions[ii];
		if( item_ii )
		{
			copy_self->m_CrossSectionPositions.push_back( dynamic_pointer_cast<IfcAxis2Placement3D>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcSectionedSpine::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSECTIONEDSPINE" << "(";
	if( m_SpineCurve ) { stream << "#" << m_SpineCurve->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_CrossSections );
	stream << ",";
	writeEntityList( stream, m_CrossSectionPositions );
	stream << ");";
}
void IfcSectionedSpine::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSectionedSpine::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSectionedSpine, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_SpineCurve, map );
	readEntityReferenceList( args[1], m_CrossSections, map );
	readEntityReferenceList( args[2], m_CrossSectionPositions, map );
}
void IfcSectionedSpine::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SpineCurve", m_SpineCurve ) );
	if( m_CrossSections.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> CrossSections_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_CrossSections.begin(), m_CrossSections.end(), std::back_inserter( CrossSections_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "CrossSections", CrossSections_vec_object ) );
	}
	if( m_CrossSectionPositions.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> CrossSectionPositions_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_CrossSectionPositions.begin(), m_CrossSectionPositions.end(), std::back_inserter( CrossSectionPositions_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "CrossSectionPositions", CrossSectionPositions_vec_object ) );
	}
}
void IfcSectionedSpine::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcSectionedSpine::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcSectionedSpine::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
