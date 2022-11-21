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
#include <set>
#include <string>
#include <vector>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/OpenMPIncludes.h>
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
#include <IfcRelAggregates.h>
#include <IfcRelContainedInSpatialStructure.h>
#include <IfcSite.h>
#include <IfcStyledItem.h>
#include <IfcSurfaceStyle.h>
#include <IfcSurfaceStyleElementSelect.h>
#include <IfcSurfaceStyleRendering.h>
#include <IfcTextStyle.h>

#ifdef _MSC_VER
#include <windows.h>
#include <tchar.h>
#include <external/XUnzip.h>
#endif

#include "ifcpp/model/OpenMPIncludes.h"
#include "ReaderUtil.h"
#include "ReaderSTEP.h"

#if defined(_OPENMP)
	#ifndef ENABLE_OPENMP
		#define ENABLE_OPENMP
	#endif
#endif

using namespace IFC4X3;

bool unzipFile(const std::string& filePathIn, std::string& bufferResult)
{
	bool bRet = false;
#ifdef _MSC_VER
	HZIP hZip = OpenZipW((void*)filePathIn.c_str(), 0, ZIP_FILENAME);

	if( hZip )
	{
		ZIPENTRYW ze;
		ZRESULT dRes = GetZipItemW(hZip, -1, &ze);

		if( dRes == ZR_OK )
		{
			int numitems = ze.index;

			for( int zi = 0; zi < numitems && !bRet; zi++ )
			{
				ZIPENTRYW ze;
				dRes = GetZipItemW(hZip, zi, &ze); // fetch individual details

				if( dRes == ZR_OK )
				{
					int nSize = ze.unc_size + 1;
					std::vector<char> vecBuffer;
					vecBuffer.resize(nSize);

					dRes = UnzipItemW(hZip, zi, vecBuffer.data(), nSize, ZIP_MEMORY);

					if( dRes != ZR_OK )
					{
						std::string strBuffer(vecBuffer.begin(), vecBuffer.end());
						size_t file_header_start = strBuffer.find("HEADER;");
						size_t file_header_end = strBuffer.find("ENDSEC;");
						if( file_header_start != std::string::npos && file_header_end != std::string::npos )
						{
							bufferResult = strBuffer;
							bRet = true;
						}
					}
				}
			}
		}
		CloseZip(hZip);
	}
#endif
	return bRet;
}


ReaderSTEP::ReaderSTEP()= default;
ReaderSTEP::~ReaderSTEP()= default;

