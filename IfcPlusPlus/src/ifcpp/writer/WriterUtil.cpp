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

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BuildingObject.h"
#include "WriterUtil.h"

std::string encodeStepString( const std::wstring& str )
{
	wchar_t* stream_pos = (wchar_t*)str.c_str();
	std::string result_str;
	std::string beginUnicodeTag = "\\X2\\";
	std::string endUnicodeTag = "\\X0\\";
	bool hasOpenedUnicodeTag = false;

	while( *stream_pos != '\0' )
	{
		wchar_t append_char = *stream_pos;
		if( append_char == 10 )
		{
			// encode new line
			result_str.append( "\\X\\0A" );
		}
		else if( append_char == 13 )
		{
			// encode carriage return
			result_str.append( "\\X\\0D" );
		}
		else if( append_char == 92 )
		{
			// encode backslash
			result_str.append( "\\\\" );
		}
		else if( append_char > 0 && append_char < 128 )
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
			int value = (int)(append_char);
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
