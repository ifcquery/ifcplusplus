/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcBinary.h"
#include "include/IfcBlobTexture.h"
#include "include/IfcBoolean.h"
#include "include/IfcCartesianTransformationOperator2D.h"
#include "include/IfcIdentifier.h"
#include "include/IfcSurfaceStyleWithTextures.h"
#include "include/IfcTextureCoordinate.h"

// ENTITY IfcBlobTexture 
IfcBlobTexture::IfcBlobTexture() { m_entity_enum = IFCBLOBTEXTURE; }
IfcBlobTexture::IfcBlobTexture( int id ) { m_id = id; m_entity_enum = IFCBLOBTEXTURE; }
IfcBlobTexture::~IfcBlobTexture() {}
shared_ptr<IfcPPObject> IfcBlobTexture::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBlobTexture> copy_self( new IfcBlobTexture() );
	if( m_RepeatS ) { copy_self->m_RepeatS = dynamic_pointer_cast<IfcBoolean>( m_RepeatS->getDeepCopy(options) ); }
	if( m_RepeatT ) { copy_self->m_RepeatT = dynamic_pointer_cast<IfcBoolean>( m_RepeatT->getDeepCopy(options) ); }
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
	if( m_RasterFormat ) { copy_self->m_RasterFormat = dynamic_pointer_cast<IfcIdentifier>( m_RasterFormat->getDeepCopy(options) ); }
	if( m_RasterCode ) { copy_self->m_RasterCode = dynamic_pointer_cast<IfcBinary>( m_RasterCode->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBlobTexture::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBLOBTEXTURE" << "(";
	if( m_RepeatS ) { m_RepeatS->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RepeatT ) { m_RepeatT->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Mode ) { m_Mode->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TextureTransform ) { stream << "#" << m_TextureTransform->m_id; } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_Parameter.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcIdentifier>& type_object = m_Parameter[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_RasterFormat ) { m_RasterFormat->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RasterCode ) { m_RasterCode->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcBlobTexture::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBlobTexture::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcBlobTexture, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_RepeatS = IfcBoolean::createObjectFromSTEP( args[0] );
	m_RepeatT = IfcBoolean::createObjectFromSTEP( args[1] );
	m_Mode = IfcIdentifier::createObjectFromSTEP( args[2] );
	readEntityReference( args[3], m_TextureTransform, map );
	readSelectList( args[4], m_Parameter, map );
	m_RasterFormat = IfcIdentifier::createObjectFromSTEP( args[5] );
	m_RasterCode = IfcBinary::createObjectFromSTEP( args[6] );
}
void IfcBlobTexture::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSurfaceTexture::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RasterFormat", m_RasterFormat ) );
	vec_attributes.push_back( std::make_pair( "RasterCode", m_RasterCode ) );
}
void IfcBlobTexture::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSurfaceTexture::getAttributesInverse( vec_attributes_inverse );
}
void IfcBlobTexture::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSurfaceTexture::setInverseCounterparts( ptr_self_entity );
}
void IfcBlobTexture::unlinkFromInverseCounterparts()
{
	IfcSurfaceTexture::unlinkFromInverseCounterparts();
}
