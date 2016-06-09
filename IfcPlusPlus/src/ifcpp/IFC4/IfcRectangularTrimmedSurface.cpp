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
#include "include/IfcBoolean.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcRectangularTrimmedSurface.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcRectangularTrimmedSurface 
IfcRectangularTrimmedSurface::IfcRectangularTrimmedSurface() { m_entity_enum = IFCRECTANGULARTRIMMEDSURFACE; }
IfcRectangularTrimmedSurface::IfcRectangularTrimmedSurface( int id ) { m_id = id; m_entity_enum = IFCRECTANGULARTRIMMEDSURFACE; }
IfcRectangularTrimmedSurface::~IfcRectangularTrimmedSurface() {}
shared_ptr<IfcPPObject> IfcRectangularTrimmedSurface::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRectangularTrimmedSurface> copy_self( new IfcRectangularTrimmedSurface() );
	if( m_BasisSurface ) { copy_self->m_BasisSurface = dynamic_pointer_cast<IfcSurface>( m_BasisSurface->getDeepCopy(options) ); }
	if( m_U1 ) { copy_self->m_U1 = dynamic_pointer_cast<IfcParameterValue>( m_U1->getDeepCopy(options) ); }
	if( m_V1 ) { copy_self->m_V1 = dynamic_pointer_cast<IfcParameterValue>( m_V1->getDeepCopy(options) ); }
	if( m_U2 ) { copy_self->m_U2 = dynamic_pointer_cast<IfcParameterValue>( m_U2->getDeepCopy(options) ); }
	if( m_V2 ) { copy_self->m_V2 = dynamic_pointer_cast<IfcParameterValue>( m_V2->getDeepCopy(options) ); }
	if( m_Usense ) { copy_self->m_Usense = dynamic_pointer_cast<IfcBoolean>( m_Usense->getDeepCopy(options) ); }
	if( m_Vsense ) { copy_self->m_Vsense = dynamic_pointer_cast<IfcBoolean>( m_Vsense->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRectangularTrimmedSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRECTANGULARTRIMMEDSURFACE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_U1 ) { m_U1->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_V1 ) { m_V1->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_U2 ) { m_U2->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_V2 ) { m_V2->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Usense ) { m_Usense->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Vsense ) { m_Vsense->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRectangularTrimmedSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRectangularTrimmedSurface::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRectangularTrimmedSurface, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_BasisSurface, map );
	m_U1 = IfcParameterValue::createObjectFromSTEP( args[1] );
	m_V1 = IfcParameterValue::createObjectFromSTEP( args[2] );
	m_U2 = IfcParameterValue::createObjectFromSTEP( args[3] );
	m_V2 = IfcParameterValue::createObjectFromSTEP( args[4] );
	m_Usense = IfcBoolean::createObjectFromSTEP( args[5] );
	m_Vsense = IfcBoolean::createObjectFromSTEP( args[6] );
}
void IfcRectangularTrimmedSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	vec_attributes.push_back( std::make_pair( "U1", m_U1 ) );
	vec_attributes.push_back( std::make_pair( "V1", m_V1 ) );
	vec_attributes.push_back( std::make_pair( "U2", m_U2 ) );
	vec_attributes.push_back( std::make_pair( "V2", m_V2 ) );
	vec_attributes.push_back( std::make_pair( "Usense", m_Usense ) );
	vec_attributes.push_back( std::make_pair( "Vsense", m_Vsense ) );
}
void IfcRectangularTrimmedSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcRectangularTrimmedSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcRectangularTrimmedSurface::unlinkFromInverseCounterparts()
{
	IfcBoundedSurface::unlinkFromInverseCounterparts();
}
