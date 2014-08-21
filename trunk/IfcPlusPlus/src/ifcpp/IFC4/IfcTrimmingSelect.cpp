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
#include "include/IfcParameterValue.h"
#include "include/IfcTrimmingSelect.h"

// TYPE IfcTrimmingSelect = SELECT	(IfcCartesianPoint	,IfcParameterValue);
shared_ptr<IfcTrimmingSelect> IfcTrimmingSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	// read TYPE
	if( arg.size() == 0 ){ return shared_ptr<IfcTrimmingSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcTrimmingSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcTrimmingSelect>();
	}
	shared_ptr<IfcTrimmingSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
