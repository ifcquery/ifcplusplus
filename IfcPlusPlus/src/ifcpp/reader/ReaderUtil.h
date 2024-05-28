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

#pragma once

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <limits>
#include <map>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingObject.h"
#include "ifcpp/IFC4X3/TypeFactory.h"

#ifdef _MSC_VER
#include <cstdio>
#include <cstring>
#pragma warning(disable : 4996)
#endif

void readBoolList( const std::string& str, std::vector<bool>& vec );
void readIntegerList( const std::string& str, std::vector<int>& vec );
void readIntegerList2D( const std::string& str, std::vector<std::vector<int> >& vec );
void readIntegerList3D( const std::string& str, std::vector<std::vector<std::vector<int> > >& vec );
void readRealList( const std::string& str, std::vector<double>& vec );
void readRealArray( const std::string& str, double(&vec)[3] );
void readRealList2D( const std::string& str, std::vector<std::vector<double> >& vec );
void readRealList3D( const std::string& str, std::vector<std::vector<std::vector<double> > >& vec );
void readBinary( const std::string& str, std::string& target );
void readBinaryString(const std::string& attribute_value, std::string& target);
void readBinaryList( const std::string& str, std::vector<std::string>& vec );
void readStringList( const std::string& str, std::vector<std::string>& vec );

IFCQUERY_EXPORT void tokenizeEntityArguments( const std::string& argument_str, std::vector<std::string>& entity_arguments );
IFCQUERY_EXPORT void tokenizeEntityArguments( const std::string& argument_str, std::vector<std::string>& entity_arguments );
void tokenizeInlineArgument(std::string arg, std::string& keyword, std::string& inline_arg);
void tokenizeList( std::string& list_str, std::vector<std::string>& list_items );
void tokenizeEntityList( std::string& list_str, std::vector<int>& list_items );
void findLeadingTrailingParanthesis(char* ch, char*& pos_opening, char*& pos_closing);
void findEndOfString(const char*& stream_pos);
bool findEndOfStepLine(char* ch, char*& pos_end);
void checkOpeningClosingParenthesis(const char* ch_check);
std::istream& bufferedGetline(std::istream& is, std::string& t);
std::istream& bufferedGetStepLine(std::istream& inputStream, std::string& lineOut);

IFCQUERY_EXPORT std::string wstring2string(const std::wstring& str);
IFCQUERY_EXPORT std::wstring string2wstring(const std::string& inputString);
IFCQUERY_EXPORT bool std_iequal(const std::string& a, const std::string& b);

inline void convertStringToUpperCase( std::string& a)
{
	std::transform(a.begin(), a.end(), a.begin(), [](char c) {return static_cast<char>(std::toupper(c)); });
}

inline std::string getFileExtension(std::string path)
{
#ifdef _MSC_VER
	return std::filesystem::path(string2wstring(path)).extension().string();
#else
	return std::filesystem::path(path).extension().string();
#endif
}

IFCQUERY_EXPORT void decodeArgumentString(const std::string& argument_str, std::string& arg_out);
IFCQUERY_EXPORT void decodeArgumentStrings( const std::vector<std::string>& entity_arguments, std::vector<std::string>& args_out );

void readBool(const std::string& attribute_value, bool& target);
void readLogical(const std::string& attribute_value, LogicalEnum& target);
void readInteger(const std::string& attribute_value, int& target);
void readIntegerValue(const std::string& str, int& int_value);
void readReal(const std::string& attribute_value, double& target);
void readString(const std::string& attribute_value, std::string& target);

