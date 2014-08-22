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

#include <set>
#include <vector>
#include <map>
#include <string>
#include <cctype>
#include <algorithm>
#include <locale.h>
#include <time.h>

#include "ifcpp/model/IfcPPOpenMP.h"
#include "ifcpp/model/IfcPPModel.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/UnknownEntityException.h"
#include "ifcpp/IfcPPTypeMap.h"
#include "ifcpp/IfcPPEntitiesMap.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/reader/IfcPPReaderSTEP.h"

static std::map<std::string,IfcPPEntityEnum> map_string2entity_enum(initializers_IfcPP_entity, initializers_IfcPP_entity + sizeof(initializers_IfcPP_entity)/sizeof(initializers_IfcPP_entity[0]));
static std::map<std::string,IfcPPTypeEnum> map_string2type_enum(initializers_IfcPP_type, initializers_IfcPP_type + sizeof(initializers_IfcPP_type)/sizeof(initializers_IfcPP_type[0]));

void applyBackwardCompatibility( const IfcPPModel::IfcPPSchemaVersion& ifc_version, IfcPPEntityEnum type_enum, std::vector<std::wstring>& args );
void applyBackwardCompatibility( std::string& keyword, std::string& step_line );
IfcPPEntity* createIfcPPEntity( const IfcPPEntityEnum entity_enum );
void findEndOfString( char*& stream_pos );
void findEndOfWString( wchar_t*& stream_pos );

IfcPPTypeEnum findTypeEnumForString( const std::wstring& type_name_w )
{
	std::string type_name;
	type_name.assign(type_name_w.begin(), type_name_w.end());
	std::map<std::string,IfcPPTypeEnum>::iterator it_type_enum = map_string2type_enum.find( type_name );
	if( it_type_enum != map_string2type_enum.end() )
	{
		IfcPPTypeEnum type_enum = it_type_enum->second;
		return type_enum;
	}
	return IFC_TYPE_UNDEFINED;
}


IfcPPEntityEnum findEntityEnumForString( const std::wstring& entity_name_w )
{
	std::string entity_name;
	entity_name.assign(entity_name_w.begin(), entity_name_w.end());
	std::map<std::string,IfcPPEntityEnum>::iterator it_enum = map_string2entity_enum.find( entity_name );
	if( it_enum != map_string2entity_enum.end() )
	{
		IfcPPEntityEnum entity_enum = it_enum->second;
		return entity_enum;
	}
	return IFC_ENTITY_UNDEFINED;
}

