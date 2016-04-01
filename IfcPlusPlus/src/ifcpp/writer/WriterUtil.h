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

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <ifcpp/model/IfcPPGlobal.h>
#include <ifcpp/model/shared_ptr.h>

class IfcPPObject;

IFCPP_EXPORT std::string encodeStepString( std::wstring arg_str );

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
void writeNumericList3D( std::stringstream& stream, const std::vector<std::vector<std::vector<int> > >& vec )
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

//void writeIntList( std::stringstream& stream, const std::vector<int>& vec );
//void writeIntList2D( std::stringstream& stream, const std::vector<std::vector<int> >& vec );
//void writeIntList3D( std::stringstream& stream, const std::vector<std::vector<std::vector<int> > >& vec );
//void writeDoubleList( std::stringstream& stream, const std::vector<double>& vec );
//void writeDoubleList2D( std::stringstream& stream, const std::vector<std::vector<double> >& vec );
//void writeDoubleList3D( std::stringstream& stream, const  std::vector<std::vector<std::vector<double> > >& vec );
void writeConstCharList( std::stringstream& stream, const std::vector<const char*>& vec );
void writeStringList( std::stringstream& stream, const std::vector<std::wstring>& vec );

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

//template<typename T>
//void writeTypeOfRealList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec )
//{
//	//(.38,12.0,.04)
//	if( vec.size() == 0 )
//	{
//		stream << "$";
//		return;
//	}
//	stream << "(";
//	typename std::vector<shared_ptr<T> >::const_iterator it;
//
//	for( it = vec.begin(); it != vec.end(); ++it )
//	{
//		if( it != vec.begin() )
//		{
//			stream << ",";
//		}
//		stream << ( *it )->m_value;
//	}
//	stream << ")";
//}

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

template<typename T>
void writeTypeList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec, bool is_select_type = false )
{
	//(#287,#291,$,#299)
	if( vec.size() == 0 )
	{
		stream << "()";
		return;
	}
	stream << "(";
	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}

		const shared_ptr<T>& type_object = vec[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, is_select_type );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
}

template<typename T>
void writeTypeList2D( std::stringstream& stream, const std::vector<std::vector<shared_ptr<T> > >& vec, bool is_select_type = false )
{
	//(#287,#291,$,#299)
	if( vec.size() == 0 )
	{
		stream << "()";
		return;
	}
	stream << "(";

	for( size_t ii = 0; ii < vec.size(); ++ii )
	{
		const std::vector<shared_ptr<T> >&inner_vec = vec[ii];
		
		if( ii > 0 )
		{
			stream << "),(";
		}

		for( size_t jj = 0; jj < inner_vec.size(); ++jj )
		{
			if( jj > 0 )
			{
				stream << ",";
			}

			const shared_ptr<T>& type_object = inner_vec[jj];
			if( type_object )
			{
				type_object->getStepParameter( stream, is_select_type );
			}
			else
			{
				stream << "$";
			}
		}
	}
	stream << ")";
}