void ReaderSTEP::loadModelFromFile( const std::string& filePath, shared_ptr<BuildingModel>& targetModel )
{
	// if file content needs to be loaded into a plain model, call resetModel() before loadModelFromFile
	std::string ext = getFileExtension(filePath);
	
	if( std_iequal( ext, ".ifc" ) )
	{
		// ok, nothing to do here
	}
	else if( std_iequal( ext, ".ifcXML" ) )
	{
		// TODO: implement xml reader
		messageCallback( "ifcXML not yet implemented", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		return;
	}
	else if( std_iequal( ext, ".ifcZIP" ) || std_iequal(ext, ".zip") )
	{
		std::string buffer;
		unzipFile(filePath, buffer);
		bool success = unzipFile(filePath, buffer);
		if( success )
		{
			loadModelFromString(buffer, targetModel);
		}
		return;
	}
	else
	{
		std::stringstream strs;
		strs << "Unsupported file type: " << ext;
		messageCallback( strs.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		return;
	}

	// open file
	setlocale(LC_ALL, "");
	std::ifstream infile;
	infile.open(filePath.c_str(), std::ifstream::in);

	if( !infile.is_open() )
	{
		std::stringstream strs;
		strs << "Could not open file: " << filePath.c_str();
		messageCallback( strs.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		return;
	}

	// get length of file content
	infile.imbue(std::locale(""));
	std::streampos file_size = infile.tellg();
	infile.seekg( 0, std::ios::end );
	file_size = infile.tellg() - file_size;
	infile.seekg( 0, std::ios::beg );

	// read file content into string
	std::string buffer( (int)file_size, '\0' );
	infile.read( &buffer[0], file_size );
	infile.close();

	size_t file_header_start = buffer.find("HEADER;");
	size_t file_header_end = buffer.find("ENDSEC;");
	if( file_header_start == std::string::npos || file_header_end == std::string::npos )
	{
		// check if it is a zipped file
		std::string buffer;
		unzipFile(filePath, buffer);
		bool success = unzipFile(filePath, buffer);
		if (success)
		{
			loadModelFromString(buffer, targetModel);
			return;
		}

		messageCallback("Not a valid IFC file", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		return;
	}

	loadModelFromString( buffer, targetModel);
}

void ReaderSTEP::loadModelFromString( std::string& content, shared_ptr<BuildingModel>& targetModel)
{
	progressTextCallback( "Reading file..." );
	progressValueCallback( 0, "parse" );
	try
	{
		removeComments( content );
		readHeader( content, targetModel);
		readData( content, targetModel);
		targetModel->resolveInverseAttributes();
		targetModel->updateCache();

		// currently generated IFC classes are IFC4X1, files with older versions are converted. So after loading, the schema is always IFC4X1
		targetModel->m_ifc_schema_version_current = BuildingModel::IFC4X3;
	}
	catch( BuildingException& e )
	{
		messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
	}
	catch( std::exception& e )
	{
		messageCallback( e.what(), StatusCallback::MESSAGE_TYPE_ERROR, "" );
	}
	catch( ... )
	{
		messageCallback( "An error occurred", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
	}
	progressValueCallback( 1, "parse" );
}

void ReaderSTEP::removeComments( std::string& buffer )
{
	const size_t length = buffer.length();
	size_t length_without_comments = 0;
	bool in_string = false;

	// sort out comments like /* ... */
	char* read_pos = static_cast<char*>(&buffer[0]);
	char* write_pos = static_cast<char*>(&buffer[0]);

	while( *read_pos != '\0' )
	{
		if (*read_pos == '\'')
		{
			in_string = !in_string;
		}

		if (!in_string)
		{
			if (*read_pos == '/')
			{
				if (*(read_pos + 1) == '*')
				{
					// we are inside comment now, proceed to end of comment
					++read_pos;
					while (*read_pos != '\0')
					{
						if (*read_pos == '/')
						{
							if (*(read_pos - 1) == '*')
							{
								++read_pos;
								break;
							}
						}
						++read_pos;
					}
					continue;
				}
			}
		}
		*write_pos = *read_pos;
		++read_pos;
		++write_pos;
		++length_without_comments;
	}
	buffer = buffer.substr( 0, length_without_comments );
}

void ReaderSTEP::readHeader( const std::string& read_in, shared_ptr<BuildingModel>& target_model )
{
	if( !target_model )
	{
		throw BuildingException( "Model not set.", __FUNC__ );
	}

	target_model->m_file_header = "";
	target_model->m_IFC_FILE_DESCRIPTION = "";
	target_model->m_IFC_FILE_NAME = "";

	size_t file_header_start = read_in.find("HEADER;");
	size_t file_header_end = read_in.find("ENDSEC;");
	if( file_header_start == std::string::npos || file_header_end == std::string::npos )
	{
		return;
	}

	target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC_VERSION_UNDEFINED;
	file_header_start += 7;
	std::string file_header = read_in.substr( file_header_start, file_header_end - file_header_start );
	std::vector<std::string> vec_header;
	std::vector<std::string> vec_header_wstr;
	vec_header.emplace_back( file_header );
	decodeArgumentStrings( vec_header, vec_header_wstr );

	std::string file_header_wstr;
	if( !vec_header_wstr.empty() )
	{
		file_header_wstr = vec_header_wstr[0];
	}
	target_model->setFileHeader( file_header_wstr );

	std::vector<std::string> vec_header_lines;
	// split into lines
	const char* stream_pos = &file_header_wstr[0];
	const char* last_token = stream_pos;

	if( stream_pos == nullptr )
	{
		throw BuildingException("Invalid file content, couldn't find HEADER section", __FUNC__);
	}

	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\'' )
		{
			findEndOfString( stream_pos );
			continue;
		}

		if( *stream_pos == ';' )
		{
			const char* begin_line = last_token;
			std::string single_step_line( begin_line, stream_pos-last_token );
			vec_header_lines.push_back( single_step_line );

			++stream_pos;
			while(isspace(*stream_pos)){++stream_pos;}
			last_token = stream_pos;

			continue;
		}
		++stream_pos;
	}

	for(auto header_line : vec_header_lines)
	{
		if( header_line.find("FILE_DESCRIPTION") != std::string::npos )
		{
			target_model->setFileDescription( header_line );
			continue;
		}

		if( header_line.find("FILE_NAME") != std::string::npos )
		{
			target_model->setFileName( header_line );
			continue;
		}

		if( header_line.find("FILE_SCHEMA") != std::string::npos )
		{
			size_t file_schema_begin = header_line.find("FILE_SCHEMA") + 11;

			std::string file_schema_args = header_line.substr( 11 );
			size_t find_whitespace = file_schema_args.find(' ');
			while(find_whitespace != std::string::npos){ file_schema_args.erase(find_whitespace,1); find_whitespace = file_schema_args.find(L' '); }

			if( file_schema_args.empty() )
			{
				continue;
			}

			if( file_schema_args.at(0) =='(' && file_schema_args.at(file_schema_args.size()-1) ==')' )
			{
				file_schema_args = file_schema_args.substr( 1, file_schema_args.size()-2 );
			}
			if( file_schema_args.at(0) =='(' && file_schema_args.at(file_schema_args.size()-1) ==')' )
			{
				file_schema_args = file_schema_args.substr( 1, file_schema_args.size()-2 );
			}
			if( file_schema_args.at(0) =='\'' && file_schema_args.at(file_schema_args.size()-1) =='\'' )
			{
				file_schema_args = file_schema_args.substr( 1, file_schema_args.size()-2 );
			}

			std::transform(file_schema_args.begin(), file_schema_args.end(), file_schema_args.begin(), ::toupper);

			if( file_schema_args.substr(0,6).compare( "IFC2X2") == 0 )
			{
				target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC2X2;
			}
			else if( file_schema_args.substr(0,6).compare( "IFC2X3") == 0 )
			{
				target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC2X3;
			}
			else if( file_schema_args.substr(0,6).compare( "IFC2X4") == 0 )
			{
				target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC2X4;
			}
			else if( file_schema_args.substr( 0, 5 ).compare( "IFC2X" ) == 0 )
			{
				target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC2X;
			}
			else if( file_schema_args.compare( "IFC4" ) == 0 )
			{
				target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC4;
			}
			else if (file_schema_args.compare( "IFC4X1") == 0)
			{
				target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC4X1;
			}
			else if (file_schema_args.compare( "IFC4X3") == 0)
			{
				target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC4X3;
			}
			else
			{
				target_model->m_ifc_schema_version_loaded_file = BuildingModel::IFC_VERSION_UNKNOWN;
			}
		}
	}
}

void ReaderSTEP::splitIntoStepLines(const std::string& read_in, std::vector<std::string>& target_vec)
{
	// set progress to 0
	double progress = 0.0;
	progressValueCallback(progress, "parse");

	// find beginning of data lines
	const size_t length = read_in.length();
	const char* stream_pos = &read_in[0];
	if( stream_pos == nullptr )
	{
		throw BuildingException("Invalid file content", __FUNC__);
	}

	stream_pos = std::strstr(stream_pos, "DATA;");
	if( stream_pos == nullptr )
	{
		throw BuildingException("Invalid file content, couldn't find DATA section", __FUNC__);
	}

	// skip whitespaces
	stream_pos += 5;
	while( isspace(*stream_pos) ){ ++stream_pos; }


	// find the first data line
	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '#' )
		{
			break;
		}
		++stream_pos;
	}

	// split into data lines: #1234=IFCOBJECTNAME(...,...,(...,...),...);
	const char* progress_anchor = stream_pos;
	std::string single_step_line;

	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\r' )
		{
			// omit newlines
			++stream_pos;
			continue;
		}

		if( *( stream_pos )  == '!' )
		{
			const char* stream_pos_next = stream_pos;
			++stream_pos_next;
			if( *stream_pos_next != '\0' )
			{
				if( *stream_pos_next == '\r' || *stream_pos_next == '\n' )
				{
					++stream_pos;  // omit newline exclamation mark
					++stream_pos;  // omit newline
					continue;
				}
			}
		}

		if( *stream_pos == '\n' )
		{
			// omit newlines
			++stream_pos;
			continue;
		}

		if( isspace(*stream_pos) )
		{
			++stream_pos;
			continue;
		}

		if( *stream_pos == '\'' )
		{
			const char* string_start = stream_pos;
			findEndOfString(stream_pos);
			std::string s(string_start, stream_pos - string_start);
			single_step_line += s;
			continue;
		}

		if( *stream_pos == ';' )
		{
			if( single_step_line[0] == '#' )
			{
				target_vec.emplace_back(single_step_line);
				single_step_line = "";
			}

			++stream_pos;
			while( isspace(*stream_pos) ){ ++stream_pos; }

			if( target_vec.size() % 100 == 0 )
			{
				double progress_since_anchor = static_cast<double>( stream_pos - progress_anchor ) / double(length);
				if( progress_since_anchor > 0.03 )
				{
					progress = 0.2*static_cast<double>( stream_pos - &read_in[0] ) / double(length);
					progressValueCallback(progress, "parse");
					progress_anchor = stream_pos;

					if ( isCanceled() )
					{
						canceledCallback();
						return;
					}
				}
			}
			continue;
		}

		single_step_line += *stream_pos;
		++stream_pos;
	}
}

