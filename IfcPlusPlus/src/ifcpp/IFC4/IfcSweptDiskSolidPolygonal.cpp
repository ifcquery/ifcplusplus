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
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcCurve.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSweptDiskSolidPolygonal.h"

// ENTITY IfcSweptDiskSolidPolygonal 
IfcSweptDiskSolidPolygonal::IfcSweptDiskSolidPolygonal() {}
IfcSweptDiskSolidPolygonal::IfcSweptDiskSolidPolygonal( int id ) { m_id = id; }
IfcSweptDiskSolidPolygonal::~IfcSweptDiskSolidPolygonal() {}
shared_ptr<IfcPPObject> IfcSweptDiskSolidPolygonal::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSweptDiskSolidPolygonal> copy_self( new IfcSweptDiskSolidPolygonal() );
	if( m_Directrix ) { copy_self->m_Directrix = dynamic_pointer_cast<IfcCurve>( m_Directrix->getDeepCopy(options) ); }
	if( m_Radius ) { copy_self->m_Radius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Radius->getDeepCopy(options) ); }
	if( m_InnerRadius ) { copy_self->m_InnerRadius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_InnerRadius->getDeepCopy(options) ); }
	if( m_StartParam ) { copy_self->m_StartParam = dynamic_pointer_cast<IfcParameterValue>( m_StartParam->getDeepCopy(options) ); }
	if( m_EndParam ) { copy_self->m_EndParam = dynamic_pointer_cast<IfcParameterValue>( m_EndParam->getDeepCopy(options) ); }
	if( m_FilletRadius ) { copy_self->m_FilletRadius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_FilletRadius->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSweptDiskSolidPolygonal::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSWEPTDISKSOLIDPOLYGONAL" << "(";
	if( m_Directrix ) { stream << "#" << m_Directrix->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Radius ) { m_Radius->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_InnerRadius ) { m_InnerRadius->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_StartParam ) { m_StartParam->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_EndParam ) { m_EndParam->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_FilletRadius ) { m_FilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSweptDiskSolidPolygonal::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSweptDiskSolidPolygonal::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSweptDiskSolidPolygonal, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_Directrix, map );
	m_Radius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[1] );
	m_InnerRadius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[2] );
	m_StartParam = IfcParameterValue::createObjectFromSTEP( args[3] );
	m_EndParam = IfcParameterValue::createObjectFromSTEP( args[4] );
	m_FilletRadius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[5] );
}
void IfcSweptDiskSolidPolygonal::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSweptDiskSolid::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "FilletRadius", m_FilletRadius ) );
}
void IfcSweptDiskSolidPolygonal::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSweptDiskSolid::getAttributesInverse( vec_attributes_inverse );
}
void IfcSweptDiskSolidPolygonal::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSweptDiskSolid::setInverseCounterparts( ptr_self_entity );
}
void IfcSweptDiskSolidPolygonal::unlinkSelf()
{
	IfcSweptDiskSolid::unlinkSelf();
}
