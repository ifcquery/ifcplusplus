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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcCompositeCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProfileDef.h"
#include "include/IfcSectionedSpine.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcSectionedSpine 
IfcSectionedSpine::IfcSectionedSpine() {}
IfcSectionedSpine::IfcSectionedSpine( int id ) { m_id = id; }
IfcSectionedSpine::~IfcSectionedSpine() {}
shared_ptr<IfcPPObject> IfcSectionedSpine::getDeepCopy()
{
	shared_ptr<IfcSectionedSpine> copy_self( new IfcSectionedSpine() );
	if( m_SpineCurve ) { copy_self->m_SpineCurve = dynamic_pointer_cast<IfcCompositeCurve>( m_SpineCurve->getDeepCopy() ); }
	for( size_t ii=0; ii<m_CrossSections.size(); ++ii )
	{
		auto item_ii = m_CrossSections[ii];
		if( item_ii )
		{
			copy_self->m_CrossSections.push_back( dynamic_pointer_cast<IfcProfileDef>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_CrossSectionPositions.size(); ++ii )
	{
		auto item_ii = m_CrossSectionPositions[ii];
		if( item_ii )
		{
			copy_self->m_CrossSectionPositions.push_back( dynamic_pointer_cast<IfcAxis2Placement3D>(item_ii->getDeepCopy() ) );
		}
	}
	return copy_self;
}
void IfcSectionedSpine::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSECTIONEDSPINE" << "(";
	if( m_SpineCurve ) { stream << "#" << m_SpineCurve->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_CrossSections );
	stream << ",";
	writeEntityList( stream, m_CrossSectionPositions );
	stream << ");";
}
void IfcSectionedSpine::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSectionedSpine::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSectionedSpine, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcSectionedSpine, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
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
void IfcSectionedSpine::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
