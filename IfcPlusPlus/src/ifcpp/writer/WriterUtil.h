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

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <ifcpp/model/IfcPPGlobal.h>
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/model/IfcPPBasicTypes.h>

inline std::string encodeStepString( const std::wstring& str )
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

template<typename T>
void writeNumericList( std::stringstream& stream, const std::vector<T>& vec )
{
	// (3,23,039)
	// (1.6,2.0,4.9382)
	// ("str1","str2","str3")
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
		stream << vec[ii];
	}
	stream << ")";
}

template<typename T>
void writeNumericList2D( std::stringstream& stream, const std::vector<std::vector<T> >& vec )
{
	// ((1,2,4),(3,23,039),(938,3,-3,6))
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	// (("str1","str2","str3"),("str1","str2","str3"),("str1","str2","str3","str4"))
	if( vec.size() == 0 )
	{
		stream << "$";
		return;
	}

	stream << "(";
	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		const std::vector<T>& inner_vec = vec[ii];
		if( ii > 0 )
		{
			stream << ",";
		}
		writeNumericList( stream, inner_vec );
	}
	stream << ")";
}

template<typename T>
void writeNumericList3D( std::stringstream& stream, const std::vector<std::vector<std::vector<T> > >& vec )
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	if( vec.size() == 0 )
	{
		stream << "$";
		return;
	}

	stream << "(";
	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		const std::vector<std::vector<int> >& inner_vec = vec[ii];
		if( ii > 0 )
		{
			stream << ",";
		}
		writeNumericList2D( stream, inner_vec );
	}
	stream << ")";
}

template<typename T>
void writeNumericTypeList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec )
{
	//(38,12,4)
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
		stream << vec[ii]->m_value;
	}
	stream << ")";
}

template<typename T>
void writeNumericTypeList2D( std::stringstream& stream, const std::vector<std::vector<shared_ptr<T> > >& vec )
{
	//((.38,12.0,.04),(.38,1.0,346.0),(1.8,1.0,.04))
	if( vec.size() == 0 )
	{
		stream << "$";
		return;
	}
	stream << "(";
	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		const std::vector<shared_ptr<T> >& inner_vec = vec[ii];

		if( ii > 0 )
		{
			stream << ",";
		}
		writeNumericTypeList( stream, inner_vec );
	}
	stream << ")";
}

template<typename T>
void writeEntityList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec )
{
	//(#287,#291,$,#299)
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

		const shared_ptr<T>& entity = vec[ii];
		if( entity )
		{
			stream << "#" << entity->m_id;
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
}

template<typename T>
void writeEntityList2D( std::stringstream& stream, const std::vector<std::vector<shared_ptr<T> > >& vec )
{
	//((#287,#291,$,#299),(#287,#291,$,#299))
	if( vec.size() == 0 )
	{
		stream << "$";
		return;
	}

	stream << "(";
	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		const std::vector<shared_ptr<T> >& inner_vec = vec[ii];
		if( ii > 0 )
		{
			stream << ",";
		}
		writeEntityList( stream, inner_vec );
	}
	stream << ")";
}

template<typename T>
void writeEntityList3D( std::stringstream& stream, const std::vector<std::vector<std::vector<shared_ptr<T> > > >& vec )
{
	//(((#287,#291,$,#299),(#287,#291,$,#299)),((#287,#291,$,#299),(#287,#291,$,#299)))
	if( vec.size() == 0 )
	{
		stream << "$";
		return;
	}

	stream << "(";
	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		const std::vector<std::vector<shared_ptr<T> > >& inner_vec = vec[ii];

		if( ii > 0 )
		{
			stream << ",";
		}
		writeEntityList2D( stream, inner_vec );
	}
	stream << ")";
}
