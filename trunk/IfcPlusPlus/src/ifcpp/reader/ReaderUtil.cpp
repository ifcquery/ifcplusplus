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

#ifdef WIN32
#include <math.h>
#include <windows.h>
#else
#include <cmath>
#endif
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ReaderUtil.h"

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif


static short HexD(unsigned char mc)
{
	short returnValue;

	if (mc >= '0' && mc <= '9')
		returnValue = (short)mc - (short)'0';
	else if (mc >= 'A' && mc <= 'F')
		returnValue = 10 + (short)mc - (short)'A';
	else if (mc >= 'a' && mc <= 'f')
		returnValue = 10 + (short)mc - (short)'a';
	else
		returnValue = 0;
	return (returnValue);
}


static wchar_t Hex2Wchar(unsigned char h1, unsigned char h2 )
{
	wchar_t returnValue = (HexD(h1) << 4) + HexD(h2);
	return (returnValue);
}

static wchar_t Hex4Wchar(unsigned char h1, unsigned char h2, unsigned char h3, unsigned char h4 )
{
	wchar_t returnValue = (HexD(h1)<< 12) + (HexD(h2) << 8) +(HexD(h3) << 4) + HexD(h4);
	return (returnValue);
}


shared_ptr<IfcPPObject> createIfcPPType( const IfcPPTypeEnum type_enum, const std::string& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities );
IfcPPEntity* createIfcPPEntity( const IfcPPEntityEnum entity_enum );

void checkOpeningClosingParenthesis( const char* ch_check )
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
		++ch_check;
	}
	if( num_opening != num_closing )
	{
		std::stringstream err;
		err << "checkOpeningClosingParenthesis: num_opening != num_closing : " << ch_check << std::endl;
		throw IfcPPException( err.str().c_str(), __func__ );
	}
}

void findLeadingTrailingParanthesis( char* ch, char*& pos_opening, char*& pos_closing )
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

void tokenizeList( std::string& list_str, std::vector<std::string>& list_items )
{
	if( list_str.size() == 0 )
	{
		return;
	}
	char* stream_pos = (char*)list_str.c_str();
	char* last_token = stream_pos;
	while( *stream_pos != '\0' )
	{
		if( *stream_pos == '\'' )
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

		if( *stream_pos == ',' )
		{
			std::string item( last_token, stream_pos-last_token );
			//if( item.at(0) == '\'' && item.at(item.size()-1) == '\'' )
			//{
			//	item.erase(item.size()-1);
			//	item.erase(0);
			//}
			list_items.push_back( item );

			++stream_pos;
			while( isspace(*stream_pos) ) { ++stream_pos; }
			last_token = stream_pos;
			if( *stream_pos == '\0' )
			{
				throw IfcPPException( "tokenizeList: *stream_pos == '\0'", __func__ );
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
			std::string item( last_token, stream_pos-last_token );
			//if( item.at(0) == '\'' && item.at(item.size()-1) == '\'' )
			//{
			//	item.erase(item.size()-1);
			//	item.erase(0);
			//}
			list_items.push_back( item );
		}
	}
}

void tokenizeEntityList( std::string& list_str, std::vector<int>& list_items )
{
	if( list_str.size() == 0 )
	{
		return;
	}
	char* stream_pos = (char*)list_str.c_str();
	while( *stream_pos != '\0' )
	{
		// skip whitespace
		while( isspace( *stream_pos ) ) { ++stream_pos; }
		
		if( *stream_pos == '#' )
		{
			++stream_pos;
			// beginning of id
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
			throw IfcPPException( err.str().c_str(), __func__ );
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

void readIntValue( const std::string& str, int& int_value )
{
	if( str.compare("$") == 0 )
	{
		int_value = std::numeric_limits<int>::quiet_NaN();
	}
	else
	{
		int_value = atoi( str.c_str() );
	}
}

void readRealValue( const std::string& str, double& real_value )
{
	if( str.compare("$") == 0 )
	{
		real_value = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		real_value = atof( str.c_str() );
	}
}

void readIntList( const std::string& str, std::vector<int>& vec )
{
	const char* ch = str.c_str();
	const unsigned int argsize = (unsigned int)str.size();
	if( argsize == 0 )
	{
		return;
	}
	unsigned int i=0;
	unsigned int last_token = 0;
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
			vec.push_back( atoi( str.substr( last_token, i-last_token ).c_str() ) );
			last_token = i+1;
		}
		else if( ch[i] == ')' )
		{
			vec.push_back( atoi( str.substr( last_token, i-last_token ).c_str() ) );
			return;
		}
		++i;
	}
}

void readIntList2D( const std::string& str, std::vector<std::vector<int> >& vec )
{
	// ((1,2,4),(3,23,039),(938,3,-3,6))
	const char* ch = str.c_str();
	const unsigned int argsize = (unsigned int)str.size();
	if( argsize == 0 )
	{
		return;
	}
	unsigned int i=0;
	unsigned int num_par_open=0;
	int last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			if( num_par_open == 1 )
			{
				std::vector<int> inner_vec;
				vec.push_back( inner_vec );
				readIntList( str.substr( last_token, i-last_token ), inner_vec );
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
				readIntList( str.substr( last_token, i-last_token ), inner_vec );
				return;
			}
		}
		++i;
	}
}

