#pragma once

#include <string>
#include "GlobalDefines.h"

///@brief Creates a GUID string with 36 characters including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
///@details These functions are non-template on purpose, to keep it simple and stupid, and to reduce compile time
IFCQUERY_EXPORT std::string createGUID32();

///@brief Compresses a GUID string
///@details Expects a string with exactly 36 characters including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
///@returns an IFC GUID string with 22 characters, for example: "3n0m0Cc6L4xhvkpCU0k1GZ"
IFCQUERY_EXPORT std::string compressGUID(const std::string& in);

///@brief Decompresses an IFC GUID string
///@details Expects a string with exactly 22 characters, for example "3n0m0Cc6L4xhvkpCU0k1GZ"
///@returns GUID string with 36 characters, including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
IFCQUERY_EXPORT std::string decompressGUID(const std::string& in);


///@brief Create IFC GUID string with 22 characters, for example "3n0m0Cc6L4xhvkpCU0k1GZ"
///@details Use desired character type as template parameter - char or wchar_t.
///IFC uses a different base64 character set than RFC4648 - it starts with digits
///instead of uppercase letters and uses '_' and '$' as last two characters.
IFCQUERY_EXPORT std::string createBase64Uuid();