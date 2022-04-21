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
#include "ifcpp/model/BasicTypes.h"

IFCQUERY_EXPORT std::string encodeStepString(const std::wstring& str);

IFCQUERY_EXPORT void appendRealWithoutTrailingZeros(std::stringstream& stream, const double number);
void writeRealList(std::stringstream& stream, const std::vector<double>& vec);
void writeRealList2D(std::stringstream& stream, const std::vector<std::vector<double> >& vec);
void writeRealList3D(std::stringstream& stream, const std::vector<std::vector<std::vector<double> > >& vec);
void writeIntList(std::stringstream& stream, const std::vector<int>& vec);
void writeIntList2D(std::stringstream& stream, const std::vector<std::vector<int> >& vec);
void writeIntList3D(std::stringstream& stream, const std::vector<std::vector<std::vector<int> > >& vec);

template<typename T>
void writeTypeOfIntList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec, bool optionalAttribute )
{
	// example: (38,12,4)
	if( vec.size() == 0 )
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
void writeTypeOfRealList(std::stringstream& stream, const std::vector<shared_ptr<T> >& vec, bool optionalAttribute)
{
	// example: (38.5, -1.2, 4.0)
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

		appendRealWithoutTrailingZeros(stream, vec[ii]->m_value);
	}
	stream << ")";
}

template<typename T>
void writeTypeOfIntList2D( std::stringstream& stream, const std::vector<std::vector<shared_ptr<T> > >& vec, bool optionalAttribute)
{
	// example: ((38,12,4),(38,1,346),(1,1,0))
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
		writeTypeOfIntList( stream, inner_vec, optionalAttribute );
	}
	stream << ")";
}

template<typename T>
void writeTypeOfRealList2D(std::stringstream& stream, const std::vector<std::vector<shared_ptr<T> > >& vec, bool optionalAttribute)
{
	// example: ((.38,12.0,.04),(.38,1.0,346.0),(1.8,1.0,.04))
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}
	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<shared_ptr<T> >& inner_vec = vec[ii];

		if (ii > 0)
		{
			stream << ",";
		}
		writeTypeOfRealList(stream, inner_vec, optionalAttribute);
	}
	stream << ")";
}

template<typename T>
void writeEntityList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec )
{
	// example: (#287,#291,$,#299)
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
			stream << "#" << entity->m_entity_id;
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
	// example: ((#287,#291,$,#299),(#287,#291,$,#299))
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
	// example: (((#287,#291,$,#299),(#287,#291,$,#299)),((#287,#291,$,#299),(#287,#291,$,#299)))
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

inline void writeStringList( std::wstringstream& stream, const std::vector<std::wstring >& vec_strings )
{
	// example: ("str1","str2","str3")
	stream << "(";
	for( size_t ii = 0; ii < vec_strings.size(); ++ii )
	{
		const std::wstring& str = vec_strings[ii];
		if( ii > 0 )
		{
			stream << ",";
		}
		stream << str.c_str();
	}
	stream << ")";
}
