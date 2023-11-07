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
#include <iostream>
#include <limits>
#include <locale>
#include <string>

#include "ifcpp/model/BuildingException.h"
#include "ReaderUtil.h"

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif
#ifdef _MSC_VER
#include <windows.h>
#else
#include <cctype>
#include <codecvt>
#endif

static short convertToHex(unsigned char mc)
{
	short returnValue;

	if (mc >= '0' && mc <= '9')
	{
		returnValue = static_cast<short>(mc) - static_cast<short>('0');
	}
	else if (mc >= 'A' && mc <= 'F')
	{
		returnValue = 10 + static_cast<short>(mc) - static_cast<short>('A');
	}
	else if (mc >= 'a' && mc <= 'f')
	{
		returnValue = 10 + static_cast<short>(mc) - static_cast<short>('a');
	}
	else
	{
		returnValue = 0;
	}
	return (returnValue);
}

static char Hex2Char(unsigned char h1, unsigned char h2)
{
	char returnValue = (convertToHex(h1) << 4) + convertToHex(h2);
	return (returnValue);
}

std::string wstring2string(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();

#ifdef _MSC_VER
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
#else

	try
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> StringConverter;
		return StringConverter.to_bytes(wstr);
	}
	catch (...)
	{
		std::cout << "std::use_facet failed" << std::endl;
	}

	return "";
#endif
}

std::wstring string2wstring(const std::string& inputString)
{
	if (inputString.empty()) return std::wstring();

#ifdef _MSC_VER
	std::wstringstream ws;
	ws << inputString.c_str();
	std::wstring result = ws.str();
	return result;
#else
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(inputString);
#endif
}

void checkOpeningClosingParenthesis(const char* ch_check)
{
	int num_opening = 0;
	int num_closing = 0;
	while (*ch_check != '\0')
	{
		if (*ch_check == '(')
		{
			++num_opening;
		}
		else if (*ch_check == ')')
		{
			++num_closing;
		}
		else if (*ch_check == '\'')
		{
			findEndOfString(ch_check);
			continue;
		}
		++ch_check;
	}
	if (num_opening != num_closing)
	{
		std::stringstream err;
		err << "checkOpeningClosingParenthesis: num_opening != num_closing " << std::endl;
		throw BuildingException(err.str(), __FUNC__);
	}
}

std::istream& bufferedGetline(std::istream& inputStream, std::string& lineOut)
{
	lineOut.clear();
	std::istream::sentry se(inputStream, true);
	std::streambuf* sb = inputStream.rdbuf();

	// std::getline does not work with all line endings, reads complete file instead.
	// Handle \n (unix), \r\n (windows), \r (mac) line endings here
	while (true)
	{
		int c = sb->sbumpc();
		switch (c)
		{
		case '\n':
			return inputStream;
		case '\r':
			if (sb->sgetc() == '\n')
			{
				sb->sbumpc();
			}
			return inputStream;
		case std::streambuf::traits_type::eof():
			// in case the last line has no line ending
			if (lineOut.empty())
			{
				inputStream.setstate(std::ios::eofbit);
			}
			return inputStream;
		default:
			lineOut += (char)c;
		}
	}
}

