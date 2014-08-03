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
#include "include/IfcMaterial.h"
#include "include/IfcMaterialList.h"

// ENTITY IfcMaterialList 
IfcMaterialList::IfcMaterialList() {}
IfcMaterialList::IfcMaterialList( int id ) { m_id = id; }
IfcMaterialList::~IfcMaterialList() {}
shared_ptr<IfcPPObject> IfcMaterialList::getDeepCopy()
{
	shared_ptr<IfcMaterialList> copy_self( new IfcMaterialList() );
	for( size_t ii=0; ii<m_Materials.size(); ++ii )
	{
		auto item_ii = m_Materials[ii];
		if( item_ii )
		{
			copy_self->m_Materials.push_back( dynamic_pointer_cast<IfcMaterial>(item_ii->getDeepCopy() ) );
		}
	}
	return copy_self;
}
void IfcMaterialList::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALLIST" << "(";
	writeEntityList( stream, m_Materials );
	stream << ");";
}
void IfcMaterialList::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialList::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialList, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcMaterialList, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Materials, map );
}
void IfcMaterialList::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_Materials.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Materials_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Materials.begin(), m_Materials.end(), std::back_inserter( Materials_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Materials", Materials_vec_object ) );
	}
}
void IfcMaterialList::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialList::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMaterialList::unlinkSelf()
{
}
