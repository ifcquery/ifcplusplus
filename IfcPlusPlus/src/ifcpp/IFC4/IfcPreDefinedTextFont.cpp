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
#include "include/IfcPreDefinedTextFont.h"

// ENTITY IfcPreDefinedTextFont 
IfcPreDefinedTextFont::IfcPreDefinedTextFont() {}
IfcPreDefinedTextFont::IfcPreDefinedTextFont( int id ) { m_id = id; }
IfcPreDefinedTextFont::~IfcPreDefinedTextFont() {}
shared_ptr<IfcPPObject> IfcPreDefinedTextFont::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPreDefinedTextFont> copy_self( new IfcPreDefinedTextFont() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPreDefinedTextFont::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPREDEFINEDTEXTFONT" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcPreDefinedTextFont::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPreDefinedTextFont::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPreDefinedTextFont, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
}
void IfcPreDefinedTextFont::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedItem::getAttributes( vec_attributes );
}
void IfcPreDefinedTextFont::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcPreDefinedTextFont::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedItem::setInverseCounterparts( ptr_self_entity );
}
void IfcPreDefinedTextFont::unlinkSelf()
{
	IfcPreDefinedItem::unlinkSelf();
}
