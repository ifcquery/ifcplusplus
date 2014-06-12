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
#include "ifcpp/reader/IfcStepReader.h"

static std::map<std::string,IfcPPEntityEnum> map_string2entity_enum(initializers_IfcPP_entity, initializers_IfcPP_entity + sizeof(initializers_IfcPP_entity)/sizeof(initializers_IfcPP_entity[0]));
static std::map<std::string,IfcPPTypeEnum> map_string2type_enum(initializers_IfcPP_type, initializers_IfcPP_type + sizeof(initializers_IfcPP_type)/sizeof(initializers_IfcPP_type[0]));

void applyBackwardCompatibility( shared_ptr<IfcPPModel>& ifc_model, IfcPPEntityEnum type_enum, std::vector<std::wstring>& args );
void applyBackwardCompatibility( std::string& keyword, std::string& step_line );
shared_ptr<IfcPPObject> createIfcPPType( const IfcPPTypeEnum type_enum, const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities );
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

void readSingleStepLine( const std::string& line, shared_ptr<IfcPPEntity>& entity )
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

	const int id = atoi( std::string( begin_id, stream_pos-begin_id ).c_str() );

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
			if( obj )
			{
				obj->setId( id );
				entity = obj;
				entity->m_entity_enum = entity_enum;
				
				size_t sub_length = line.size() -(stream_pos-line.c_str());
				std::string entity_arg(stream_pos, sub_length);
				entity->m_entity_argument_str.assign( entity_arg.begin(), entity_arg.end() );
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
IfcStepReader::IfcStepReader()
{
}

IfcStepReader::~IfcStepReader()
{
}

void IfcStepReader::readStreamHeader( const std::string& read_in )
{
	if( !m_model )
	{
		throw IfcPPException( "Model not set.", __func__ );
	}

	m_model->m_file_header = L"";
	m_model->m_ifc_schema_version = IfcPPModel::IFC_VERSION_UNDEFINED;
	m_model->m_IFC_FILE_DESCRIPTION = L"";
	m_model->m_IFC_FILE_NAME = L"";
	m_model->m_IFC_FILE_SCHEMA = L"";
	
	size_t file_header_start = read_in.find("HEADER;");
	size_t file_header_end = read_in.find("ENDSEC;");
	if( file_header_start == std::string::npos || file_header_end == std::string::npos )
	{
		throw IfcPPException( "Missing file header", __func__ );
	}
	
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
	m_model->setFileHeader( file_header_wstr );
	
	std::vector<std::string> vec_header_lines;
	// split into lines
	char* stream_pos = &file_header[0];
	char* last_token = stream_pos;

	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\'' )
		{
			findEndOfString( stream_pos );
			continue;
		}

		if( *stream_pos == ';' )
		{
			char* begin_line = last_token;
			std::string single_step_line( begin_line, stream_pos-last_token );
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
		std::string header_line = vec_header_lines[i];
		std::wstring header_line_wstr;
		header_line_wstr.assign(header_line.begin(), header_line.end());

		if( header_line.find("FILE_DESCRIPTION") != std::string::npos )
		{
			m_model->setFileDescription( header_line_wstr );
			continue;
		}

		if( header_line.find("FILE_NAME") != std::string::npos )
		{
			m_model->setFileName( header_line_wstr );
			continue;
		}

		if( header_line.find("FILE_SCHEMA") != std::string::npos )
		{
			size_t file_schema_begin = header_line.find("FILE_SCHEMA") + 11;
			m_model->setFileSchema( header_line_wstr );

			std::string file_schema_args = header_line.substr( 11 );
			size_t find_whitespace = file_schema_args.find(" ");
			while(find_whitespace != std::string::npos){ file_schema_args.erase(find_whitespace,1); find_whitespace = file_schema_args.find(" "); }
			
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
				
			if( file_schema_args.substr(0,6).compare("IFC2X2") == 0 )
			{
				m_model->m_ifc_schema_version = IfcPPModel::IFC2X2;
			}
			else if( file_schema_args.substr(0,6).compare("IFC2X3") == 0 )
			{
				m_model->m_ifc_schema_version = IfcPPModel::IFC2X3;
			}
			else if( file_schema_args.substr(0,6).compare("IFC2X4") == 0 )
			{
				m_model->m_ifc_schema_version = IfcPPModel::IFC2X4;
			}
			else if( file_schema_args.substr(0,5).compare("IFC2X") == 0 )
			{
				m_model->m_ifc_schema_version = IfcPPModel::IFC2X;
			}
			else if( file_schema_args.compare("IFC4") == 0 )
			{
				m_model->m_ifc_schema_version = IfcPPModel::IFC4;
			}
			else if( file_schema_args.compare("IFC4RC4") == 0 )
			{
				m_model->m_ifc_schema_version = IfcPPModel::IFC4;
			}
			else
			{
				m_model->m_ifc_schema_version = IfcPPModel::IFC_VERSION_UNDEFINED;
			}
		}
	}
}