void readSingleStepLine( const std::string& line, IfcPPReaderSTEP::EntityReadContainer& target_read_object )
{
	char* stream_pos = (char*)line.c_str();
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

	const int entity_id = atoi( std::string( begin_id, stream_pos-begin_id ).c_str() );

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
	const char* keyword_begin = stream_pos;
	while( isalnum( *stream_pos ) ) { ++stream_pos; }

	std::string keyword( keyword_begin, stream_pos-keyword_begin );
	std::transform(keyword.begin(), keyword.end(), keyword.begin(), toupper);

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

	if( keyword.size() > 0 )
	{
		std::map<std::string,IfcPPEntityEnum>::iterator it_entity_enum = map_string2entity_enum.find( keyword );
		if( it_entity_enum == map_string2entity_enum.end() )
		{
			throw UnknownEntityException( keyword );
		}
		else
		{
			IfcPPEntityEnum entity_enum = it_entity_enum->second;

			shared_ptr<IfcPPEntity> obj( createIfcPPEntity( entity_enum ) );
			if( !obj )
			{
				std::stringstream strs;
				strs << __func__ << ": could not create object of type " << keyword << ", entity id " << entity_id << std::endl;
				throw IfcPPException( strs.str().c_str() );
			}
			obj->m_id = entity_id;
			obj->m_entity_enum = entity_enum;
			target_read_object.m_entity = obj;
			size_t sub_length = line.size() -(stream_pos-line.c_str());
			std::string entity_arg(stream_pos, sub_length);
			target_read_object.m_entity_argument_str.assign( entity_arg.begin(), entity_arg.end() );
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
IfcPPReaderSTEP::IfcPPReaderSTEP()
{
}

IfcPPReaderSTEP::~IfcPPReaderSTEP()
{
}

void IfcPPReaderSTEP::removeComments( std::string& buffer )
{
	const size_t length = buffer.length();
	size_t length_without_comments = 0;

	// sort out comments like /* ... */
	char* read_pos = (char*)&buffer[0];
	char* write_pos = (char*)&buffer[0];

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

void IfcPPReaderSTEP::readStreamHeader( const std::string& read_in, shared_ptr<IfcPPModel>& target_model )
{
	if( !target_model )
	{
		throw IfcPPException( "Model not set.", __func__ );
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

	IfcPPModel::IfcPPSchemaVersion schema_version( L"", IfcPPModel::IFC_VERSION_UNDEFINED );
	target_model->setIfcSchemaVersion( schema_version );
	file_header_start += 7;
	std::string file_header = read_in.substr( file_header_start, file_header_end - file_header_start );
	std::vector<std::string> vec_header;
	std::vector<std::wstring> vec_header_wstr;
	vec_header.push_back( file_header );
	decodeArgumentStrings( vec_header, vec_header_wstr );
	
	std::wstring file_header_wstr;
	if( vec_header_wstr.size() > 0 )
	{
		file_header_wstr = vec_header_wstr[0];
	}
	target_model->setFileHeader( file_header_wstr );
	
	std::vector<std::wstring> vec_header_lines;
	// split into lines
	wchar_t* stream_pos = &file_header_wstr[0];
	wchar_t* last_token = stream_pos;

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

	for( int i=0; i<vec_header_lines.size(); ++i )
	{
		std::wstring header_line = vec_header_lines[i];

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
			target_model->getIfcSchemaVersion().m_IFC_FILE_SCHEMA = header_line;

			std::wstring file_schema_args = header_line.substr( 11 );
			size_t find_whitespace = file_schema_args.find(L" ");
			while(find_whitespace != std::string::npos){ file_schema_args.erase(find_whitespace,1); find_whitespace = file_schema_args.find(L" "); }
			
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
				target_model->m_ifc_schema_version.m_ifc_file_schema_enum = IfcPPModel::IFC2X2;
			}
			else if( file_schema_args.substr(0,6).compare(L"IFC2X3") == 0 )
			{
				target_model->m_ifc_schema_version.m_ifc_file_schema_enum = IfcPPModel::IFC2X3;
			}
			else if( file_schema_args.substr(0,6).compare(L"IFC2X4") == 0 )
			{
				target_model->m_ifc_schema_version.m_ifc_file_schema_enum = IfcPPModel::IFC2X4;
			}
			else if( file_schema_args.substr(0,5).compare(L"IFC2X") == 0 )
			{
				target_model->m_ifc_schema_version.m_ifc_file_schema_enum = IfcPPModel::IFC2X;
			}
			else if( file_schema_args.compare(L"IFC4") == 0 )
			{
				target_model->m_ifc_schema_version.m_ifc_file_schema_enum = IfcPPModel::IFC4;
			}
			else if( file_schema_args.compare(L"IFC4RC4") == 0 )
			{
				target_model->m_ifc_schema_version.m_ifc_file_schema_enum = IfcPPModel::IFC4;
			}
			else
			{
				target_model->m_ifc_schema_version.m_ifc_file_schema_enum = IfcPPModel::IFC_VERSION_UNDEFINED;
			}
		}
	}
}

void IfcPPReaderSTEP::splitIntoStepLines(const std::string& read_in, std::vector<std::string>& target_vec)
{
	// set progress to 0
	double progress = 0.0;
	progressCallback(progress, "parse");

	// find beginning of data lines
	const size_t length = read_in.length();
	char* stream_pos = (char*)&read_in[0];
	stream_pos = strstr(stream_pos, "DATA;");
	if( stream_pos != NULL )
	{
		stream_pos += 5;
		while( isspace(*stream_pos) ){ ++stream_pos; }
	}

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
	std::string single_step_line = "";

	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\r' )
		{
			// omit newlines
			++stream_pos;
			continue;
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
				target_vec.push_back(single_step_line);
				single_step_line = "";
			}

			++stream_pos;
			while( isspace(*stream_pos) ){ ++stream_pos; }

			if( target_vec.size() % 100 == 0 )
			{
				double progress_since_anchor = (double)( stream_pos - progress_anchor ) / double(length);
				if( progress_since_anchor > 0.03 )
				{
					progress = 0.2*(double)( stream_pos - &read_in[0] ) / double(length);
					progressCallback(progress, "parse");
					progress_anchor = stream_pos;
				}
			}
			continue;
		}

		single_step_line += *stream_pos;
		++stream_pos;
	}
}

void IfcPPReaderSTEP::readStepLines( const std::vector<std::string>& step_lines, std::vector<EntityReadContainer>& target_entity_vec )
{
	std::set<std::string> unkown_entities;
	std::stringstream err_unknown_entity;

	double progress = 0.2;
	double last_progress = 0.2;
	const size_t num_lines = step_lines.size();
	
	target_entity_vec.resize( num_lines );
	std::vector<EntityReadContainer>* target_vec_ptr = &target_entity_vec;
	std::set<std::string>* unkown_entities_ptr = &unkown_entities;
	const std::vector<std::string>* step_lines_ptr = &step_lines;

#ifdef IFCPP_OPENMP
#pragma omp parallel firstprivate(num_lines) shared(target_vec_ptr,unkown_entities_ptr,step_lines_ptr)
	{
		// time for reading a step line does not differ much, so schedule many per thread
#pragma omp for schedule(dynamic,100)
#endif
		for( int i=0; i<num_lines; ++i )
		{
			const std::string& step_line = (*step_lines_ptr)[i];
			EntityReadContainer& entity_read_obj = (*target_vec_ptr)[i];;

			// read lines: #1234=IFCOBJECTNAME(...,...,(...,...),...)
			try
			{
				readSingleStepLine( step_line,  entity_read_obj );
			}
			catch(UnknownEntityException& e)
			{
				std::string unknown_keyword = e.m_keyword;
				std::string step_line = step_lines[i];
				applyBackwardCompatibility( unknown_keyword, step_line );
				try
				{
					readSingleStepLine( step_line, entity_read_obj );
				}
				catch( UnknownEntityException& )
				{
					if( unkown_entities.find( unknown_keyword ) == unkown_entities.end() )
					{
#ifdef IFCPP_OPENMP
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
#ifdef IFCPP_OPENMP
					if( omp_get_thread_num() == 0 )
#endif
					{
						progressCallback( progress, "parse" );
						last_progress = progress;
					}
				}
			}
		}
#ifdef IFCPP_OPENMP
	}
#endif
	
	if( err_unknown_entity.tellp() > 0 )
	{
		throw UnknownEntityException( err_unknown_entity.str() );
	}
}

void IfcPPReaderSTEP::readEntityArguments( const IfcPPModel::IfcPPSchemaVersion& ifc_version, const std::vector<EntityReadContainer>& vec_entities, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities  )
{
	// second pass, now read arguments
	// every object can be initialized independently in parallel
	const size_t num_objects = vec_entities.size();
	std::stringstream err;

	// set progress
	double progress = 0.3;
	progressCallback( progress, "parse" );
	double last_progress = 0.3;
	const std::map<int,shared_ptr<IfcPPEntity> >* map_entities_ptr = &map_entities;
	const std::vector<EntityReadContainer>* vec_entities_ptr = &vec_entities;

#ifdef IFCPP_OPENMP
#pragma omp parallel firstprivate(num_objects) shared(map_entities_ptr,vec_entities_ptr)
#endif
	{
		const std::map<int,shared_ptr<IfcPPEntity> > &map_entities_ptr_local = *map_entities_ptr;

#ifdef IFCPP_OPENMP
#pragma omp for schedule(dynamic, 100)
#endif
		for( int i=0; i<num_objects; ++i )
		{
			const EntityReadContainer& entity_read_object = (*vec_entities_ptr)[i];
			const shared_ptr<IfcPPEntity>& entity = entity_read_object.m_entity;
			if( !entity )
			{
				continue;
			}
			const std::string& argument_str = entity_read_object.m_entity_argument_str;
			std::vector<std::string> arguments;
			std::vector<std::wstring> arguments_w;
			tokenizeEntityArguments( argument_str, arguments );

			// character decoding:
			decodeArgumentStrings( arguments, arguments_w );

			if( ifc_version.m_ifc_file_schema_enum != IfcPPModel::IFC4 )
			{
				if( ifc_version.m_ifc_file_schema_enum != IfcPPModel::IFC_VERSION_UNDEFINED && ifc_version.m_ifc_file_schema_enum != IfcPPModel::IFC_VERSION_UNKNOWN )
				{
					IfcPPEntityEnum entity_enum = entity->m_entity_enum;
					applyBackwardCompatibility( ifc_version, entity_enum, arguments_w );
				}
			}
#ifdef _DEBUG
			int entity_id = entity->m_id;
#endif

			try
			{
				entity->readStepArguments( arguments_w, map_entities_ptr_local );
			}
			catch( std::exception& e )
			{
#ifdef IFCPP_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_id << "=" << typeid(*entity).name() << ": " << e.what();
			}
			catch( std::exception* e )
			{
#ifdef IFCPP_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_id << "=" << typeid(*entity).name() << ": " << e->what();
			}
			catch(...)
			{
#ifdef IFCPP_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->m_id << "=" << typeid(*entity).name() << " readStepData: error occurred" << std::endl;
			}

			if( i%10 == 0 )
			{
				progress = 0.3 + 0.6*double(i)/double(num_objects);
				if( progress - last_progress > 0.03 )
				{
#ifdef IFCPP_OPENMP
					if( omp_get_thread_num() == 0 )
#endif
					{
						progressCallback( progress, "parse" );
						last_progress = progress;
					}
				}
			}
		}
	}   // implicic barrier
	
	if( err.tellp() > 0 )
	{
		throw IfcPPException( err.str() );
	}
}

void IfcPPReaderSTEP::readStreamData( std::string& read_in, shared_ptr<IfcPPModel>& model )
{
	IfcPPModel::IfcPPSchemaVersion& file_schema_version = model->getIfcSchemaVersion();
	std::map<int,shared_ptr<IfcPPEntity> >& map_entities = model->m_map_entities;
	readStreamData( read_in, file_schema_version, map_entities );
}

void IfcPPReaderSTEP::readStreamData(	std::string& read_in, const IfcPPModel::IfcPPSchemaVersion& ifc_version, std::map<int,shared_ptr<IfcPPEntity> >& target_map )
{
	char* current_numeric_locale = setlocale(LC_NUMERIC, nullptr);
	setlocale(LC_NUMERIC,"C");

	if( ifc_version.m_ifc_file_schema_enum  == IfcPPModel::IFC_VERSION_UNDEFINED || ifc_version.m_ifc_file_schema_enum == IfcPPModel::IFC_VERSION_UNKNOWN )
	{
		std::wstring error_message;
		error_message.append( L"Unsupported IFC version: " );
		error_message.append( ifc_version.m_IFC_FILE_SCHEMA );
		errorCallback( error_message );
		progressCallback(0.0, "parse");
		return;
	}
	
	if( read_in.size() == 0 )
	{
		return;
	}
	messageCallback( std::wstring( L"Detected IFC version: ") + ifc_version.m_IFC_FILE_SCHEMA );

	std::stringstream err;
	std::vector<std::string> step_lines;
	try
	{
		splitIntoStepLines( read_in, step_lines );
	}
	catch(std::exception& e)
	{
		err << e.what();
	}
	// the input string is not needed any more
	read_in.clear();
	
	std::vector<EntityReadContainer> vec_entities;
	vec_entities.resize( step_lines.size() );
	try
	{
		readStepLines( step_lines, vec_entities );
	}
	catch( UnknownEntityException& e )
	{
		std::string unknown_keyword = e.m_keyword;
		err << __func__ << ": unknown entity: " << unknown_keyword.c_str() << std::endl;
	}
	catch( IfcPPException& e )
	{
		err << e.what();
	}
	catch(std::exception& e)
	{
		err << e.what();
	}
	catch(...)
	{
		err << __func__ << ": error occurred" << std::endl;
	}
	
	// copy entities into map so that they can be found during entity attribute initialization
	for( auto it_entity_vec = vec_entities.begin(); it_entity_vec!=vec_entities.end(); ++it_entity_vec )
	{
		EntityReadContainer& entity_read_object = (*it_entity_vec);
		shared_ptr<IfcPPEntity>& entity = entity_read_object.m_entity;
		if( entity ) // skip aborted entities
		{
			target_map.insert( std::make_pair( entity->m_id, entity ) );
		}
	}

	try
	{
		readEntityArguments( ifc_version, vec_entities, target_map );
	}
	catch( IfcPPException& e )
	{
		err << e.what();
	}
	catch(std::exception& e)
	{
		err << e.what();
	}
	catch(...)
	{
		err << __func__ << ": error occurred" << std::endl;
	}

	setlocale(LC_NUMERIC,current_numeric_locale);
	if( err.tellp() > 0 )
	{
		throw IfcPPException( err.str() );
	}
}

void applyBackwardCompatibility( std::string& keyword, std::string& step_line )
{
	// TODO: systematic backward compatibility, possibly generated from schema diff
	if( keyword.compare("IFC2DCOMPOSITECURVE") == 0 )
	{
		size_t pos_find = step_line.find("IFC2DCOMPOSITECURVE");
		step_line = step_line.erase(pos_find+3,2);
		return;
	}

	if( keyword.compare("IFCDATEANDTIME") == 0 )
	{
		// TODO: replace IFCDATEANDTIME
		return;
	}

	if( keyword.compare("IFCCOORDINATEDUNIVERSALTIMEOFFSET") == 0 )
	{
		// TODO: replace IFCDATEANDTIME
		return;
	}
}

static std::map<IfcPPEntityEnum, int> global_map_num_args = {
	{ IfcPPEntityEnum::IFCBEAM, 9 },
	{ IfcPPEntityEnum::IFCBUILDINGELEMENTPART, 9 },
	{ IfcPPEntityEnum::IFCCLASSIFICATION, 7 },
	{ IfcPPEntityEnum::IFCCLASSIFICATIONREFERENCE, 6 },
	{ IfcPPEntityEnum::IFCCOLOURRGB, 4 },
	{ IfcPPEntityEnum::IFCCOLUMN, 9 },
	{ IfcPPEntityEnum::IFCCSHAPEPROFILEDEF, 8 },
	{ IfcPPEntityEnum::IFCCURTAINWALL, 9 },
	{ IfcPPEntityEnum::IFCCURVESTYLE, 5 },
	{ IfcPPEntityEnum::IFCDISCRETEACCESSORY, 9 },
	{ IfcPPEntityEnum::IFCDISCRETEACCESSORYTYPE, 10 },
	{ IfcPPEntityEnum::IFCDISTRIBUTIONPORT, 10 },
	{ IfcPPEntityEnum::IFCDOCUMENTREFERENCE, 5 },
	{ IfcPPEntityEnum::IFCDOOR, 13 },
	{ IfcPPEntityEnum::IFCDOORLININGPROPERTIES, 17 },
	{ IfcPPEntityEnum::IFCFASTENER, 9 },
	{ IfcPPEntityEnum::IFCFILLAREASTYLE, 3 },
	{ IfcPPEntityEnum::IFCFLOWTERMINAL, 8 },
	{ IfcPPEntityEnum::IFCFURNITURETYPE, 11 },
	{ IfcPPEntityEnum::IFCGRID, 11 },
	{ IfcPPEntityEnum::IFCISHAPEPROFILEDEF, 10 },
	{ IfcPPEntityEnum::IFCLIBRARYREFERENCE, 6 },
	{ IfcPPEntityEnum::IFCLSHAPEPROFILEDEF, 9 },
	{ IfcPPEntityEnum::IFCMATERIAL, 3 },
	{ IfcPPEntityEnum::IFCMATERIALLAYER, 7 },
	{ IfcPPEntityEnum::IFCMATERIALLAYERSET, 3 },
	{ IfcPPEntityEnum::IFCMATERIALLAYERSETUSAGE, 5 },
	{ IfcPPEntityEnum::IFCMATERIALPROFILESETUSAGE, 3 },
	{ IfcPPEntityEnum::IFCMECHANICALFASTENER, 11 },
	{ IfcPPEntityEnum::IFCMECHANICALFASTENERTYPE, 12 },
	{ IfcPPEntityEnum::IFCMEMBER, 9 },
	{ IfcPPEntityEnum::IFCOPENINGELEMENT, 9 },
	{ IfcPPEntityEnum::IFCPLATE, 9 },
	{ IfcPPEntityEnum::IFCPOSTALADDRESS, 10 },
	{ IfcPPEntityEnum::IFCPROJECT, 9 },
	{ IfcPPEntityEnum::IFCPROPERTYBOUNDEDVALUE, 6 },
	{ IfcPPEntityEnum::IFCPROPERTYSINGLEVALUE, 4 },
	{ IfcPPEntityEnum::IFCPROPERTYTABLEVALUE, 8 },
	{ IfcPPEntityEnum::IFCQUANTITYAREA, 5 },
	{ IfcPPEntityEnum::IFCQUANTITYCOUNT, 5 },
	{ IfcPPEntityEnum::IFCQUANTITYLENGTH, 5 },
	{ IfcPPEntityEnum::IFCQUANTITYVOLUME, 5 },
	{ IfcPPEntityEnum::IFCQUANTITYWEIGHT, 5 },
	{ IfcPPEntityEnum::IFCRAMPFLIGHT, 9 },
	{ IfcPPEntityEnum::IFCREINFORCINGMESH, 18 },
	{ IfcPPEntityEnum::IFCSIMPLEPROPERTYTEMPLATE, 12 },
	{ IfcPPEntityEnum::IFCSPACETYPE, 11 },
	{ IfcPPEntityEnum::IFCSTAIRFLIGHT, 13 },
	{ IfcPPEntityEnum::IFCSTRUCTURALANALYSISMODEL, 10 },
	{ IfcPPEntityEnum::IFCSTRUCTURALPOINTCONNECTION, 9 },
	{ IfcPPEntityEnum::IFCSTRUCTURALCURVEMEMBER, 9 },
	{ IfcPPEntityEnum::IFCSURFACESTYLE, 3 },
	{ IfcPPEntityEnum::IFCSYSTEMFURNITUREELEMENTTYPE, 10 },
	{ IfcPPEntityEnum::IFCTSHAPEPROFILEDEF, 12 },
	{ IfcPPEntityEnum::IFCTELECOMADDRESS, 9 },
	{ IfcPPEntityEnum::IFCTEXTSTYLE, 5 },
	{ IfcPPEntityEnum::IFCTRANSPORTELEMENT, 9 },
	{ IfcPPEntityEnum::IFCUSHAPEPROFILEDEF, 10 },
	{ IfcPPEntityEnum::IFCWALL, 9 },
	{ IfcPPEntityEnum::IFCWALLSTANDARDCASE, 9 },
	{ IfcPPEntityEnum::IFCWINDOW, 13 },
	{ IfcPPEntityEnum::IFCWINDOWLININGPROPERTIES, 16 },
	{ IfcPPEntityEnum::IFCZONE, 6 }
};

void applyBackwardCompatibility( const IfcPPModel::IfcPPSchemaVersion& ifc_version, IfcPPEntityEnum type_enum, std::vector<std::wstring>& args )
{

	// TODO: replace this workaround with a systematic backward compatibility, possibly generated from schema diff
	if( ifc_version.m_ifc_file_schema_enum < IfcPPModel::IFC2X )
	{
		throw IfcPPException( "Unsupported IFC version", __func__ );
	}

	std::map<IfcPPEntityEnum, int>::iterator it_find_num_args = global_map_num_args.find( type_enum );
	if( it_find_num_args != global_map_num_args.end() )
	{
		int num_args = it_find_num_args->second;
		while( args.size() > num_args ){ args.pop_back(); }
		while( args.size() < num_args ){ args.push_back( L"$" ); }
	}

	//if( ifc_version.m_ifc_file_schema_enum < IfcPPModel::IFC4 )
	//{
	//	switch( type_enum )
	//	{
	//	
	//	case IFCCOLOURRGB:
	//		if( args.size() == 3 ) //#315= IFCCOLOURRGB($,0.65882353,0.6627451,0.61960784);
	//		{
	//			args.insert( args.begin(), L"$" );
	//		}
	//		break;
	//	}
	//}

	//IfcRelDecomposes -> IfcRelAggregates
}
