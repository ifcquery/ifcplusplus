/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <clocale>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "ifcpp/model/BuildingModel.h"
#include "ifcpp/writer/WriterUtil.h"
#include "IfcProduct.h"
#include "IfcProject.h"
#include "ifcpp/writer/WriterSTEP.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/IFC4X3/EntityFactory.h"

//#define EXTERNAL_WRITE_METHODS


void WriterSTEP::writeModelToStream( std::stringstream& stream, shared_ptr<BuildingModel> model )
{
	//imbue C locale to always use dots as decimal separator
	stream.imbue(std::locale("C"));

	const std::string& file_header_str = model->getFileHeader();
	stream << "ISO-10303-21;\n";
	stream << file_header_str.c_str();
	stream << "DATA;\n";
	stream << std::setprecision( m_writeNumberPrecision );
	stream << std::setiosflags( std::ios::showpoint );
	stream << std::fixed;
	const std::map<int,shared_ptr<BuildingEntity> >& mapEntities = model->getMapIfcEntities();
	size_t i = 0;
	double last_progress = 0.0;
	double num_objects = double(mapEntities.size());
	for( auto it=mapEntities.begin(); it!=mapEntities.end(); ++it )
	{
		shared_ptr<BuildingEntity> obj = it->second;

		if( obj.use_count() < 2 )
		{
			// entity is referenced only in model map, not by other entities
			if( !dynamic_pointer_cast<IFC4X3::IfcProduct>(obj) && !dynamic_pointer_cast<IFC4X3::IfcProject>(obj) )
			{
				continue;
			}
		}
#ifdef EXTERNAL_WRITE_METHODS
		getStepLine(obj, stream);
#else
		obj->getStepLine( stream, m_writeNumberPrecision );
#endif
		stream << std::endl;

		if( i % 10 == 0 )
		{
			double progress = double( i ) / num_objects;
			if( progress - last_progress > 0.03 )
			{
				progressValueCallback( progress, "write" );
				last_progress = progress;
			}
		}
		++i;
	}

	stream << "ENDSEC;\n";
	stream << "END-ISO-10303-21; \n";
}
