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
#include <ifcpp/model/shared_ptr.h>

class IfcPPObject;

std::string encodeStepString( std::wstring arg_str );
void writeIntList( std::stringstream& stream, const std::vector<int>& vec );
void writeIntList2D( std::stringstream& stream, const std::vector<std::vector<int> >& vec );
void writeIntList3D( std::stringstream& stream, const std::vector<std::vector<std::vector<int> > >& vec );
void writeDoubleList( std::stringstream& stream, const std::vector<double>& vec );
void writeDoubleList2D( std::stringstream& stream, const std::vector<std::vector<double> >& vec );
void writeDoubleList3D( std::stringstream& stream, const  std::vector<std::vector<std::vector<double> > >& vec );
void writeConstCharList( std::stringstream& stream, const std::vector<const char*>& vec );
void writeStringList( std::stringstream& stream, const std::vector<std::wstring>& vec );

template<typename T>
void writeTypeOfIntList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec )
{
	//(38,12,4)
	stream << "(";
	typename std::vector<shared_ptr<T> >::const_iterator it;
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		if( it != vec.begin() )
		{
			stream << ",";
		}
		stream << (*it)->m_value;
	}
	stream << ")";
}

template<typename T>
void writeTypeOfRealList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec )
{
	//(.38,12.0,.04)
	stream << "(";
	typename std::vector<shared_ptr<T> >::const_iterator it;
	
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		if( it != vec.begin() )
		{
			stream << ",";
		}
		stream << (*it)->m_value;
	}
	stream << ")";
}

template<typename T>
void writeTypeOfRealList2D( std::stringstream& stream, const std::vector<std::vector<shared_ptr<T> > >& vec )
{
	//((.38,12.0,.04),(.38,1.0,346.0),(1.8,1.0,.04))
	typename std::vector<std::vector<shared_ptr<T> > >::const_iterator it;
	stream << "(";
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		const std::vector<shared_ptr<T> >& inner_vec = (*it);

		if( it != vec.begin() )
		{
			stream << ",";
		}
		writeTypeOfRealList( stream, inner_vec );
	}
	stream << ")";
}

template<typename T>
void writeEntityList( std::stringstream& stream, const std::vector<shared_ptr<T> >& vec )
{
	//(#287,#291,$,#299)
	stream << "(";
	typename std::vector<shared_ptr<T> >::const_iterator it;
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		if( it != vec.begin() )
		{
			stream << ",";
		}

		if( (*it) )
		{
			stream << "#" << (*it)->m_id;
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
	stream << "(";
	typename std::vector<std::vector<shared_ptr<T> > >::const_iterator it;
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		const std::vector<shared_ptr<T> >& inner_vec = (*it);

		if( it != vec.begin() )
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
	stream << "(";
	typename std::vector<std::vector<std::vector<shared_ptr<T> > > >::const_iterator it;
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		const std::vector<std::vector<shared_ptr<T> > >& inner_vec = (*it);

		if( it != vec.begin() )
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
	typename std::vector<shared_ptr<T> >::const_iterator it;
	for( it=vec.begin(); it!=vec.end(); ++it )
	{
		if( it != vec.begin() )
		{
			stream << ",";
		}
		if( (*it) )
		{
			(*it)->getStepParameter( stream, is_select_type );
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

	typename std::vector<std::vector<shared_ptr<T> > >::const_iterator it_outer;
	typename std::vector<shared_ptr<T> >::const_iterator it_inner;

	for( it_outer=vec.begin(); it_outer!=vec.end(); ++it_outer )
	{
		const std::vector<shared_ptr<T> >&inner_vec = (*it_outer);
		
		if( it_outer != vec.begin() )
		{
			stream << "),(";
		}

		for( it_inner=inner_vec.begin(); it_inner!=inner_vec.end(); ++it_inner )
		{
			if( it_inner != inner_vec.begin() )
			{
				stream << ",";
			}
			if( (*it_inner) )
			{
				(*it_inner)->getStepParameter( stream, is_select_type );
			}
			else
			{
				stream << "$";
			}
		}
	}
	stream << ")";
}
