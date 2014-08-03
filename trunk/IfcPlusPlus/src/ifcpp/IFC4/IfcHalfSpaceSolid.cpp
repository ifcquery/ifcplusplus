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
#include "include/IfcHalfSpaceSolid.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcHalfSpaceSolid 
IfcHalfSpaceSolid::IfcHalfSpaceSolid() {}
IfcHalfSpaceSolid::IfcHalfSpaceSolid( int id ) { m_id = id; }
IfcHalfSpaceSolid::~IfcHalfSpaceSolid() {}
shared_ptr<IfcPPObject> IfcHalfSpaceSolid::getDeepCopy()
{
	shared_ptr<IfcHalfSpaceSolid> copy_self( new IfcHalfSpaceSolid() );
	if( m_BaseSurface ) { copy_self->m_BaseSurface = dynamic_pointer_cast<IfcSurface>( m_BaseSurface->getDeepCopy() ); }
	if( m_AgreementFlag ) { copy_self->m_AgreementFlag = m_AgreementFlag; }
	return copy_self;
}
void IfcHalfSpaceSolid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCHALFSPACESOLID" << "(";
	if( m_BaseSurface ) { stream << "#" << m_BaseSurface->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AgreementFlag == false ) { stream << ".F."; }
	else if( m_AgreementFlag == true ) { stream << ".T."; }
	stream << ");";
}
void IfcHalfSpaceSolid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcHalfSpaceSolid::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcHalfSpaceSolid, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcHalfSpaceSolid, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BaseSurface, map );
	if( boost::iequals( args[1], L".F." ) ) { m_AgreementFlag = false; }
	else if( boost::iequals( args[1], L".T." ) ) { m_AgreementFlag = true; }
}
void IfcHalfSpaceSolid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BaseSurface", m_BaseSurface ) );
	vec_attributes.push_back( std::make_pair( "AgreementFlag", shared_ptr<IfcPPBool>( new IfcPPBool( m_AgreementFlag ) ) ) );
}
void IfcHalfSpaceSolid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcHalfSpaceSolid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcHalfSpaceSolid::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
