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

#define _USE_MATH_DEFINES
#include <cmath>
#ifdef WIN32
#include <windows.h>
#endif

#include <cctype>
#include <codecvt>
#include <limits>

#include "ifcpp/model/BuildingException.h"
#include "ReaderUtil.h"

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif

static short convertToHex(unsigned char mc)
{
	short returnValue;

	if( mc >= '0' && mc <= '9' )
	{
		returnValue = static_cast<short>(mc) - static_cast<short>('0');
	}
	else if( mc >= 'A' && mc <= 'F' )
	{
		returnValue = 10 + static_cast<short>(mc) - static_cast<short>('A');
	}
	else if( mc >= 'a' && mc <= 'f' )
	{
		returnValue = 10 + static_cast<short>(mc) - static_cast<short>('a');
	}
	else
	{
		returnValue = 0;
	}
	return (returnValue);
}


static wchar_t Hex2Wchar(unsigned char h1, unsigned char h2 )
{
	wchar_t returnValue = (convertToHex(h1) << 4) + convertToHex(h2);
	return (returnValue);
}

static wchar_t Hex4Wchar(unsigned char h1, unsigned char h2, unsigned char h3, unsigned char h4 )
{
	wchar_t returnValue = (convertToHex(h1)<< 12) + (convertToHex(h2) << 8) +(convertToHex(h3) << 4) + convertToHex(h4);
	return (returnValue);
}

std::string wstring2string(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> StringConverter;
	return StringConverter.to_bytes(wstr);
}

std::wstring string2wstring(std::string& str_in)
{
	std::wstring wstr_out;
	wstr_out.assign(str_in.begin(), str_in.end());
	return wstr_out;
}

void checkOpeningClosingParenthesis( const wchar_t* ch_check )
{
	int num_opening=0;
	int num_closing=0;
	while( *ch_check != '\0' )
	{
		if( *ch_check == '(' )
		{
			++num_opening;
		}
		else if( *ch_check == ')' )
		{
			++num_closing;
		}
		else if ( *ch_check == '\'' )
		{
			findEndOfWString( ch_check );
			continue;
		}
		++ch_check;
	}
	if( num_opening != num_closing )
	{
		std::stringstream err;
		err << "checkOpeningClosingParenthesis: num_opening != num_closing : " << ch_check << std::endl;
		throw BuildingException( err.str(), __FUNC__ );
	}
}

void findLeadingTrailingParanthesis( wchar_t* ch, wchar_t*& pos_opening, wchar_t*& pos_closing )
{
	int num_opening = 0;
	while( *ch != '\0' )
	{
		if( *ch == '\'' )
		{
			++ch;
			// beginning of string, continue to end
			while( *ch != '\0' )
			{
				if( *ch == '\'' )
				{
					break;
				}
				++ch;
			}
			++ch;
			continue;
		}

		if( *ch == '(' )
		{
			if( num_opening == 0 )
			{
				pos_opening = ch;
			}
			++num_opening;
		}
		else if( *ch == ')' )
		{
			--num_opening;
			if( num_opening == 0 )
			{
				pos_closing = ch;
			}
		}
		++ch;
	}
}

void tokenizeList( std::wstring& list_str, std::vector<std::wstring>& list_items )
{
	if( list_str.empty() )
	{
		return;
	}
	wchar_t* stream_pos = const_cast<wchar_t*>(list_str.c_str());
	wchar_t* last_token = stream_pos;
	int numNestedLists = 0;
	while( *stream_pos != '\0' )
	{
		if (*stream_pos == '(')
		{
			++stream_pos;
			++numNestedLists;
			continue;
		}
		else if (*stream_pos == ')')
		{
			++stream_pos;
			--numNestedLists;
			continue;
		}
		else if( *stream_pos == '\'' )
		{
			++stream_pos;
			// beginning of string, continue to end
			while( *stream_pos != '\0' )
			{
				if( *stream_pos == '\'' )
				{
					break;
				}
				++stream_pos;
			}
			++stream_pos;
			continue;
		}
		
		if( *stream_pos == ',' && numNestedLists == 0)
		{
			std::wstring item( last_token, stream_pos-last_token );
			list_items.push_back( item );

			++stream_pos;
			while( isspace(*stream_pos) ) { ++stream_pos; }
			last_token = stream_pos;
			if( *stream_pos == '\0' )
			{
				throw BuildingException( "tokenizeList: *stream_pos == '\0'", __FUNC__ );
			}
			continue;
		}
		++stream_pos;
	}
	// pick up last element
	if( last_token != nullptr )
	{
		if( last_token != stream_pos )
		{
			std::wstring item( last_token, stream_pos-last_token );
			list_items.push_back( item );
		}
	}
}

