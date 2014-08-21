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
#include "include/IfcDraughtingPreDefinedColour.h"
#include "include/IfcLabel.h"

// ENTITY IfcDraughtingPreDefinedColour 
IfcDraughtingPreDefinedColour::IfcDraughtingPreDefinedColour() {}
IfcDraughtingPreDefinedColour::IfcDraughtingPreDefinedColour( int id ) { m_id = id; }
IfcDraughtingPreDefinedColour::~IfcDraughtingPreDefinedColour() {}
shared_ptr<IfcPPObject> IfcDraughtingPreDefinedColour::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDraughtingPreDefinedColour> copy_self( new IfcDraughtingPreDefinedColour() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcDraughtingPreDefinedColour::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDRAUGHTINGPREDEFINEDCOLOUR" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcDraughtingPreDefinedColour::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDraughtingPreDefinedColour::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDraughtingPreDefinedColour, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
}
void IfcDraughtingPreDefinedColour::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedColour::getAttributes( vec_attributes );
}
void IfcDraughtingPreDefinedColour::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedColour::getAttributesInverse( vec_attributes_inverse );
}
void IfcDraughtingPreDefinedColour::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedColour::setInverseCounterparts( ptr_self_entity );
}
void IfcDraughtingPreDefinedColour::unlinkSelf()
{
	IfcPreDefinedColour::unlinkSelf();
}
