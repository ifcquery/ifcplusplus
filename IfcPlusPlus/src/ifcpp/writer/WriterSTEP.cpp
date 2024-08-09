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
#include <algorithm>

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

void WriterSTEP::writeModelToStream(std::stringstream& stream, shared_ptr<BuildingModel> model)
{
	//imbue C locale to always use dots as decimal separator
	stream.imbue(std::locale("C"));

	const std::string& file_header_str = model->getFileHeader();
	if(file_header_str.size() == 0)
	{
		std::string applicationName = "IfcPlusPlus";
		model->initFileHeader("fileName.ifc", applicationName);
	}
	stream << "ISO-10303-21;\n";
	stream << file_header_str.c_str();
	stream << "DATA;\n";
	stream << std::setprecision(m_writeNumberPrecision);
	stream << std::setiosflags(std::ios::showpoint);
	stream << std::fixed;

	const std::unordered_map<int, shared_ptr<BuildingEntity> >& mapEntities = model->getMapIfcEntities();
	std::map<int, shared_ptr<BuildingEntity> > mapEntitiesAscendingTags;
	for (auto it : mapEntities)
	{
		mapEntitiesAscendingTags.insert(it);
	}
	std::vector<std::tuple<int, shared_ptr<BuildingEntity>, std::string>> entityDataStrings;
	for (auto entity : mapEntitiesAscendingTags)
	{
		entityDataStrings.push_back(std::tuple<int, shared_ptr<BuildingEntity>, std::string>(entity.first, entity.second, ""));
	}

	std::mutex mutexProgress;
	auto t_start = std::chrono::high_resolution_clock::now();
	std::atomic<int> counter = 0;
	size_t numEntities = entityDataStrings.size();
	FOR_EACH_LOOP entityDataStrings.begin(), entityDataStrings.end(), [&, this](std::tuple<int, shared_ptr<BuildingEntity>, std::string>& entityDataForOutput) {
		shared_ptr<BuildingEntity> obj = std::get<1>(entityDataForOutput);
		if (obj.use_count() < 2)
		{
			// entity is referenced only in model map, not by other entities
			if (!dynamic_pointer_cast<IFC4X3::IfcProduct>(obj) && !dynamic_pointer_cast<IFC4X3::IfcProject>(obj))
			{
				return;
			}
		}
		std::stringstream tmpStream;
		tmpStream.imbue(std::locale("C"));
#ifdef EXTERNAL_WRITE_METHODS
		getStepLine(obj, tmpStream);
#else
		obj->getStepLine(tmpStream, m_writeNumberPrecision);
#endif
		tmpStream << std::endl;
		std::get<2>(entityDataForOutput) = tmpStream.str();

		counter.fetch_add(1);
		int currentCount = counter.load();
		if (currentCount % 60 == 0)
		{
			auto t_now = std::chrono::high_resolution_clock::now();
			double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_now - t_start).count();

			if (elapsed_time_ms > 1000)
			{
				t_start = t_now;
				const std::lock_guard<std::mutex> lock(mutexProgress);
				double progress = 0.1 + 0.8 * (currentCount / double(numEntities));
				progressValueCallback(progress, "parse");
			}
		}
		});

	for (auto line : entityDataStrings)
	{
		stream << std::get<2>(line);
	}

	stream << "ENDSEC;\n";
	stream << "END-ISO-10303-21; \n";
}
