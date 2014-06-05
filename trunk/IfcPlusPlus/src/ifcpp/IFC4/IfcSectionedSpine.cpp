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

// method setEntity takes over all attributes from another instance of the class
void IfcSectionedSpine::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSectionedSpine> other = dynamic_pointer_cast<IfcSectionedSpine>(other_entity);
	if( !other) { return; }
	m_SpineCurve = other->m_SpineCurve;
	m_CrossSections = other->m_CrossSections;
	m_CrossSectionPositions = other->m_CrossSectionPositions;
}
void IfcSectionedSpine::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSECTIONEDSPINE" << "(";
	if( m_SpineCurve ) { stream << "#" << m_SpineCurve->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_CrossSections );
	stream << ",";
	writeEntityList( stream, m_CrossSectionPositions );
	stream << ");";
}
void IfcSectionedSpine::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSectionedSpine::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
}
void IfcSectionedSpine::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSectionedSpine::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcSectionedSpine::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