void tokenizeEntityList( std::wstring& list_str, std::vector<int>& list_items )
{
	if( list_str.empty() )
	{
		return;
	}
	wchar_t* stream_pos = const_cast<wchar_t*>(list_str.c_str());
	while( *stream_pos != '\0' )
	{
		// skip whitespace
		while( isspace( *stream_pos ) ) { ++stream_pos; }
		
		if( *stream_pos == '#' )
		{
			++stream_pos;
			// beginning of id
			wchar_t* begin_id = stream_pos;

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
			const int id = std::stoi( std::wstring( begin_id, stream_pos-begin_id ) );
			list_items.push_back( id );
		}
		else if( *stream_pos == '$' )
		{
			// empty
		}
		else
		{
			std::stringstream err;
			err << "tokenizeEntityList: unexpected argument: " << list_str.c_str() << std::endl;
			throw BuildingException( err.str(), __FUNC__ );
		}

		while( isspace( *stream_pos ) )
		{
			++stream_pos;
		}
		if( *stream_pos == ',' )
		{
			++stream_pos;
			//last_token = stream_pos;
			continue;
		}
		else
		{
			break;
		}
	}
}

void readIntegerList( const std::wstring& str, std::vector<int>& vec )
{
	const wchar_t* ch = str.c_str();
	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}
	size_t i=0;
	size_t last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == '(' )
		{
			++i;
			last_token = i;
			break;
		}
		++i;
	}
	
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			size_t str_length = i - last_token;
			if( str_length > 0 )
			{
				vec.push_back(std::stoi(str.substr(last_token, i - last_token)));
			}
			last_token = i+1;
		}
		else if( ch[i] == ')' )
		{
			size_t str_length = i - last_token;
			if( str_length > 0 )
			{
				vec.push_back(std::stoi(str.substr(last_token, i - last_token)));
			}
			return;
		}
		++i;
	}
}

void readIntegerList2D( const std::wstring& str, std::vector<std::vector<int> >& vec )
{
	// ((1,2,4),(3,23,039),(938,3,-3,6))
	const wchar_t* ch = str.c_str();
	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}
	size_t i=0;
	size_t num_par_open=0;
	size_t last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			if( num_par_open == 1 )
			{
				std::vector<int> inner_vec;
				vec.push_back( inner_vec );
				readIntegerList( str.substr( last_token, i-last_token ), inner_vec );
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
				std::vector<int> inner_vec;
				vec.push_back( inner_vec );
				readIntegerList( str.substr( last_token, i-last_token ), inner_vec );
				return;
			}
		}
		++i;
	}
}

void readRealList( const std::wstring& str, std::vector<double>& vec )
{
	const wchar_t* ch = str.c_str();
	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}
	size_t i=0;
	size_t last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == '(' )
		{
			++i;
			last_token = i;
			break;
		}
		++i;
	}

	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			vec.push_back( std::stod( str.substr( last_token, i-last_token ) ) );
			last_token = i+1;
		}
		else if( ch[i] == ')' )
		{
			vec.push_back( std::stod( str.substr( last_token, i-last_token ) ) );
			return;
		}
		++i;
	}
}

