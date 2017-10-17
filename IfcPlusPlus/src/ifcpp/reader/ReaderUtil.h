/* -*-c++-*- IFC++ www.ifcquery.com
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
#include <locale>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/optional.hpp>
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/IFC4/IfcPPTypeFactory.h"

#ifdef _MSC_VER
#include <cstdio>
#include <cstring>
#pragma warning(disable : 4996)
#endif

void readBoolList( const std::wstring& str, std::vector<bool>& vec );
void readIntList( const std::wstring& str, std::vector<int>& vec );
void readIntList2D( const std::wstring& str, std::vector<std::vector<int> >& vec );
void readIntList3D( const std::wstring& str, std::vector<std::vector<std::vector<int> > >& vec );
void readRealList( const std::wstring& str, std::vector<double>& vec );
void readRealList2D( const std::wstring& str, std::vector<std::vector<double> >& vec );
void readRealList3D( const std::wstring& str, std::vector<std::vector<std::vector<double> > >& vec );
void readBinary( const std::wstring& str, std::wstring& target );
void readBinaryList( const std::wstring& str, std::vector<std::wstring>& vec );
void readStringList( const std::wstring& str, std::vector<std::wstring>& vec );

void checkOpeningClosingParenthesis( const wchar_t* ch_check );
IFCPP_EXPORT void tokenizeEntityArguments( const std::string& argument_str, std::vector<std::string>& entity_arguments );
IFCPP_EXPORT void tokenizeEntityArguments( const std::wstring& argument_str, std::vector<std::wstring>& entity_arguments );
void tokenizeInlineArgument( const std::wstring input, std::wstring& keyword, std::wstring& inner_argument );
void findLeadingTrailingParanthesis( wchar_t* ch, wchar_t*& pos_opening, wchar_t*& pos_closing );
void tokenizeList( std::wstring& list_str, std::vector<std::wstring>& list_items );
void tokenizeEntityList( std::wstring& list_str, std::vector<int>& list_items );

inline void readIntValue( const std::wstring& str, int& int_value )
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
		int_value = std::stoi( str.c_str() );
	}
}
inline void readIntValue( const std::wstring& str, boost::optional<int>& int_value )
{
	if( str.compare( L"$" ) == 0 )
	{
		int_value = boost::none;
	}
	else if( str.compare( L"*" ) == 0 )
	{
		int_value = boost::none;
	}
	else
	{
		int_value = std::stoi( str.c_str() );
	}
}
inline void readRealValue( const std::wstring& str, double& real_value )
{
	if( str.compare( L"$" ) == 0 )
	{
		real_value = std::numeric_limits<double>::quiet_NaN();
	}
	else if( str.compare( L"*" ) == 0 )
	{
		real_value = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		real_value = std::stod( str.c_str() );
	}
}
inline void readRealValue( const std::wstring& str, boost::optional<double>& real_value )
{
	if( str.compare( L"$" ) == 0 )
	{
		real_value = boost::none;
	}
	else if( str.compare( L"*" ) == 0 )
	{
		real_value = boost::none;
	}
	else
	{
		real_value = std::stod( str.c_str() );
	}
}

void copyToEndOfStepString( char*& stream_pos, char*& stream_pos_source );
IFCPP_EXPORT void decodeArgumentStrings( std::vector<std::string>& entity_arguments, std::vector<std::wstring>& args_out );

inline void readBool( const std::wstring& attribute_value, bool& target )
{
	if( boost::iequals( attribute_value, L".F." ) )
	{
		target = false;
	}
	else if( boost::iequals( attribute_value, L".T." ) )
	{
		target = true;;
	}
}

inline void readLogical( const std::wstring& attribute_value, LogicalEnum& target )
{
	if( boost::iequals(attribute_value, L".F." ) )
	{
		target = LOGICAL_FALSE;
	}
	else if( boost::iequals( attribute_value, L".T." ) )
	{
		target = LOGICAL_TRUE;
	}
	else if( boost::iequals( attribute_value, L".U." ) )
	{
		target = LOGICAL_UNKNOWN;;
	}
}

inline void readInt( const std::wstring& attribute_value, int& target )
{
	target = std::stoi( attribute_value );
}

inline void readReal( const std::wstring& attribute_value, double& target )
{
	target = std::stod( attribute_value.c_str() );
}

inline void readString( const std::wstring& attribute_value, std::wstring& target )
{
	if( attribute_value.size() < 2 )
	{
		target = attribute_value.c_str();
		return;
	}
	if( attribute_value[0] == '\'' && attribute_value[attribute_value.size()-1] == '\'' )
	{
		target = attribute_value.substr( 1, attribute_value.size()-2 ).c_str();
	}
}

template<typename T>
void readTypeOfIntList( const std::wstring& str, std::vector<shared_ptr<T> >& target_vec )
{
	//(38,12,4)
	wchar_t* ch = (wchar_t*)str.c_str();
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
void readTypeOfRealList( const wchar_t* str, std::vector<shared_ptr<T> >& target_vec )
{
	//(.38,12.0,.04)
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
void readTypeOfRealList( const std::wstring& str, std::vector<shared_ptr<T> >& target_vec )
{
	//(.38,12.0,.04)
	wchar_t* ch = (wchar_t*)str.c_str();
	readTypeOfRealList( ch, target_vec );
}


template<typename T>
void readTypeOfRealList2D( const std::wstring& str, std::vector<std::vector<shared_ptr<T> > >& target_vec )
{
	//((.38,12.0,.04),(.38,1.0,346.0),(1.8,1.0,.04))
	char* ch = (char*)str.c_str();

	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}

	if( ch[0] != '(' )
	{
		throw IfcPPException( "string does not start with (", __FUNC__ );
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
				// closing paranthesis found
				target_vec.resize(target_vec.size()+1);
				std::wstring s = str.substr( last_token, i-last_token );
				readTypeOfRealList( s, target_vec.back() );
				return;
			}
		}
		++i;
	}

	// no closing paranthesis found
	std::wstringstream err;
	err << "no closing paranthesis found: " << str << std::endl;
	throw IfcPPException( err.str(), __FUNC__ );
}

template<typename T>
void readTypeOfStringList( const wchar_t* str, std::vector<shared_ptr<T> >& target_vec )
{
	//(.38,12.0,.04)
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
void readEntityReference( const std::wstring& str, shared_ptr<T>& target, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	if( str.length() == 0)
	{
		return;
	}
	if( str.at(0) == '#' )
	{
		int entity_id = std::stoi( str.substr( 1 ) );
		std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it_entity = map_entities.find( entity_id );
		if( it_entity != map_entities.end() )
		{
			shared_ptr<IfcPPEntity> found_obj = it_entity->second;
			target = dynamic_pointer_cast<T>(found_obj);
		}
		else
		{
			std::stringstream err;
			err << "object with id " << entity_id << " not found" << std::endl;
			throw IfcPPException( err.str(), __FUNC__ );
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
		throw IfcPPException( "unexpected argument", __FUNC__ );
	}
}

template<typename T>
void readTypeList( const std::wstring arg_complete, std::vector<shared_ptr<T> >& vec, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	//(IFCPARAMETERVALUE(0.5),*,IFCPARAMETERVALUE(2.0))
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
		throw IfcPPException( err.str(), __FUNC__ );
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
void readSelectType( const std::wstring& item, shared_ptr<select_t>& result, const std::map<int, shared_ptr<IfcPPEntity> >& map_entities )
{
	wchar_t* ch = (wchar_t*)item.c_str();
	if( *ch == '#' )
	{
		++ch;
		const int id = std::stoi( ch );
		auto it_entity = map_entities.find( id );
		if( it_entity != map_entities.end() )
		{
			shared_ptr<IfcPPEntity> found_obj = it_entity->second;
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

	std::string type_name_upper( keyword.begin(), keyword.end() );
	std::transform( type_name_upper.begin(), type_name_upper.end(), type_name_upper.begin(), toupper );
	
	shared_ptr<IfcPPObject> type_instance = IfcPPTypeFactory::createTypeObject( type_name_upper.c_str(), inline_arg, map_entities );
	if( type_instance )
	{
		result = dynamic_pointer_cast<select_t>(type_instance);
		return;
	}

	//shared_ptr<IfcPPEntity> entity_instance( IfcPPEntityFactory::createEntityObject( type_name_upper.c_str() ) );
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
	throw IfcPPException( strs.str() );
}

template<typename select_t>
void readSelectList( const std::wstring& arg_complete, std::vector<shared_ptr<select_t> >& vec, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	//(#287,#291,#295,#299) or (IfcLabel('label'),'',IfcLengthMeasure(2.0),#299)
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
		throw IfcPPException( err.str().c_str(), __FUNC__ );
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
		catch( IfcPPOutOfMemoryException& e)
		{
			throw e;
		}
		catch( IfcPPException& e )
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
		throw IfcPPException( err.str().c_str(), __FUNC__ );
	}
	return;
}

template<typename T>
void readEntityReferenceList( const wchar_t* arg_complete, std::vector<shared_ptr<T> >& vec, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	//(#287,#291,#295,#299)
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
		throw IfcPPException( err.str(), __FUNC__ );
	}
	std::wstring arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<int> list_items;
	tokenizeEntityList( arg, list_items );
	std::vector<int> vec_not_found;
	std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it_entity;
	for( size_t i=0; i<list_items.size(); ++i )
	{
		const int id = list_items[i];
		it_entity = map_entities.find( id );
		if( it_entity != map_entities.end() )
		{
			shared_ptr<IfcPPEntity> found_obj = it_entity->second;
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
		throw IfcPPException( err.str(), __FUNC__ );
	}
}

template<typename T>
void readEntityReferenceList( const std::wstring& str, std::vector<shared_ptr<T> >& vec, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	//(#287,#291,#295,#299)
	wchar_t* ch = (wchar_t*)str.c_str();
	readEntityReferenceList( ch, vec, map_entities );
}

template<typename T>
void readEntityReferenceList2D( const std::wstring& str, std::vector<std::vector<shared_ptr<T> > >& vec, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	//((#287,#291,#295,#299),(#287,#291,#295,#299))
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
	// no closing paranthesis found
	std::wstringstream err;
	err << "no closing paranthesis found: " << str << std::endl;
	throw IfcPPException( err.str(), __FUNC__ );
}

template<typename T>
void readEntityReferenceList3D( const std::string& str, std::vector<std::vector<std::vector<shared_ptr<T> > > >& vec, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	//(((#287,#291,#295,#299),(#287,#291,#295,#299)),((#287,#291,#295,#299),(#287,#291,#295,#299)))
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
	// no closing paranthesis found
	std::stringstream err;
	err << "no closing paranthesis found: " << str << std::endl;
	throw IfcPPException( err.str(), __FUNC__ );
}
