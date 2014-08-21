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
#include "include/IfcFace.h"
#include "include/IfcOpenShell.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcOpenShell 
IfcOpenShell::IfcOpenShell() {}
IfcOpenShell::IfcOpenShell( int id ) { m_id = id; }
IfcOpenShell::~IfcOpenShell() {}
shared_ptr<IfcPPObject> IfcOpenShell::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcOpenShell> copy_self( new IfcOpenShell() );
	for( size_t ii=0; ii<m_CfsFaces.size(); ++ii )
	{
		auto item_ii = m_CfsFaces[ii];
		if( item_ii )
		{
			copy_self->m_CfsFaces.push_back( dynamic_pointer_cast<IfcFace>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcOpenShell::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCOPENSHELL" << "(";
	writeEntityList( stream, m_CfsFaces );
	stream << ");";
}
void IfcOpenShell::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcOpenShell::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcOpenShell, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_CfsFaces, map );
}
void IfcOpenShell::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConnectedFaceSet::getAttributes( vec_attributes );
}
void IfcOpenShell::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConnectedFaceSet::getAttributesInverse( vec_attributes_inverse );
}
void IfcOpenShell::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConnectedFaceSet::setInverseCounterparts( ptr_self_entity );
}
void IfcOpenShell::unlinkSelf()
{
	IfcConnectedFaceSet::unlinkSelf();
}
