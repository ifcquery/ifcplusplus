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

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_set>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/UnknownEntityException.h>
#include <ifcpp/IFC4X3/EntityFactory.h>
#include <IfcBuilding.h>
#include <IfcBuildingStorey.h>
#include <IfcColour.h>
#include <IfcColourOrFactor.h>
#include <IfcColourRgb.h>
#include <IfcCurveStyle.h>
#include <IfcFillAreaStyle.h>
#include <IfcLabel.h>
#include <IfcNormalisedRatioMeasure.h>
#include <IfcPresentationStyle.h>
#include <IfcPresentationStyleAssignment.h>
#include <IfcProject.h>
#include <IfcProductRepresentation.h>
#include <IfcRelAggregates.h>
#include <IfcRelContainedInSpatialStructure.h>
#include <IfcRepresentation.h>
#include <IfcSite.h>
#include <IfcStyledItem.h>
#include <IfcSurfaceStyle.h>
#include <IfcSurfaceStyleElementSelect.h>
#include <IfcSurfaceStyleRendering.h>
#include <IfcTextStyle.h>

#ifdef _MSC_VER
#include <windows.h>
#include <tchar.h>
#endif

#include <external/zippy/zippy.hpp>

#include "ReaderUtil.h"
#include "ReaderSTEP.h"

using namespace IFC4X3;

ReaderSTEP::ReaderSTEP() = default;
ReaderSTEP::~ReaderSTEP() = default;

