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
#include "include/IfcColourRgbList.h"
#include "include/IfcNormalisedRatioMeasure.h"

// ENTITY IfcColourRgbList 
IfcColourRgbList::IfcColourRgbList() {}
IfcColourRgbList::IfcColourRgbList( int id ) { m_id = id; }
IfcColourRgbList::~IfcColourRgbList() {}
shared_ptr<IfcPPObject> IfcColourRgbList::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcColourRgbList> copy_self( new IfcColourRgbList() );
	copy_self->m_ColourList.resize( m_ColourList.size() );
	for( size_t ii=0; ii<m_ColourList.size(); ++ii )
	{
		std::vector<shared_ptr<IfcNormalisedRatioMeasure> >& vec_ii = m_ColourList[ii];
		std::vector<shared_ptr<IfcNormalisedRatioMeasure> >& vec_ii_target = copy_self->m_ColourList[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcNormalisedRatioMeasure>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcNormalisedRatioMeasure>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	return copy_self;
}
void IfcColourRgbList::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOLOURRGBLIST" << "(";
	writeTypeList2D( stream, m_ColourList );
	stream << ");";
}
void IfcColourRgbList::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcColourRgbList::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcColourRgbList, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList2D( args[0], m_ColourList, map );
}
void IfcColourRgbList::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
}
void IfcColourRgbList::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcColourRgbList::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcColourRgbList::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
