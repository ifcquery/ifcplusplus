/* -*-c++-*- IfcPlusPlus - www.ifcquery.com - Copyright (C) 2011 Fabian Gerold
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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcToroidalSurface.h"

// ENTITY IfcToroidalSurface 
IfcToroidalSurface::IfcToroidalSurface() { m_entity_enum = IFCTOROIDALSURFACE; }
IfcToroidalSurface::IfcToroidalSurface( int id ) { m_id = id; m_entity_enum = IFCTOROIDALSURFACE; }
IfcToroidalSurface::~IfcToroidalSurface() {}
shared_ptr<IfcPPObject> IfcToroidalSurface::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcToroidalSurface> copy_self( new IfcToroidalSurface() );
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_MajorRadius ) { copy_self->m_MajorRadius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_MajorRadius->getDeepCopy(options) ); }
	if( m_MinorRadius ) { copy_self->m_MinorRadius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_MinorRadius->getDeepCopy(options) ); }
	return copy_self;
}
void IfcToroidalSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTOROIDALSURFACE" << "(";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_MajorRadius ) { m_MajorRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MinorRadius ) { m_MinorRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcToroidalSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcToroidalSurface::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcToroidalSurface, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Position, map );
	m_MajorRadius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[1] );
	m_MinorRadius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[2] );
}
void IfcToroidalSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcElementarySurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "MajorRadius", m_MajorRadius ) );
	vec_attributes.push_back( std::make_pair( "MinorRadius", m_MinorRadius ) );
}
void IfcToroidalSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcElementarySurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcToroidalSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcElementarySurface::setInverseCounterparts( ptr_self_entity );
}
void IfcToroidalSurface::unlinkFromInverseCounterparts()
{
	IfcElementarySurface::unlinkFromInverseCounterparts();
}
