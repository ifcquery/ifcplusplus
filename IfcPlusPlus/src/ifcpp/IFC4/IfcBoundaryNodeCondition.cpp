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
#include "include/IfcBoundaryNodeCondition.h"
#include "include/IfcLabel.h"
#include "include/IfcRotationalStiffnessSelect.h"
#include "include/IfcTranslationalStiffnessSelect.h"

// ENTITY IfcBoundaryNodeCondition 
IfcBoundaryNodeCondition::IfcBoundaryNodeCondition() {}
IfcBoundaryNodeCondition::IfcBoundaryNodeCondition( int id ) { m_id = id; }
IfcBoundaryNodeCondition::~IfcBoundaryNodeCondition() {}
shared_ptr<IfcPPObject> IfcBoundaryNodeCondition::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBoundaryNodeCondition> copy_self( new IfcBoundaryNodeCondition() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessX ) { copy_self->m_TranslationalStiffnessX = dynamic_pointer_cast<IfcTranslationalStiffnessSelect>( m_TranslationalStiffnessX->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessY ) { copy_self->m_TranslationalStiffnessY = dynamic_pointer_cast<IfcTranslationalStiffnessSelect>( m_TranslationalStiffnessY->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessZ ) { copy_self->m_TranslationalStiffnessZ = dynamic_pointer_cast<IfcTranslationalStiffnessSelect>( m_TranslationalStiffnessZ->getDeepCopy(options) ); }
	if( m_RotationalStiffnessX ) { copy_self->m_RotationalStiffnessX = dynamic_pointer_cast<IfcRotationalStiffnessSelect>( m_RotationalStiffnessX->getDeepCopy(options) ); }
	if( m_RotationalStiffnessY ) { copy_self->m_RotationalStiffnessY = dynamic_pointer_cast<IfcRotationalStiffnessSelect>( m_RotationalStiffnessY->getDeepCopy(options) ); }
	if( m_RotationalStiffnessZ ) { copy_self->m_RotationalStiffnessZ = dynamic_pointer_cast<IfcRotationalStiffnessSelect>( m_RotationalStiffnessZ->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBoundaryNodeCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBOUNDARYNODECONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TranslationalStiffnessX ) { m_TranslationalStiffnessX->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TranslationalStiffnessY ) { m_TranslationalStiffnessY->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TranslationalStiffnessZ ) { m_TranslationalStiffnessZ->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_RotationalStiffnessX ) { m_RotationalStiffnessX->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_RotationalStiffnessY ) { m_RotationalStiffnessY->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_RotationalStiffnessZ ) { m_RotationalStiffnessZ->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcBoundaryNodeCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundaryNodeCondition::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBoundaryNodeCondition, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_TranslationalStiffnessX = IfcTranslationalStiffnessSelect::createObjectFromSTEP( args[1], map );
	m_TranslationalStiffnessY = IfcTranslationalStiffnessSelect::createObjectFromSTEP( args[2], map );
	m_TranslationalStiffnessZ = IfcTranslationalStiffnessSelect::createObjectFromSTEP( args[3], map );
	m_RotationalStiffnessX = IfcRotationalStiffnessSelect::createObjectFromSTEP( args[4], map );
	m_RotationalStiffnessY = IfcRotationalStiffnessSelect::createObjectFromSTEP( args[5], map );
	m_RotationalStiffnessZ = IfcRotationalStiffnessSelect::createObjectFromSTEP( args[6], map );
}
void IfcBoundaryNodeCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundaryCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessX", m_TranslationalStiffnessX ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessY", m_TranslationalStiffnessY ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessZ", m_TranslationalStiffnessZ ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessX", m_RotationalStiffnessX ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessY", m_RotationalStiffnessY ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessZ", m_RotationalStiffnessZ ) );
}
void IfcBoundaryNodeCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundaryCondition::getAttributesInverse( vec_attributes_inverse );
}
void IfcBoundaryNodeCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundaryCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundaryNodeCondition::unlinkSelf()
{
	IfcBoundaryCondition::unlinkSelf();
}
