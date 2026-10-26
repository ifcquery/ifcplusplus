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

#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BuildingObject.h"
#include "WriterUtil.h"

void appendRealWithoutTrailingZeros(std::stringstream& stream, const double number, size_t precision)
{
	std::ostringstream temp;
	temp.imbue(std::locale("C"));
	temp.precision(precision);
	temp << std::fixed << number;
	std::string str = temp.str();
	size_t pos_dot = str.find_last_of('.');
	if (pos_dot != std::string::npos)
	{
		// 1.000 -> 1.
		size_t pos_last_non_zero = str.find_last_not_of('0');
		if (pos_last_non_zero != std::string::npos && pos_last_non_zero >= pos_dot)
		{
			str.erase(pos_last_non_zero + 1, std::string::npos);
		}
	}
	stream << str;
}

void writeIntList(std::stringstream& stream, const std::vector<int>& vec)
{
	// example: (3,23,039)
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}
	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		if (ii > 0)
		{
			stream << ",";
		}

		stream << vec[ii];
	}
	stream << ")";
}

void writeIntList2D(std::stringstream& stream, const std::vector<std::vector<int> >& vec)
{
	// example: ((1,2,4),(3,23,039),(938,3,-3,6))
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}

	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<int>& inner_vec = vec[ii];
		if (ii > 0)
		{
			stream << ",";
		}
		writeIntList(stream, inner_vec);
	}
	stream << ")";
}

void writeIntList3D(std::stringstream& stream, const std::vector<std::vector<std::vector<int> > >& vec)
{
	// example: ((1,2,4),(3,23,39),(938,3,-35,6))
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}

	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<std::vector<int> >& inner_vec = vec[ii];
		if (ii > 0)
		{
			stream << ",";
		}
		writeIntList2D(stream, inner_vec);
	}
	stream << ")";
}

void writeRealList(std::stringstream& stream, const std::vector<double>& vec, bool optionalAttribute, size_t precision)
{
	// example: (3,23,039)
	if (vec.size() == 0)
	{
		if (optionalAttribute)
		{
			stream << "$";
		}
		else
		{
			stream << "()";
		}
		return;
	}
	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		if (ii > 0)
		{
			stream << ",";
		}

		appendRealWithoutTrailingZeros(stream, vec[ii], precision);
	}
	stream << ")";
}

void writeRealArray3(std::stringstream& stream, const double(&vec)[3], bool optionalAttribute, size_t precision)
{
	// example: (3,23,039)
	if (std::isnan(vec[0]) || std::isnan(vec[1]))
	{
		// less than 2 values are not allowed
		if (optionalAttribute)
		{
			stream << "$";
		}
		else
		{
			stream << "()";
		}

		return;
	}
	stream << "(";
	appendRealWithoutTrailingZeros(stream, vec[0], precision);
	stream << ",";
	appendRealWithoutTrailingZeros(stream, vec[1], precision);

	if (!std::isnan(vec[2]))
	{
		stream << ",";
		appendRealWithoutTrailingZeros(stream, vec[2], precision);
	}

	stream << ")";
}

void writeRealList2D(std::stringstream& stream, const std::vector<std::vector<double> >& vec, bool optionalAttribute, size_t precision)
{
	// example: ((1,2,4),(3,23,039),(938,3,-3,6))
	if (vec.size() == 0)
	{
		//stream << "$";
		if (optionalAttribute)
		{
			stream << "$";
		}
		else
		{
			stream << "()";
		}
		return;
	}

	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<double>& inner_vec = vec[ii];
		if (ii > 0)
		{
			stream << ",";
		}
		writeRealList(stream, inner_vec, false, precision);
	}
	stream << ")";
}

void writeRealList3D(std::stringstream& stream, const std::vector<std::vector<std::vector<double> > >& vec, size_t precision)
{
	// example: ((1,2,4),(3,23,39),(938,3,-35,6))
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}

	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<std::vector<double> >& inner_vec = vec[ii];
		if (ii > 0)
		{
			stream << ",";
		}
		writeRealList2D(stream, inner_vec, false, precision);
	}
	stream << ")";
}

