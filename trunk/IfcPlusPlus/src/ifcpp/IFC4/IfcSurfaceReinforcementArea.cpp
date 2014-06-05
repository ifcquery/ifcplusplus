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
#include "include/IfcLengthMeasure.h"
#include "include/IfcRatioMeasure.h"
#include "include/IfcSurfaceReinforcementArea.h"

// ENTITY IfcSurfaceReinforcementArea 
IfcSurfaceReinforcementArea::IfcSurfaceReinforcementArea() {}
IfcSurfaceReinforcementArea::IfcSurfaceReinforcementArea( int id ) { m_id = id; }
IfcSurfaceReinforcementArea::~IfcSurfaceReinforcementArea() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSurfaceReinforcementArea::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSurfaceReinforcementArea> other = dynamic_pointer_cast<IfcSurfaceReinforcementArea>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_SurfaceReinforcement1 = other->m_SurfaceReinforcement1;
	m_SurfaceReinforcement2 = other->m_SurfaceReinforcement2;
	m_ShearReinforcement = other->m_ShearReinforcement;
}
void IfcSurfaceReinforcementArea::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSURFACEREINFORCEMENTAREA" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeOfRealList( stream, m_SurfaceReinforcement1 );
	stream << ",";
	writeTypeOfRealList( stream, m_SurfaceReinforcement2 );
	stream << ",";
	if( m_ShearReinforcement ) { m_ShearReinforcement->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceReinforcementArea::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceReinforcementArea::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSurfaceReinforcementArea, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcSurfaceReinforcementArea, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	readTypeOfRealList( args[1], m_SurfaceReinforcement1 );
	readTypeOfRealList( args[2], m_SurfaceReinforcement2 );
	m_ShearReinforcement = IfcRatioMeasure::createObjectFromStepData( args[3] );
}
void IfcSurfaceReinforcementArea::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoadOrResult::getAttributes( vec_attributes );
	shared_ptr<IfcPPAttributeObjectVector> SurfaceReinforcement1_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_SurfaceReinforcement1.begin(), m_SurfaceReinforcement1.end(), std::back_inserter( SurfaceReinforcement1_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "SurfaceReinforcement1", SurfaceReinforcement1_vec_object ) );
	shared_ptr<IfcPPAttributeObjectVector> SurfaceReinforcement2_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_SurfaceReinforcement2.begin(), m_SurfaceReinforcement2.end(), std::back_inserter( SurfaceReinforcement2_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "SurfaceReinforcement2", SurfaceReinforcement2_vec_object ) );
	vec_attributes.push_back( std::make_pair( "ShearReinforcement", m_ShearReinforcement ) );
}
void IfcSurfaceReinforcementArea::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSurfaceReinforcementArea::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoadOrResult::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceReinforcementArea::unlinkSelf()
{
	IfcStructuralLoadOrResult::unlinkSelf();
}
