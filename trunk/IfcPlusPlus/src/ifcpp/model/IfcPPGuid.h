#pragma once

#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {		  // size is 16
	unsigned long Data1;
	unsigned short  Data2;
	unsigned short  Data3;
	unsigned char Data4[8];
} GUID;
#endif

const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
void getGuid( GUID   *pGuid );
std::string createGUID32();
std::wstring CreateCompressedGuidString22();
char* CreateCompressedGuidString( char * buf, int len );					   // len >= 23
char* getString64FromGuid( const GUID *pGuid, char * buf, int len );
bool getGuidFromString64( const char *string, GUID *pGuid );
