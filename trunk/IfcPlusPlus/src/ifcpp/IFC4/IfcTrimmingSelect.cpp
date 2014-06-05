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

// TYPE IfcTrimmingSelect 
IfcTrimmingSelect::IfcTrimmingSelect() {}
IfcTrimmingSelect::~IfcTrimmingSelect() {}
shared_ptr<IfcTrimmingSelect> IfcTrimmingSelect::createObjectFromStepData( const std::string& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	// Read SELECT TYPE
	if( arg.size() == 0 ){ return shared_ptr<IfcTrimmingSelect>(); }
	if( arg[0] == '#' )
	{
		int id=atoi( arg.substr(1,arg.length()-1).c_str() );
		std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it_entity = map.find( id );
		if( it_entity != map.end() )
		{
			shared_ptr<IfcTrimmingSelect> type_object = dynamic_pointer_cast<IfcTrimmingSelect>(it_entity->second);
			return type_object;
		}
		else
		{
			std::stringstream strs;
			strs << "Object width id " << id << " not found";
			throw IfcPPException( strs.str() );
		}
	}
	else if( arg.compare("$")==0 )
	{
		return shared_ptr<IfcTrimmingSelect>();
	}
	else if( arg.compare("*")==0 )
	{
		return shared_ptr<IfcTrimmingSelect>();
	}
	else
	{
		// inline arguments
		std::string keyword;
		std::string inline_arg;
		tokenizeInlineArgument( arg, keyword, inline_arg );
		shared_ptr<IfcPPObject> result_object;
		readInlineTypeOrEntity( keyword, inline_arg, result_object, map );
		if( result_object )
		{
			shared_ptr<IfcPPObject> result_ptr( result_object );
			shared_ptr<IfcTrimmingSelect> result_ptr_self = dynamic_pointer_cast<IfcTrimmingSelect>( result_ptr );
			if( result_ptr_self )
			{
				return result_ptr_self;
			}
		}
		std::stringstream strs;
		strs << "unhandled inline argument: " << arg << " in function IfcTrimmingSelect::readStepData" << std::endl;
		throw IfcPPException( strs.str() );
	}
	return shared_ptr<IfcTrimmingSelect>();
}
