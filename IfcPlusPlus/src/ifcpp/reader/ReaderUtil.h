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

#pragma warning ( disable: 4996 )  // for boost\random\detail\polynomial.hpp

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <cwctype>
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingObject.h"
#include "ifcpp/IFC4/TypeFactory.h"

#ifdef _MSC_VER
#include <cstdio>
#include <cstring>
#pragma warning(disable : 4996)
#endif

void readBoolList( const std::wstring& str, std::vector<bool>& vec );
void readIntegerList( const std::wstring& str, std::vector<int>& vec );
void readIntegerList2D( const std::wstring& str, std::vector<std::vector<int> >& vec );
void readIntegerList3D( const std::wstring& str, std::vector<std::vector<std::vector<int> > >& vec );
void readRealList( const std::wstring& str, std::vector<double>& vec );
void readRealList2D( const std::wstring& str, std::vector<std::vector<double> >& vec );
void readRealList3D( const std::wstring& str, std::vector<std::vector<std::vector<double> > >& vec );
void readBinary( const std::wstring& str, std::wstring& target );
void readBinaryString(const std::wstring& attribute_value, std::wstring& target);
void readBinaryList( const std::wstring& str, std::vector<std::wstring>& vec );
void readStringList( const std::wstring& str, std::vector<std::wstring>& vec );

IFCQUERY_EXPORT void tokenizeEntityArguments( const std::string& argument_str, std::vector<std::string>& entity_arguments );
IFCQUERY_EXPORT void tokenizeEntityArguments( const std::wstring& argument_str, std::vector<std::wstring>& entity_arguments );
void tokenizeInlineArgument(std::wstring arg, std::wstring& keyword, std::wstring& inline_arg);
void tokenizeList( std::wstring& list_str, std::vector<std::wstring>& list_items );
void tokenizeEntityList( std::wstring& list_str, std::vector<int>& list_items );
void findLeadingTrailingParanthesis(wchar_t* ch, wchar_t*& pos_opening, wchar_t*& pos_closing);
void findEndOfString(char*& stream_pos);
void findEndOfWString(wchar_t*& stream_pos);
void checkOpeningClosingParenthesis(const wchar_t* ch_check);

inline bool std_iequal(const std::wstring& a, const std::wstring& b)
{
	if (a.size() == b.size())
	{
		return std::equal(a.begin(), a.end(), b.begin(), [](const wchar_t l, const wchar_t r) { return std::towupper(l) == std::towupper(r); });
	}
	return false;
}

inline void readIntegerValue( const std::wstring& str, int& int_value )
{
	if( str.compare( L"$" ) == 0 )
	{
		int_value = std::numeric_limits<int>::quiet_NaN();
	}
	else if( str.compare( L"*" ) == 0 )
	{
		int_value = std::numeric_limits<int>::quiet_NaN();
	}
	else
	{
		int_value = std::stoi( str );
	}
}

void copyToEndOfStepString( char*& stream_pos, char*& stream_pos_source );
IFCQUERY_EXPORT void decodeArgumentStrings( std::vector<std::string>& entity_arguments, std::vector<std::wstring>& args_out );

inline void readBool( const std::wstring& attribute_value, bool& target )
{
	if( std_iequal( attribute_value, L".F." ) )
	{
		target = false;
	}
	else if( std_iequal( attribute_value, L".T." ) )
	{
		target = true;;
	}
}

inline void readLogical( const std::wstring& attribute_value, LogicalEnum& target )
{
	if( std_iequal(attribute_value, L".F." ) )
	{
		target = LOGICAL_FALSE;
	}
	else if( std_iequal( attribute_value, L".T." ) )
	{
		target = LOGICAL_TRUE;
	}
	else if( std_iequal( attribute_value, L".U." ) )
	{
		target = LOGICAL_UNKNOWN;;
	}
}

inline void readInteger( const std::wstring& attribute_value, int& target )
{
	target = std::stoi( attribute_value );
}

inline void readReal( const std::wstring& attribute_value, double& target )
{
	target = std::stod( attribute_value );
}

inline void readString( const std::wstring& attribute_value, std::wstring& target )
{
	if( attribute_value.size() < 2 )
	{
		target = attribute_value;
		return;
	}
	if( attribute_value[0] == '\'' && attribute_value[attribute_value.size()-1] == '\'' )
	{
		target = attribute_value.substr( 1, attribute_value.size()-2 );
	}
}