void writeStepParameterDouble( double value, std::stringstream& stream, const std::string& classIDstr, bool is_select_type, size_t precision )
{
	if( std::isnan(value) )
	{
		stream << "$";
		return;
	}

	if( is_select_type ) 
	{
		//std::string classIDstr = IFC4X3::EntityFactory::getStringForClassID(classID);
		//std::transform(classIDstr.begin(), classIDstr.end(), classIDstr.begin(), [](char c) {return std::toupper(c); });
		stream << classIDstr << "(";     // for example "IFCLABEL(";
	}
	//stream << "'" << encodeStepString( m_value ) << "'";
	appendRealWithoutTrailingZeros( stream, value, precision );
	if( is_select_type ) { stream << ")"; }
}

std::string encodeStepString( const std::string& str )
{
	char* stream_pos = const_cast<char*>(str.c_str());
	std::string result_str;
	std::string beginUnicodeTag = "\\X2\\";
	std::string endUnicodeTag = "\\X0\\";
	bool hasOpenedUnicodeTag = false;

	auto closeUnicodeBlockIfOpened = [&]
	{
		if( hasOpenedUnicodeTag )
		{
			result_str += endUnicodeTag;
			hasOpenedUnicodeTag = false;
		}
	};

	while( *stream_pos != '\0' )
	{
		char append_char = *stream_pos;
		if( append_char == 10 )
		{
			closeUnicodeBlockIfOpened();
			// encode new line
			result_str.append( "\\X\\0A" );
		}
		else if( append_char == 13 )
		{
			closeUnicodeBlockIfOpened();
			// encode carriage return
			result_str.append( "\\X\\0D" );
		}
		else if( append_char == 39 )
		{
			closeUnicodeBlockIfOpened();
			// encode apostrophe
			result_str.append( "\\X\\27" );
		}
		else if( append_char == 92 )
		{
			closeUnicodeBlockIfOpened();
			// encode backslash
			result_str.append( "\\\\" );
		}
		else if( append_char >= 32 && append_char < 127 )
		{
			closeUnicodeBlockIfOpened();
			result_str.push_back( static_cast<char>(append_char) );
		}
		else if( append_char > 0 && append_char < 128 )
		{
			// other control characters, for example TAB, are not allowed unencoded in a STEP string
			closeUnicodeBlockIfOpened();
			char temporary[8];
			sprintf( temporary, "\\X\\%02X", static_cast<unsigned int>(append_char) );
			result_str += temporary;
		}
		else
		{
			// strings are UTF-8: decode the sequence to one code point. Bytes that are not valid UTF-8 are kept as ISO 8859-1 characters
			const unsigned char lead = static_cast<unsigned char>(append_char);
			size_t num_bytes = 1;
			uint32_t code_point = lead;
			if( lead >= 0xC2 && lead <= 0xDF ) { num_bytes = 2; code_point = lead & 0x1F; }
			else if( lead >= 0xE0 && lead <= 0xEF ) { num_bytes = 3; code_point = lead & 0x0F; }
			else if( lead >= 0xF0 && lead <= 0xF4 ) { num_bytes = 4; code_point = lead & 0x07; }

			for( size_t ii = 1; ii < num_bytes; ++ii )
			{
				const unsigned char continuation = static_cast<unsigned char>(stream_pos[ii]);
				if( (continuation & 0xC0) != 0x80 )
				{
					num_bytes = 1;
					break;
				}
				code_point = (code_point << 6) | (continuation & 0x3F);
			}

			const uint32_t min_code_point[5] = { 0, 0, 0x80, 0x800, 0x10000 };
			if( num_bytes == 1 || code_point < min_code_point[num_bytes] || (code_point >= 0xD800 && code_point <= 0xDFFF) || code_point > 0x10FFFF )
			{
				num_bytes = 1;
				code_point = lead;
			}

			char temporary[20];
			if( code_point <= 0xFFFF )
			{
				if( !hasOpenedUnicodeTag )
				{
					result_str += beginUnicodeTag;
					hasOpenedUnicodeTag = true;
				}
				sprintf( temporary, "%04X", static_cast<unsigned int>(code_point) );
				result_str += temporary;
			}
			else
			{
				// characters outside the BMP have no two-octet representation
				closeUnicodeBlockIfOpened();
				sprintf( temporary, "\\X4\\%08X\\X0\\", static_cast<unsigned int>(code_point) );
				result_str += temporary;
			}
			stream_pos += num_bytes - 1;
		}
		++stream_pos;
	}

	closeUnicodeBlockIfOpened();
	return result_str;
}