void ReaderSTEP::readStepLines( const std::vector<std::string>& step_lines, std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > >& target_entity_vec )
{
	std::set<std::string> unkown_entities;
	std::stringstream err_unknown_entity;

	double progress = 0.2;
	double last_progress = 0.2;
	const int num_lines = static_cast<int>(step_lines.size());

	target_entity_vec.resize( num_lines );
	std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > >* target_vec_ptr = &target_entity_vec;
	const std::vector<std::string>* step_lines_ptr = &step_lines;
	const std::set<std::string>* unkown_entities_ptr = &unkown_entities;

#ifdef ENABLE_OPENMP
#pragma omp parallel firstprivate(num_lines) shared(target_vec_ptr,unkown_entities_ptr,step_lines_ptr)
	{
		// time for reading a step line does not differ much, so schedule many per thread
#pragma omp for schedule(dynamic,100)
#endif
		for( int i = 0; i<num_lines; ++i )
		{
			const std::string& step_line = (*step_lines_ptr)[i];
			std::pair<std::string, shared_ptr<BuildingEntity> >& entity_read_obj = (*target_vec_ptr)[i];

			// read lines: #1234=IFCOBJECTNAME(...,...,(...,...),...)
			try
			{
				readSingleStepLine( step_line, entity_read_obj );
			}
			catch(UnknownEntityException& e)
			{
				std::string step_line_fix = step_line;
				std::string unknown_keyword = e.m_keyword;

				std::map<std::string, std::string > mapFindReplaceTypes;
 				mapFindReplaceTypes["IFC2DCOMPOSITECURVE"] = "IFCCOMPOSITECURVE";
				mapFindReplaceTypes["IFCELECTRICDISTRIBUTIONPOINT"] = "IFCFLOWCONTROLLER";
				// IfcElectricDistributionPoint	DELETED   ->  IfcFlowController

				for( auto it : mapFindReplaceTypes )
				{
					const std::string& find1 = it.first;
					const std::string& replace1 = it.second;

					size_t pos1 = step_line_fix.find(find1);
					if( pos1 != std::string::npos )
					{
						step_line_fix.replace(pos1, find1.size(), replace1);
					}
				}

				if( step_line_fix.compare( step_line ) != 0 )
				{
					try
					{
						readSingleStepLine( step_line_fix, entity_read_obj );
						continue;
					}
					catch( UnknownEntityException& )
					{

					}
				}

				{
					if( unkown_entities.find( unknown_keyword ) == unkown_entities.end() )
					{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
						{
							unkown_entities.insert( unknown_keyword );
							err_unknown_entity << "unknown IFC entity: " << unknown_keyword << std::endl;
						}
					}
				}
			}

			if( i%10 == 0)
			{
				progress = 0.2 + 0.1*double(i)/double(num_lines);
				if( progress - last_progress > 0.03 )
				{
#ifdef ENABLE_OPENMP
					if( omp_get_thread_num() == 0 )
#endif
					{
						progressValueCallback( progress, "parse" );
						last_progress = progress;
					}
				}
			}
		}
#ifdef ENABLE_OPENMP
	}
#endif

	if( err_unknown_entity.tellp() > 0 )
	{
		shared_ptr<Message> m( new Message() );
		m->m_message_text = err_unknown_entity.str();
		m->m_message_type = StatusCallback::MESSAGE_TYPE_WARNING;
		messageCallback(m);
	}
}

