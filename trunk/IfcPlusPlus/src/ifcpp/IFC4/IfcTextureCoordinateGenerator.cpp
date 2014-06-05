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
#include "include/IfcLabel.h"
#include "include/IfcReal.h"
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTextureCoordinateGenerator.h"

// ENTITY IfcTextureCoordinateGenerator 
IfcTextureCoordinateGenerator::IfcTextureCoordinateGenerator() {}
IfcTextureCoordinateGenerator::IfcTextureCoordinateGenerator( int id ) { m_id = id; }
IfcTextureCoordinateGenerator::~IfcTextureCoordinateGenerator() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTextureCoordinateGenerator::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTextureCoordinateGenerator> other = dynamic_pointer_cast<IfcTextureCoordinateGenerator>(other_entity);
	if( !other) { return; }
	m_Maps = other->m_Maps;
	m_Mode = other->m_Mode;
	m_Parameter = other->m_Parameter;
}
void IfcTextureCoordinateGenerator::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTEXTURECOORDINATEGENERATOR" << "(";
	writeEntityList( stream, m_Maps );
	stream << ",";
	if( m_Mode ) { m_Mode->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeOfRealList( stream, m_Parameter );
	stream << ");";
}
void IfcTextureCoordinateGenerator::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextureCoordinateGenerator::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextureCoordinateGenerator, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcTextureCoordinateGenerator, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Maps, map );
	m_Mode = IfcLabel::createObjectFromStepData( args[1] );
	readTypeOfRealList( args[2], m_Parameter );
}
void IfcTextureCoordinateGenerator::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTextureCoordinate::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Mode", m_Mode ) );
	shared_ptr<IfcPPAttributeObjectVector> Parameter_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Parameter.begin(), m_Parameter.end(), std::back_inserter( Parameter_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Parameter", Parameter_vec_object ) );
}
void IfcTextureCoordinateGenerator::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTextureCoordinateGenerator::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTextureCoordinate::setInverseCounterparts( ptr_self_entity );
}
void IfcTextureCoordinateGenerator::unlinkSelf()
{
	IfcTextureCoordinate::unlinkSelf();
}
