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

#include <set>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/OpenMPIncludes.h>
#include <ifcpp/model/UnknownEntityException.h>
#include <ifcpp/IFC4/EntityFactory.h>
#include <ifcpp/IFC4/include/IfcBuilding.h>
#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcSite.h>

#include "ReaderUtil.h"
#include "ReaderSTEP.h"

ReaderSTEP::ReaderSTEP()= default;
ReaderSTEP::~ReaderSTEP()= default;

void ReaderSTEP::loadModelFromFile( const std::wstring& filePath, shared_ptr<BuildingModel>& targetModel )
{
	// if file content needs to be loaded into a plain model, call resetModel() before loadModelFromFile
	size_t posDot = filePath.find_last_of(L".");
	if( filePath.size() < posDot + 3 || posDot > filePath.size() )
	{
		messageCallback("not an .ifc file", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		return;
	}
	std::wstring ext = filePath.substr(posDot + 1);
	
	if( boost::iequals( ext, "ifc" ) )
	{
		// ok, nothing to do here
	}
	else if( boost::iequals( ext, "ifcXML" ) )
	{
		// TODO: implement xml reader
		messageCallback( "ifcXML not yet implemented", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		return;
	}
	else if( boost::iequals( ext, "ifcZIP" ) || boost::iequals(ext, "zip") )
	{
		messageCallback( "ifcZIP not implemented, see www.ifcquery.com for details", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		return;
	}
	else
	{
		std::wstringstream strs;
		strs << "Unsupported file type: " << ext;
		messageCallback( strs.str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		return;
	}

	// open file
	if( !(setlocale(LC_ALL, "en_us.UTF-8") ||  setlocale(LC_ALL, "en_US.utf8")))
	{
		std::wstringstream strs;
		strs << L"setlocale failed" << std::endl;
		messageCallback(strs.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		return;
	}

	char* buf = nullptr;
	size_t len = std::wcstombs(buf, filePath.c_str(), 0);
	buf = new char[len + 1];
	std::wcstombs(buf, filePath.c_str(), (len + 1) * 6);
	std::string filePathStr(buf);
	delete[] buf;
	std::ifstream infile(filePathStr.c_str(), std::ifstream::in);
	
	if( !infile.is_open() )
	{
		std::wstringstream strs;
		strs << "Could not open file: " << filePath.c_str();
		messageCallback( strs.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		return;
	}

	// get length of file content
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
		messageCallback("Not a valid IFC file, might be zip archive, see www.ifcquery.com for details", StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__);
		return;
	}

	loadModelFromString( buffer, targetModel);
}

void ReaderSTEP::loadModelFromString( std::string& content, shared_ptr<BuildingModel>& targetModel)
{
	progressTextCallback( L"Reading file..." );
	progressValueCallback( 0, "parse" );
	try
	{
		removeComments( content );
		readHeader( content, targetModel);
		readData( content, targetModel);
		targetModel->resolveInverseAttributes();
		targetModel->updateCache();

		// currently generated IFC classes are IFC4, files with older versions are converted. So after loading, the schema is always IFC4
		targetModel->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"IFC4";
		targetModel->getIfcSchemaVersion().m_ifc_file_schema_enum = BuildingModel::IFC4;
	}
	catch( OutOfMemoryException& e)
	{
		throw e;
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
}

void ReaderSTEP::removeComments( std::string& buffer )
{
	const size_t length = buffer.length();
	size_t length_without_comments = 0;

	// sort out comments like /* ... */
	char* read_pos = static_cast<char*>(&buffer[0]);
	char* write_pos = static_cast<char*>(&buffer[0]);

	while( *read_pos != '\0' )
	{
		if( *read_pos == '/' )
		{
			if( *( read_pos + 1 ) == '*' )
			{
				// we are inside comment now, proceed to end of comment
				++read_pos;
				while( *read_pos != '\0' )
				{
					if( *read_pos == '/' )
					{
						if( *( read_pos - 1 ) == '*' )
						{
							break;
						}
					}
					++read_pos;
				}
				++read_pos;
				continue;
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

	target_model->m_file_header = L"";
	target_model->m_IFC_FILE_DESCRIPTION = L"";
	target_model->m_IFC_FILE_NAME = L"";
	
	size_t file_header_start = read_in.find("HEADER;");
	size_t file_header_end = read_in.find("ENDSEC;");
	if( file_header_start == std::string::npos || file_header_end == std::string::npos )
	{
		return;
	}

	target_model->getIfcSchemaVersion().m_ifc_file_schema_enum = BuildingModel::IFC_VERSION_UNDEFINED;
	file_header_start += 7;
	std::string file_header = read_in.substr( file_header_start, file_header_end - file_header_start );
	std::vector<std::string> vec_header;
	std::vector<std::wstring> vec_header_wstr;
	vec_header.emplace_back( file_header );
	decodeArgumentStrings( vec_header, vec_header_wstr );
	
	std::wstring file_header_wstr;
	if( !vec_header_wstr.empty() )
	{
		file_header_wstr = vec_header_wstr[0];
	}
	target_model->setFileHeader( file_header_wstr );
	
	std::vector<std::wstring> vec_header_lines;
	// split into lines
	wchar_t* stream_pos = &file_header_wstr[0];
	wchar_t* last_token = stream_pos;

	if( stream_pos == nullptr )
	{
		throw BuildingException("Invalid file content, couldn't find HEADER section", __FUNC__);
	}

	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\'' )
		{
			findEndOfWString( stream_pos );
			continue;
		}

		if( *stream_pos == ';' )
		{
			wchar_t* begin_line = last_token;
			std::wstring single_step_line( begin_line, stream_pos-last_token );
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
		if( header_line.find(L"FILE_DESCRIPTION") != std::string::npos )
		{
			target_model->setFileDescription( header_line );
			continue;
		}

		if( header_line.find(L"FILE_NAME") != std::string::npos )
		{
			target_model->setFileName( header_line );
			continue;
		}

		if( header_line.find(L"FILE_SCHEMA") != std::string::npos )
		{
			size_t file_schema_begin = header_line.find(L"FILE_SCHEMA") + 11;

			std::wstring file_schema_args = header_line.substr( 11 );
			size_t find_whitespace = file_schema_args.find(L' ');
			while(find_whitespace != std::string::npos){ file_schema_args.erase(find_whitespace,1); find_whitespace = file_schema_args.find(L' '); }
			
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
			
			if( file_schema_args.substr(0,6).compare(L"IFC2X2") == 0 )
			{
				target_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"IFC2X2";
				target_model->getIfcSchemaVersion().m_ifc_file_schema_enum = BuildingModel::IFC2X2;
			}
			else if( file_schema_args.substr(0,6).compare(L"IFC2X3") == 0 )
			{
				target_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"IFC2X3";
				target_model->getIfcSchemaVersion().m_ifc_file_schema_enum = BuildingModel::IFC2X3;
			}
			else if( file_schema_args.substr(0,6).compare(L"IFC2X4") == 0 )
			{
				target_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"IFC2X4";
				target_model->getIfcSchemaVersion().m_ifc_file_schema_enum = BuildingModel::IFC2X4;
			}
			else if( file_schema_args.substr( 0, 5 ).compare( L"IFC2X" ) == 0 )
			{
				target_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"IFC2X";
				target_model->getIfcSchemaVersion().m_ifc_file_schema_enum = BuildingModel::IFC2X;
			}
			else if( file_schema_args.compare( L"IFC4" ) == 0 )
			{
				target_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"IFC4";
				target_model->getIfcSchemaVersion().m_ifc_file_schema_enum = BuildingModel::IFC4;
			}
			else
			{
				target_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = L"";
				target_model->getIfcSchemaVersion().m_ifc_file_schema_enum = BuildingModel::IFC_VERSION_UNDEFINED;
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
	char* stream_pos = const_cast<char*>(&read_in[0]);
	if( stream_pos == nullptr )
	{
		throw BuildingException("Invalid file content", __FUNC__);
	}

	stream_pos = strstr(stream_pos, "DATA;");
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
	char* progress_anchor = stream_pos;
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
			char* stream_pos_next = stream_pos;
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
			char* string_start = stream_pos;
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
			std::pair<std::string, shared_ptr<BuildingEntity> >& entity_read_obj = (*target_vec_ptr)[i];;
			
			// read lines: #1234=IFCOBJECTNAME(...,...,(...,...),...)
			try
			{
				readSingleStepLine( step_line, entity_read_obj );
			}
			catch(UnknownEntityException& e)
			{
				std::string step_line_fix = step_line;
				std::string unknown_keyword = e.m_keyword;

				if( unknown_keyword.compare( "IFC2DCOMPOSITECURVE" ) == 0 )
				{
					size_t pos_find = step_line_fix.find( "IFC2DCOMPOSITECURVE" );
					step_line_fix = step_line_fix.erase( pos_find + 3, 2 );
				
					try
					{
						readSingleStepLine( step_line_fix, entity_read_obj );
					}
					catch( UnknownEntityException& )
					{
						if( unkown_entities.find( unknown_keyword ) == unkown_entities.end() )
						{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
							unkown_entities.insert( unknown_keyword );
							err_unknown_entity << "unknown IFC entity: " << unknown_keyword << std::endl;
						}
					}
				}
				else
				{
					if( unkown_entities.find( unknown_keyword ) == unkown_entities.end() )
					{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
						unkown_entities.insert( unknown_keyword );
						err_unknown_entity << "unknown IFC entity: " << unknown_keyword << std::endl;
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
		throw UnknownEntityException( err_unknown_entity.str() );
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

	const int entity_id = atoi( std::string( begin_id, stream_pos - begin_id ).c_str() );

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
	std::transform( entity_name_upper.begin(), entity_name_upper.end(), entity_name_upper.begin(), toupper );

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
		obj->m_entity_id = entity_id;
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

void ReaderSTEP::readEntityArguments( const BuildingModel::SchemaVersion& ifc_version,
	const std::vector<std::pair<std::string, shared_ptr<BuildingEntity> > >& vec_entities,  const std::map<int,shared_ptr<BuildingEntity> >& map_entities  )
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
			const shared_ptr<BuildingEntity> entity = entity_read_object.second;
			if( !entity )
			{
				continue;
			}
			const std::string& argument_str = entity_read_object.first;
			std::vector<std::string> arguments;
			std::vector<std::wstring> arguments_w;
			tokenizeEntityArguments( argument_str, arguments );

			// character decoding:
			decodeArgumentStrings( arguments, arguments_w );

			if( ifc_version.m_ifc_file_schema_enum != BuildingModel::IFC4 )
			{
				size_t num_expected_arguments = entity->getNumAttributes();
				if( num_expected_arguments != arguments_w.size() )
				{
					while( arguments_w.size() > num_expected_arguments ) { arguments_w.pop_back(); }
					while( arguments_w.size() < num_expected_arguments ) { arguments_w.emplace_back( L"$" ); }
				}
			}

			if( std::string( entity->className() ).compare( "IfcColourRGB" ) == 0 )
			{
				if( arguments_w.size() < 4 )
				{
					arguments_w.insert( arguments_w.begin(), L"$" );
				}
			}

			try
			{
				entity->readStepArguments( arguments_w, map_entities_ptr_local );
			}
			catch( std::exception& e )
			{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_entity_id << "=" << entity->className() << ": " << e.what();
			}
			catch( std::exception* e )
			{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_entity_id << "=" << entity->className() << ": " << e->what();
			}
			catch(...)
			{
#ifdef ENABLE_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_entity_id << "=" << entity->className() << " readStepData: error occurred" << std::endl;
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
	BuildingModel::SchemaVersion& file_schema_version = model->getIfcSchemaVersion();
	std::map<int, shared_ptr<BuildingEntity> >& map_entities = model->m_map_entities;
	readData( read_in, file_schema_version, map_entities );
}

void ReaderSTEP::readData(	std::string& read_in, const BuildingModel::SchemaVersion& ifc_version, std::map<int, shared_ptr<BuildingEntity> >& target_map )
{
	std::string current_numeric_locale(setlocale(LC_NUMERIC, nullptr));
	setlocale(LC_NUMERIC,"C");

	if( ifc_version.m_ifc_file_schema_enum  == BuildingModel::IFC_VERSION_UNDEFINED || ifc_version.m_ifc_file_schema_enum == BuildingModel::IFC_VERSION_UNKNOWN )
	{
		std::wstring error_message;
		error_message.append( L"Unsupported IFC version: " );
		error_message.append( ifc_version.m_IFC_FILE_SCHEMA );
		messageCallback( error_message, StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
		progressValueCallback(0.0, "parse");
		return;
	}
	
	if( read_in.empty() )
	{
		return;
	}
	messageCallback( std::wstring( L"Detected IFC version: ") + ifc_version.m_IFC_FILE_SCHEMA, StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE, "" );

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
	catch( OutOfMemoryException& e)
	{
		throw e;
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
			target_map.insert( std::make_pair( entity->m_entity_id, entity ) );
		}
	}

	try
	{
		readEntityArguments( ifc_version, vec_entities, target_map );
	}
	catch( OutOfMemoryException& e)
	{
		throw e;
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
