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
#include <map>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "include/IfcBoolean.h"
#include "include/IfcRotationalStiffnessMeasure.h"
#include "include/IfcRotationalStiffnessSelect.h"

// TYPE IfcRotationalStiffnessSelect = SELECT	(IfcBoolean	,IfcRotationalStiffnessMeasure);
shared_ptr<IfcRotationalStiffnessSelect> IfcRotationalStiffnessSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	// read TYPE
	if( arg.size() == 0 ){ return shared_ptr<IfcRotationalStiffnessSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcRotationalStiffnessSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcRotationalStiffnessSelect>();
	}
	shared_ptr<IfcRotationalStiffnessSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
