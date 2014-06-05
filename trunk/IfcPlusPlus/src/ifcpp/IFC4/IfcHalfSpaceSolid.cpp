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

// method setEntity takes over all attributes from another instance of the class
void IfcHalfSpaceSolid::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcHalfSpaceSolid> other = dynamic_pointer_cast<IfcHalfSpaceSolid>(other_entity);
	if( !other) { return; }
	m_BaseSurface = other->m_BaseSurface;
	m_AgreementFlag = other->m_AgreementFlag;
}
void IfcHalfSpaceSolid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCHALFSPACESOLID" << "(";
	if( m_BaseSurface ) { stream << "#" << m_BaseSurface->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AgreementFlag == false ) { stream << ".F."; }
	else if( m_AgreementFlag == true ) { stream << ".T."; }
	stream << ");";
}
void IfcHalfSpaceSolid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcHalfSpaceSolid::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcHalfSpaceSolid, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcHalfSpaceSolid, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_BaseSurface, map );
	if( _stricmp( args[1].c_str(), ".F." ) == 0 ) { m_AgreementFlag = false; }
	else if( _stricmp( args[1].c_str(), ".T." ) == 0 ) { m_AgreementFlag = true; }
}
void IfcHalfSpaceSolid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BaseSurface", m_BaseSurface ) );
	vec_attributes.push_back( std::make_pair( "AgreementFlag", shared_ptr<IfcPPBool>( new IfcPPBool( m_AgreementFlag ) ) ) );
}
void IfcHalfSpaceSolid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcHalfSpaceSolid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcHalfSpaceSolid::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