void ReaderSTEP::loadModelFromFile(const std::string& filePath, shared_ptr<BuildingModel>& targetModel)
{
	// if file content needs to be loaded into a plain model, call resetModel() before loadModelFromFile
	std::string ext = getFileExtension(filePath);
	std::string uncompressedFileName = "";
	std::string filePathRead = filePath;

	if (std_iequal(ext, ".ifc"))
	{
		// ok, nothing to do here
	}
	else if (std_iequal(ext, ".ifcXML"))
	{
		// TODO: implement xml reader
		messageCallback("ifcXML not yet implemented", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		return;
	}
	else if (std_iequal(ext, ".ifcZIP") || std_iequal(ext, ".zip"))
	{
		std::stringstream buffer;
		buffer.seekp(0, std::ios::end);
		std::stringstream::pos_type buffer_size = buffer.tellp();

		uncompressedFileName = filePath + "_uncompressed77334.ifc";

		std::shared_ptr<Zippy::ZipArchive> archive(new Zippy::ZipArchive());
		archive->Open(filePath.c_str());

		std::vector<std::string> entryNames = archive->GetEntryNames();
		if (entryNames.size() == 0)
		{
			return;
		}

		std::string zipcontent = archive->GetEntry(entryNames[0]).GetDataAsString();
		std::ofstream zipFileUncompressed(uncompressedFileName, std::ofstream::out);
		zipFileUncompressed << zipcontent;
		zipFileUncompressed.close();

		std::ifstream infile;
		infile.open(uncompressedFileName.c_str(), std::ifstream::in);

		if (!infile.is_open())
		{
			std::stringstream strs;
			strs << "Could not unzip file: " << filePath;
			messageCallback(strs.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
			return;
		}


		filePathRead = uncompressedFileName;
	}
	else
	{
		std::stringstream strs;
		strs << "Unsupported file type: " << ext;
		messageCallback(strs.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		return;
	}

	// open file
	setlocale(LC_ALL, "");
	std::ifstream infile;
	infile.open(filePathRead.c_str(), std::ifstream::in);

	if (!infile.is_open())
	{
		std::stringstream strs;
		strs << "Could not open file: " << filePathRead.c_str();
		messageCallback(strs.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		return;
	}

	// get length of file content
	infile.imbue(std::locale(""));
	infile.seekg(0, std::ios::end);
	std::streampos file_end_pos = infile.tellg();
	infile.seekg(0, std::ios::beg);

	loadModelFromStream(infile, file_end_pos, targetModel);
	infile.close();

	if (uncompressedFileName.size() > 0)
	{
		try
		{
			std::filesystem::remove(uncompressedFileName);
		}
		catch (std::exception& e)
		{
			std::cout << __FUNCTION__ << ": " << __LINE__ << ": exception: " << e.what() << ", input file: " << uncompressedFileName << std::endl;
		}
	}
}

void ReaderSTEP::loadModelFromStream(std::istream& content, std::streampos file_end_pos, shared_ptr<BuildingModel>& targetModel)
{
	int millisecs_begin = clock();

	readHeader(content, targetModel);

	// currently generated IFC classes are IFC4X3, files with older versions are converted. So after loading, the schema is always IFC4X3
	targetModel->setIfcSchemaVersionEnumCurrent( BuildingModel::IFC4X3 );
	readData(content, file_end_pos, targetModel);
	targetModel->resolveInverseAttributes();
	targetModel->updateCache();

	int millisecs_end = clock();
	double seconds = (millisecs_end - millisecs_begin) * 0.001;
	double progress = 1.0;
	progressValueCallback(progress, "parse");
}

void removeComments(std::string& line)
{
	for (size_t ii = 0; ii < line.size(); ++ii)
	{
		size_t found_comment_start = line.find("/*");
		if (found_comment_start == std::string::npos)
		{
			break;
		}
		size_t found_comment_end = line.find("*/");
		if (found_comment_end != std::string::npos)
		{
			if (found_comment_end > found_comment_start)
			{
				line.erase(found_comment_start, found_comment_end - found_comment_start + 2);
			}
		}
		else
		{
			line.erase(found_comment_start);
		}
	}
}

void ReaderSTEP::readHeader(std::istream& content, shared_ptr<BuildingModel>& target_model)
{
	if (!target_model)
	{
		throw BuildingException("Model not set.", __FUNC__);
	}

	target_model->setFileHeader("");
	target_model->setFileDescription("");
	target_model->setFileName("");

	std::string line;
	std::string strHeader;
	size_t line_header_start = std::string::npos;
	size_t line_header_end = std::string::npos;
	size_t lineCount = 0;
	bool inComment = false;

	while (!bufferedGetline(content, line).eof())
	{
		size_t found_comment_start = line.find("/*");
		if (found_comment_start != std::string::npos)
		{
			inComment = true;
		}

		if (inComment)
		{
			size_t found_comment_end = line.find("*/");
			if (found_comment_end != std::string::npos)
			{
				if (found_comment_start != std::string::npos)
				{
					// comment start and end in same line
					if (found_comment_end > found_comment_start)
					{
						inComment = false;
						line.erase(found_comment_start, found_comment_end - found_comment_start + 2);
						//continue;
					}
					// TODO: remove comment from line, and check rest of line
				}
				else
				{
					inComment = false;
					continue;
				}
			}
		}
		if (inComment)
		{
			continue;
		}

		if (line_header_start == std::string::npos)
		{
			size_t found = line.find("HEADER;");
			if (found != std::string::npos)
			{
				line_header_start = lineCount;
				strHeader += line + '\n';
			}
		}
		else
		{
			if (line_header_end == std::string::npos)
			{
				strHeader += line + '\n';
			}
		}

		if (line_header_end == std::string::npos)
		{
			size_t found = line.find("ENDSEC;");
			if (found != std::string::npos)
			{
				line_header_end = lineCount;
				break;
			}
		}

		++lineCount;
	}

	removeComments(strHeader);
	target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC_VERSION_UNDEFINED);
	std::vector<std::string> vec_header;
	std::vector<std::string> vec_header_str;
	vec_header.emplace_back(strHeader);
	decodeArgumentStrings(vec_header, vec_header_str);

	std::string file_header_str;
	if (!vec_header_str.empty())
	{
		file_header_str = vec_header_str[0];
	}
	target_model->setFileHeader(file_header_str);

	std::vector<std::string> vec_header_lines;
	// split into lines
	const char* stream_pos = &file_header_str[0];
	const char* last_token = stream_pos;

	if (stream_pos == nullptr)
	{
		throw BuildingException("Invalid file content, couldn't find HEADER section", __FUNC__);
	}

	while (*stream_pos != '\0')
	{
		if (*stream_pos == '\'')
		{
			findEndOfString(stream_pos);
			continue;
		}

		if (*stream_pos == ';')
		{
			const char* begin_line = last_token;
			std::string single_step_line(begin_line, stream_pos - last_token);
			vec_header_lines.push_back(single_step_line);

			++stream_pos;
			while (isspace(*stream_pos)) { ++stream_pos; }
			last_token = stream_pos;

			continue;
		}
		++stream_pos;
	}

	for (auto header_line : vec_header_lines)
	{
		if (header_line.find("FILE_DESCRIPTION") != std::string::npos)
		{
			target_model->setFileDescription(header_line);
			continue;
		}

		if (header_line.find("FILE_NAME") != std::string::npos)
		{
			target_model->setFileName(header_line);
			continue;
		}

		if (header_line.find("FILE_SCHEMA") != std::string::npos)
		{
			size_t file_schema_begin = header_line.find("FILE_SCHEMA") + 11;

			std::string file_schema_args = header_line.substr(11);
			size_t find_whitespace = file_schema_args.find(' ');
			while (find_whitespace != std::string::npos) { file_schema_args.erase(find_whitespace, 1); find_whitespace = file_schema_args.find(L' '); }

			if (file_schema_args.empty())
			{
				continue;
			}

			if (file_schema_args.at(0) == '(' && file_schema_args.at(file_schema_args.size() - 1) == ')')
			{
				file_schema_args = file_schema_args.substr(1, file_schema_args.size() - 2);
			}
			if (file_schema_args.at(0) == '(' && file_schema_args.at(file_schema_args.size() - 1) == ')')
			{
				file_schema_args = file_schema_args.substr(1, file_schema_args.size() - 2);
			}
			if (file_schema_args.at(0) == '\'' && file_schema_args.at(file_schema_args.size() - 1) == '\'')
			{
				file_schema_args = file_schema_args.substr(1, file_schema_args.size() - 2);
			}

			convertStringToUpperCase(file_schema_args);

			if (file_schema_args.compare("IFC4X3") == 0)
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC4X3);
			}
			else if (file_schema_args.compare("IFC4X1") == 0)
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC4X1);
			}
			else if (file_schema_args.compare("IFC4") == 0)
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC4);
			}
			else if (file_schema_args.substr(0, 6).compare("IFC2X4") == 0)
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC2X4);
			}
			else if (file_schema_args.substr(0, 6).compare("IFC2X3") == 0)
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC2X3);
			}
			else if (file_schema_args.substr(0, 6).compare("IFC2X2") == 0)
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC2X2);
			}
			else if (file_schema_args.substr(0, 5).compare("IFC2X") == 0)
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC2X);
			}
			else if (file_schema_args.substr(0, 5).compare("IFC20") == 0)
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC2X);
			}
			else
			{
				target_model->setIfcSchemaVersionEnumCurrent(BuildingModel::IFC_VERSION_UNKNOWN);
			}
		}
	}
}

