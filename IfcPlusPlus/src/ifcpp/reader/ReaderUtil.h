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

#pragma once

#include <algorithm>
#include <locale>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/IfcPPTypeEnums.h"
#include "ifcpp/IfcPPEntityEnums.h"

#ifndef WIN32
#include <cstdio>
#include <cstring>
#endif

#pragma warning(disable : 4996)

void readBoolList( const std::wstring& str, std::vector<bool>& vec );
void readIntList( const std::wstring& str, std::vector<int>& vec );
void readIntList2D( const std::wstring& str, std::vector<std::vector<int> >& vec );
void readIntList3D( const std::wstring& str, std::vector<std::vector<std::vector<int> > >& vec );
void readDoubleList( const std::wstring& str, std::vector<double>& vec );
void readDoubleList2D( const std::wstring& str, std::vector<std::vector<double> >& vec );
void readDoubleList3D( const std::wstring& str, std::vector<std::vector<std::vector<double> > >& vec );
void readConstCharList( const std::wstring& str, std::vector<const char*>& vec );
void readStringList( const std::wstring& str, std::vector<std::wstring>& vec );

void checkOpeningClosingParenthesis( const wchar_t* ch_check );
void tokenizeEntityArguments( const std::string& argument_str, std::vector<std::string>& entity_arguments );
void tokenizeEntityArguments( const std::wstring& argument_str, std::vector<std::wstring>& entity_arguments );
void tokenizeInlineArgument( const std::wstring input, std::wstring& keyword, std::wstring& inner_argument );
void findLeadingTrailingParanthesis( wchar_t* ch, wchar_t*& pos_opening, wchar_t*& pos_closing );
void tokenizeList( std::wstring& list_str, std::vector<std::wstring>& list_items );
void tokenizeEntityList( std::wstring& list_str, std::vector<int>& list_items );

void readIntValue( const std::wstring& str, int& value );
void readRealValue( const std::wstring& str, double& value );
void copyToEndOfStepString( char*& stream_pos, char*& stream_pos_source );
void decodeArgumentStrings( std::vector<std::string>& entity_arguments, std::vector<std::wstring>& args_out );
IfcPPTypeEnum findTypeEnumForString( const std::wstring& type_name );
IfcPPEntityEnum findEntityEnumForString( const std::wstring& entity_name );
void readInlineTypeOrEntity( const std::wstring& arg, shared_ptr<IfcPPObject>& result, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities );
void readInlineTypeOrEntity( const std::wstring& keyword, const std::wstring& inline_arg, shared_ptr<IfcPPObject>& result, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities );

template<typename T>
void readTypeOfIntList( const std::wstring& str, std::vector<shared_ptr<T> >& vec )
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
        //if (*ch == ',')
          //      return ((wchar_t *)s);
        //return (NULL);

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
				vec.push_back(shared_ptr<T>(new T(int_value)));
			}
		}

		//ch = strchr( ch, ',' );
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
void readTypeOfRealList( const wchar_t* str, std::vector<shared_ptr<T> >& vec )
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
				
				vec.push_back(shared_ptr<T>(new T(real_value)));
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
void readTypeOfRealList( const std::wstring& str, std::vector<shared_ptr<T> >& vec )
{
	//(.38,12.0,.04)
	wchar_t* ch = (wchar_t*)str.c_str();
	readTypeOfRealList( ch, vec );
}


template<typename T>
void readTypeOfRealList2D( const std::wstring& str, std::vector<std::vector<shared_ptr<T> > >& vec )
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
		throw IfcPPException( "string does not start with (", __func__ );
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
				vec.resize(vec.size()+1);
				std::wstring s = str.substr( last_token, i-last_token );
				readTypeOfRealList( s, vec.back() );
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
				vec.resize(vec.size()+1);
				std::wstring s = str.substr( last_token, i-last_token );
				readTypeOfRealList( s, vec.back() );
				return;
			}
		}
		++i;
	}

	// no closing paranthesis found
	std::wstringstream err;
	err << "no closing paranthesis found: " << str << std::endl;
	throw IfcPPException( err.str(), __func__ );
}

template<typename T>
void readEntityReference( const std::wstring& str, shared_ptr<T>& smart, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
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
			smart = dynamic_pointer_cast<T>(found_obj);
		}
		else
		{
			std::stringstream err;
			err << "object with id " << entity_id << " not found" << std::endl;
			throw IfcPPException( err.str(), __func__ );
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
		throw IfcPPException( "unexpected argument", __func__ );
	}
}

template<typename T>
void readTypeList( const std::wstring arg_complete, std::vector<shared_ptr<T> >& vec )
{
	//(IfcLabel('label'),'',IfcLengthMeasure(2.0),#299)
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
		throw IfcPPException( err.str(), __func__ );
	}
	std::wstring arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<std::wstring> list_items;
	tokenizeList( arg, list_items );

	for( size_t i=0; i<list_items.size(); ++i )
	{
		std::wstring& item = list_items[i];
		vec.push_back( T::createObjectFromStepData( item ) );
	}
}

template<typename T>
void readSelectList( const std::wstring& arg_complete, std::vector<shared_ptr<T> >& vec, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	//(#287,#291,#295,#299)
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
		throw IfcPPException( err.str(), __func__ );
	}
	std::wstring arg( pos_opening+1, pos_closing-pos_opening-1 );
	std::vector<std::wstring> list_items;
	tokenizeList( arg, list_items );

	std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it_entity;
	for( size_t i=0; i<list_items.size(); ++i )
	{
		std::wstring& item = list_items[i];
		wchar_t* ch = (wchar_t*)item.c_str();
		if( *ch == '#' )
		{
			++ch;
			const int id = std::stoi( ch );
			it_entity = map_entities.find( id );
			if( it_entity != map_entities.end() )
			{
				shared_ptr<IfcPPEntity> found_obj = it_entity->second;
				vec.push_back( dynamic_pointer_cast<T>(found_obj) );
			}
		}
		else
		{
			// could be type like IFCPARAMETERVALUE(90)
			std::wstring keyword;
			std::wstring inline_arg;
			tokenizeInlineArgument( item, keyword, inline_arg );
			shared_ptr<IfcPPObject> result_object;
			readInlineTypeOrEntity( item, result_object, map_entities );
			if( result_object )
			{
				shared_ptr<T> ptr_t = dynamic_pointer_cast<T>( result_object );
				if( ptr_t )
				{
					vec.push_back( ptr_t );
					continue;
				}
			}
			std::wstringstream strs;
			strs << "unhandled inline argument: " << item << " in function IfcAppliedValueSelect::readStepData" << std::endl;
			throw IfcPPException( strs.str() );

		}
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
		throw IfcPPException( err.str(), __func__ );
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
		throw IfcPPException( err.str(), __func__ );
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
	throw IfcPPException( err.str(), __func__ );
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
	throw IfcPPException( err.str(), __func__ );
}