template<typename T>
void readTypeOfIntegerList( const std::string& str, std::vector<shared_ptr<T> >& target_vec )
{
	// example: (38,12,4)
	char const* ch = str.c_str();
	char const* last_token = nullptr;

	// ignore leading space or opening parenthesis
	while( *ch != '\0' )
	{
		if( *ch == '(' )
		{
			checkOpeningClosingParenthesis( ch );
			++ch;
			last_token = ch;
			break;
		}
		else if( isspace( *ch ) )	{	++ch; }
		else { break; }
	}
	
	while( *ch != '\0' )
	{
		if( isspace(*ch) )
		{
			++ch;
			continue;
		}

		while( *ch != ',' && *ch != '\0' && *ch != ')' )
		{
			++ch;
		}

		if( last_token != nullptr )
		{
			size_t length_str = ch - last_token;
			if( length_str > 0 )
			{
				std::string int_str(last_token, length_str);
				int int_value = 0;
				try
				{
					int_value = std::stoi(int_str);
				}
				catch( std::exception&  )
				{
#ifdef _DEBUG
					std::cout << "bad number: " << int_str << std::endl;
#endif
				}
				target_vec.push_back(shared_ptr<T>(new T(int_value)));
			}
		}

		if( *ch == '\0' )
		{
			break;
		}
		
		if( *ch == ')' )
		{
			break;
		}
#ifdef _DEBUG
		if( *ch != ',' )
		{
			std::cout << __FUNC__ << ": *ch != ','" << std::endl;
		}
#endif

		++ch;
		last_token = ch;
	}
}

template<typename T>
void readTypeOfIntegerList2D( const std::string& str, std::vector<std::vector<shared_ptr<T> > >& target_vec )
{
	// example: ((8,12,4),(58,10,34),(18,10,4))
	char const* ch = str.c_str();

	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}

	//Optional lists can be represented by a $ sign
	if( ch[0] == '$' )
		return;

	if( ch[0] != '(' )
	{
		throw BuildingException( "string does not start with (", __FUNC__ );
	}
	size_t i = 0;
	size_t last_token = 1;
	int num_par_open = 0;
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			if( num_par_open == 1 )
			{
				target_vec.resize( target_vec.size()+1 );
				std::string s = str.substr( last_token, i-last_token );
				readTypeOfIntegerList( s, target_vec.back() );
				last_token = i+1;
			}
		}
		else if( ch[i] == '(' )
		{
			++num_par_open;
		}
		else if( ch[i] == ')' )
		{
			--num_par_open;
			if( num_par_open == 0 )
			{
				// closing parenthesis found
				target_vec.resize( target_vec.size()+1 );
				std::string s = str.substr( last_token, i-last_token );
				readTypeOfIntegerList( s, target_vec.back() );
				return;
			}
		}
		++i;
	}

	// no closing parenthesis found
	std::stringstream err;
	err << "no closing parenthesis found: " << str << std::endl;
	throw BuildingException( err.str(), __FUNC__ );
}

template<typename T>
void readTypeOfRealList( const std::string& str, std::vector<shared_ptr<T> >& target_vec )
{
	// example: (.38,12.0,.04)
	char const* ch = str.c_str();
	char const* last_token = nullptr;

	// ignore leading space or opening parenthesis
	while( *ch != '\0' )
	{
		if( *ch == '(' )
		{
			checkOpeningClosingParenthesis( ch );
			++ch;
			last_token = ch;
			break;
		}
		else if( isspace( *ch ) )	{	++ch; }
		else { break; }
	}

	while( *ch != '\0' )
	{
		if( isspace(*ch) )
		{
			++ch;
			continue;
		}

		while( *ch != ',' && *ch != '\0' && *ch != ')' )
		{
			++ch;
		}

		if( last_token != nullptr )
		{
			size_t length_str = ch - last_token;
			if( length_str > 0 )
			{
				std::string double_str(last_token, length_str);
				double real_value = 0;
				try
				{
					real_value = std::stod(double_str);
				}
				catch( std::exception&  )
				{

				}

				target_vec.push_back(shared_ptr<T>(new T(real_value)));
			}
		}

		if( *ch == '\0' )
		{
			break;
		}
		
		if( *ch == ')' )
		{
			break;
		}
#ifdef _DEBUG
		if( *ch != ',' )
		{
			std::cout << __FUNC__ << ": *ch != ','" << std::endl;
		}
#endif

		++ch;
		last_token = ch;
	}
}

