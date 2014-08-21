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
#include "include/IfcAxis1Placement.h"
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProfileDef.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurfaceOfRevolution.h"

// ENTITY IfcSurfaceOfRevolution 
IfcSurfaceOfRevolution::IfcSurfaceOfRevolution() {}
IfcSurfaceOfRevolution::IfcSurfaceOfRevolution( int id ) { m_id = id; }
IfcSurfaceOfRevolution::~IfcSurfaceOfRevolution() {}
shared_ptr<IfcPPObject> IfcSurfaceOfRevolution::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceOfRevolution> copy_self( new IfcSurfaceOfRevolution() );
	if( m_SweptCurve )
	{
		if( options.shallow_copy_IfcProfileDef ) { copy_self->m_SweptCurve = m_SweptCurve; }
		else { copy_self->m_SweptCurve = dynamic_pointer_cast<IfcProfileDef>( m_SweptCurve->getDeepCopy(options) ); }
	}
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_AxisPosition ) { copy_self->m_AxisPosition = dynamic_pointer_cast<IfcAxis1Placement>( m_AxisPosition->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSurfaceOfRevolution::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSURFACEOFREVOLUTION" << "(";
	if( m_SweptCurve ) { stream << "#" << m_SweptCurve->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_AxisPosition ) { stream << "#" << m_AxisPosition->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceOfRevolution::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceOfRevolution::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSurfaceOfRevolution, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_SweptCurve, map );
	readEntityReference( args[1], m_Position, map );
	readEntityReference( args[2], m_AxisPosition, map );
}
void IfcSurfaceOfRevolution::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSweptSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "AxisPosition", m_AxisPosition ) );
}
void IfcSurfaceOfRevolution::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSweptSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceOfRevolution::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSweptSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceOfRevolution::unlinkSelf()
{
	IfcSweptSurface::unlinkSelf();
}
