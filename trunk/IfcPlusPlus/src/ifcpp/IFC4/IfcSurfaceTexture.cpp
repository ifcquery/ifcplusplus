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
#include "include/IfcCartesianTransformationOperator2D.h"
#include "include/IfcIdentifier.h"
#include "include/IfcSurfaceStyleWithTextures.h"
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTextureCoordinate.h"

// ENTITY IfcSurfaceTexture 
IfcSurfaceTexture::IfcSurfaceTexture() {}
IfcSurfaceTexture::IfcSurfaceTexture( int id ) { m_id = id; }
IfcSurfaceTexture::~IfcSurfaceTexture() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSurfaceTexture::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSurfaceTexture> other = dynamic_pointer_cast<IfcSurfaceTexture>(other_entity);
	if( !other) { return; }
	m_RepeatS = other->m_RepeatS;
	m_RepeatT = other->m_RepeatT;
	m_Mode = other->m_Mode;
	m_TextureTransform = other->m_TextureTransform;
	m_Parameter = other->m_Parameter;
}
void IfcSurfaceTexture::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSURFACETEXTURE" << "(";
	if( m_RepeatS == false ) { stream << ".F."; }
	else if( m_RepeatS == true ) { stream << ".T."; }
	stream << ",";
	if( m_RepeatT == false ) { stream << ".F."; }
	else if( m_RepeatT == true ) { stream << ".T."; }
	stream << ",";
	if( m_Mode ) { m_Mode->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TextureTransform ) { stream << "#" << m_TextureTransform->getId(); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_Parameter );
	stream << ");";
}
void IfcSurfaceTexture::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceTexture::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSurfaceTexture, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcSurfaceTexture, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	if( _stricmp( args[0].c_str(), ".F." ) == 0 ) { m_RepeatS = false; }
	else if( _stricmp( args[0].c_str(), ".T." ) == 0 ) { m_RepeatS = true; }
	if( _stricmp( args[1].c_str(), ".F." ) == 0 ) { m_RepeatT = false; }
	else if( _stricmp( args[1].c_str(), ".T." ) == 0 ) { m_RepeatT = true; }
	m_Mode = IfcIdentifier::createObjectFromStepData( args[2] );
	readEntityReference( args[3], m_TextureTransform, map );
	readTypeList( args[4], m_Parameter );
}
void IfcSurfaceTexture::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RepeatS", shared_ptr<IfcPPBool>( new IfcPPBool( m_RepeatS ) ) ) );
	vec_attributes.push_back( std::make_pair( "RepeatT", shared_ptr<IfcPPBool>( new IfcPPBool( m_RepeatT ) ) ) );
	vec_attributes.push_back( std::make_pair( "Mode", m_Mode ) );
	vec_attributes.push_back( std::make_pair( "TextureTransform", m_TextureTransform ) );
	shared_ptr<IfcPPAttributeObjectVector> Parameter_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Parameter.begin(), m_Parameter.end(), std::back_inserter( Parameter_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Parameter", Parameter_vec_object ) );
}
void IfcSurfaceTexture::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> IsMappedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_IsMappedBy_inverse.size(); ++i ) { IsMappedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcTextureCoordinate>( m_IsMappedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "IsMappedBy_inverse", IsMappedBy_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> UsedInStyles_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_UsedInStyles_inverse.size(); ++i ) { UsedInStyles_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcSurfaceStyleWithTextures>( m_UsedInStyles_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "UsedInStyles_inverse", UsedInStyles_inverse_vec_obj ) );
}
void IfcSurfaceTexture::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceTexture::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
