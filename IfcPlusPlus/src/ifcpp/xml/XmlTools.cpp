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

#include "XmlTools.h"
#include <map>
#include <string>

#ifndef WIN32
#include <cstdio>
#include <cstring>
#endif

void findCompleteTag( const char* data, int& begin, int& end )
{
	const char* pos_begin = &data[0];
	char* pos = (char*)pos_begin;
	
	while( *pos != '\0' )
	{
		char* pos_tag_open = strchr( pos, '<' );
		if( !pos_tag_open )
		{
			begin = 0;
			end = 0;
			return;
		}
		pos = pos_tag_open;
		++pos;
		if( *pos == '\0' )
		{
			// end of string, no opening tag found
			begin = 0;
			end = 0;
			return;
		}
		if( *pos == '/' )
		{
			// found closing tag before opening tag
			// ignore and continue to next opening tag
			++pos;
			continue;
		}
		else
		{
			// opening tag found
			begin = (int)(pos_tag_open - pos_begin);
			break;
		}
	}

	char* pos_keyword_begin = pos;
	// proceed to end of keyword
	while( isalnum( *pos ) )
	{
		// keyword should be alphanumeric
		++pos;
	}
	int keyword_length = (int)(pos-pos_keyword_begin);
	std::string keyword( pos_keyword_begin, keyword_length );

	// short tag: <keyword />
	char* pos_closing_tag_short = strchr( pos, '>' );
	if( pos_closing_tag_short != nullptr )
	{
		pos = pos_closing_tag_short;
		--pos;

		// skip whitespace
		while( isspace( *pos ) ) 
		{
			--pos;
		}

		if( *pos == '/' )
		{
			end = (int)(pos_closing_tag_short - pos_begin + 1);
			return;
		}
	}

	// long tag: <keyword>...</keyword>
	char* pos_closing_tag = strstr( pos, "</" );

	if( pos_closing_tag != nullptr )
	{
		pos = strstr( pos_closing_tag, keyword.c_str() );
		if( pos != nullptr )
		{
			pos = strchr( pos, '>' );
			if( pos != nullptr )
			{
				end = (int)(pos - pos_begin + 1);
				return;
			}
		}
	}

	// No closing tag found
	begin = 0;
	end = 0;
}

int readNextXmlTag( const char* data, std::stringstream& err, XmlDataContainer& container )
{
	char* pos = (char*)data;
	if(  *pos != '<' )
	{
		// search for opening <
		pos = strchr( pos, '<' );
		if( !pos )
		{
			return 0;
		}
	}
	++pos;
	
	// skip whitespaces
	while( isspace( *pos ) ) { ++pos; }
	
	const char* pos_keyword_begin = pos;
	// proceed to end of keyword
	// keyword should be alphanumeric
	while( isalnum( *pos ) ) { ++pos; }
	const int keyword_length = (int)(pos-pos_keyword_begin);
		
	// search for attributes
	container.keyword.assign( pos_keyword_begin, keyword_length );
	std::string attribute_name;
	std::string attribute_value;

	while( *pos != '\0' )
	{
		// skip whitespace
		while( isspace( *pos ) ) { ++pos; }

		// pattern: attribute="value"
		if( isalnum( *pos ) )
		{
			char* pos_attribute_name = pos;
			// attribute name is alphanumeric
			while( isalnum( *pos ) ) { ++pos; }

			attribute_name.assign( pos_attribute_name, pos-pos_attribute_name );
						
			// skip whitespace
			while( isspace( *pos ) ) { ++pos; }

			if( *pos == '=' )
			{
				++pos;
			}
			else
			{
				err << "error in attribute pattern: " << data << std::endl;
			}

			// skip whitespace
			while( isspace( *pos ) ) { ++pos; }
			
			
			if( *pos == '"' || *pos == '\'' )
			{
				++pos;
				// begin of value
				char* pos_attribute_value = pos;
				
				// find end of value
				while( *pos != '\0' )
				{
					if( *pos == '"' || *pos == '\'' )
					{
						attribute_value.assign( pos_attribute_value, pos-pos_attribute_value );
						container.attributes.insert( std::make_pair( attribute_name, attribute_value ) );
						break;
					}
					else
					{
						++pos;
					}
				}
				++pos;
			}
			
			// skip whitespace
			while( isspace( *pos ) ) { ++pos; }
		}

		// skip whitespace
		while( isspace( *pos ) ) { ++pos; }

		// end of tag or continue to next attribute
		if( *pos == '>' || *pos == '/' )
		{
			if( *pos == '/' )
			{
				++pos;

				// empty element tag
				while( isspace( *pos ) ) { ++pos; }

				if( *pos == '>' )
				{
					// successfully completed tag
					return (int)(pos-data+1);
				}
				else
				{
					err << "error in attribute pattern: " << data << std::endl;
					return (int)(pos-data);
				}
			}
			else if( *pos == '>' )
			{
				++pos;

				// skip whitespace
				while( isspace( *pos ) ) { ++pos; }

				// <keyword attribute=""><![CDATA[cdata]]></keyword>
				if( strncmp( pos, "<![CDATA[", 9 ) == 0 )
				{
					pos += 9;
					char* cdata_begin = pos;
					char* cdata_end = strstr( pos, "]]>" );
					if( *cdata_end != '\0' )
					{
						pos = cdata_end+3;
						int cdata_length = (int)(cdata_end-cdata_begin);
						container.cdata.assign( cdata_begin, cdata_length );
					}
				}

				// <keyword attribute1="value1"></keyword>

				// search for closing tag: </keyword>
				char* pos_closing_tag = strchr( pos, '<' );
				if( !pos_closing_tag )
				{
					err << "error in attribute pattern: " << data << std::endl;
					return (int)(pos-data);
				}
			
				pos = pos_closing_tag;
			
				// skip whitespace
				while( isspace( *pos ) ) { ++pos; }

				++pos;
				if( *pos == '/' )
				{
					++pos;
					// skip whitespace
					while( isspace( *pos ) ) { ++pos; }
	
					if( strncmp( pos, pos_keyword_begin, keyword_length ) != 0 )
					{
						// error: keyword does not match
						err << "keyword does not match: " << std::string( pos_keyword_begin, pos-pos_keyword_begin ) << std::endl;

						// find next closing > and return
						char* pos_end = strchr( pos, '>' );
						if( pos_end )
						{
							return (int)(pos_end-data);
						}
						else
						{
							return (int)(pos-data);
						}
					}
	
					pos += keyword_length;

					// skip whitespace
					while( isspace( *pos ) ) { ++pos; }
	
					// next character should be a closing >
					if( *pos == '>' )
					{
						// successfully completed tag
						return (int)(pos-data+1);
					}
					else
					{
						err << "error in attribute pattern: " << data << std::endl;
						return (int)(pos-data);
					}
				}
				else
				{
					// TODO: handle nested tags
					--pos;
					break;
				}
			}
			else
			{
				// not a complete tag
				break;
			}
		}
	}
	return 0;
}