void readRealList2D( const std::wstring& str, std::vector<std::vector<double> >& vec )
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	const wchar_t* ch = str.c_str();
	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}
	size_t i=0;
	size_t num_par_open=0;
	size_t last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			if( num_par_open == 1 )
			{
				std::vector<double> inner_vec;
				vec.push_back(inner_vec);
				readRealList( str.substr( last_token, i-last_token ), vec.back() );
				last_token = i;
			}
		}
		else if( ch[i] == '(' )
		{
			++num_par_open;
			last_token = i;
		}
		else if( ch[i] == ')' )
		{
			--num_par_open;
			if( num_par_open == 0 )
			{
				std::vector<double> inner_vec;
				vec.push_back(inner_vec);
				readRealList( str.substr( last_token, i-last_token ), vec.back() );
				return;
			}
		}
		++i;
	}
}

void readRealList3D( const std::wstring& str, std::vector<std::vector<std::vector<double> > >& vec )
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	const wchar_t* ch = str.c_str();
	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}
	size_t i=0;
	size_t num_par_open=0;
	size_t last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			if( num_par_open == 1 )
			{
				std::vector<std::vector<double> > inner_vec;
				vec.push_back(inner_vec);
				readRealList2D( str.substr( last_token, i-last_token ), vec.back() );
				last_token = i;
			}
		}
		else if( ch[i] == '(' )
		{
			++num_par_open;
			last_token = i;
		}
		else if( ch[i] == ')' )
		{
			--num_par_open;
			if( num_par_open == 0 )
			{
				std::vector<std::vector<double> > inner_vec;
				vec.push_back(inner_vec);
				readRealList2D( str.substr( last_token, i-last_token ), vec.back() );
				return;
			}
		}
		++i;
	}
}

void readBinary( const std::wstring& str, std::wstring& target )
{
	target = str;
}

void readBinaryString(const std::wstring& attribute_value, std::wstring& target)
{
	if( attribute_value.size() < 2 )
	{
		target = attribute_value;
		return;
	}
	if( attribute_value[0] == '"' && attribute_value[attribute_value.size() - 1] == '"' )
	{
		target = attribute_value.substr(1, attribute_value.size() - 2);
	}
}

void readBinaryList( const std::wstring& str, std::vector<std::wstring>& vec )
{
	readStringList( str, vec );
}

void readStringList( const std::wstring& str, std::vector<std::wstring>& vec )
{
	const wchar_t* ch = str.c_str();
	const size_t argsize = str.size();
	if( argsize == 0 )
	{
		return;
	}
	size_t i=0;
	size_t last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == '(' )
		{
			++i;
			last_token = i;
			break;
		}
		++i;
	}
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			vec.push_back( str.substr( last_token, i-last_token ) );
			last_token = i+1;
		}
		else if( ch[i] == ')' )
		{
			vec.push_back( str.substr( last_token, i-last_token ) );
			return;
		}
		++i;
	}
}

void findEndOfString( const char*& stream_pos )
{
	++stream_pos;
	const char* pos_begin = stream_pos;

	// beginning of string, continue to end
	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\\' )
		{
			if( *(stream_pos+1) == 'X' )
			{
				if( *(stream_pos+2) == '0' || *(stream_pos+2) == '2' || *(stream_pos+2) == '4' )
				{
					if( *(stream_pos+3) == '\\' )
					{
						// ISO 10646 encoding, continue
						stream_pos += 4;
						continue;
					}
				}
			}

			if( *(stream_pos+1) == '\\' )
			{
				// we have a double backslash, so just continue
				++stream_pos;
				++stream_pos;
				continue;
			}
			if( *(stream_pos+1) == '\'' )
			{
				// quote is escaped
				++stream_pos;
				++stream_pos;
				continue;
			}
		}
				
		if( *stream_pos == '\'' )
		{
			if( *(stream_pos+1) == '\'' )
			{
				// two single quotes in string
				if( stream_pos != pos_begin )
				{
					++stream_pos;
					++stream_pos;
					continue;
				}
			}
			++stream_pos;

			// end of string
			break;
		}
		++stream_pos;
	}
}

