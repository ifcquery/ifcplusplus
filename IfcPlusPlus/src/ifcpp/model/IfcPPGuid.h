#pragma once

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
