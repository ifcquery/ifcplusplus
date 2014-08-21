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
#include "include/IfcLabel.h"
#include "include/IfcReal.h"
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTextureCoordinateGenerator.h"

// ENTITY IfcTextureCoordinateGenerator 
IfcTextureCoordinateGenerator::IfcTextureCoordinateGenerator() {}
IfcTextureCoordinateGenerator::IfcTextureCoordinateGenerator( int id ) { m_id = id; }
IfcTextureCoordinateGenerator::~IfcTextureCoordinateGenerator() {}
shared_ptr<IfcPPObject> IfcTextureCoordinateGenerator::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextureCoordinateGenerator> copy_self( new IfcTextureCoordinateGenerator() );
	for( size_t ii=0; ii<m_Maps.size(); ++ii )
	{
		auto item_ii = m_Maps[ii];
		if( item_ii )
		{
			copy_self->m_Maps.push_back( dynamic_pointer_cast<IfcSurfaceTexture>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Mode ) { copy_self->m_Mode = dynamic_pointer_cast<IfcLabel>( m_Mode->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Parameter.size(); ++ii )
	{
		auto item_ii = m_Parameter[ii];
		if( item_ii )
		{
			copy_self->m_Parameter.push_back( dynamic_pointer_cast<IfcReal>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTextureCoordinateGenerator::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTURECOORDINATEGENERATOR" << "(";
	writeEntityList( stream, m_Maps );
	stream << ",";
	if( m_Mode ) { m_Mode->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeOfRealList( stream, m_Parameter );
	stream << ");";
}
void IfcTextureCoordinateGenerator::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextureCoordinateGenerator::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextureCoordinateGenerator, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_Maps, map );
	m_Mode = IfcLabel::createObjectFromSTEP( args[1] );
	readTypeOfRealList( args[2], m_Parameter );
}
void IfcTextureCoordinateGenerator::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTextureCoordinate::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Mode", m_Mode ) );
	if( m_Parameter.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Parameter_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Parameter.begin(), m_Parameter.end(), std::back_inserter( Parameter_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Parameter", Parameter_vec_object ) );
	}
}
void IfcTextureCoordinateGenerator::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTextureCoordinate::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextureCoordinateGenerator::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTextureCoordinate::setInverseCounterparts( ptr_self_entity );
}
void IfcTextureCoordinateGenerator::unlinkSelf()
{
	IfcTextureCoordinate::unlinkSelf();
}