void readDoubleList( const std::string& str, std::vector<double>& vec )
{
	const char* ch = str.c_str();
	const unsigned int argsize = (unsigned int)str.size();
	if( argsize == 0 )
	{
		return;
	}
	unsigned int i=0;
	unsigned int last_token = 0;
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
			vec.push_back( atof( str.substr( last_token, i-last_token ).c_str() ) );
			last_token = i+1;
		}
		else if( ch[i] == ')' )
		{
			vec.push_back( atof( str.substr( last_token, i-last_token ).c_str() ) );
			return;
		}
		++i;
	}
}

void readDoubleList2D( const std::string& str, std::vector<std::vector<double> >& vec )
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	const char* ch = str.c_str();
	const unsigned int argsize = (unsigned int)str.size();
	if( argsize == 0 )
	{
		return;
	}
	unsigned int i=0;
	unsigned int num_par_open=0;
	unsigned int last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			if( num_par_open == 1 )
			{
				std::vector<double> inner_vec;
				vec.push_back(inner_vec);
				readDoubleList( str.substr( last_token, i-last_token ), vec.back() );
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
				readDoubleList( str.substr( last_token, i-last_token ), vec.back() );
				return;
			}
		}
		++i;
	}
}

void readDoubleList3D( const std::string& str, std::vector<std::vector<std::vector<double> > >& vec )
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	const char* ch = str.c_str();
	const unsigned int argsize = (unsigned int)str.size();
	if( argsize == 0 )
	{
		return;
	}
	unsigned int i=0;
	unsigned int num_par_open=0;
	unsigned int last_token = 0;
	while( i<argsize )
	{
		if( ch[i] == ',' )
		{
			if( num_par_open == 1 )
			{
				std::vector<std::vector<double> > inner_vec;
				vec.push_back(inner_vec);
				readDoubleList2D( str.substr( last_token, i-last_token ), vec.back() );
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
				readDoubleList2D( str.substr( last_token, i-last_token ), vec.back() );
				return;
			}
		}
		++i;
	}
}

void readConstCharList( const std::string& str, std::vector<const char*>& vec )
{
	const char* ch = str.c_str();
	const unsigned int argsize = (unsigned int)str.size();
	if( argsize == 0 )
	{
		return;
	}
	unsigned int i=0;
	unsigned int last_token = 0;
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
			vec.push_back( str.substr( last_token, i-last_token ).c_str() );
			last_token = i+1;
		}
		else if( ch[i] == ')' )
		{
			vec.push_back( str.substr( last_token, i-last_token ).c_str() );
			return;
		}
		++i;
	}
}

void readStringList( const std::string& str, std::vector<std::string>& vec )
{
	const char* ch = str.c_str();
	const unsigned int argsize = (unsigned int)str.size();
	if( argsize == 0 )
	{
		return;
	}
	unsigned int i=0;
	unsigned int last_token = 0;
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

void findEndOfString( char*& stream_pos )
{
	++stream_pos;
	char* pos_begin = stream_pos;

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

void decodeArgumentStrings( std::vector<std::string>& entity_arguments )
{
	std::vector<std::string>::iterator it = entity_arguments.begin();
	for( ; it != entity_arguments.end(); ++it )
	{
		std::string& argument_str = (*it);
		const size_t arg_length = argument_str.length();
		
		std::string arg_str_new = "";

		if( arg_length > 0 )
		{
			arg_str_new.reserve(arg_length);

			char* stream_pos = (char*)argument_str.c_str();				// ascii characters from STEP file
			//char* stream_pos_new = (char*)arg_str_new.c_str();			// ascii characters from STEP file
			{
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

													//*stream_pos_new = char(125 + first + second);
													//++stream_pos_new;
													arg_str_new += char(125 + first + second);

													stream_pos += 8;
													continue;
												}
											}
										}
									}
									else 
									{
										// next characters code value v shall be interpreted as v + 128
										char char_pos = *(stream_pos+3);
										char char_pos_128 =  char_pos + 128;

										//*stream_pos_new = char_pos_128;
										//++stream_pos_new;
										arg_str_new += char_pos_128;
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
								char char_ascii = wctob(wc);
										
								if( char_ascii < 0)
								{
#ifdef _MSC_VER
									//we got a multibyte character here
									char buf[2];
									int len = WideCharToMultiByte(CP_UTF8, 0, &wc, 1, buf, 2, nullptr, nullptr);
									arg_str_new+= buf[0];
									arg_str_new+= buf[1];
#endif
								}
								else
								{
									arg_str_new += char_ascii;
								}

								stream_pos += 4;
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
										char char_ascii = wctob(wc);

										if( char_ascii < 0 )
										{
											//we got a multibyte character here
#ifdef _MSC_VER
											char buf[2];
											int len = WideCharToMultiByte(CP_UTF8, 0, &wc, 1, buf, 2, nullptr, nullptr);
											arg_str_new+= buf[0];
											arg_str_new+= buf[1];
#endif
										}
										else
										{
											arg_str_new+= char_ascii;
										}
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
								arg_str_new += "\n";
								stream_pos += 3;
								continue;
							}
						}
					}
					
					arg_str_new+= *stream_pos;
					++stream_pos;
				}
			}
		}
		argument_str.assign( arg_str_new );
	}
}

