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
#include "include/IfcFaceOuterBound.h"
#include "include/IfcLoop.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcFaceOuterBound 
IfcFaceOuterBound::IfcFaceOuterBound() {}
IfcFaceOuterBound::IfcFaceOuterBound( int id ) { m_id = id; }
IfcFaceOuterBound::~IfcFaceOuterBound() {}
shared_ptr<IfcPPObject> IfcFaceOuterBound::getDeepCopy()
{
	shared_ptr<IfcFaceOuterBound> copy_self( new IfcFaceOuterBound() );
	if( m_Bound ) { copy_self->m_Bound = dynamic_pointer_cast<IfcLoop>( m_Bound->getDeepCopy() ); }
	if( m_Orientation ) { copy_self->m_Orientation = m_Orientation; }
	return copy_self;
}
void IfcFaceOuterBound::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCFACEOUTERBOUND" << "(";
	if( m_Bound ) { stream << "#" << m_Bound->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_Orientation == false ) { stream << ".F."; }
	else if( m_Orientation == true ) { stream << ".T."; }
	stream << ");";
}
void IfcFaceOuterBound::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFaceOuterBound::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFaceOuterBound, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcFaceOuterBound, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Bound, map );
	if( boost::iequals( args[1], L".F." ) ) { m_Orientation = false; }
	else if( boost::iequals( args[1], L".T." ) ) { m_Orientation = true; }
}
void IfcFaceOuterBound::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFaceBound::getAttributes( vec_attributes );
}
void IfcFaceOuterBound::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcFaceBound::getAttributesInverse( vec_attributes_inverse );
}
void IfcFaceOuterBound::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFaceBound::setInverseCounterparts( ptr_self_entity );
}
void IfcFaceOuterBound::unlinkSelf()
{
	IfcFaceBound::unlinkSelf();
}
