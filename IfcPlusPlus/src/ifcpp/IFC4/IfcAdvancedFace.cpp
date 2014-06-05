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
#include "include/IfcAdvancedFace.h"
#include "include/IfcFaceBound.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"
#include "include/IfcTextureMap.h"

// ENTITY IfcAdvancedFace 
IfcAdvancedFace::IfcAdvancedFace() {}
IfcAdvancedFace::IfcAdvancedFace( int id ) { m_id = id; }
IfcAdvancedFace::~IfcAdvancedFace() {}

// method setEntity takes over all attributes from another instance of the class
void IfcAdvancedFace::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcAdvancedFace> other = dynamic_pointer_cast<IfcAdvancedFace>(other_entity);
	if( !other) { return; }
	m_Bounds = other->m_Bounds;
	m_FaceSurface = other->m_FaceSurface;
	m_SameSense = other->m_SameSense;
}
void IfcAdvancedFace::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCADVANCEDFACE" << "(";
	writeEntityList( stream, m_Bounds );
	stream << ",";
	if( m_FaceSurface ) { stream << "#" << m_FaceSurface->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_SameSense == false ) { stream << ".F."; }
	else if( m_SameSense == true ) { stream << ".T."; }
	stream << ");";
}
void IfcAdvancedFace::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAdvancedFace::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcAdvancedFace, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcAdvancedFace, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Bounds, map );
	readEntityReference( args[1], m_FaceSurface, map );
	if( _stricmp( args[2].c_str(), ".F." ) == 0 ) { m_SameSense = false; }
	else if( _stricmp( args[2].c_str(), ".T." ) == 0 ) { m_SameSense = true; }
}
void IfcAdvancedFace::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFaceSurface::getAttributes( vec_attributes );
}
void IfcAdvancedFace::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcAdvancedFace::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFaceSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcAdvancedFace::unlinkSelf()
{
	IfcFaceSurface::unlinkSelf();
}