void ReaderSTEP::readSingleStepLine( const std::string& line, std::pair<std::string, shared_ptr<BuildingEntity> >& target_read_object )
{
	if( line.empty() )
	{
		return;
	}
	char* stream_pos = const_cast<char*>(line.c_str());
	if( *stream_pos != '#' )
	{
		return;
	}

	// need at least one integer here
	++stream_pos;
	if( *stream_pos == '\0' )
	{
		return;
	}

	if( !isdigit( *stream_pos ) )
	{
		return;
	}
	char* begin_id = stream_pos;

	// proceed until end of integer
	++stream_pos;
	while( *stream_pos != '\0' )
	{
		if( isdigit( *stream_pos ) )
		{
			++stream_pos;
		}
		else
		{
			break;
		}
	}

	const int tag = atoi( std::string( begin_id, stream_pos - begin_id ).c_str() );

	// skip whitespace
	while( isspace( *stream_pos ) ) { ++stream_pos; }

	// next char after whitespace needs to be an "="
	if( *stream_pos != '=' )
	{
		// print error
		return;
	}
	++stream_pos;

	// skip whitespaces
	while( isspace( *stream_pos ) ) { ++stream_pos; }

	// extract keyword
	const char* entity_name_begin = stream_pos;
	while( isalnum( *stream_pos ) ) { ++stream_pos; }

	std::string entity_name_upper( entity_name_begin, stream_pos - entity_name_begin );
	std::transform(entity_name_upper.begin(), entity_name_upper.end(), entity_name_upper.begin(), ::toupper);

	// proceed to '('
	if( *stream_pos != '(' )
	{
		while( *stream_pos != '\0' )
		{
			if( *stream_pos == '(' )
			{
				break;
			}
			++stream_pos;
		}
	}

	if( entity_name_upper.empty() )
	{
		std::stringstream strs;
		strs << "Could not read STEP line: " << line.c_str();
		messageCallback( strs.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		return;
	}

	shared_ptr<BuildingEntity> obj( EntityFactory::createEntityObject( entity_name_upper ) );
	if( obj )
	{
		obj->m_tag = tag;
		target_read_object.second = obj;
		size_t sub_length = line.size() - (stream_pos - line.c_str());
		std::string entity_arg( stream_pos, sub_length );
		target_read_object.first.assign( entity_arg.begin(), entity_arg.end() );
	}
	else
	{
		throw UnknownEntityException( entity_name_upper );
	}
}

void ReaderSTEP::readEntityArguments( const std::string& ifc_version, const std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > >& vec_entities,  const std::map<int,shared_ptr<BuildingEntity> >& map_entities  )
{
	// second pass, now read arguments
	// every object can be initialized independently in parallel
	const int num_objects = static_cast<int>(vec_entities.size());
	std::stringstream err;

	// set progress
	double progress = 0.3;
	progressValueCallback( progress, "parse" );
	double last_progress = 0.3;
	const std::map<int,shared_ptr<BuildingEntity> >* map_entities_ptr = &map_entities;
	const std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > >* vec_entities_ptr = &vec_entities;
	bool canceled = isCanceled();

#ifdef _DEBUG
	std::set<std::string> setClassesWithAdjustedArguments;
#endif

#ifdef ENABLE_OPENMP
#pragma omp parallel firstprivate(num_objects) shared(map_entities_ptr,vec_entities_ptr)
#endif
	{
		const std::map<int,shared_ptr<BuildingEntity> > &map_entities_ptr_local = *map_entities_ptr;

#ifdef ENABLE_OPENMP
#pragma omp for schedule(dynamic, 100)
#endif
		for( int i=0; i<num_objects; ++i )
		{
			if ( canceled )
			{
				continue;
			}

			const std::pair<std::string, shared_ptr<BuildingEntity> >& entity_read_object = (*vec_entities_ptr)[i];
			const shared_ptr<BuildingEntity>& entity = entity_read_object.second;
			if( !entity )
			{
				continue;
			}
			std::stringstream errorStream;
			const std::string& argument_str = entity_read_object.first;
			std::vector<std::string> arguments_raw;
			tokenizeEntityArguments( argument_str, arguments_raw );

			// character decoding:
			std::vector<std::string> arguments_decoded;
			decodeArgumentStrings( arguments_raw, arguments_decoded );


			const size_t num_expected_arguments = entity->getNumAttributes();
			if( entity->classID() == IFCCOLOURRGB )
			{
				if( arguments_decoded.size() < num_expected_arguments )
				{
					arguments_decoded.insert(arguments_decoded.begin(), "$");
				}
			}
			else if( entity->classID() == IFCPRESENTATIONSTYLEASSIGNMENT )
			{
				if( num_expected_arguments > arguments_decoded.size() )
				{
					arguments_decoded.insert(arguments_decoded.begin(), "$");
				}
			}
			else if( entity->classID() == IFCTRIANGULATEDFACESET )
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

				if( arguments_decoded.size() > 2 )
				{
					if( arguments_decoded[2].compare(".T.") == 0 || arguments_decoded[2].compare(".F.") == 0 )
					{
						std::swap(arguments_decoded[2], arguments_decoded[1]);
					}
				}
			}

			if( num_expected_arguments != arguments_decoded.size() )
			{
				while( arguments_decoded.size() > num_expected_arguments ) { arguments_decoded.pop_back(); }
				while( arguments_decoded.size() < num_expected_arguments ) { arguments_decoded.emplace_back("$"); }
#ifdef _DEBUG
				std::string className = EntityFactory::getStringForClassID(entity->classID());
				if( setClassesWithAdjustedArguments.find(className) == setClassesWithAdjustedArguments.end() )
				{
					//std::cout << "adjusted number of arguments for entity #" << entity->m_tag << "=" << className << std::endl;
					setClassesWithAdjustedArguments.insert(className);
				}
#endif
			}

			try
			{
				entity->readStepArguments(arguments_decoded, map_entities_ptr_local, errorStream);

				if( entity->classID() == IFCSTYLEDITEM )
				{
					int tag = entity->m_tag;
					shared_ptr<IfcStyledItem> styledItem = dynamic_pointer_cast<IfcStyledItem>(entity);
					if( styledItem )
					{
						std::vector<shared_ptr<IfcPresentationStyle> >			vec_presentationStylesReplaced;
						for( shared_ptr<IfcPresentationStyle>& presentationStyle : styledItem->m_Styles )
						{
							if( !presentationStyle )
							{
								continue;
							}

							shared_ptr<IfcPresentationStyleAssignment> presentationStyleAssignment = dynamic_pointer_cast<IfcPresentationStyleAssignment>(presentationStyle);
							if( presentationStyleAssignment )
							{
								// IFCPRESENTATIONSTYLEASSIGNMENT has been removed in IFC4X3
								// old:    IfcRepresentationItem  <- IFCSTYLEDITEM ->  IFCPRESENTATIONSTYLEASSIGNMENT -> IFCSURFACESTYLE
								// new      IfcRepresentationItem  <- IFCSTYLEDITEM ->     [x]   ->      IFCSURFACESTYLE

								for( shared_ptr<IfcPresentationStyle>& presentationStyle : presentationStyleAssignment->m_Styles )
								{
									if( !presentationStyle )
									{
										continue;
									}

									//ENTITY IfcPresentationStyle ABSTRACT SUPERTYPE OF (ONEOF (IfcCurveStyle ,IfcFillAreaStyle ,IfcSurfaceStyle ,IfcTextStyle));

									shared_ptr<IfcSurfaceStyle> surfaceStyle = dynamic_pointer_cast<IfcSurfaceStyle>(presentationStyle);
									if( surfaceStyle )
									{
										vec_presentationStylesReplaced.push_back(surfaceStyle);
										continue;
									}

									shared_ptr<IfcCurveStyle> curveStyle = dynamic_pointer_cast<IfcCurveStyle>(presentationStyle);
									if( curveStyle )
									{
										vec_presentationStylesReplaced.push_back(curveStyle);
										continue;
									}

									shared_ptr<IfcFillAreaStyle> fillAreaStyle = dynamic_pointer_cast<IfcFillAreaStyle>(presentationStyle);
									if( fillAreaStyle )
									{
										vec_presentationStylesReplaced.push_back(fillAreaStyle);
										continue;
									}

									shared_ptr<IfcTextStyle> textStyle = dynamic_pointer_cast<IfcTextStyle>(presentationStyle);
									if( textStyle )
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
			catch( std::exception& e )
			{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_tag << "=" << EntityFactory::getStringForClassID( entity->classID() ) << ": " << e.what();
			}
			catch( std::exception* e )
			{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_tag << "=" << EntityFactory::getStringForClassID( entity->classID() ) << ": " << e->what();
			}
			catch(...)
			{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_tag << "=" << EntityFactory::getStringForClassID( entity->classID() ) << " readStepData: error occurred" << std::endl;
			}

			if( errorStream.tellp() > 0 )
			{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
				err << errorStream.str();
			}

			if( i%10 == 0 )
			{
				progress = 0.3 + 0.6*double(i)/double(num_objects);
				if( progress - last_progress > 0.03 )
				{
#ifdef ENABLE_OPENMP
					if( omp_get_thread_num() == 0 )
#endif
					{
						progressValueCallback( progress, "parse" );
						last_progress = progress;

						if ( isCanceled() )
						{
							canceledCallback();
#ifdef ENABLE_OPENMP
							canceled = true;
#pragma omp flush(canceled)
#else
							break; // If we're not using OpenMP, we can safely break the loop
#endif
						}
					}
				}
			}
		}
	}   // implicic barrier

	if( err.tellp() > 0 )
	{
		messageCallback( err.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
	}
}

void ReaderSTEP::readData( std::string& read_in, shared_ptr<BuildingModel>& model )
{
	std::string file_schema_version = model->getIfcSchemaVersionOfLoadedFile();
	std::map<int, shared_ptr<BuildingEntity> >& map_entities = model->m_map_entities;
	readData( read_in, file_schema_version, map_entities );
}

void ReaderSTEP::readData(	std::string& read_in, const std::string& ifc_version, std::map<int, shared_ptr<BuildingEntity> >& target_map )
{
	std::string current_numeric_locale(setlocale(LC_NUMERIC, nullptr));
	setlocale(LC_NUMERIC,"C");

	if( ifc_version.compare( "IFC_VERSION_UNDEFINED" ) == 0 || ifc_version.compare( "IFC_VERSION_UNKNOWN" ) == 0 )
	{
		std::string error_message;
		error_message.append( "Unsupported IFC version: " );
		error_message.append( ifc_version );
		messageCallback( error_message, StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		progressValueCallback(0.0, "parse");
		return;
	}

	if( read_in.empty() )
	{
		return;
	}
	messageCallback( std::string( "Detected IFC version: ") + ifc_version, StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, "" );

	std::stringstream err;
	std::vector<std::string> step_lines;

	std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > > vec_entities;
	try
	{
		splitIntoStepLines( read_in, step_lines );
		read_in.clear(); // the input string is not needed any more
		const size_t num_lines = step_lines.size();
		vec_entities.resize( num_lines );
		readStepLines( step_lines, vec_entities );
	}
	catch( UnknownEntityException& e )
	{
		std::string unknown_keyword = e.m_keyword;
		err << __FUNC__ << ": unknown entity: " << unknown_keyword.c_str() << std::endl;
	}
	catch( BuildingException& e )
	{
		err << e.what();
	}
	catch(std::exception& e)
	{
		err << e.what();
	}
	catch(...)
	{
		err << __FUNC__ << ": error occurred" << std::endl;
	}
	step_lines.clear();

	// copy entities into map so that they can be found during entity attribute initialization
	for(auto & entity_read_object : vec_entities)
	{
		shared_ptr<BuildingEntity> entity = entity_read_object.second;

		if( entity ) // skip aborted entities
		{
			target_map.insert( std::make_pair( entity->m_tag, entity ) );
		}
	}

	try
	{
		readEntityArguments( ifc_version, vec_entities, target_map );
	}
	catch( BuildingException& e )
	{
		err << e.what();
	}
	catch(std::exception& e)
	{
		err << e.what();
	}
	catch(...)
	{
		err << __FUNC__ << ": error occurred" << std::endl;
	}

	setlocale(LC_NUMERIC,current_numeric_locale.c_str());
	if( err.tellp() > 0 )
	{
		messageCallback( err.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
	}
}