template<typename T>
void readTypeOfRealList2D( const std::string& str, std::vector<std::vector<shared_ptr<T> > >& target_vec )
{
	// example: ((.38,12.0,.04),(.38,1.0,346.0),(1.8,1.0,.04))
	char const* ch = str.c_str();

	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}

	//Optional lists can be represented by a $ sign
	if( ch[0] == '$' )
		return;

	if( ch[0] != '(' )
	{
		throw BuildingException( "string does not start with (", __FUNC__ );
	}
	size_t i=0;
	size_t last_token = 1;
	int num_par_open = 0;
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			if( num_par_open == 1 )
			{
				target_vec.resize(target_vec.size()+1);
				std::string s = str.substr( last_token, i-last_token );
				readTypeOfRealList( s, target_vec.back() );
				last_token = i+1;
			}
		}
		else if( ch[i] == '(' )
		{
			++num_par_open;
		}
		else if( ch[i] == ')' )
		{
			--num_par_open;
			if( num_par_open == 0 )
			{
				// closing parenthesis found
				target_vec.resize(target_vec.size()+1);
				std::string s = str.substr( last_token, i-last_token );
				readTypeOfRealList( s, target_vec.back() );
				return;
			}
		}
		++i;
	}

	// no closing parenthesis found
	std::stringstream err;
	err << "no closing parenthesis found: " << str << std::endl;
	throw BuildingException( err.str(), __FUNC__ );
}

template<typename T>
void readTypeOfStringList( const char* str, std::vector<shared_ptr<T> >& target_vec )
{
	// example: ('Tahoma')
	const char* ch = str;
	const char* last_token = nullptr;

	// ignore leading space or opening parenthesis
	while( *ch != '\0' )
	{
		if( *ch == '(' )
		{
			checkOpeningClosingParenthesis( ch );
			++ch;
			last_token = ch;
			break;
		}
		else if( isspace( *ch ) ) { ++ch; }
		else { break; }
	}

	while( *ch != '\0' )
	{
		if( isspace( *ch ) )
		{
			// ignore leading space characters
			++ch;
			continue;
		}

		if ( *ch == '\'' )
			findEndOfString( ch );

		while( *ch != ',' && *ch != '\0' && *ch != ')' )
		{
			++ch;
		}

		if( last_token != nullptr )
		{
			size_t length_str = ch - last_token;
			if( length_str > 0 )
			{
				std::string str_value( last_token, length_str );
				if (length_str > 1)
				{
					if (str_value.front() == '\'' && str_value.back() == '\'')
					{
						// "'Tahoma'" -> "Tahoma"
						str_value = str_value.substr(1, str_value.size() - 2);
					}
				}
				target_vec.push_back( shared_ptr<T>( new T( str_value ) ) );
			}
		}

		if( *ch == '\0' )
		{
			break;
		}

		if( *ch == ')' )
		{
			break;
		}
#ifdef _DEBUG
		if( *ch != ',' )
		{
			std::cout << __FUNC__ << ": *ch != ','" << std::endl;
		}
#endif

		++ch;
		last_token = ch;
	}
}

template<typename T>
void readTypeOfStringList( const std::string& str, std::vector<shared_ptr<T> >& target_vec )
{
	char* ch = (char*)str.c_str();
	readTypeOfStringList( ch, target_vec );
}

template<typename T>
void readEntityReference( const std::string& str, shared_ptr<T>& target, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& mapEntities,
	std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound)
{
	if( str.length() == 0)
	{
		return;
	}
	if( str.at(0) == '#' )
	{
		int tag = std::stoi( str.substr( 1 ) );
		BuildingModelMapType<int,shared_ptr<BuildingEntity> >::const_iterator it_entity = mapEntities.find( tag );
		if( it_entity != mapEntities.end() )
		{
			shared_ptr<BuildingEntity> found_obj = it_entity->second;
			target = dynamic_pointer_cast<T>(found_obj);
		}
		else
		{
			entityIdNotFound.insert(tag);
		}
	}
	else if( str.compare("$")==0 )
	{
		
	}
	else if( str.compare("*")==0 )
	{

	}
	else
	{
		errorStream << __FUNC__ << ": unexpected argument" << std::endl;
	}
}