void findEndOfWString( const wchar_t*& stream_pos )
{
	++stream_pos;
	const wchar_t* pos_begin = stream_pos;

	// beginning of string, continue to end
	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\\' )
		{
			if( *(stream_pos+1) == 'X' )
			{
				if( *(stream_pos+2) == '0' || *(stream_pos+2) == '2' || *(stream_pos+2) == '4' )
				{
					if( *(stream_pos+3) == '\\' )
					{
						// ISO 10646 encoding, continue
						stream_pos += 4;
						continue;
					}
				}
			}

			if( *(stream_pos+1) == '\\' )
			{
				// we have a double backslash, so just continue
				++stream_pos;
				++stream_pos;
				continue;
			}
			if( *(stream_pos+1) == '\'' )
			{
				// quote is escaped
				++stream_pos;
				++stream_pos;
				continue;
			}
		}
				
		if( *stream_pos == '\'' )
		{
			if( *(stream_pos+1) == '\'' )
			{
				// two single quotes in string
				if( stream_pos != pos_begin )
				{
					++stream_pos;
					++stream_pos;
					continue;
				}
			}
			++stream_pos;

			// end of string
			break;
		}
		++stream_pos;
	}
}

void decodeArgumentStrings( std::vector<std::string>& entity_arguments, std::vector<std::wstring>& args_out )
{
	for(auto & argument_str : entity_arguments)
	{
			const size_t arg_length = argument_str.length();
		if( arg_length == 0 )
		{
			continue;
		}

		std::wstring arg_str_new;
		arg_str_new.reserve(arg_length);

		char* stream_pos = const_cast<char*>(argument_str.c_str());		// ascii characters from STEP file
		while( *stream_pos != '\0' )
		{
			if( *stream_pos == '\\' )
			{
				if( *(stream_pos+1) == 'S' )
				{
					if( *(stream_pos+2) == '\\' )
					{
						if( *(stream_pos+3) != '\0' )
						{
							if( *(stream_pos+4) == '\\' )
							{
								if( *(stream_pos+5) == 'S' )
								{
									if( *(stream_pos+6) == '\\' )
									{
										if( *(stream_pos+7) != '\0' )
										{
											char first = *(stream_pos+3);
											char second = *(stream_pos+7);
											wchar_t append_wchar = wchar_t(125 + first + second);
											arg_str_new += append_wchar;
											stream_pos += 8;
											continue;
										}
									}
								}
								else if( *(stream_pos+5) == 'Q' )
								{
									if( *(stream_pos+6) == '\\' )
									{
										if( *(stream_pos+7) != '\0' )
										{
											char first = *(stream_pos+3);
											char second = *(stream_pos+7);
											wchar_t append_wchar = wchar_t(125 + first + second);
											arg_str_new += append_wchar;
											stream_pos += 8;
											continue;
										}
									}
								}
							}
							else 
							{
								// next characters code value v shall be interpreted as v + 128
								char first = *(stream_pos+3);
								wchar_t append_wchar = wchar_t(128 + first);
								arg_str_new += append_wchar;
								stream_pos += 4;
								continue;
							}
						}
					}
				}
				else if( *(stream_pos+1) == 'X' )
				{
					if( *(stream_pos+2) == '\\' )
					{
						wchar_t wc = Hex2Wchar(*(stream_pos+3), *(stream_pos+4));

						//unsigned char char_ascii = wctob(wc);
						arg_str_new += wc;

						stream_pos += 5;
						continue;
					}
					else if( *(stream_pos+2) == '0' )
					{
						if( *(stream_pos+3) == '\\' )
						{
							stream_pos += 4;
							continue;
						}
					}
					else if( *(stream_pos+2) == '2' )
					{
						if( *(stream_pos+3) == '\\' )
						{
							// the following sequence of multiples of four hexadecimal characters shall be interpreted as encoding the 
							// two-octet representation of characters from the BMP in ISO 10646

							bool finished = false;
							stream_pos += 4;

							do
							{
								wchar_t wc = Hex4Wchar(*(stream_pos), *(stream_pos+1), *(stream_pos+2), *(stream_pos+3));
								//unsigned char char_ascii = wctob(wc);
								arg_str_new += wc;
								stream_pos += 4;

							} while (( *stream_pos != '\0' ) && ( *stream_pos != '\\' ));

							continue;
						}
					}
				}
				else if( *(stream_pos+1) == 'N' )
				{
					if( *(stream_pos+2) == '\\' )
					{
						arg_str_new.append( L"\n" );
						stream_pos += 3;
						continue;
					}
				}
			}
					
			char current_char = *stream_pos;
			arg_str_new += current_char;
			++stream_pos;
		}
		
		args_out.push_back( arg_str_new );
	}
}


