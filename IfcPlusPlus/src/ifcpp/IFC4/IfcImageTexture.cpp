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
#include "include/IfcCartesianTransformationOperator2D.h"
#include "include/IfcIdentifier.h"
#include "include/IfcImageTexture.h"
#include "include/IfcSurfaceStyleWithTextures.h"
#include "include/IfcTextureCoordinate.h"
#include "include/IfcURIReference.h"

// ENTITY IfcImageTexture 
IfcImageTexture::IfcImageTexture() {}
IfcImageTexture::IfcImageTexture( int id ) { m_id = id; }
IfcImageTexture::~IfcImageTexture() {}
shared_ptr<IfcPPObject> IfcImageTexture::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcImageTexture> copy_self( new IfcImageTexture() );
	if( m_RepeatS ) { copy_self->m_RepeatS = m_RepeatS; }
	if( m_RepeatT ) { copy_self->m_RepeatT = m_RepeatT; }
	if( m_Mode ) { copy_self->m_Mode = dynamic_pointer_cast<IfcIdentifier>( m_Mode->getDeepCopy(options) ); }
	if( m_TextureTransform ) { copy_self->m_TextureTransform = dynamic_pointer_cast<IfcCartesianTransformationOperator2D>( m_TextureTransform->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Parameter.size(); ++ii )
	{
		auto item_ii = m_Parameter[ii];
		if( item_ii )
		{
			copy_self->m_Parameter.push_back( dynamic_pointer_cast<IfcIdentifier>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_URLReference ) { copy_self->m_URLReference = dynamic_pointer_cast<IfcURIReference>( m_URLReference->getDeepCopy(options) ); }
	return copy_self;
}
void IfcImageTexture::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCIMAGETEXTURE" << "(";
	if( m_RepeatS == false ) { stream << ".F."; }
	else if( m_RepeatS == true ) { stream << ".T."; }
	stream << ",";
	if( m_RepeatT == false ) { stream << ".F."; }
	else if( m_RepeatT == true ) { stream << ".T."; }
	stream << ",";
	if( m_Mode ) { m_Mode->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TextureTransform ) { stream << "#" << m_TextureTransform->m_id; } else { stream << "*"; }
	stream << ",";
	writeTypeList( stream, m_Parameter );
	stream << ",";
	if( m_URLReference ) { m_URLReference->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcImageTexture::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcImageTexture::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcImageTexture, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	if( boost::iequals( args[0], L".F." ) ) { m_RepeatS = false; }
	else if( boost::iequals( args[0], L".T." ) ) { m_RepeatS = true; }
	if( boost::iequals( args[1], L".F." ) ) { m_RepeatT = false; }
	else if( boost::iequals( args[1], L".T." ) ) { m_RepeatT = true; }
	m_Mode = IfcIdentifier::createObjectFromSTEP( args[2] );
	readEntityReference( args[3], m_TextureTransform, map );
	readSelectList( args[4], m_Parameter, map );
	m_URLReference = IfcURIReference::createObjectFromSTEP( args[5] );
}
void IfcImageTexture::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSurfaceTexture::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "URLReference", m_URLReference ) );
}
void IfcImageTexture::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSurfaceTexture::getAttributesInverse( vec_attributes_inverse );
}
void IfcImageTexture::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSurfaceTexture::setInverseCounterparts( ptr_self_entity );
}
void IfcImageTexture::unlinkSelf()
{
	IfcSurfaceTexture::unlinkSelf();
}
