/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
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

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <locale.h>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPModel.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcProduct.h"
#include "ifcpp/IFC4/include/IfcProject.h"

#include "ifcpp/writer/IfcPPWriterSTEP.h"

IfcPPWriterSTEP::IfcPPWriterSTEP()
{
}
IfcPPWriterSTEP::~IfcPPWriterSTEP()
{
}

void IfcPPWriterSTEP::writeStream( std::stringstream& stream, shared_ptr<IfcPPModel> model )
{
	char* current_numeric_locale = setlocale(LC_NUMERIC, nullptr);
	setlocale(LC_NUMERIC,"C");
	
	const std::wstring& file_header_wstr = model->getFileHeader();
	std::string file_header_str = encodeStepString( file_header_wstr );
	stream << file_header_str.c_str();
	stream << "DATA;\n";
	stream << std::setprecision( 15 );
	const std::map<int,shared_ptr<IfcPPEntity> >& map = model->getMapIfcEntities();
	std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it;
	for( it=map.begin(); it!=map.end(); ++it )
	{
		shared_ptr<IfcPPEntity> obj = it->second;

		if( obj.use_count() < 2 )
		{
			// entity is referenced only in model map, not by other entities
			if( !dynamic_pointer_cast<IfcProduct>(obj) && !dynamic_pointer_cast<IfcProject>(obj) )
			{
				continue;
			}
		}
		obj->getStepLine( stream );
		stream << std::endl;
	}

	stream << "ENDSEC;\nEND-ISO-10303-21;\n";

	setlocale(LC_NUMERIC, current_numeric_locale);
}