//\brief split one string into a vector of argument strings
// caution: when using OpenMP, this method runs in parallel threads
void tokenizeEntityArguments( const std::string& argument_str, std::vector<std::string>& entity_arguments )
{
	const char* stream_pos = argument_str.c_str();
	if( *stream_pos != '(' )
	{
		return;
	}

	++stream_pos;
	int num_open_braces = 1;
	const char* last_token = stream_pos;

	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\'' )
		{
			findEndOfString( stream_pos );
			continue;
		}

		if( *stream_pos == '(' )
		{
			++num_open_braces;
		}
		else if( *stream_pos == ',' )
		{
			if( num_open_braces == 1 )
			{
				if( *last_token == ',' )
				{
					++last_token;
				}

				const char* begin_arg = last_token;

				// skip whitespace
				while( isspace( *begin_arg ) ) 
				{
					++begin_arg; 
				}
				const char* end_arg = stream_pos-1;
				entity_arguments.emplace_back( begin_arg, end_arg-begin_arg+1 );
				last_token = stream_pos;
			}
		}
		else if( *stream_pos == ')' )
		{
			--num_open_braces;
			if( num_open_braces == 0 )
			{
				if( *last_token == ',' )
				{
					++last_token;
				}

				const char* begin_arg = last_token;

				// skip whitespace
				while( isspace( *begin_arg ) ) 
				{
					++begin_arg; 
				}

				int remaining_size = static_cast<int>(stream_pos - begin_arg);
				if( remaining_size > 0 )
				{
					const char* end_arg = stream_pos-1;
					entity_arguments.emplace_back( begin_arg, end_arg-begin_arg+1 );
				}
				break;
			}
		}
		++stream_pos;
	}
}

//\brief split one string into a vector of argument strings
// caution: when using OpenMP, this method runs in parallel threads
void tokenizeEntityArguments( const std::wstring& argument_str, std::vector<std::wstring>& entity_arguments )
{
	const wchar_t* stream_pos = argument_str.c_str();
	if( *stream_pos != '(' )
	{
		return;
	}

	++stream_pos;
	int num_open_braces = 1;
	const wchar_t* last_token = stream_pos;

	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\'' )
		{
			findEndOfWString( stream_pos );
			continue;
		}

		if( *stream_pos == '(' )
		{
			++num_open_braces;
		}
		else if( *stream_pos == ',' )
		{
			if( num_open_braces == 1 )
			{
				if( *last_token == ',' )
				{
					++last_token;
				}

				const wchar_t* begin_arg = last_token;

				// skip whitespace
				while( isspace( *begin_arg ) ) 
				{
					++begin_arg; 
				}
				const wchar_t* end_arg = stream_pos-1;
				entity_arguments.emplace_back( begin_arg, end_arg-begin_arg+1 );
				last_token = stream_pos;
			}
		}
		else if( *stream_pos == ')' )
		{
			--num_open_braces;
			if( num_open_braces == 0 )
			{
				if( *last_token == ',' )
				{
					++last_token;
				}

				const wchar_t* begin_arg = last_token;

				// skip whitespace
				while( isspace( *begin_arg ) ) 
				{
					++begin_arg; 
				}

				int remaining_size = static_cast<int>(stream_pos - begin_arg);
				if( remaining_size > 0 )
				{
					const wchar_t* end_arg = stream_pos-1;
					entity_arguments.emplace_back( begin_arg, end_arg-begin_arg+1 );
				}
				break;
			}
		}
		++stream_pos;
	}
}

