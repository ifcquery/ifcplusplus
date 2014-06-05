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
#include "include/IfcInteger.h"
#include "include/IfcPixelTexture.h"
#include "include/IfcSurfaceStyleWithTextures.h"
#include "include/IfcTextureCoordinate.h"

// ENTITY IfcPixelTexture 
IfcPixelTexture::IfcPixelTexture() {}
IfcPixelTexture::IfcPixelTexture( int id ) { m_id = id; }
IfcPixelTexture::~IfcPixelTexture() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPixelTexture::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPixelTexture> other = dynamic_pointer_cast<IfcPixelTexture>(other_entity);
	if( !other) { return; }
	m_RepeatS = other->m_RepeatS;
	m_RepeatT = other->m_RepeatT;
	m_Mode = other->m_Mode;
	m_TextureTransform = other->m_TextureTransform;
	m_Parameter = other->m_Parameter;
	m_Width = other->m_Width;
	m_Height = other->m_Height;
	m_ColourComponents = other->m_ColourComponents;
	m_Pixel = other->m_Pixel;
}
void IfcPixelTexture::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPIXELTEXTURE" << "(";
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
	stream << ",";
	if( m_Width ) { m_Width->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Height ) { m_Height->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ColourComponents ) { m_ColourComponents->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeConstCharList( stream, m_Pixel );
	stream << ");";
}
void IfcPixelTexture::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPixelTexture::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPixelTexture, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcPixelTexture, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	if( _stricmp( args[0].c_str(), ".F." ) == 0 ) { m_RepeatS = false; }
	else if( _stricmp( args[0].c_str(), ".T." ) == 0 ) { m_RepeatS = true; }
	if( _stricmp( args[1].c_str(), ".F." ) == 0 ) { m_RepeatT = false; }
	else if( _stricmp( args[1].c_str(), ".T." ) == 0 ) { m_RepeatT = true; }
	m_Mode = IfcIdentifier::createObjectFromStepData( args[2] );
	readEntityReference( args[3], m_TextureTransform, map );
	readTypeList( args[4], m_Parameter );
	m_Width = IfcInteger::createObjectFromStepData( args[5] );
	m_Height = IfcInteger::createObjectFromStepData( args[6] );
	m_ColourComponents = IfcInteger::createObjectFromStepData( args[7] );
	readConstCharList( args[8], m_Pixel );
}
void IfcPixelTexture::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSurfaceTexture::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Width", m_Width ) );
	vec_attributes.push_back( std::make_pair( "Height", m_Height ) );
	vec_attributes.push_back( std::make_pair( "ColourComponents", m_ColourComponents ) );
}
void IfcPixelTexture::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPixelTexture::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSurfaceTexture::setInverseCounterparts( ptr_self_entity );
}
void IfcPixelTexture::unlinkSelf()
{
	IfcSurfaceTexture::unlinkSelf();
}
