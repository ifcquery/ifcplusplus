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
#include "include/IfcDirection.h"
#include "include/IfcExtrudedAreaSolidTapered.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProfileDef.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcExtrudedAreaSolidTapered 
IfcExtrudedAreaSolidTapered::IfcExtrudedAreaSolidTapered() {}
IfcExtrudedAreaSolidTapered::IfcExtrudedAreaSolidTapered( int id ) { m_id = id; }
IfcExtrudedAreaSolidTapered::~IfcExtrudedAreaSolidTapered() {}

// method setEntity takes over all attributes from another instance of the class
void IfcExtrudedAreaSolidTapered::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcExtrudedAreaSolidTapered> other = dynamic_pointer_cast<IfcExtrudedAreaSolidTapered>(other_entity);
	if( !other) { return; }
	m_SweptArea = other->m_SweptArea;
	m_Position = other->m_Position;
	m_ExtrudedDirection = other->m_ExtrudedDirection;
	m_Depth = other->m_Depth;
	m_EndSweptArea = other->m_EndSweptArea;
}
void IfcExtrudedAreaSolidTapered::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCEXTRUDEDAREASOLIDTAPERED" << "(";
	if( m_SweptArea ) { stream << "#" << m_SweptArea->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_ExtrudedDirection ) { stream << "#" << m_ExtrudedDirection->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Depth ) { m_Depth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EndSweptArea ) { stream << "#" << m_EndSweptArea->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcExtrudedAreaSolidTapered::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcExtrudedAreaSolidTapered::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcExtrudedAreaSolidTapered, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcExtrudedAreaSolidTapered, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_SweptArea, map );
	readEntityReference( args[1], m_Position, map );
	readEntityReference( args[2], m_ExtrudedDirection, map );
	m_Depth = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_EndSweptArea, map );
}
void IfcExtrudedAreaSolidTapered::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExtrudedAreaSolid::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "EndSweptArea", m_EndSweptArea ) );
}
void IfcExtrudedAreaSolidTapered::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcExtrudedAreaSolidTapered::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExtrudedAreaSolid::setInverseCounterparts( ptr_self_entity );
}
void IfcExtrudedAreaSolidTapered::unlinkSelf()
{
	IfcExtrudedAreaSolid::unlinkSelf();
}