std::istream& bufferedGetStepLine(std::istream& inputStream, std::string& lineOut)
{
	lineOut.clear();
	std::istream::sentry se(inputStream, true);
	std::streambuf* sb = inputStream.rdbuf();
	bool inString = false;

#ifdef _DEBUG
	std::string string40;
#endif	

	// std::getline does not work with all line endings, reads complete file instead.
	// Handle \n (unix), \r\n (windows), \r (mac) line endings here
	while (true)
	{
		int c = sb->sbumpc();  // sbumpc: character at the current position and advances the current position to the next character
#ifdef _DEBUG
		std::string charAsString;
		charAsString += ((char)c);
		string40 += charAsString;
#endif
		switch (c)
		{
		case ';':
		{
			if (!inString)
			{
				int nextChar = sb->sgetc();
				if (isspace(nextChar))
				{
					sb->sbumpc();  // sbumpc: character at the current position and advances the current position to the next character
				}
				return inputStream;
			}
			lineOut += (char)c;
			continue;
		}
		case '\'':
		{
			inString = !inString;
			lineOut += (char)c;
			continue;
		}
		case '/':
			if (!inString)
			{
				int nextChar = sb->sgetc();
#ifdef _DEBUG
				std::string charAsString2;
				charAsString2 += ((char)nextChar);
#endif
				if (nextChar == '*')
				{
#ifdef _DEBUG
					std::string commentIgnored = "/*";
#endif
					sb->sbumpc();

					// continue till end of /*   */ comment
					bool inMultiLineComment = true;
					while (inMultiLineComment)
					{
						int c2 = sb->sbumpc();
						if (c2 == '*')
						{
							char c3 = sb->sgetc();
							if (c3 == '/')
							{
								sb->sbumpc();
								inMultiLineComment = false;

								// skip whitespaces
								while (true)
								{
									char c4 = sb->sgetc();
									if (isspace(c4))
									{
										sb->sbumpc();
									}
									else if (c4 == std::streambuf::traits_type::eof())
									{
										if (lineOut.empty())
										{
											inputStream.setstate(std::ios::eofbit);
										}
										return inputStream;
									}
									else
									{
										break;
									}
								}

								break;
							}
						}
						else if (c2 == std::streambuf::traits_type::eof())
						{
							// in case the last line has no line ending
							if (lineOut.empty())
							{
								inputStream.setstate(std::ios::eofbit);
							}
							return inputStream;
						}
#ifdef _DEBUG
						commentIgnored += c2;
#endif
					}
					continue;
				}
			}
			lineOut += (char)c;
			continue;

		case '\n':
			continue;

		case '\r':
			continue;

		case std::streambuf::traits_type::eof():
			// in case the last line has no line ending
			if (lineOut.empty())
			{
				inputStream.setstate(std::ios::eofbit);
			}
			return inputStream;
		default:
			lineOut += (char)c;
		}
	}
}

void findLeadingTrailingParanthesis(char* ch, char*& pos_opening, char*& pos_closing)
{
	short num_opening = 0;
	while (*ch != '\0')
	{
		if (*ch == '\'')
		{
			++ch;
			// beginning of string, continue to end
			while (*ch != '\0')
			{
				if (*ch == '\'')
				{
					break;
				}
				++ch;
			}
			++ch;
			continue;
		}

		if (*ch == '(')
		{
			if (num_opening == 0)
			{
				pos_opening = ch;
			}
			++num_opening;
		}
		else if (*ch == ')')
		{
			--num_opening;
			if (num_opening == 0)
			{
				pos_closing = ch;
			}
		}
		++ch;
	}
}

bool findEndOfStepLine(char* ch, char*& pos_end)
{
	short num_opening = 0;
	while (*ch != '\0')
	{
		if (*ch == '\'')
		{
			++ch;
			// beginning of string, continue to end
			while (*ch != '\0')
			{
				if (*ch == '\'')
				{
					break;
				}
				++ch;
			}
			if (*ch == '\0')
			{
				// end of string inside comment, so not a valid end of STEP line
				return false;
			}
			++ch;
			continue;
		}

		if (*ch == '(')
		{
			++num_opening;
		}
		else if (*ch == ')')
		{
			--num_opening;
			if (num_opening == 0)
			{
				while (isspace(*ch)) { ++ch; }

				if (*ch == ';')
				{
					pos_end = ch;
					return true;
				}
			}
		}
		else if (*ch == ';')
		{
			if (num_opening == 0)
			{
				pos_end = ch;
				return true;
			}
		}
		++ch;
	}
	return false;
}