#ifdef _MSC_VER
void decodeArgumentStrings( std::vector<std::string>& entity_arguments, std::vector<std::wstring>& args_out )
{
	std::vector<std::string>::iterator it = entity_arguments.begin();
	for( ; it != entity_arguments.end(); ++it )
	{
		std::string& argument_str = (*it);
		const size_t arg_length = argument_str.length();
		
		std::wstring arg_str_new;

		if( arg_length > 0 )
		{
			arg_str_new.reserve(arg_length);

			char* stream_pos = (char*)argument_str.c_str();				// ascii characters from STEP file
			//char* stream_pos_new = (char*)arg_str_new.c_str();			// ascii characters from STEP file
			{
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

													//*stream_pos_new = char(125 + first + second);
													//++stream_pos_new;
													arg_str_new += char(125 + first + second);

													stream_pos += 8;
													continue;
												}
											}
										}
									}
									else 
									{
										// next characters code value v shall be interpreted as v + 128
										char char_pos = *(stream_pos+3);
										char char_pos_128 =  char_pos + 128;

										//*stream_pos_new = char_pos_128;
										//++stream_pos_new;
										arg_str_new += char_pos_128;
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

								stream_pos += 4;
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
					wchar_t c;
					//mbstowcs(  &c, &current_char, 1 );
					arg_str_new += current_char;//.append( &c );
					++stream_pos;
				}
			}
		}
		//argument_str.assign( arg_str_new );
		args_out.push_back( arg_str_new );
	}
}
#endif

// @brief split one string into a vector of argument strings
// caution: when using OpenMP, this method runs in parallel threads
void tokenizeEntityArguments( const std::string& argument_str, std::vector<std::string>& entity_arguments )
{
	char* stream_pos = (char*)argument_str.c_str();
	if( *stream_pos != '(' )
	{
		return;
	}

	++stream_pos;
	int num_open_braces = 1;
	char* last_token = stream_pos;

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

				char* begin_arg = last_token;

				// skip whitespace
				while( isspace( *begin_arg ) ) 
				{
					++begin_arg; 
				}
				char* end_arg = stream_pos-1;
				//if( *begin_arg == '\'' && *end_arg == '\'' )
				//{
				//	std::string arg_str( begin_arg+1, end_arg-begin_arg-1 );
				//	entity_arguments.push_back( arg_str );
				//}
				//else
				//{
					entity_arguments.push_back( std::string( begin_arg, end_arg-begin_arg+1 ) );
//				}
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

				char* begin_arg = last_token;

				// skip whitespace
				while( isspace( *begin_arg ) ) 
				{
					++begin_arg; 
				}

				int remaining_size = (int)(stream_pos - begin_arg);
				if( remaining_size > 0 )
				{
					char* end_arg = stream_pos-1;
					//if( *begin_arg == '\'' && *end_arg == '\'' )
					//{
					//	entity_arguments.push_back( std::string( begin_arg+1, end_arg-begin_arg-1 ) );
					//}
					//else
					//{
						entity_arguments.push_back( std::string( begin_arg, end_arg-begin_arg+1 ) );
//					}
				}
				break;
			}
		}
		++stream_pos;
	}
}

