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
#include "include/IfcCartesianPointList3D.h"
#include "include/IfcIndexedColourMap.h"
#include "include/IfcIndexedTextureMap.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTriangulatedFaceSet.h"

// ENTITY IfcTriangulatedFaceSet 
IfcTriangulatedFaceSet::IfcTriangulatedFaceSet() {}
IfcTriangulatedFaceSet::IfcTriangulatedFaceSet( int id ) { m_id = id; }
IfcTriangulatedFaceSet::~IfcTriangulatedFaceSet() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTriangulatedFaceSet::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTriangulatedFaceSet> other = dynamic_pointer_cast<IfcTriangulatedFaceSet>(other_entity);
	if( !other) { return; }
	m_Coordinates = other->m_Coordinates;
	m_Normals = other->m_Normals;
	m_Closed = other->m_Closed;
	m_CoordIndex = other->m_CoordIndex;
	m_NormalIndex = other->m_NormalIndex;
}
void IfcTriangulatedFaceSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTRIANGULATEDFACESET" << "(";
	if( m_Coordinates ) { stream << "#" << m_Coordinates->getId(); } else { stream << "$"; }
	stream << ",";
	writeTypeOfRealList2D( stream, m_Normals );
	stream << ",";
	if( m_Closed == false ) { stream << ".F."; }
	else if( m_Closed == true ) { stream << ".T."; }
	stream << ",";
	writeIntList2D( stream, m_CoordIndex );
	stream << ",";
	writeIntList2D( stream, m_NormalIndex );
	stream << ");";
}
void IfcTriangulatedFaceSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTriangulatedFaceSet::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTriangulatedFaceSet, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcTriangulatedFaceSet, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Coordinates, map );
	readTypeOfRealList2D( args[1], m_Normals );
	if( _stricmp( args[2].c_str(), ".F." ) == 0 ) { m_Closed = false; }
	else if( _stricmp( args[2].c_str(), ".T." ) == 0 ) { m_Closed = true; }
	readIntList2D(  args[3], m_CoordIndex );
	readIntList2D(  args[4], m_NormalIndex );
}
void IfcTriangulatedFaceSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTessellatedFaceSet::getAttributes( vec_attributes );
}
void IfcTriangulatedFaceSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTriangulatedFaceSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTessellatedFaceSet::setInverseCounterparts( ptr_self_entity );
}
void IfcTriangulatedFaceSet::unlinkSelf()
{
	IfcTessellatedFaceSet::unlinkSelf();
}
