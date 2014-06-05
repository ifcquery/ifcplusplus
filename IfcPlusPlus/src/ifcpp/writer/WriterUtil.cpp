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

void writeIntList( std::stringstream& stream, const std::vector<int>& vec )
{
	// (3,23,039)
	stream << "(";
	std::vector<int>::const_iterator it;
	
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		if( it != vec.begin() )
		{
			stream << ",";
		}
		stream << (*it);
	}
	stream << ")";
}

void writeIntList2D( std::stringstream& stream, const std::vector<std::vector<int> >& vec )
{
	// ((1,2,4),(3,23,039),(938,3,-3,6))
	std::vector<std::vector<int> >::const_iterator it1;
	stream << "(";
	for( it1=vec.begin(); it1!=vec.end(); ++it1 )
	{
		const std::vector<int>& inner_vec = (*it1);
		if( it1 != vec.begin() )
		{
			stream << ",";
		}
		writeIntList( stream, inner_vec );
	}
	stream << ")";
}

void writeIntList3D( std::stringstream& stream, const std::vector<std::vector<std::vector<int> > >& vec )
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	std::vector<std::vector<std::vector<int> > >::const_iterator it1;
	stream << "(";
	for( it1=vec.begin(); it1!=vec.end(); ++it1 )
	{
		const std::vector<std::vector<int> >& inner_vec = (*it1);
		if( it1 != vec.begin() )
		{
			stream << ",";
		}
		writeIntList2D( stream, inner_vec );
	}
	stream << ")";
}

void writeDoubleList( std::stringstream& stream, const std::vector<double>& vec )
{
	stream << "(";
	std::vector<double>::const_iterator it;
	
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		if( it != vec.begin() )
		{
			stream << ",";
		}
		stream << (*it);
	}
	stream << ")";
}

void writeDoubleList2D( std::stringstream& stream, const std::vector<std::vector<double> >& vec )
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	std::vector<std::vector<double> >::const_iterator it1;
	stream << "(";
	for( it1=vec.begin(); it1!=vec.end(); ++it1 )
	{
		const std::vector<double>& inner_vec = (*it1);

		if( it1 != vec.begin() )
		{
			stream << ",";
		}
		writeDoubleList( stream, inner_vec );
	}
	stream << ")";
}

void writeDoubleList3D( std::stringstream& stream, const std::vector<std::vector<std::vector<double> > >& vec )
{
	// ((1.6,2.0,4.9382),(3.78,23.34,039.938367),(938.034,3.0,-3.45,6.9182))
	std::vector<std::vector<std::vector<double> > >::const_iterator it1;
	stream << "(";
	for( it1=vec.begin(); it1!=vec.end(); ++it1 )
	{
		const std::vector<std::vector<double> >& inner_vec = (*it1);

		if( it1 != vec.begin() )
		{
			stream << ",";
		}
		writeDoubleList2D( stream, inner_vec );
	}
	stream << ")";
}

void writeConstCharList( std::stringstream& stream, const std::vector<const char*>& vec )
{
	stream << "(";
	std::vector<const char*>::const_iterator it;
	
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		if( it != vec.begin() )
		{
			stream << ",";
		}
		stream << encodeStepString( (*it) );
	}
	stream << ")";
}

void writeStringList( std::stringstream& stream, const std::vector<std::string>& vec )
{
	stream << "(";
	std::vector<std::string>::const_iterator it;
	
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		if( it != vec.begin() )
		{
			stream << ",";
		}
		stream << encodeStepString( (*it) );
	}
	stream << ")";
}


std::string encodeStepString( std::string str )
{
	char* stream_pos = (char*)str.c_str();
	std::string result_str;

	while( *stream_pos != '\0' )
	{
		// to utf-8
		char append_char = *stream_pos;
		if( append_char > 0 && append_char < 128 )
		{
			result_str.push_back( append_char );
		}
		else
		{
			result_str.push_back( '\\' );
			result_str.push_back( 'S' );
			result_str.push_back( '\\' );
			result_str.push_back( append_char-128 );
		}
		++stream_pos;
	}
	return result_str;
}