template<typename T>
void readTypeOfIntegerList( const std::wstring& str, std::vector<shared_ptr<T> >& target_vec )
{
	// example: (38,12,4)
	wchar_t const* ch = str.c_str();
	wchar_t const* last_token = nullptr;

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

		while( *ch != ',' && *ch != L'\0' && *ch != ')' )
		{
			++ch;
		}

		if( last_token != nullptr )
		{
			size_t length_str = ch - last_token;
			if( length_str > 0 )
			{
				std::wstring int_str(last_token, length_str);
				int int_value = 0;
				try
				{
					int_value = std::stoi(int_str);
				}
				catch( std::exception&  )
				{

				}
				target_vec.push_back(shared_ptr<T>(new T(int_value)));
			}
		}

		if( *ch == L'\0' )
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
void readTypeOfIntegerList2D( const std::wstring& str, std::vector<std::vector<shared_ptr<T> > >& target_vec )
{
	// example: ((8,12,4),(58,10,34),(18,10,4))
	wchar_t const* ch = str.c_str();

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
				std::wstring s = str.substr( last_token, i-last_token );
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
				std::wstring s = str.substr( last_token, i-last_token );
				readTypeOfIntegerList( s, target_vec.back() );
				return;
			}
		}
		++i;
	}

	// no closing parenthesis found
	std::wstringstream err;
	err << "no closing parenthesis found: " << str << std::endl;
	throw BuildingException( err.str(), __FUNC__ );
}

template<typename T>
void readTypeOfRealList( const std::wstring& str, std::vector<shared_ptr<T> >& target_vec )
{
	// example: (.38,12.0,.04)
	wchar_t const* ch = str.c_str();
	wchar_t const* last_token = nullptr;

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

		while( *ch != ',' && *ch != L'\0' && *ch != ')' )
		{
			++ch;
		}

		if( last_token != nullptr )
		{
			size_t length_str = ch - last_token;
			if( length_str > 0 )
			{
				std::wstring double_str(last_token, length_str);
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

		if( *ch == L'\0' )
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
void readTypeOfRealList2D( const std::wstring& str, std::vector<std::vector<shared_ptr<T> > >& target_vec )
{
	// example: ((.38,12.0,.04),(.38,1.0,346.0),(1.8,1.0,.04))
	wchar_t const* ch = str.c_str();

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
				std::wstring s = str.substr( last_token, i-last_token );
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
				std::wstring s = str.substr( last_token, i-last_token );
				readTypeOfRealList( s, target_vec.back() );
				return;
			}
		}
		++i;
	}

	// no closing parenthesis found
	std::wstringstream err;
	err << "no closing parenthesis found: " << str << std::endl;
	throw BuildingException( err.str(), __FUNC__ );
}