template<typename T>
void readTypeList( const std::string arg_complete, std::vector<shared_ptr<T> >& vec, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& mapEntities,
	std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound)
{
	// example: (IFCPARAMETERVALUE(0.5),*,IFCPARAMETERVALUE(2.0))
	char* pos_opening = nullptr;
	char* pos_closing = nullptr;
	char* ch = (char*)arg_complete.c_str();
	findLeadingTrailingParanthesis( ch, pos_opening, pos_closing );
	if( pos_opening == nullptr || pos_closing == nullptr )
	{
		if( arg_complete.compare("$") == 0 )
		{
			// empty list
			return;
		}
		std::stringstream err;
		err << "num_opening != num_closing : " << arg_complete.c_str() << std::endl;
		throw BuildingException( err.str(), __FUNC__ );
	}
	std::string arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<std::string> list_items;
	tokenizeList( arg, list_items );

	for( size_t i=0; i<list_items.size(); ++i )
	{
		std::string& item = list_items[i];
		shared_ptr<T> type_obj = T::createObjectFromSTEP( item, mapEntities, errorStream, entityIdNotFound );
		if( type_obj )
		{
			vec.push_back( type_obj );
		}
	}
}

template<typename select_t>
void readSelectType( const std::string& item, shared_ptr<select_t>& result, const std::string& defaultType,
	const std::unordered_map<int, shared_ptr<BuildingEntity> >& mapEntities, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound)
{
	char* ch = (char*)item.c_str();
	if( *ch == '#' )
	{
		++ch;
		const int id = std::stoi( ch );
		auto it_entity = mapEntities.find( id );
		if( it_entity != mapEntities.end() )
		{
			shared_ptr<BuildingEntity> found_obj = it_entity->second;
			result = dynamic_pointer_cast<select_t>(found_obj);
		}
		else
		{
			entityIdNotFound.insert(id);
		}
		return;
	}
	
	// could be type like IFCPARAMETERVALUE(90)
	std::string type_name;
	std::string inline_arg;
	tokenizeInlineArgument( item, type_name, inline_arg );

	if(type_name.size() == 0 )
	{
		return;
	}

	convertStringToUpperCase(type_name);
	
	shared_ptr<BuildingObject> type_instance = IFC4X3::TypeFactory::createTypeObject(type_name.c_str(), inline_arg, mapEntities, errorStream, entityIdNotFound );
	if( type_instance )
	{
		result = dynamic_pointer_cast<select_t>(type_instance);
		return;
	}
	else if (defaultType.size() > 0)
	{
		if (inline_arg.size() == 0 && type_name.size() > 0)
		{
			inline_arg = type_name;
		}
		shared_ptr<BuildingObject> type_instance = IFC4X3::TypeFactory::createTypeObject(defaultType.c_str(), inline_arg, mapEntities, errorStream, entityIdNotFound);
		if (type_instance)
		{
			result = dynamic_pointer_cast<select_t>(type_instance);
			return;
		}
	}

	errorStream << "unhandled select argument: " << item << " in function readSelectType" << std::endl;
}

template<typename select_t>
void readSelectList( const std::string& arg_complete, std::vector<shared_ptr<select_t> >& vec, const std::string& defaultType,
	const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& mapEntities, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound)
{
	// example: (#287,#291,#295,#299) or (IfcLabel('label'),'',IfcLengthMeasure(2.0),#299)
	char* pos_opening = nullptr;
	char* pos_closing = nullptr;
	char* ch = (char*)arg_complete.c_str();
	findLeadingTrailingParanthesis( ch, pos_opening, pos_closing );
	if( pos_opening == nullptr || pos_closing == nullptr )
	{
		if( arg_complete.compare("$") == 0 )
		{
			// empty list
			return;
		}
		std::stringstream err;
		err << "num_opening != num_closing : " << arg_complete.c_str() << std::endl;
		throw BuildingException( err.str().c_str(), __FUNC__ );
	}
	std::string arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<std::string> list_items;
	tokenizeList( arg, list_items );
	
	for( size_t i=0; i<list_items.size(); ++i )
	{
		std::string& item = list_items[i];

		shared_ptr<select_t> select_object;
		try
		{
			readSelectType( item, select_object, defaultType, mapEntities, errorStream, entityIdNotFound );
		}
		catch( BuildingException& e )
		{
			errorStream << e.what();
		}
		if( select_object )
		{
			vec.push_back( select_object );
		}
	}
}

