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
std::string CreateCompressedGuidString22();
char* CreateCompressedGuidString( char * buf, int len );					   // len >= 23
char* String64_To_HexaGuidString( const char *string64, char * buf, int len ); // len >= 39
char* String64_To_String85( const char *string64, char * buf, int len );	   // len >= 21
char* String85_To_String64( const char *string85, char * buf, int len );	   // len >= 23
char* getString16FromGuid( const GUID   *pGuid, char * buf, int len );
char* getString64FromGuid( const GUID *pGuid, char * buf, int len );
bool getGuidFromString64( const char *string, GUID *pGuid );
bool getGuidFromString85( const char	*string, GUID *pGuid );
char* getString85FromGuid( const GUID *pGuid, char * buf, int len );