void IfcStepReader::splitIntoStepLines(const std::string& read_in, std::vector<std::string>& target_vec)
{
	// set progress to 0
	double progress = 0.0;
	progressCallback(progress, "parse");
	const int length = (int)read_in.length();

	// sort out comments like /* ... */
	char* stream_pos = (char*)&read_in[0];


	// find beginning of data lines
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
		if( *stream_pos == '*' )
		{
			if( *( stream_pos - 1 ) == '/' )
			{
				--stream_pos;
				// we are inside comment now, proceed to end of comment
				++stream_pos;
				while( *stream_pos != '\0' )
				{
					if( *stream_pos == '/' )
					{
						if( *( stream_pos - 1 ) == '*' )
						{
							break;
						}
					}
					++stream_pos;
				}
				++stream_pos;
				continue;
			}
		}

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


		if( isspace(*stream_pos) ){
			++stream_pos;
			continue;
		}

		if( *stream_pos == '\'' )
		{
			char* string_start = stream_pos;
			findEndOfString(stream_pos);

			std::string s(string_start, stream_pos - string_start);

			single_step_line += s;

			//++stream_pos;
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

void IfcStepReader::readStepLines( const std::vector<std::string>& step_lines, std::vector<shared_ptr<IfcPPEntity> >& target_entity_vec )
{
	std::set<std::string> unkown_entities;
	std::stringstream err_unknown_entity;

	double progress = 0.2;
	double last_progress = 0.2;
	const int num_lines = (int)step_lines.size();
	
	std::vector<shared_ptr<IfcPPEntity> >* target_vec_ptr = &target_entity_vec;
	std::set<std::string>* unkown_entities_ptr = &unkown_entities;

#ifdef IFCPP_OPENMP
#pragma omp parallel firstprivate(num_lines) shared(target_vec_ptr,unkown_entities_ptr)
	{
		// time for reading a step line does not differ much, so schedule many per thread
#pragma omp for schedule(dynamic,100)
#endif
		for( int i=0; i<num_lines; ++i )
		{
			const std::string& step_line = step_lines[i];

			// read lines: #1234=IFCOBJECTNAME(...,...,(...,...),...)
			try
			{
				shared_ptr<IfcPPEntity> entity;
				readSingleStepLine( step_line,  entity );

				if( !entity )
				{
					continue;
				}

#ifdef IFCPP_OPENMP
#pragma omp critical
#endif
				{
					target_vec_ptr->push_back( entity );
				}
			}
			catch(UnknownEntityException& e)
			{
				std::string unknown_keyword = e.m_keyword;
				std::string step_line = step_lines[i];
				applyBackwardCompatibility( unknown_keyword, step_line );
				try
				{
					shared_ptr<IfcPPEntity> entity;
					readSingleStepLine( step_line, entity );

					if( entity )
					{
#ifdef IFCPP_OPENMP
#pragma omp critical
#endif
						target_vec_ptr->push_back( entity );
					}
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
				progress = 0.2 + 0.1*(double)i/num_lines;
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

void IfcStepReader::readEntityArguments( const std::vector<shared_ptr<IfcPPEntity> >& vec_entities, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities  )
{
	// second pass, now read arguments
	// every object can be initialized independently in parallel
	int num_objects = (int)vec_entities.size();
	std::stringstream err;

	// set progress
	double progress = 0.3;
	progressCallback( progress, "parse" );
	double last_progress = 0.3;
	const std::map<int,shared_ptr<IfcPPEntity> >* map_ptr = &map_entities;
	const std::vector<shared_ptr<IfcPPEntity> >* vec_entities_ptr = &vec_entities;

#ifdef IFCPP_OPENMP
#pragma omp parallel firstprivate(num_objects) shared(map_ptr,vec_entities_ptr)
#endif
	{
		const std::map<int,shared_ptr<IfcPPEntity> > &map_loc = *map_ptr;
		std::stringstream thread_err;

#ifdef IFCPP_OPENMP
#pragma omp for schedule(dynamic, 100)
#endif
		for( int i=0; i<num_objects; ++i )
		{
			shared_ptr<IfcPPEntity> entity = vec_entities_ptr->at(i);
			std::string& argument_str = entity->m_entity_argument_str;
			std::vector<std::string> arguments;
			std::vector<std::wstring> arguments_w;
			tokenizeEntityArguments( argument_str, arguments );

#ifndef SET_ENTIY_ARGUMENT_STRING
			entity->m_entity_argument_str = "";
#endif
			// character decoding:
			decodeArgumentStrings( arguments, arguments_w );

			if( m_model->getIfcSchemaVersion() != IfcPPModel::IFC4 )
			{
				if( m_model->getIfcSchemaVersion() != IfcPPModel::IFC_VERSION_UNDEFINED && m_model->getIfcSchemaVersion() != IfcPPModel::IFC_VERSION_UNKNOWN )
				{
					IfcPPEntityEnum entity_enum = entity->m_entity_enum;
					applyBackwardCompatibility( m_model, entity_enum, arguments_w );
				}
			}
#ifdef _DEBUG
			int entity_id = entity->getId();
#endif

			try
			{
				entity->readStepArguments( arguments_w, map_loc );
			}
			catch( std::exception& e )
			{
#ifdef IFCPP_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->getId() << "=" << typeid(*entity).name() << ": " << e.what();
			}
			catch( std::exception* e )
			{
#ifdef IFCPP_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->getId() << "=" << typeid(*entity).name() << ": " << e->what();
			}
			catch(...)
			{
#ifdef IFCPP_OPENMP
#pragma omp critical
#endif
				err << "#" << entity->getId() << "=" << typeid(*entity).name() << " readStepData: error occurred" << std::endl;

			}

			if( i%10 == 0 )
			{
				progress = 0.3 + 0.6*(double)i/num_objects;
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
	}
	
	if( err.tellp() > 0 )
	{
		throw IfcPPException( err.str() );
	}
}

void IfcStepReader::readStreamData(	const std::string& read_in, std::map<int,shared_ptr<IfcPPEntity> >& target_map )
{
	char* current_numeric_locale = setlocale(LC_NUMERIC, nullptr);
	setlocale(LC_NUMERIC,"C");

	if( m_model->getIfcSchemaVersion()  == IfcPPModel::IFC_VERSION_UNDEFINED || m_model->getIfcSchemaVersion() == IfcPPModel::IFC_VERSION_UNKNOWN )
	{
		std::wstring error_message;
		error_message.append( L"Unsupported IFC version: " );
		error_message.append( m_model->getFileSchema() );
		errorCallback( error_message );
		progressCallback(0.0, "parse");
		return;
	}
	
	//std::string message( "Detected IFC version: " );
	//message.append( m_model->getFileSchema() );
	messageCallback( std::wstring( L"Detected IFC version: ") + m_model->getFileSchema() );

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
	
	std::vector<shared_ptr<IfcPPEntity> > vec_entities;
	try
	{
		readStepLines( step_lines, vec_entities );
	}
	catch( UnknownEntityException& e )
	{
		std::string unknown_keyword = e.m_keyword;
		err << "readStreamData: unknown entity: " << unknown_keyword.c_str() << std::endl;
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
		err << "readStreamData: error occurred" << std::endl;
	}
	
	// copy entities into map so that they can be found during entity attribute initialization
	std::vector<shared_ptr<IfcPPEntity> >::const_iterator it_entity_vec = vec_entities.begin();
	for( ; it_entity_vec!=vec_entities.end(); ++it_entity_vec )
	{
		shared_ptr<IfcPPEntity> entity = (*it_entity_vec);
		target_map.insert( std::make_pair(entity->getId(), entity ) );
	}

	try
	{
		readEntityArguments( vec_entities, target_map );
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
		err << "readStreamData: error occurred" << std::endl;
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

void appendEmptyArgs(std::vector<std::wstring>& args, const size_t target_size)
{
	while( args.size() > target_size ){ args.pop_back(); }
	while( args.size() < target_size ){	args.push_back( L"$" );	}
}

void applyBackwardCompatibility( shared_ptr<IfcPPModel>& ifc_model, IfcPPEntityEnum type_enum, std::vector<std::wstring>& args )
{
	IfcPPModel::IfcVersion version = ifc_model->getIfcSchemaVersion();
	std::wstring ifc_file_name = ifc_model->getFileName();

	// TODO: replace this workaround with a systematic backward compatibility, possibly generated from schema diff
	if( version < IfcPPModel::IFC2X )
	{
		throw IfcPPException( "Unsupported IFC version", __func__ );
	}

	if( version < IfcPPModel::IFC4 )
	{
		switch( type_enum )
		{
			// B
		case IFCBEAM:
			appendEmptyArgs(args, 9);
			break;
		case IFCBUILDINGELEMENTPART:
			appendEmptyArgs(args, 9);
			break;
			// C
		case IFCCLASSIFICATION:
			appendEmptyArgs(args, 7);
			break;
		case IFCCLASSIFICATIONREFERENCE:
			appendEmptyArgs(args, 6);
			break;
		case IFCCOLOURRGB:
			if( args.size() == 3 ) //#315= IFCCOLOURRGB($,0.65882353,0.6627451,0.61960784);
			{
				args.insert( args.begin(), L"$" );
			}
			break;
		case IFCCOLUMN:
			appendEmptyArgs(args, 9);
			break;
		case IFCCSHAPEPROFILEDEF:
			appendEmptyArgs(args, 8);
			break;
		case IFCCURTAINWALL:
			appendEmptyArgs(args, 9);
			break;
		case IFCCURVESTYLE:
			appendEmptyArgs(args, 5);
			break;
			//D
		case IFCDISCRETEACCESSORY:
			appendEmptyArgs(args, 9);
			break;
		case IFCDISCRETEACCESSORYTYPE:
			appendEmptyArgs(args, 10);
			break;
		case IFCDISTRIBUTIONPORT:
			appendEmptyArgs(args, 10);
			break;
		case IFCDOCUMENTREFERENCE:
			appendEmptyArgs(args, 5);
			break;
		case IFCDOOR:
			appendEmptyArgs(args, 13);
			break;
		case IFCDOORLININGPROPERTIES:
			appendEmptyArgs(args, 17);
			break;
			// F
		case IFCFILLAREASTYLE:
			appendEmptyArgs(args, 3);
			break;
		case IFCFLOWTERMINAL:
			appendEmptyArgs(args, 8);
			break;
		case IFCFURNITURETYPE:
			appendEmptyArgs(args, 11);
			break;

			// I
		case IFCISHAPEPROFILEDEF:
			appendEmptyArgs(args, 10);
			break;

			// L
		case IFCLIBRARYREFERENCE:
			appendEmptyArgs(args, 6);
			break;
		case IFCLSHAPEPROFILEDEF:
			appendEmptyArgs(args, 9);
			break;
			// M
		case IFCMATERIAL:
			appendEmptyArgs(args, 3);
			break;
		case IFCMATERIALLAYER:
			appendEmptyArgs(args, 7);
			break;
		case IFCMATERIALLAYERSET:
			appendEmptyArgs(args, 3);
			break;
		case IFCMATERIALLAYERSETUSAGE:
			appendEmptyArgs(args, 5);
			break;
		case IFCMATERIALPROFILESETUSAGE:
			appendEmptyArgs(args, 3);
			break;
		case IFCMECHANICALFASTENER:
			appendEmptyArgs(args, 11);
			break;
		case IFCMECHANICALFASTENERTYPE:
			appendEmptyArgs(args, 12);
			break;
		case IFCMEMBER:
			appendEmptyArgs(args, 9);
			break;
		
			// O
		case IFCOPENINGELEMENT:
			appendEmptyArgs(args, 9);
			break;

			// P
		case IFCPLATE:
			appendEmptyArgs(args, 9);
			break;
		case IFCPROJECT:
			appendEmptyArgs(args, 9);
			break;
		case IFCPROPERTYBOUNDEDVALUE:
			appendEmptyArgs(args, 6);
			break;
		case IFCPROPERTYSINGLEVALUE:
			appendEmptyArgs(args, 4);
			break;
		case IFCPROPERTYTABLEVALUE:
			appendEmptyArgs(args, 8);
			break;
			
			// Q
		case IFCQUANTITYAREA:
			appendEmptyArgs(args, 5);
			break;
		case IFCQUANTITYCOUNT:
			appendEmptyArgs(args, 5);
			break;
		case IFCQUANTITYLENGTH:
			appendEmptyArgs(args, 5);
			break;
		case IFCQUANTITYVOLUME:
			appendEmptyArgs(args, 5);
			break;
		case IFCQUANTITYWEIGHT:
			appendEmptyArgs(args, 5);
			break;

			// R
		case IFCRAMPFLIGHT:
			appendEmptyArgs(args, 9);
			break;
		case IFCREINFORCINGMESH:
			appendEmptyArgs(args, 18);
			break;

			// S
		case IFCSIMPLEPROPERTYTEMPLATE:
			appendEmptyArgs(args, 12);
			break;
		case IFCSTAIRFLIGHT:
			appendEmptyArgs(args, 13);
			break;
		case IFCSTRUCTURALANALYSISMODEL:
			appendEmptyArgs(args, 10);
			break;
		case IFCSTRUCTURALPOINTCONNECTION:
			appendEmptyArgs(args, 9);
			break;
		case IFCSTRUCTURALCURVEMEMBER:
			appendEmptyArgs(args, 9);
			break;
		case IFCSURFACESTYLE:
			appendEmptyArgs(args, 3);
			break;
		case IFCSYSTEMFURNITUREELEMENTTYPE:
			appendEmptyArgs(args, 10);
			break;

			// T
		case IFCTSHAPEPROFILEDEF:
			appendEmptyArgs(args, 12);
			break;
		
		case IFCTEXTSTYLE:
			appendEmptyArgs(args, 5);
			break;

		case IFCTRANSPORTELEMENT:
			appendEmptyArgs(args, 9);
			break;

			// U
		case IFCUSHAPEPROFILEDEF:
			appendEmptyArgs(args, 10);
			break;

			// W
		case IFCWALL:
			appendEmptyArgs(args, 9);
			break;
		case IFCWALLSTANDARDCASE:
			appendEmptyArgs(args, 9);
			break;
		case IFCWINDOW:
			appendEmptyArgs(args, 13);
			break;
		case IFCWINDOWLININGPROPERTIES:
			appendEmptyArgs(args, 16);
			break;

		case IFCZONE:
			appendEmptyArgs(args, 6);
			break;
		}
	}

	
	//IfcRelDecomposes -> IfcRelAggregates
}
