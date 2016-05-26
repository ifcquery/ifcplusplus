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
#include "include/IfcHalfSpaceSolid.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcHalfSpaceSolid 
IfcHalfSpaceSolid::IfcHalfSpaceSolid() { m_entity_enum = IFCHALFSPACESOLID; }
IfcHalfSpaceSolid::IfcHalfSpaceSolid( int id ) { m_id = id; m_entity_enum = IFCHALFSPACESOLID; }
IfcHalfSpaceSolid::~IfcHalfSpaceSolid() {}
shared_ptr<IfcPPObject> IfcHalfSpaceSolid::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcHalfSpaceSolid> copy_self( new IfcHalfSpaceSolid() );
	if( m_BaseSurface ) { copy_self->m_BaseSurface = dynamic_pointer_cast<IfcSurface>( m_BaseSurface->getDeepCopy(options) ); }
	if( m_AgreementFlag ) { copy_self->m_AgreementFlag = dynamic_pointer_cast<IfcBoolean>( m_AgreementFlag->getDeepCopy(options) ); }
	return copy_self;
}
void IfcHalfSpaceSolid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCHALFSPACESOLID" << "(";
	if( m_BaseSurface ) { stream << "#" << m_BaseSurface->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_AgreementFlag ) { m_AgreementFlag->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcHalfSpaceSolid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcHalfSpaceSolid::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcHalfSpaceSolid, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_BaseSurface, map );
	m_AgreementFlag = IfcBoolean::createObjectFromSTEP( args[1] );
}
void IfcHalfSpaceSolid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BaseSurface", m_BaseSurface ) );
	vec_attributes.push_back( std::make_pair( "AgreementFlag", m_AgreementFlag ) );
}
void IfcHalfSpaceSolid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcHalfSpaceSolid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcHalfSpaceSolid::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