void tokenizeInlineArgument( std::wstring arg, std::wstring& keyword, std::wstring& inline_arg )
{
	if( arg.empty() )
	{
		throw BuildingException( "arg.size() == 0", __FUNC__ );
	}
	if( arg[0] == '$' )
	{
		return;
	}
	if( arg[0] == '*' )
	{
		return;
	}
	if( arg[0] == '#' )
	{
		throw BuildingException( "tokenizeInlineArgument: argument begins with #, so it is not inline", __FUNC__ );
	}

	wchar_t* stream_pos = const_cast<wchar_t*>(arg.c_str());
	while(isspace(*stream_pos)){ ++stream_pos; }

	wchar_t* begin_keyword = stream_pos;
	while(isalnum(*stream_pos)){ ++stream_pos; }

	// get type name
	std::wstring key( begin_keyword, stream_pos-begin_keyword );
	
	if( key.empty() )
	{
		// single argument, for example .T.
		inline_arg = arg;
		return;
	}

	// proceed to '('
	int numOpenBraces = 0;
	if( *stream_pos == '(' )
	{
		++stream_pos;
		++numOpenBraces;
	}
	else
	{
		while( *stream_pos != '\0' )
		{
			if( *stream_pos == '(' )
			{
				++stream_pos;
				++numOpenBraces;
				break;
			}
			++stream_pos;
		}
	}

	// proceed to ')'
	std::wstring inline_argument;
	wchar_t* inline_argument_begin = stream_pos;
	
	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\'' )
		{
			++stream_pos;
			// inside string
			while( *stream_pos != '\0' )
			{
				if( *stream_pos == '\'' )
				{
					// check if tick is escaped
					wchar_t* tick_pos = stream_pos;
					bool tick_escaped = false;
					while( tick_pos != begin_keyword )
					{
						--tick_pos;
						if( *tick_pos == '\\' )
						{
							tick_escaped = !tick_escaped;
							continue;
						}
						break;
					}
					if( tick_escaped )
					{
						++stream_pos;
						continue;
					}
					// else tick marks the end of argument
					break;
				}
				++stream_pos;
			}
		}

		if (*stream_pos == '(')
		{
			++numOpenBraces;
			++stream_pos;
			continue;
		}

		if( *stream_pos == ')' )
		{
			--numOpenBraces;
			// skip whitespace
			while( isspace( *inline_argument_begin ) ) 
			{
				++inline_argument_begin;
			}
			if (numOpenBraces == 0)
			{
				wchar_t* end_arg = stream_pos - 1;
				inline_argument = std::wstring(inline_argument_begin, end_arg - inline_argument_begin + 1);
				break;
			}
		}
		++stream_pos;
	}

	std::transform(key.begin(), key.end(), key.begin(), [](wchar_t c) {return static_cast<wchar_t>(std::toupper(c)); });
	keyword = key;
	inline_arg = inline_argument;
}

bool std_iequal(const std::wstring& a, const std::wstring& b)
{
	if (a.size() == b.size())
	{
		return std::equal(a.begin(), a.end(), b.begin(), [](const wchar_t l, const wchar_t r) { return std::towupper(l) == std::towupper(r); });
	}
	return false;
}

bool std_iequal(const std::string& a, const std::string& b)
{
	if (a.size() == b.size())
	{
		return std::equal(a.begin(), a.end(), b.begin(), [](const char l, const char r) { return std::toupper(l) == std::toupper(r); });
	}
	return false;
}
