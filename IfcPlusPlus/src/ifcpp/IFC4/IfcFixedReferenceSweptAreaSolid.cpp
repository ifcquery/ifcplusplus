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
#include "include/IfcCurve.h"
#include "include/IfcDirection.h"
#include "include/IfcFixedReferenceSweptAreaSolid.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProfileDef.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcFixedReferenceSweptAreaSolid 
IfcFixedReferenceSweptAreaSolid::IfcFixedReferenceSweptAreaSolid() {}
IfcFixedReferenceSweptAreaSolid::IfcFixedReferenceSweptAreaSolid( int id ) { m_id = id; }
IfcFixedReferenceSweptAreaSolid::~IfcFixedReferenceSweptAreaSolid() {}

// method setEntity takes over all attributes from another instance of the class
void IfcFixedReferenceSweptAreaSolid::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcFixedReferenceSweptAreaSolid> other = dynamic_pointer_cast<IfcFixedReferenceSweptAreaSolid>(other_entity);
	if( !other) { return; }
	m_SweptArea = other->m_SweptArea;
	m_Position = other->m_Position;
	m_Directrix = other->m_Directrix;
	m_StartParam = other->m_StartParam;
	m_EndParam = other->m_EndParam;
	m_FixedReference = other->m_FixedReference;
}
void IfcFixedReferenceSweptAreaSolid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCFIXEDREFERENCESWEPTAREASOLID" << "(";
	if( m_SweptArea ) { stream << "#" << m_SweptArea->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Directrix ) { stream << "#" << m_Directrix->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_StartParam ) { m_StartParam->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EndParam ) { m_EndParam->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FixedReference ) { stream << "#" << m_FixedReference->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcFixedReferenceSweptAreaSolid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFixedReferenceSweptAreaSolid::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFixedReferenceSweptAreaSolid, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcFixedReferenceSweptAreaSolid, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_SweptArea, map );
	readEntityReference( args[1], m_Position, map );
	readEntityReference( args[2], m_Directrix, map );
	m_StartParam = IfcParameterValue::createObjectFromStepData( args[3] );
	m_EndParam = IfcParameterValue::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_FixedReference, map );
}
void IfcFixedReferenceSweptAreaSolid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSweptAreaSolid::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Directrix", m_Directrix ) );
	vec_attributes.push_back( std::make_pair( "StartParam", m_StartParam ) );
	vec_attributes.push_back( std::make_pair( "EndParam", m_EndParam ) );
	vec_attributes.push_back( std::make_pair( "FixedReference", m_FixedReference ) );
}
void IfcFixedReferenceSweptAreaSolid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcFixedReferenceSweptAreaSolid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSweptAreaSolid::setInverseCounterparts( ptr_self_entity );
}
void IfcFixedReferenceSweptAreaSolid::unlinkSelf()
{
	IfcSweptAreaSolid::unlinkSelf();
}