void ReaderSTEP::readSingleStepLine(const std::string& line, std::pair<std::string, shared_ptr<BuildingEntity> >& target_read_object)
{
	if (line.empty())
	{
		return;
	}
	char* stream_pos = const_cast<char*>(line.c_str());
	while (isspace(*stream_pos))
	{
		++stream_pos;
	}

	if (*stream_pos != '#')
	{
		return;
	}

	// need at least one integer here
	++stream_pos;
	if (*stream_pos == '\0')
	{
		return;
	}

	if (!isdigit(*stream_pos))
	{
		return;
	}
	char* begin_id = stream_pos;

	// proceed until end of integer
	++stream_pos;
	while (*stream_pos != '\0')
	{
		if (isdigit(*stream_pos))
		{
			++stream_pos;
		}
		else
		{
			break;
		}
	}

	const int tag = atoi(std::string(begin_id, stream_pos - begin_id).c_str());

	// skip whitespace
	while (isspace(*stream_pos)) { ++stream_pos; }

	// next char after whitespace needs to be an "="
	if (*stream_pos != '=')
	{
		// print error
		return;
	}
	++stream_pos;

	// skip whitespaces
	while (isspace(*stream_pos)) { ++stream_pos; }

	// extract keyword
	const char* entity_name_begin = stream_pos;
	while (isalnum(*stream_pos)) { ++stream_pos; }

	std::string entity_name_upper(entity_name_begin, stream_pos - entity_name_begin);
	convertStringToUpperCase(entity_name_upper);

	// proceed to '('
	if (*stream_pos != '(')
	{
		while (*stream_pos != '\0')
		{
			if (*stream_pos == '(')
			{
				break;
			}
			++stream_pos;
		}
	}

	if (entity_name_upper.empty())
	{
		std::stringstream strs;
		strs << "Could not read STEP line: " << line.c_str();
		messageCallback(strs.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		return;
	}

	shared_ptr<BuildingEntity> obj(EntityFactory::createEntityObject(entity_name_upper));
	if (obj)
	{
		obj->m_tag = tag;
		target_read_object.second = obj;
		size_t sub_length = line.size() - (stream_pos - line.c_str());
		std::string entity_arg(stream_pos, sub_length);
		if (entity_arg.size() > 2)
		{
			if (entity_arg[0] == '(')
			{
				if (entity_arg[entity_arg.size() - 1] == ')')
				{
					// semicolon already removed
					entity_arg = entity_arg.substr(1, entity_arg.size() - 2);
				}
				else if (entity_arg[entity_arg.size() - 1] == ';')
				{
					if (entity_arg[entity_arg.size() - 2] == ')')
					{
						entity_arg = entity_arg.substr(1, entity_arg.size() - 3);
					}
				}
			}
		}
		target_read_object.first.assign(entity_arg.begin(), entity_arg.end());
	}
	else
	{
		throw UnknownEntityException(entity_name_upper);
	}
}

void ReaderSTEP::readEntityArguments(std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > >& vec_entities, const std::unordered_map<int, shared_ptr<BuildingEntity> >& map_entities, shared_ptr<BuildingModel>& model)
{
	// second pass, now read arguments
	// every object can be initialized independently in parallel
	const int num_objects = static_cast<int>(vec_entities.size());
	std::stringstream err;
	std::string ifc_version = model->getIfcSchemaVersionOfLoadedFile();

	// set progress
	double progress = 0.3;
	progressValueCallback(progress, "parse");
	double last_progress = 0.3;
	const std::unordered_map<int, shared_ptr<BuildingEntity> >* map_entities_ptr = &map_entities;
	std::unordered_set<int> entityIdNotFoundAll;

	std::mutex mutexProgress;
	std::mutex mutexError;
	std::mutex mutexEntityIdNotFound;
	int i = 0;

#ifdef _DEBUG
	std::unordered_set<std::string> setClassesWithAdjustedArguments;
#endif
	
	FOR_EACH_LOOP vec_entities.begin(), vec_entities.end(), [&](std::pair<std::string, shared_ptr<BuildingEntity> >& entity_read_object) {
			if (model->isLoadingCancelled())
			{
				return;
			}

			const shared_ptr<BuildingEntity>& entity = entity_read_object.second;
			if (!entity)
			{
				return;
			}
			std::stringstream errorStream;
			std::unordered_set<int> entityIdNotFound;
			std::string& argument_str = entity_read_object.first;
			std::vector<std::string> arguments_raw;
			tokenizeEntityArguments(argument_str, arguments_raw);
			argument_str.clear();

			// character decoding:
			std::vector<std::string> arguments_decoded;
			decodeArgumentStrings(arguments_raw, arguments_decoded);
			arguments_raw.clear();

			const size_t num_expected_arguments = entity->getNumAttributes();
			if (entity->classID() == IFCCOLOURRGB)
			{
				if (arguments_decoded.size() < num_expected_arguments)
				{
					arguments_decoded.insert(arguments_decoded.begin(), "$");
				}
			}
			else if (entity->classID() == IFCPRESENTATIONSTYLEASSIGNMENT)
			{
				if (num_expected_arguments > arguments_decoded.size())
				{
					arguments_decoded.insert(arguments_decoded.begin(), "$");
				}
			}
			else if (entity->classID() == IFCTRIANGULATEDFACESET)
			{
				// IFC4: second argument: Closed : OPTIONAL IfcBoolean;
				// Coordinates : IfcCartesianPointList3D;
				// Normals : OPTIONAL LIST [1:?] OF LIST [3:3] OF IfcParameterValue;
				// Closed : OPTIONAL IfcBoolean;
				// CoordIndex : LIST [1:?] OF LIST [3:3] OF IfcPositiveInteger;
				// PnIndex : OPTIONAL LIST [1:?] OF IfcPositiveInteger;

				// IFC4X3 arguments:
				// Coordinates : IfcCartesianPointList3D;
				// Closed : OPTIONAL IfcBoolean;
				// Normals : OPTIONAL LIST [1:?] OF LIST [3:3] OF IfcParameterValue;
				// CoordIndex : LIST [1:?] OF LIST [3:3] OF IfcPositiveInteger;
				// PnIndex : OPTIONAL LIST [1:?] OF IfcPositiveInteger;

				if (arguments_decoded.size() > 2)
				{
					if (arguments_decoded[2].compare(".T.") == 0 || arguments_decoded[2].compare(".F.") == 0)
					{
						//std::swap(arguments_decoded[2], arguments_decoded[1]);
					}
				}
			}
#ifdef _DEBUG
			if (entity->classID() == IFCRELVOIDSELEMENT)
			{
				int tag = entity->m_tag;
			}
			if (entity->m_tag == 23)
			{
				std::string className = EntityFactory::getStringForClassID(entity->classID());
			}
#endif

			if (num_expected_arguments != arguments_decoded.size())
			{
				while (arguments_decoded.size() > num_expected_arguments) { arguments_decoded.pop_back(); }
				while (arguments_decoded.size() < num_expected_arguments) { arguments_decoded.emplace_back("$"); }
#ifdef _DEBUG
				std::string className = EntityFactory::getStringForClassID(entity->classID());
				if (setClassesWithAdjustedArguments.find(className) == setClassesWithAdjustedArguments.end())
				{
					//std::cout << "adjusted number of arguments for entity #" << entity->m_tag << "=" << className << std::endl;
					setClassesWithAdjustedArguments.insert(className);
				}
#endif
			}

			try
			{
				entity->readStepArguments(arguments_decoded, map_entities, errorStream, entityIdNotFound);
			}
			catch (std::exception& e)
			{
				const std::lock_guard<std::mutex> lock(mutexError);
				err << "#" << entity->m_tag << "=" << EntityFactory::getStringForClassID(entity->classID()) << ": " << e.what();
			}
			catch (std::exception* e)
			{
				const std::lock_guard<std::mutex> lock(mutexError);
				err << "#" << entity->m_tag << "=" << EntityFactory::getStringForClassID(entity->classID()) << ": " << e->what();
			}
			catch (...)
			{
				const std::lock_guard<std::mutex> lock(mutexError);
				err << "#" << entity->m_tag << "=" << EntityFactory::getStringForClassID(entity->classID()) << " readStepData: error occurred" << std::endl;
			}

			// in case there are unresolved references
			if (entityIdNotFound.size() > 0)
			{
				const std::lock_guard<std::mutex> lock(mutexEntityIdNotFound);
				std::copy(entityIdNotFound.begin(), entityIdNotFound.end(), std::inserter(entityIdNotFoundAll, entityIdNotFoundAll.end()));
			}

			if (errorStream.tellp() > 0)
			{
				const std::lock_guard<std::mutex> lock(mutexError);
				err << errorStream.str();
			}

			//if (i % 10 == 0)
			{
				progress = 0.3 + 0.6 * double(i) / double(num_objects);
				if (progress - last_progress > 0.03)
				{
					const std::lock_guard<std::mutex> lock(mutexProgress);
					progressValueCallback(progress, "parse");
					last_progress = progress;
				}
			}

			++i;
		});

	for (auto it = vec_entities.begin(); it != vec_entities.end(); ++it)
	{
		if (model->isLoadingCancelled())
		{
			vec_entities.clear();
			break;
		}

		std::pair<std::string, shared_ptr<BuildingEntity> >& entity_read_object = *it;
		const shared_ptr<BuildingEntity>& entity = entity_read_object.second;

		if (entity->classID() == IFCSTYLEDITEM)
		{
			int tag = entity->m_tag;
			shared_ptr<IfcStyledItem> styledItem = dynamic_pointer_cast<IfcStyledItem>(entity);
			if (styledItem)
			{
				std::vector<shared_ptr<IfcPresentationStyle> >			vec_presentationStylesReplaced;
				for (shared_ptr<IfcPresentationStyle>& presentationStyle : styledItem->m_Styles)
				{
					if (!presentationStyle)
					{
						continue;
					}

					shared_ptr<IfcPresentationStyleAssignment> presentationStyleAssignment = dynamic_pointer_cast<IfcPresentationStyleAssignment>(presentationStyle);
					if (presentationStyleAssignment)
					{
						// IFCPRESENTATIONSTYLEASSIGNMENT has been removed in IFC4X3
						// old:    IfcRepresentationItem  <- IFCSTYLEDITEM ->  IFCPRESENTATIONSTYLEASSIGNMENT -> IFCSURFACESTYLE
						// new      IfcRepresentationItem  <- IFCSTYLEDITEM ->     [x]   ->      IFCSURFACESTYLE

						for (shared_ptr<IfcPresentationStyle>& presentationStyle : presentationStyleAssignment->m_Styles)
						{
							if (!presentationStyle)
							{
								continue;
							}

							//ENTITY IfcPresentationStyle ABSTRACT SUPERTYPE OF (ONEOF (IfcCurveStyle ,IfcFillAreaStyle ,IfcSurfaceStyle ,IfcTextStyle));

							shared_ptr<IfcSurfaceStyle> surfaceStyle = dynamic_pointer_cast<IfcSurfaceStyle>(presentationStyle);
							if (surfaceStyle)
							{
								vec_presentationStylesReplaced.push_back(surfaceStyle);
								continue;
							}

							shared_ptr<IfcCurveStyle> curveStyle = dynamic_pointer_cast<IfcCurveStyle>(presentationStyle);
							if (curveStyle)
							{
								vec_presentationStylesReplaced.push_back(curveStyle);
								continue;
							}

							shared_ptr<IfcFillAreaStyle> fillAreaStyle = dynamic_pointer_cast<IfcFillAreaStyle>(presentationStyle);
							if (fillAreaStyle)
							{
								vec_presentationStylesReplaced.push_back(fillAreaStyle);
								continue;
							}

							shared_ptr<IfcTextStyle> textStyle = dynamic_pointer_cast<IfcTextStyle>(presentationStyle);
							if (textStyle)
							{
								vec_presentationStylesReplaced.push_back(textStyle);
								continue;
							}
						}
						continue;
					}

					vec_presentationStylesReplaced.push_back(presentationStyle);
				}

				styledItem->m_Styles = vec_presentationStylesReplaced;
			}
		}
	}

	if (entityIdNotFoundAll.size() > 0)
	{
		// unresolved entity references
		err << "Entity with id # ";

		for (auto it = entityIdNotFoundAll.begin(); it != entityIdNotFoundAll.end(); ++it)
		{
			if (it != entityIdNotFoundAll.begin())
			{
				err << ", ";
			}
			err << *it;
		}

		err << "  not found" << std::endl;
	}

	if (err.tellp() > 0)
	{
		messageCallback(err.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
	}
}

void ReaderSTEP::readData(std::istream& read_in, std::streampos file_size, shared_ptr<BuildingModel>& model)
{
	std::string current_numeric_locale(setlocale(LC_NUMERIC, nullptr));
	setlocale(LC_NUMERIC, "C");

	if (read_in.peek() == EOF)
	{
		return;
	}
	std::string file_schema_version = model->getIfcSchemaVersionOfLoadedFile();
	messageCallback(std::string("Detected IFC version: ") + file_schema_version, StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, __FUNC__);

	size_t read_size = model->getFileHeader().size();
	std::stringstream err;
	std::unordered_set<std::string> unkown_entities;
	std::stringstream err_unknown_entity;
	std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > > vec_entities;
	try
	{
		std::string line;
		std::string linePreviousRemaining;
		size_t lineCount = 0;
		double progress = 0;
		double last_progress = 0;

		bool hasMoreLines = true;
		while (hasMoreLines)
		{
			if (bufferedGetStepLine(read_in, line).eof())
			{
				hasMoreLines = false;
			}

			read_size += line.size();
			std::pair<std::string, shared_ptr<BuildingEntity> > entity_read_obj;
			try
			{
				readSingleStepLine(line, entity_read_obj);
				if (entity_read_obj.second)
				{
					vec_entities.push_back(entity_read_obj);
					line = "";
				}
			}
			catch (UnknownEntityException& e)
			{
				std::string step_line_fix = line;
				std::string unknown_keyword = e.m_keyword;

				std::map<std::string, std::string > mapFindReplaceTypes;
				mapFindReplaceTypes["IFCBEAMSTANDARDCASE"] = "IFCBEAM";
				mapFindReplaceTypes["IFC2DCOMPOSITECURVE"] = "IFCCOMPOSITECURVE";
				mapFindReplaceTypes["IFCELECTRICDISTRIBUTIONPOINT"] = "IFCFLOWCONTROLLER";
				// IfcElectricDistributionPoint	DELETED   ->  IfcFlowController

				for (auto it : mapFindReplaceTypes)
				{
					const std::string& find1 = it.first;
					const std::string& replace1 = it.second;

					size_t pos1 = step_line_fix.find(find1);
					if (pos1 != std::string::npos)
					{
						step_line_fix.replace(pos1, find1.size(), replace1);
					}
				}

				if (step_line_fix.compare(line) != 0)
				{
					try
					{
						readSingleStepLine(step_line_fix, entity_read_obj);
						if (entity_read_obj.second)
						{
							vec_entities.push_back(entity_read_obj);
							line = "";
						}
						continue;
					}
					catch (UnknownEntityException&)
					{

					}
				}

				if (unkown_entities.find(unknown_keyword) == unkown_entities.end())
				{
					unkown_entities.insert(unknown_keyword);
					err_unknown_entity << "unknown IFC entity: " << unknown_keyword << std::endl;
				}
			}
			catch (std::exception& e)
			{
				err << e.what();
			}

			if (lineCount % 100 == 0)
			{
				progress = 0.05 + 0.2 * double(read_size) / double(file_size);
				if (progress - last_progress > 0.01)
				{
					// TODO read arguments already in parallel thread
					progressValueCallback(progress, "parse");
					last_progress = progress;
				}

				if (model->isLoadingCancelled())
				{
					vec_entities.clear();
					return;
				}
			}

			++lineCount;
		}
	}
	catch (BuildingException& e)
	{
		err << e.what();
	}
	catch (std::exception& e)
	{
		err << e.what();
	}
	catch (...)
	{
		err << __FUNC__ << ": error occurred" << std::endl;
	}

	if (err_unknown_entity.tellp() > 0)
	{
		messageCallback(err_unknown_entity.str(), StatusCallback::MESSAGE_TYPE_UNKNOWN_ENTITY, __FUNC__);
	}

	// copy entities into map so that they can be found during entity attribute initialization
	std::unordered_map<int, shared_ptr<BuildingEntity> >& map_entities = model->getMapIfcEntities();
	for (auto& entity_read_object : vec_entities)
	{
		shared_ptr<BuildingEntity> entity = entity_read_object.second;

		if (entity) // skip aborted entities
		{
			model->insertEntity(entity);
		}
	}

	try
	{
		readEntityArguments(vec_entities, map_entities, model);
	}
	catch (BuildingException& e)
	{
		err << e.what();
	}
	catch (std::exception& e)
	{
		err << e.what();
	}
	catch (...)
	{
		err << __FUNC__ << ": error occurred" << std::endl;
	}

	setlocale(LC_NUMERIC, current_numeric_locale.c_str());
	if (err.tellp() > 0)
	{
		messageCallback(err.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
	}
}
