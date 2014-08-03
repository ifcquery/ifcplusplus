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
shared_ptr<IfcPPObject> IfcPixelTexture::getDeepCopy()
{
	shared_ptr<IfcPixelTexture> copy_self( new IfcPixelTexture() );
	if( m_RepeatS ) { copy_self->m_RepeatS = m_RepeatS; }
	if( m_RepeatT ) { copy_self->m_RepeatT = m_RepeatT; }
	if( m_Mode ) { copy_self->m_Mode = dynamic_pointer_cast<IfcIdentifier>( m_Mode->getDeepCopy() ); }
	if( m_TextureTransform ) { copy_self->m_TextureTransform = dynamic_pointer_cast<IfcCartesianTransformationOperator2D>( m_TextureTransform->getDeepCopy() ); }
	for( size_t ii=0; ii<m_Parameter.size(); ++ii )
	{
		auto item_ii = m_Parameter[ii];
		if( item_ii )
		{
			copy_self->m_Parameter.push_back( dynamic_pointer_cast<IfcIdentifier>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_Width ) { copy_self->m_Width = dynamic_pointer_cast<IfcInteger>( m_Width->getDeepCopy() ); }
	if( m_Height ) { copy_self->m_Height = dynamic_pointer_cast<IfcInteger>( m_Height->getDeepCopy() ); }
	if( m_ColourComponents ) { copy_self->m_ColourComponents = dynamic_pointer_cast<IfcInteger>( m_ColourComponents->getDeepCopy() ); }
	return copy_self;
}
void IfcPixelTexture::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPIXELTEXTURE" << "(";
	if( m_RepeatS == false ) { stream << ".F."; }
	else if( m_RepeatS == true ) { stream << ".T."; }
	stream << ",";
	if( m_RepeatT == false ) { stream << ".F."; }
	else if( m_RepeatT == true ) { stream << ".T."; }
	stream << ",";
	if( m_Mode ) { m_Mode->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TextureTransform ) { stream << "#" << m_TextureTransform->getId(); } else { stream << "*"; }
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
void IfcPixelTexture::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPixelTexture, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcPixelTexture, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	if( boost::iequals( args[0], L".F." ) ) { m_RepeatS = false; }
	else if( boost::iequals( args[0], L".T." ) ) { m_RepeatS = true; }
	if( boost::iequals( args[1], L".F." ) ) { m_RepeatT = false; }
	else if( boost::iequals( args[1], L".T." ) ) { m_RepeatT = true; }
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
	if( m_Pixel.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Pixel_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_Pixel.size(); ++i )
		{
			Pixel_vec_obj->m_vec.push_back( shared_ptr<IfcPPBinary>( new IfcPPBinary(m_Pixel[i] ) ) );
		}
		vec_attributes.push_back( std::make_pair( "Pixel", Pixel_vec_obj ) );
	}
}
void IfcPixelTexture::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSurfaceTexture::getAttributesInverse( vec_attributes_inverse );
}
void IfcPixelTexture::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSurfaceTexture::setInverseCounterparts( ptr_self_entity );
}
void IfcPixelTexture::unlinkSelf()
{
	IfcSurfaceTexture::unlinkSelf();
}