void tokenizeList(std::string& list_str, std::vector<std::string>& list_items)
{
	if (list_str.empty())
	{
		return;
	}
	char* stream_pos = const_cast<char*>(list_str.c_str());
	char* last_token = stream_pos;
	int numNestedLists = 0;
	while (*stream_pos != '\0')
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
		else if (*stream_pos == '\'')
		{
			++stream_pos;
			// beginning of string, continue to end
			while (*stream_pos != '\0')
			{
				if (*stream_pos == '\'')
				{
					break;
				}
				++stream_pos;
			}
			++stream_pos;
			continue;
		}

		if (*stream_pos == ',' && numNestedLists == 0)
		{
			std::string item(last_token, stream_pos - last_token);
			list_items.push_back(item);

			++stream_pos;
			while (isspace(*stream_pos)) { ++stream_pos; }
			last_token = stream_pos;
			if (*stream_pos == '\0')
			{
				throw BuildingException("tokenizeList: *stream_pos == '\0'", __FUNC__);
			}
			continue;
		}
		++stream_pos;
	}
	// pick up last element
	if (last_token != nullptr)
	{
		if (last_token != stream_pos)
		{
			std::string item(last_token, stream_pos - last_token);
			list_items.push_back(item);
		}
	}
}

void tokenizeEntityList(std::string& list_str, std::vector<int>& list_items)
{
	if (list_str.empty())
	{
		return;
	}
	char* stream_pos = const_cast<char*>(list_str.c_str());
	while (*stream_pos != '\0')
	{
		// skip whitespace
		while (isspace(*stream_pos)) { ++stream_pos; }

		if (*stream_pos == '#')
		{
			++stream_pos;
			// beginning of id
			char* begin_id = stream_pos;

			// proceed until end of integer
			++stream_pos;
			while (*stream_pos != '\0')
			{
				if (isdigit(*stream_pos))
				{
					++stream_pos;
				}
				else
				{
					break;
				}
			}
			const int id = std::stoi(std::string(begin_id, stream_pos - begin_id));
			list_items.push_back(id);
		}
		else if (*stream_pos == '$')
		{
			// empty
		}
		else
		{
			std::stringstream err;
			err << "tokenizeEntityList: unexpected argument: " << list_str.c_str() << std::endl;
			throw BuildingException(err.str(), __FUNC__);
		}

		while (isspace(*stream_pos))
		{
			++stream_pos;
		}
		if (*stream_pos == ',')
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

void readIntegerList(const std::string& str, std::vector<int>& vec)
{
	const char* ch = str.c_str();
	const size_t argsize = str.size();
	if (argsize == 0)
	{
		return;
	}
	size_t i = 0;
	size_t last_token = 0;
	while (i < argsize)
	{
		if (ch[i] == '(')
		{
			++i;
			last_token = i;
			break;
		}
		++i;
	}

	while (i < argsize)
	{
		if (ch[i] == ',')
		{
			size_t str_length = i - last_token;
			if (str_length > 0)
			{
				vec.push_back(std::stoi(str.substr(last_token, i - last_token)));
			}
			last_token = i + 1;
		}
		else if (ch[i] == ')')
		{
			size_t str_length = i - last_token;
			if (str_length > 0)
			{
				vec.push_back(std::stoi(str.substr(last_token, i - last_token)));
			}
			return;
		}
		++i;
	}
}

void readIntegerList2D(const std::string& str, std::vector<std::vector<int> >& vec)
{
	// ((1,2,4),(3,23,039),(938,3,-3,6))
	const char* ch = str.c_str();
	const size_t argsize = str.size();
	if (argsize == 0)
	{
		return;
	}
	size_t i = 0;
	size_t num_par_open = 0;
	size_t last_token = 0;
	while (i < argsize)
	{
		if (ch[i] == ',')
		{
			if (num_par_open == 1)
			{
				std::vector<int> inner_vec;
				vec.push_back(inner_vec);
				readIntegerList(str.substr(last_token, i - last_token), inner_vec);
				last_token = i + 1;
			}
		}
		else if (ch[i] == '(')
		{
			++num_par_open;
		}
		else if (ch[i] == ')')
		{
			--num_par_open;
			if (num_par_open == 0)
			{
				std::vector<int> inner_vec;
				vec.push_back(inner_vec);
				readIntegerList(str.substr(last_token, i - last_token), inner_vec);
				return;
			}
		}
		++i;
	}
}

void readRealList(const std::string& str, std::vector<double>& vec)
{
	const char* ch = str.c_str();
	const size_t argsize = str.size();
	if (argsize == 0)
	{
		return;
	}
	size_t i = 0;
	size_t last_token = 0;
	while (i < argsize)
	{
		if (ch[i] == '(')
		{
			++i;
			last_token = i;
			break;
		}
		++i;
	}

	while (i < argsize)
	{
		if (ch[i] == ',')
		{
			vec.push_back(std::stod(str.substr(last_token, i - last_token)));
			last_token = i + 1;
		}
		else if (ch[i] == ')')
		{
			vec.push_back(std::stod(str.substr(last_token, i - last_token)));
			return;
		}
		++i;
	}
}

void readRealArray(const std::string& str, double(&vec)[3], short int& size)
{
	const char* ch = str.c_str();
	const size_t argsize = str.size();
	if (argsize == 0)
	{
		return;
	}
	size_t i = 0;
	size_t last_token = 0;
	while (i < argsize)
	{
		if (ch[i] == '(')
		{
			++i;
			last_token = i;
			break;
		}
		++i;
	}
	short idx = 0;
	while (i < argsize)
	{
		if (ch[i] == ',')
		{
			if (idx < 3)
			{
				vec[idx] = std::stod(str.substr(last_token, i - last_token));
			}
			++idx;
			last_token = i + 1;
		}
		else if (ch[i] == ')')
		{
			if (idx < 3)
			{
				vec[idx] = std::stod(str.substr(last_token, i - last_token));
			}
			size = idx + 1;
			return;
		}
		++i;
	}
}

void readRealList2D(const std::string& str, std::vector<std::vector<double> >& vec)
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	const char* ch = str.c_str();
	const size_t argsize = str.size();
	if (argsize == 0)
	{
		return;
	}
	size_t i = 0;
	size_t num_par_open = 0;
	size_t last_token = 0;
	while (i < argsize)
	{
		if (ch[i] == ',')
		{
			if (num_par_open == 1)
			{
				std::vector<double> inner_vec;
				vec.push_back(inner_vec);
				readRealList(str.substr(last_token, i - last_token), vec.back());
				last_token = i;
			}
		}
		else if (ch[i] == '(')
		{
			++num_par_open;
			last_token = i;
		}
		else if (ch[i] == ')')
		{
			--num_par_open;
			if (num_par_open == 0)
			{
				std::vector<double> inner_vec;
				vec.push_back(inner_vec);
				readRealList(str.substr(last_token, i - last_token), vec.back());
				return;
			}
		}
		++i;
	}
}

void readRealList3D(const std::string& str, std::vector<std::vector<std::vector<double> > >& vec)
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	const char* ch = str.c_str();
	const size_t argsize = str.size();
	if (argsize == 0)
	{
		return;
	}
	size_t i = 0;
	size_t num_par_open = 0;
	size_t last_token = 0;
	while (i < argsize)
	{
		if (ch[i] == ',')
		{
			if (num_par_open == 1)
			{
				std::vector<std::vector<double> > inner_vec;
				vec.push_back(inner_vec);
				readRealList2D(str.substr(last_token, i - last_token), vec.back());
				last_token = i;
			}
		}
		else if (ch[i] == '(')
		{
			++num_par_open;
			last_token = i;
		}
		else if (ch[i] == ')')
		{
			--num_par_open;
			if (num_par_open == 0)
			{
				std::vector<std::vector<double> > inner_vec;
				vec.push_back(inner_vec);
				readRealList2D(str.substr(last_token, i - last_token), vec.back());
				return;
			}
		}
		++i;
	}
}

