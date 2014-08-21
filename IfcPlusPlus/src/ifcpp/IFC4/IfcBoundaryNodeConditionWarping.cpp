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
#include "include/IfcBoundaryNodeConditionWarping.h"
#include "include/IfcLabel.h"
#include "include/IfcRotationalStiffnessSelect.h"
#include "include/IfcTranslationalStiffnessSelect.h"
#include "include/IfcWarpingStiffnessSelect.h"

// ENTITY IfcBoundaryNodeConditionWarping 
IfcBoundaryNodeConditionWarping::IfcBoundaryNodeConditionWarping() {}
IfcBoundaryNodeConditionWarping::IfcBoundaryNodeConditionWarping( int id ) { m_id = id; }
IfcBoundaryNodeConditionWarping::~IfcBoundaryNodeConditionWarping() {}
shared_ptr<IfcPPObject> IfcBoundaryNodeConditionWarping::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBoundaryNodeConditionWarping> copy_self( new IfcBoundaryNodeConditionWarping() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessX ) { copy_self->m_TranslationalStiffnessX = dynamic_pointer_cast<IfcTranslationalStiffnessSelect>( m_TranslationalStiffnessX->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessY ) { copy_self->m_TranslationalStiffnessY = dynamic_pointer_cast<IfcTranslationalStiffnessSelect>( m_TranslationalStiffnessY->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessZ ) { copy_self->m_TranslationalStiffnessZ = dynamic_pointer_cast<IfcTranslationalStiffnessSelect>( m_TranslationalStiffnessZ->getDeepCopy(options) ); }
	if( m_RotationalStiffnessX ) { copy_self->m_RotationalStiffnessX = dynamic_pointer_cast<IfcRotationalStiffnessSelect>( m_RotationalStiffnessX->getDeepCopy(options) ); }
	if( m_RotationalStiffnessY ) { copy_self->m_RotationalStiffnessY = dynamic_pointer_cast<IfcRotationalStiffnessSelect>( m_RotationalStiffnessY->getDeepCopy(options) ); }
	if( m_RotationalStiffnessZ ) { copy_self->m_RotationalStiffnessZ = dynamic_pointer_cast<IfcRotationalStiffnessSelect>( m_RotationalStiffnessZ->getDeepCopy(options) ); }
	if( m_WarpingStiffness ) { copy_self->m_WarpingStiffness = dynamic_pointer_cast<IfcWarpingStiffnessSelect>( m_WarpingStiffness->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBoundaryNodeConditionWarping::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBOUNDARYNODECONDITIONWARPING" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TranslationalStiffnessX ) { m_TranslationalStiffnessX->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_TranslationalStiffnessY ) { m_TranslationalStiffnessY->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_TranslationalStiffnessZ ) { m_TranslationalStiffnessZ->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_RotationalStiffnessX ) { m_RotationalStiffnessX->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_RotationalStiffnessY ) { m_RotationalStiffnessY->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_RotationalStiffnessZ ) { m_RotationalStiffnessZ->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_WarpingStiffness ) { m_WarpingStiffness->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcBoundaryNodeConditionWarping::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundaryNodeConditionWarping::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBoundaryNodeConditionWarping, expecting 8, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_TranslationalStiffnessX = IfcTranslationalStiffnessSelect::createObjectFromSTEP( args[1], map );
	m_TranslationalStiffnessY = IfcTranslationalStiffnessSelect::createObjectFromSTEP( args[2], map );
	m_TranslationalStiffnessZ = IfcTranslationalStiffnessSelect::createObjectFromSTEP( args[3], map );
	m_RotationalStiffnessX = IfcRotationalStiffnessSelect::createObjectFromSTEP( args[4], map );
	m_RotationalStiffnessY = IfcRotationalStiffnessSelect::createObjectFromSTEP( args[5], map );
	m_RotationalStiffnessZ = IfcRotationalStiffnessSelect::createObjectFromSTEP( args[6], map );
	m_WarpingStiffness = IfcWarpingStiffnessSelect::createObjectFromSTEP( args[7], map );
}
void IfcBoundaryNodeConditionWarping::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundaryNodeCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "WarpingStiffness", m_WarpingStiffness ) );
}
void IfcBoundaryNodeConditionWarping::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundaryNodeCondition::getAttributesInverse( vec_attributes_inverse );
}
void IfcBoundaryNodeConditionWarping::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundaryNodeCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundaryNodeConditionWarping::unlinkSelf()
{
	IfcBoundaryNodeCondition::unlinkSelf();
}
