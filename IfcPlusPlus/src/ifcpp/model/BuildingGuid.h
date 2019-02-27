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

#include <array>
#include <string>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

inline std::string createGUID32()
{
	std::stringstream uuid_strs;
	uuid_strs << std::uppercase;
	boost::uuids::uuid uuid = boost::uuids::random_generator()( );
	uuid_strs << uuid;
	return uuid_strs.str();
}

inline std::wstring createGUID32_wstr()
{
	std::wstringstream uuid_strs;
	uuid_strs << std::uppercase;
	boost::uuids::uuid uuid = boost::uuids::random_generator()( );
	uuid_strs << uuid;
	return uuid_strs.str();
}

///@brief Create IFC GUID string
///@details Use desired character type as template parameter - char or wchar_t.
///IFC uses a different base64 character set than RFC4648 - it starts with digits
///instead of uppercase letters and uses '_' and '$' as last two characters.
template<typename T>
inline std::basic_string<T> createBase64Uuid()
{
	static constexpr std::array<T, 64> base64Chars = {
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'_','$'
	};
	auto uuid = boost::uuids::random_generator()( );
	std::array<T const*, 22> string_builder;
	auto const stop = 16;
	//3 input byte will be encoded to 4 output characters
	for (size_t i = 0, j = 0; i < stop; ++i)
	{
		string_builder[j++] = &base64Chars[(uuid.data[i] & 0xFC) >> 2];
		string_builder[j] = &base64Chars[(uuid.data[i] & 0x03) << 4];
		if(stop <= ++i) break;
		string_builder[j++] += (uuid.data[i] & 0xF0) >> 4;
		string_builder[j] = &base64Chars[(uuid.data[i] & 0x0F) << 2];
		if(stop <= ++i) break;
		string_builder[j++] += (uuid.data[i] & 0xC0) >> 6;
		string_builder[j++] = &base64Chars[(uuid.data[i] & 0x3F)];
	}
	//IFC does not use fill characters
	std::basic_string<T> result;
	result.reserve(22);
	for(auto const& element : string_builder)
		result.push_back(*element);
	return result;
}

///@brief Decodes an IFC GUID string
///@details Expects a string with exactly 22 characters.
///@returns NIL UUID if the input string does not have exactly 22 characters or
///contains characters which are not part of the IFC base64 set.
template<typename T>
inline boost::uuids::uuid decodeBase64Uuid(std::basic_string<T> const& input)
{
	if(input.size() != 22)
		return boost::uuids::nil_generator()();
	static constexpr std::array<T, 64> base64Chars = {
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'_','$'
	};
	if(input.npos != input.find_first_not_of(base64Chars.data(), 0, base64Chars.size()))
		return boost::uuids::nil_generator()();
	auto uuid = boost::uuids::uuid{};
	auto const in_stop = 22;
	auto const out_stop = 16;
	auto index_of = [](T const in){
		for (uint8_t i = 0; i < base64Chars.size(); i++)
			if(in == base64Chars[i]) return i;
		return 0;
	};
	//4 input characters will be decoded to 3 output byte
	for (size_t i = 0, j = 0; i < in_stop && j < out_stop; ++i)
	{
		uuid.data[j] = index_of(input[i]) << 2;
		if(in_stop <= ++i) break;
		uuid.data[j++] += (index_of(input[i]) & 0x30) >> 4;
		if(out_stop <= j) break;
		uuid.data[j] = (index_of(input[i]) & 0x0F) << 4;
		if(in_stop <= ++i) break;
		uuid.data[j++] += (index_of(input[i]) & 0x3C) >> 2;
		if(out_stop <= j) break;
		uuid.data[j] = (index_of(input[i]) & 0x03) << 6;
		if(in_stop <= ++i) break;
		uuid.data[j++] += (index_of(input[i]) & 0x3F);
	}
	return uuid;
}
