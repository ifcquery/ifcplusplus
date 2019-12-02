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
#include "GlobalDefines.h"

///@brief Creates a GUID string with 36 characters including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
///@details These functions are non-template on purpose, to keep it simple and stupid, and to reduce compile time
IFCQUERY_EXPORT std::wstring createGUID32_wstr();
IFCQUERY_EXPORT std::string createGUID32();

///@brief Compresses a GUID string
///@details Expects a string with exactly 36 characters including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
///@returns an IFC GUID string with 22 characters, for example: "3n0m0Cc6L4xhvkpCU0k1GZ"
IFCQUERY_EXPORT std::wstring compressGUID_wstr(const std::wstring& in);
IFCQUERY_EXPORT std::string compressGUID(const std::string& in);

///@brief Decompresses an IFC GUID string
///@details Expects a string with exactly 22 characters, for example "3n0m0Cc6L4xhvkpCU0k1GZ"
///@returns GUID string with 36 characters, including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
IFCQUERY_EXPORT std::wstring decompressGUID_wstr(const std::wstring& in);
IFCQUERY_EXPORT std::string decompressGUID(const std::string& in);


///@brief Create IFC GUID string with 22 characters, for example "3n0m0Cc6L4xhvkpCU0k1GZ"
///@details Use desired character type as template parameter - char or wchar_t.
///IFC uses a different base64 character set than RFC4648 - it starts with digits
///instead of uppercase letters and uses '_' and '$' as last two characters.
IFCQUERY_EXPORT std::wstring createBase64Uuid_wstr();
IFCQUERY_EXPORT std::string createBase64Uuid();