void readBinary(const std::string& str, std::string& target)
{
	target = str;
}

void readBinaryString(const std::string& attribute_value, std::string& target)
{
	if (attribute_value.size() < 2)
	{
		target = attribute_value;
		return;
	}
	if (attribute_value[0] == '"' && attribute_value[attribute_value.size() - 1] == '"')
	{
		target = attribute_value.substr(1, attribute_value.size() - 2);
	}
}

void readBinaryList(const std::string& str, std::vector<std::string>& vec)
{
	readStringList(str, vec);
}

void readStringList(const std::string& str, std::vector<std::string>& vec)
{
	const char* ch = str.c_str();
	const size_t argsize = str.size();
	if (argsize == 0)
	{
		return;
	}
	size_t i = 0;
	size_t last_token = 0;
	while (i < argsize)
	{
		if (ch[i] == '(')
		{
			++i;
			last_token = i;
			break;
		}
		++i;
	}
	while (i < argsize)
	{
		if (ch[i] == ',')
		{
			vec.push_back(str.substr(last_token, i - last_token));
			last_token = i + 1;
		}
		else if (ch[i] == ')')
		{
			vec.push_back(str.substr(last_token, i - last_token));
			return;
		}
		++i;
	}
}

void findEndOfString(const char*& stream_pos)
{
	++stream_pos;
	const char* pos_begin = stream_pos;

	// beginning of string, continue to end
	while (*stream_pos != '\0')
	{
		if (*stream_pos == '\\')
		{
			if (*(stream_pos + 1) == 'X')
			{
				if (*(stream_pos + 2) == '0' || *(stream_pos + 2) == '2' || *(stream_pos + 2) == '4')
				{
					if (*(stream_pos + 3) == '\\')
					{
						// ISO 10646 encoding, continue
						stream_pos += 4;
						continue;
					}
				}
			}

			if (*(stream_pos + 1) == '\\')
			{
				// we have a double backslash, so just continue
				++stream_pos;
				++stream_pos;
				continue;
			}
			if (*(stream_pos + 1) == '\'')
			{
				// quote is escaped
				++stream_pos;
				++stream_pos;
				continue;
			}
		}

		if (*stream_pos == '\'')
		{
			if (*(stream_pos + 1) == '\'')
			{
				// two single quotes in string
				if (stream_pos != pos_begin)
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

void decodeArgumentStrings(std::vector<std::string>& entity_arguments, std::vector<std::string>& args_out)
{
	for (auto& argument_str : entity_arguments)
	{
		const size_t arg_length = argument_str.length();
		if (arg_length == 0)
		{
			continue;
		}

		std::string arg_str_new;

		char* stream_pos = const_cast<char*>(argument_str.c_str());		// ascii characters from STEP file
		while (*stream_pos != '\0')
		{
			if (*stream_pos == '\\')
			{
				if (*(stream_pos + 1) == 'S')
				{
					if (*(stream_pos + 2) == '\\')
					{
						if (*(stream_pos + 3) != '\0')
						{
							if (*(stream_pos + 4) == '\\')
							{
								if (*(stream_pos + 5) == 'S')
								{
									if (*(stream_pos + 6) == '\\')
									{
										if (*(stream_pos + 7) != '\0')
										{
											char first = *(stream_pos + 3);
											char second = *(stream_pos + 7);
											char append_char = char(125 + first + second);
											arg_str_new += append_char;
											stream_pos += 8;
											continue;
										}
									}
								}
								else if (*(stream_pos + 5) == 'Q')
								{
									if (*(stream_pos + 6) == '\\')
									{
										if (*(stream_pos + 7) != '\0')
										{
											char first = *(stream_pos + 3);
											char second = *(stream_pos + 7);
											char append_char = char(125 + first + second);
											arg_str_new += append_char;
											stream_pos += 8;
											continue;
										}
									}
								}
							}
							else
							{
								// next characters code value v shall be interpreted as v + 128
								char first = *(stream_pos + 3);
								char append_char = char(128 + first);
								arg_str_new += append_char;
								stream_pos += 4;
								continue;
							}
						}
					}
				}
				else if (*(stream_pos + 1) == 'X')
				{
					if (*(stream_pos + 2) == '\\')
					{
						char wc = Hex2Char(*(stream_pos + 3), *(stream_pos + 4));

						//unsigned char char_ascii = wctob(wc);
						arg_str_new += wc;

						stream_pos += 5;
						continue;
					}
					else if (*(stream_pos + 2) == '0')
					{
						if (*(stream_pos + 3) == '\\')
						{
							stream_pos += 4;
							continue;
						}
					}
					else if (*(stream_pos + 2) == '2')
					{
						if (*(stream_pos + 3) == '\\')
						{
							// the following sequence of multiples of four hexadecimal characters shall be interpreted as encoding the 
							// two-octet representation of characters from the BMP in ISO 10646

							bool finished = false;
							stream_pos += 4;

							std::vector<char> utf16Characters;
							do
							{
								char h1 = *(stream_pos);
								char h2 = *(stream_pos + 1);
								char h3 = *(stream_pos + 2);
								char h4 = *(stream_pos + 3);

								char c1 = Hex2Char(h1, h2);
								char c2 = Hex2Char(h3, h4);
								utf16Characters.push_back(c1);
								utf16Characters.push_back(c2);

								stream_pos += 4;

							} while ((*stream_pos != '\0') && (*stream_pos != '\\'));

							for (int i = 0; i < utf16Characters.size(); i += 2)
							{
								std::swap(utf16Characters[i], utf16Characters[i + 1]);
							}

#ifdef _MSC_VER
							std::wstring w_str(reinterpret_cast<wchar_t*>(&utf16Characters[0]), utf16Characters.size() / 2);
							std::string convertedStr = wstring2string(w_str);
							arg_str_new += convertedStr;
#else
							std::u16string u16str(reinterpret_cast<char16_t*>(&utf16Characters[0]), utf16Characters.size() / 2);
							std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
							std::string utf8 = convert.to_bytes(u16str);
							arg_str_new += utf8;
#endif



							continue;
						}
					}
				}
				else if (*(stream_pos + 1) == 'N')
				{
					if (*(stream_pos + 2) == '\\')
					{
						arg_str_new.append("\n");
						stream_pos += 3;
						continue;
					}
				}
			}

			char current_char = *stream_pos;
			arg_str_new += current_char;
			++stream_pos;
		}

		args_out.push_back(arg_str_new);

	}
}

void readBool(const std::string& attribute_value, bool& target)
{
	if (std_iequal(attribute_value, ".F."))
	{
		target = false;
	}
	else if (std_iequal(attribute_value, ".T."))
	{
		target = true;
	}
}

void readLogical(const std::string& attribute_value, LogicalEnum& target)
{
	if (std_iequal(attribute_value, ".F."))
	{
		target = LOGICAL_FALSE;
	}
	else if (std_iequal(attribute_value, ".T."))
	{
		target = LOGICAL_TRUE;
	}
	else if (std_iequal(attribute_value, ".U."))
	{
		target = LOGICAL_UNKNOWN;
	}
}

void readInteger(const std::string& attribute_value, int& target)
{
	target = std::stoi(attribute_value);
}

void readIntegerValue(const std::string& str, int& int_value)
{
	if (str.compare("$") == 0)
	{
		int_value = std::numeric_limits<int>::quiet_NaN();
	}
	else if (str.compare("*") == 0)
	{
		int_value = std::numeric_limits<int>::quiet_NaN();
	}
	else
	{
		int_value = std::stoi(str);
	}
}

void readReal(const std::string& attribute_value, double& target)
{
	target = std::stod(attribute_value);
}

void readString(const std::string& attribute_value, std::string& target)
{
	if (attribute_value.size() < 2)
	{
		target = attribute_value;
		return;
	}
	if (attribute_value[0] == '\'' && attribute_value[attribute_value.size() - 1] == '\'')
	{
		target = attribute_value.substr(1, attribute_value.size() - 2);
	}
	else
	{
		target = attribute_value;
	}
}

void addArgument(const char* stream_pos, const char*& last_token, std::vector<std::string>& entity_arguments)
{
	if (*last_token == ',')
	{
		++last_token;
	}

	const char* begin_arg = last_token;

	// skip whitespace
	while (isspace(*begin_arg))
	{
		++begin_arg;
	}

	int remaining_size = static_cast<int>(stream_pos - begin_arg);
	if (remaining_size > 0)
	{
		const char* end_arg = stream_pos - 1;
		entity_arguments.emplace_back(begin_arg, end_arg - begin_arg + 1);
	}
	last_token = stream_pos;
}

//\brief split one string into a vector of argument strings
// caution: when using threads, this method runs in parallel threads
void tokenizeEntityArguments(const std::string& argument_str, std::vector<std::string>& entity_arguments)
{
	if (argument_str.size() == 0)
	{
		return;
	}
	const char* stream_pos = argument_str.c_str();
	int num_open_braces = 0;
	const char* last_token = stream_pos;

	while (*stream_pos != '\0')
	{
		if (*stream_pos == '\'')
		{
			findEndOfString(stream_pos);
			continue;
		}

		if (*stream_pos == '(')
		{
			++num_open_braces;
		}
		else if (*stream_pos == ',')
		{
			if (num_open_braces == 0)
			{
				const char* last_token_check = last_token;
				addArgument(stream_pos, last_token, entity_arguments);
			}
		}
		else if (*stream_pos == ')')
		{
			--num_open_braces;
			if (num_open_braces == 0)
			{
				++stream_pos;
				addArgument(stream_pos, last_token, entity_arguments);

				const char* stream_pos_begin = argument_str.c_str();
				size_t readCount = stream_pos - stream_pos_begin;

				if (readCount == argument_str.size())
				{
					break;
				}
			}
		}
		++stream_pos;
	}

	if (*last_token != *stream_pos)
	{
		if (*last_token != '\0')
		{
			addArgument(stream_pos, last_token, entity_arguments);
		}
	}
}

//\brief split one string into a vector of argument strings
// caution: when using threads, this method runs in parallel threads
void tokenizeInlineArgument(std::string arg, std::string& keyword, std::string& inline_arg)
{
	if (arg.empty())
	{
		throw BuildingException("arg.size() == 0", __FUNC__);
	}
	if (arg[0] == '$')
	{
		return;
	}
	if (arg[0] == '*')
	{
		return;
	}
	if (arg[0] == '#')
	{
		throw BuildingException("tokenizeInlineArgument: argument begins with #, so it is not inline", __FUNC__);
	}

	char* stream_pos = const_cast<char*>(arg.c_str());
	while (isspace(*stream_pos)) { ++stream_pos; }

	char* begin_keyword = stream_pos;
	while (isalnum(*stream_pos)) { ++stream_pos; }

	// get type name
	std::string key(begin_keyword, stream_pos - begin_keyword);

	if (key.empty())
	{
		// single argument, for example .T.
		inline_arg = arg;
		return;
	}

	// proceed to '('
	int numOpenBraces = 0;
	if (*stream_pos == '(')
	{
		++stream_pos;
		++numOpenBraces;
	}
	else
	{
		while (*stream_pos != '\0')
		{
			if (*stream_pos == '(')
			{
				++stream_pos;
				++numOpenBraces;
				break;
			}
			++stream_pos;
		}
	}

	// proceed to ')'
	std::string inline_argument;
	char* inline_argument_begin = stream_pos;

	while (*stream_pos != '\0')
	{
		if (*stream_pos == '\'')
		{
			++stream_pos;
			// inside string
			while (*stream_pos != '\0')
			{
				if (*stream_pos == '\'')
				{
					// check if tick is escaped
					char* tick_pos = stream_pos;
					bool tick_escaped = false;
					while (tick_pos != begin_keyword)
					{
						--tick_pos;
						if (*tick_pos == '\\')
						{
							tick_escaped = !tick_escaped;
							continue;
						}
						break;
					}
					if (tick_escaped)
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

		if (*stream_pos == ')')
		{
			--numOpenBraces;
			// skip whitespace
			while (isspace(*inline_argument_begin))
			{
				++inline_argument_begin;
			}
			if (numOpenBraces == 0)
			{
				char* end_arg = stream_pos - 1;
				inline_argument = std::string(inline_argument_begin, end_arg - inline_argument_begin + 1);
				break;
			}
		}
		++stream_pos;
	}

	std::transform(key.begin(), key.end(), key.begin(), [](char c) {return static_cast<char>(std::toupper(c)); });
	keyword = key;
	inline_arg = inline_argument;
}

bool std_iequal(const std::string& a, const std::string& b)
{
	if (a.size() == b.size())
	{
		return std::equal(a.begin(), a.end(), b.begin(), [](const char l, const char r) { return std::toupper(l) == std::toupper(r); });
	}
	return false;
}