void tokenizeInlineArgument( std::string arg, std::string& keyword, std::string& inline_arg )
{
	if( arg.size() == 0 )
	{
		throw IfcPPException( "arg.size() == 0", __func__ );
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
		throw IfcPPException( "tokenizeInlineArgument: argument begins with #, so it is not inline", __func__ );
	}

	char* stream_pos = (char*)arg.c_str();
	while(isspace(*stream_pos)){ ++stream_pos; }

	char* begin_keyword = stream_pos;
	while(isalnum(*stream_pos)){ ++stream_pos; }

	// get type name
	std::string key( begin_keyword, stream_pos-begin_keyword );
	
	if( key.size() == 0 )
	{
		// single argument, for example .T.
		inline_arg = arg;
		return;
	}

	// proceed to '('
	if( *stream_pos == '(' )
	{
		++stream_pos;
	}
	else
	{
		while( *stream_pos != '\0' )
		{
			if( *stream_pos == '(' )
			{
				++stream_pos;
				break;
			}
			++stream_pos;
		}
	}

	// proceed to ')'
	std::string inline_argument;
	char* inline_argument_begin = stream_pos;
	
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
					char* tick_pos = stream_pos;
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

		if( *stream_pos == ')' )
		{
			// skip whitespace
			while( isspace( *inline_argument_begin ) ) 
			{
				++inline_argument_begin;
			}
			char* end_arg = stream_pos-1;
			//if( *inline_argument_begin == '\'' && *end_arg == '\'' )
			//{
			//	inline_argument = std::string( inline_argument_begin+1, end_arg-inline_argument_begin-1 );
			//}
			//else
			//{
				inline_argument = std::string( inline_argument_begin, end_arg-inline_argument_begin+1 );
			//}
			
			break;
		}
		++stream_pos;
	}

	std::transform(key.begin(), key.end(), key.begin(), toupper);
	keyword = key;
	inline_arg = inline_argument;
}

void copyToEndOfStepString( char*& stream_pos, char*& stream_pos_source )
{
	char* pos_begin = stream_pos_source;
	findEndOfString( stream_pos_source );

	size_t length = stream_pos_source - pos_begin;
	memcpy( stream_pos, pos_begin, (length)*sizeof( char) );
	stream_pos += length;
}

void readInlineTypeOrEntity( const std::string& arg, shared_ptr<IfcPPObject>& result, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	std::string keyword;
	std::string inline_arg;
	tokenizeInlineArgument( arg, keyword, inline_arg );

	if( keyword.size() == 0 )
	{
		return;
	}

	IfcPPTypeEnum type_enum = findTypeEnumForString( keyword );
	if( type_enum != IfcPPTypeEnum::IFC_TYPE_UNDEFINED )
	{
		shared_ptr<IfcPPObject> type_instance = createIfcPPType( type_enum, inline_arg, map_entities );
		if( type_instance )
		{
			//type_instance->m_type_enum = type_enum;
			result = type_instance;
			return;
		}
	}

	IfcPPEntityEnum entity_enum = findEntityEnumForString( keyword );
	if( entity_enum != IfcPPEntityEnum::IFC_ENTITY_UNDEFINED )
	{
		shared_ptr<IfcPPEntity> entity_instance( createIfcPPEntity( entity_enum ) );
		if( entity_instance )
		{
			entity_instance->setId( -1 );
			entity_instance->m_entity_enum = entity_enum;
			entity_instance->m_entity_argument_str.assign( inline_arg );
			std::vector<std::string> args;
			args.push_back( inline_arg );
			entity_instance->readStepArguments( args, map_entities );

			result = entity_instance;
			return;
		}
	}
}

void readInlineTypeOrEntity( const std::string& keyword, const std::string& inline_arg, shared_ptr<IfcPPObject>& result, const std::map<int,shared_ptr<IfcPPEntity> >& map_entities )
{
	if( keyword.size() == 0 )
	{
		return;
	}

	IfcPPTypeEnum type_enum = findTypeEnumForString( keyword );
	if( type_enum != IfcPPTypeEnum::IFC_TYPE_UNDEFINED )
	{
		shared_ptr<IfcPPObject> type_instance = createIfcPPType( type_enum, inline_arg, map_entities );
		if( type_instance )
		{
			//type_instance->m_type_enum = type_enum;
			result = type_instance;
			return;
		}
	}

	IfcPPEntityEnum entity_enum = findEntityEnumForString( keyword );
	if( entity_enum != IfcPPEntityEnum::IFC_ENTITY_UNDEFINED )
	{
		shared_ptr<IfcPPEntity> entity_instance( createIfcPPEntity( entity_enum ) );
		if( entity_instance )
		{
			entity_instance->setId( -1 );
			entity_instance->m_entity_enum = entity_enum;
			entity_instance->m_entity_argument_str.assign( inline_arg );
			std::vector<std::string> args;
			args.push_back( inline_arg );
			entity_instance->readStepArguments( args, map_entities );

			result = entity_instance;
			return;
		}
	}
}
