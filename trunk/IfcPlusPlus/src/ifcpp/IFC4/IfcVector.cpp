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
#include "include/IfcDirection.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVector.h"

// ENTITY IfcVector 
IfcVector::IfcVector() {}
IfcVector::IfcVector( int id ) { m_id = id; }
IfcVector::~IfcVector() {}
shared_ptr<IfcPPObject> IfcVector::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcVector> copy_self( new IfcVector() );
	if( m_Orientation ) { copy_self->m_Orientation = dynamic_pointer_cast<IfcDirection>( m_Orientation->getDeepCopy(options) ); }
	if( m_Magnitude ) { copy_self->m_Magnitude = dynamic_pointer_cast<IfcLengthMeasure>( m_Magnitude->getDeepCopy(options) ); }
	return copy_self;
}
void IfcVector::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCVECTOR" << "(";
	if( m_Orientation ) { stream << "#" << m_Orientation->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Magnitude ) { m_Magnitude->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcVector::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcVector::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcVector, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_Orientation, map );
	m_Magnitude = IfcLengthMeasure::createObjectFromSTEP( args[1] );
}
void IfcVector::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Orientation", m_Orientation ) );
	vec_attributes.push_back( std::make_pair( "Magnitude", m_Magnitude ) );
}
void IfcVector::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcVector::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcVector::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
