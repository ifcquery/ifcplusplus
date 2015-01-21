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

#include <math.h>
#include "WriterUtil.h"
#include <sstream>

void writeConstCharList( std::stringstream& stream, const std::vector<const char*>& vec )
{
	if( vec.size() == 0 )
	{
		stream << "$";
		return;
	}
	stream << "(";
	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const char* ch = vec[ii];
		stream << ch;// encodeStepString(( *it ));
	}
	stream << ")";
}

void writeStringList( std::stringstream& stream, const std::vector<std::wstring>& vec )
{
	if( vec.size() == 0 )
	{
		stream << "$";
	}
	stream << "(";
	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		stream << encodeStepString( vec[ii] );
	}
	stream << ")";
}

std::string encodeStepString( std::wstring str )
{
	wchar_t* stream_pos = (wchar_t*)str.c_str();
	std::string result_str;
	std::string beginUnicodeTag = "\\X2\\";
	std::string endUnicodeTag = "\\X0\\";
	bool hasOpenedUnicodeTag = false;

	while( *stream_pos != '\0' )
	{
		wchar_t append_char = *stream_pos;
		if( append_char > 0 && append_char < 128 )
		{
			if( hasOpenedUnicodeTag )
			{
				result_str += endUnicodeTag;
				hasOpenedUnicodeTag = false;
			}

			result_str.push_back( (char)append_char );
		}
		else
		{
			int value = (int)( append_char );
			wchar_t temporary[8];
			swprintf( temporary, 5, L"%04X", value );

			if( !hasOpenedUnicodeTag )
			{
				result_str += beginUnicodeTag;
				hasOpenedUnicodeTag = true;
			}

			char mb[8];
			wctomb( mb, temporary[0] );
			result_str.push_back( mb[0] );
			wctomb( mb, temporary[1] );
			result_str.push_back( mb[0] );
			wctomb( mb, temporary[2] );
			result_str.push_back( mb[0] );
			wctomb( mb, temporary[3] );
			result_str.push_back( mb[0] );
		}
		++stream_pos;
	}

	if( hasOpenedUnicodeTag )
	{
		result_str += endUnicodeTag;
		hasOpenedUnicodeTag = false;
	}

	return result_str;
}