template<typename T>
void readTypeOfStringList( const wchar_t* str, std::vector<shared_ptr<T> >& target_vec )
{
	// example: ('Tahoma')
	wchar_t* ch = (wchar_t*)str;
	wchar_t* last_token = nullptr;

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
			// ingore leading space characters
			++ch;
			continue;
		}

		while( *ch != ',' && *ch != L'\0' && *ch != ')' )
		{
			++ch;
		}

		if( last_token != nullptr )
		{
			size_t length_str = ch - last_token;
			if( length_str > 0 )
			{
				std::wstring str_value( last_token, length_str );
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

		if( *ch == L'\0' )
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
void readTypeOfStringList( const std::wstring& str, std::vector<shared_ptr<T> >& target_vec )
{
	wchar_t* ch = (wchar_t*)str.c_str();
	readTypeOfStringList( ch, target_vec );
}

template<typename T>
void readEntityReference( const std::wstring& str, shared_ptr<T>& target, const std::map<int,shared_ptr<BuildingEntity> >& map_entities )
{
	if( str.length() == 0)
	{
		return;
	}
	if( str.at(0) == '#' )
	{
		int entity_id = std::stoi( str.substr( 1 ) );
		std::map<int,shared_ptr<BuildingEntity> >::const_iterator it_entity = map_entities.find( entity_id );
		if( it_entity != map_entities.end() )
		{
			shared_ptr<BuildingEntity> found_obj = it_entity->second;
			target = dynamic_pointer_cast<T>(found_obj);
		}
		else
		{
			std::stringstream err;
			err << "object with id " << entity_id << " not found" << std::endl;
			throw BuildingException( err.str(), __FUNC__ );
		}
	}
	else if( str.compare(L"$")==0 )
	{
		
	}
	else if( str.compare(L"*")==0 )
	{

	}
	else
	{
		throw BuildingException( "unexpected argument", __FUNC__ );
	}
}

template<typename T>
void readTypeList( const std::wstring arg_complete, std::vector<shared_ptr<T> >& vec, const std::map<int,shared_ptr<BuildingEntity> >& map_entities )
{
	// example: (IFCPARAMETERVALUE(0.5),*,IFCPARAMETERVALUE(2.0))
	wchar_t* pos_opening = nullptr;
	wchar_t* pos_closing = nullptr;
	wchar_t* ch = (wchar_t*)arg_complete.c_str();
	findLeadingTrailingParanthesis( ch, pos_opening, pos_closing );
	if( pos_opening == nullptr || pos_closing == nullptr )
	{
		if( arg_complete.compare(L"$") == 0 )
		{
			// empty list
			return;
		}
		std::wstringstream err;
		err << "num_opening != num_closing : " << arg_complete.c_str() << std::endl;
		throw BuildingException( err.str(), __FUNC__ );
	}
	std::wstring arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<std::wstring> list_items;
	tokenizeList( arg, list_items );

	for( size_t i=0; i<list_items.size(); ++i )
	{
		std::wstring& item = list_items[i];
		shared_ptr<T> type_obj = T::createObjectFromSTEP( item, map_entities );
		if( type_obj )
		{
			vec.push_back( type_obj );
		}
	}
}

template<typename select_t>
void readSelectType( const std::wstring& item, shared_ptr<select_t>& result, const std::map<int, shared_ptr<BuildingEntity> >& map_entities )
{
	wchar_t* ch = (wchar_t*)item.c_str();
	if( *ch == '#' )
	{
		++ch;
		const int id = std::stoi( ch );
		auto it_entity = map_entities.find( id );
		if( it_entity != map_entities.end() )
		{
			shared_ptr<BuildingEntity> found_obj = it_entity->second;
			result = dynamic_pointer_cast<select_t>(found_obj);
		}
		return;
	}
	
	// could be type like IFCPARAMETERVALUE(90)
	std::wstring keyword;
	std::wstring inline_arg;
	tokenizeInlineArgument( item, keyword, inline_arg );

	if( keyword.size() == 0 )
	{
		return;
	}

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> StringConverter;
	std::string type_name_upper = StringConverter.to_bytes(keyword);
	std::transform( type_name_upper.begin(), type_name_upper.end(), type_name_upper.begin(), toupper );
	
	shared_ptr<BuildingObject> type_instance = TypeFactory::createTypeObject( type_name_upper.c_str(), inline_arg, map_entities );
	if( type_instance )
	{
		result = dynamic_pointer_cast<select_t>(type_instance);
		return;
	}

	//shared_ptr<BuildingEntity> entity_instance( BuildingEntityFactory::createEntityObject( type_name_upper.c_str() ) );
	//if( entity_instance )
	//{
	//	entity_instance->m_id = -1;
	//	std::vector<std::wstring> args;
	//	args.push_back( inline_arg );
	//	entity_instance->readStepArguments( args, map_entities );
	//	result = dynamic_pointer_cast<select_t>(entity_instance);
	//	return;
	//}

	std::wstringstream strs;
	strs << "unhandled select argument: " << item << " in function " << __FUNC__ << std::endl;
	throw BuildingException( strs.str() );
}

template<typename select_t>
void readSelectList( const std::wstring& arg_complete, std::vector<shared_ptr<select_t> >& vec, const std::map<int,shared_ptr<BuildingEntity> >& map_entities )
{
	// example: (#287,#291,#295,#299) or (IfcLabel('label'),'',IfcLengthMeasure(2.0),#299)
	wchar_t* pos_opening = nullptr;
	wchar_t* pos_closing = nullptr;
	wchar_t* ch = (wchar_t*)arg_complete.c_str();
	findLeadingTrailingParanthesis( ch, pos_opening, pos_closing );
	if( pos_opening == nullptr || pos_closing == nullptr )
	{
		if( arg_complete.compare(L"$") == 0 )
		{
			// empty list
			return;
		}
		std::stringstream err;
		err << "num_opening != num_closing : " << arg_complete.c_str() << std::endl;
		throw BuildingException( err.str().c_str(), __FUNC__ );
	}
	std::wstring arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<std::wstring> list_items;
	tokenizeList( arg, list_items );
	
	std::stringstream err;
	for( size_t i=0; i<list_items.size(); ++i )
	{
		std::wstring& item = list_items[i];

		shared_ptr<select_t> select_object;
		try
		{
			readSelectType( item, select_object, map_entities );
		}
		catch( OutOfMemoryException& e)
		{
			throw e;
		}
		catch( BuildingException& e )
		{
			err << e.what();
		}
		if( select_object )
		{
			vec.push_back( select_object );
		}
	}
	if( err.tellp() > 0 )
	{
		throw BuildingException( err.str().c_str(), __FUNC__ );
	}
	return;
}

template<typename T>
void readEntityReferenceList( const wchar_t* arg_complete, std::vector<shared_ptr<T> >& vec, const std::map<int,shared_ptr<BuildingEntity> >& map_entities )
{
	// example: (#287,#291,#295,#299)
	wchar_t* pos_opening = nullptr;
	wchar_t* pos_closing = nullptr;
	wchar_t* ch = (wchar_t*)arg_complete;
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
		std::wstringstream err;
		err << "num_opening != num_closing : " << arg_complete << std::endl;
		throw BuildingException( err.str(), __FUNC__ );
	}
	std::wstring arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<int> list_items;
	tokenizeEntityList( arg, list_items );
	std::vector<int> vec_not_found;
	std::map<int,shared_ptr<BuildingEntity> >::const_iterator it_entity;
	for( size_t i=0; i<list_items.size(); ++i )
	{
		const int id = list_items[i];
		it_entity = map_entities.find( id );
		if( it_entity != map_entities.end() )
		{
			shared_ptr<BuildingEntity> found_obj = it_entity->second;
			vec.push_back( dynamic_pointer_cast<T>(found_obj) );
		}
		else
		{
			vec_not_found.push_back( id );
		}
	}

	// in case there are unresolved references
	if( vec_not_found.size() > 0 )
	{
		std::stringstream err;
		err << "object with id ";
		
		for( size_t i=0; i<vec_not_found.size(); ++i )
		{
			err	<< vec_not_found[i];
			if( i <vec_not_found.size()-1 )
			{
				err << ", ";
			}
		}

		err << "  not found" << std::endl;
		throw BuildingException( err.str(), __FUNC__ );
	}
}

template<typename T>
void readEntityReferenceList( const std::wstring& str, std::vector<shared_ptr<T> >& vec, const std::map<int,shared_ptr<BuildingEntity> >& map_entities )
{
	// example: (#287,#291,#295,#299)
	wchar_t* ch = (wchar_t*)str.c_str();
	readEntityReferenceList( ch, vec, map_entities );
}

template<typename T>
void readEntityReferenceList2D( const std::wstring& str, std::vector<std::vector<shared_ptr<T> > >& vec, const std::map<int,shared_ptr<BuildingEntity> >& map_entities )
{
	// example: ((#287,#291,#295,#299),(#287,#291,#295,#299))
	wchar_t* ch = (wchar_t*)str.c_str();

	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}
	
	wchar_t* last_token = ch;
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
				readEntityReferenceList( std::wstring( last_token, ch-last_token ), vec.back(), map_entities );
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
				readEntityReferenceList( std::wstring( last_token, ch-last_token ), vec.back(), map_entities );
				last_token = ch+1;
			}
		}
		++ch;
	}
	// no closing parenthesis found
	std::wstringstream err;
	err << "no closing parenthesis found: " << str << std::endl;
	throw BuildingException( err.str(), __FUNC__ );
}

template<typename T>
void readEntityReferenceList3D( const std::string& str, std::vector<std::vector<std::vector<shared_ptr<T> > > >& vec, const std::map<int,shared_ptr<BuildingEntity> >& map_entities )
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
				readEntityReferenceList2D( inner_argument, vec.back(), map_entities );
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
				readEntityReferenceList2D( inner_argument, vec.back(), map_entities );
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