template<typename T>
void readEntityReferenceList( const char* arg_complete, std::vector<shared_ptr<T> >& vec, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& mapEntities,
	std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound)
{
	// example: (#287,#291,#295,#299)
	char* pos_opening = nullptr;
	char* pos_closing = nullptr;
	char* ch = (char*)arg_complete;
	findLeadingTrailingParanthesis( ch, pos_opening, pos_closing );
	if( pos_opening == nullptr || pos_closing == nullptr )
	{
		if( arg_complete != nullptr )
		{
			if( *arg_complete == '$' )
			{ 
				// empty list
				return;
			}
		}
		errorStream << "num_opening != num_closing " << std::endl;
		return;
	}
	std::string arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<int> list_items;
	tokenizeEntityList( arg, list_items );
	
	BuildingModelMapType<int,shared_ptr<BuildingEntity> >::const_iterator it_entity;
	for( size_t i=0; i<list_items.size(); ++i )
	{
		const int id = list_items[i];
		it_entity = mapEntities.find( id );
		if( it_entity != mapEntities.end() )
		{
			shared_ptr<BuildingEntity> found_obj = it_entity->second;
			vec.push_back( dynamic_pointer_cast<T>(found_obj) );
		}
		else
		{
			entityIdNotFound.insert( id );
		}
	}
}

template<typename T>
void readEntityReferenceList( const std::string& str, std::vector<shared_ptr<T> >& vec, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& mapEntities,
	std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound)
{
	// example: (#287,#291,#295,#299)
	char* ch = (char*)str.c_str();
	readEntityReferenceList( ch, vec, mapEntities, errorStream, entityIdNotFound);
}

template<typename T>
void readEntityReferenceList2D( const std::string& str, std::vector<std::vector<shared_ptr<T> > >& vec, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& mapEntities,
	std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound)
{
	// example: ((#287,#291,#295,#299),(#287,#291,#295,#299))
	char* ch = (char*)str.c_str();

	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}
	
	char* last_token = ch;
	int num_par_open = 0;
	while( *ch != '\0' )
	{
		if( *ch == ')' )
		{
			--num_par_open;
			if( num_par_open == 0 )
			{
				// last list
				vec.resize(vec.size()+1);
				readEntityReferenceList( std::string( last_token, ch-last_token ), vec.back(), mapEntities, errorStream, entityIdNotFound );
				return;
			}
		}
		else if( *ch == '(' )
		{
			++num_par_open;
			if( num_par_open == 1 )
			{
				last_token = ch+1;
			}
		}
		else if( *ch == ',' )
		{
			if( num_par_open == 1 )
			{
				vec.resize(vec.size()+1);
				readEntityReferenceList( std::string( last_token, ch-last_token ), vec.back(), mapEntities, errorStream, entityIdNotFound );
				last_token = ch+1;
			}
		}
		++ch;
	}
	// no closing parenthesis found
	std::stringstream err;
	err << "no closing parenthesis found: " << str << std::endl;
	throw BuildingException( err.str(), __FUNC__ );
}

template<typename T>
void readEntityReferenceList3D( const std::string& str, std::vector<std::vector<std::vector<shared_ptr<T> > > >& vec, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& mapEntities,
	std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound)
{
	// example: (((#287,#291,#295,#299),(#287,#291,#295,#299)),((#287,#291,#295,#299),(#287,#291,#295,#299)))
	const size_t argsize = str.size();
	if( argsize < 8 )
	{
		return;
	}
	
	int num_par_open = 0;
	char* ch = (char*)str.c_str();
	char* last_token = ch;

	while( *ch != '\0' )
	{
		if( *ch == ')' )
		{
			--num_par_open;
			if( num_par_open == 0 )
			{
				// last list
				vec.resize(vec.size()+1);
				std::string inner_argument( last_token, ch-last_token );
				readEntityReferenceList2D( inner_argument, vec.back(), mapEntities, errorStream, entityIdNotFound );
				return;
			}
		}
		else if( *ch == '(' )
		{
			++num_par_open;
			if( num_par_open == 1 )
			{
				last_token = ch+1;
			}
		}
		else if( *ch == ',' )
		{
			if( num_par_open == 1 )
			{
				vec.resize(vec.size()+1);
				std::string inner_argument( last_token, ch-last_token );
				readEntityReferenceList2D( inner_argument, vec.back(), mapEntities, errorStream, entityIdNotFound );
				last_token = ch+1;
			}
		}
		++ch;
	}
	// no closing parenthesis found
	std::stringstream err;
	err << "no closing parenthesis found: " << str << std::endl;
	throw BuildingException( err.str(), __FUNC__ );
}
