#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <random>
#include <sstream>
#include "BuildingGuid.h"

///@brief Creates a GUID string with 36 characters including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
std::string createGUID32()
{
	std::random_device              rd;
	std::mt19937                    gen(rd());
	std::uniform_int_distribution<> dis(0, 15);
	std::uniform_int_distribution<> dis2(8, 11);
	std::basic_stringstream<char> uuid_strs;
	uuid_strs << std::uppercase;

	size_t i;
	uuid_strs << std::hex;
	for (i = 0; i < 8; i++) { uuid_strs << dis(gen); }
	uuid_strs << "-";
	for (i = 0; i < 4; i++) { uuid_strs << dis(gen); }
	uuid_strs << "-4";
	for (i = 0; i < 3; i++) { uuid_strs << dis(gen); }
	uuid_strs << "-";
	uuid_strs << dis2(gen);
	for (i = 0; i < 3; i++) { uuid_strs << dis(gen); }
	uuid_strs << "-";
	for (i = 0; i < 12; i++) { uuid_strs << dis(gen); }
	std::string str = uuid_strs.str();
	return str;
}

static const char base16mask[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				- 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				- 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, \
				- 1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				- 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				- 1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				- 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
static const char base64mask[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				- 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				- 1, -1, -1, -1, 63, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
				0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, \
				- 1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, \
				25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, 62, \
				- 1, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, \
				51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1 };

static constexpr std::array<char, 64> base64Chars = {
	'0','1','2','3','4','5','6','7','8','9',// 0-9 (Indices 0-9)
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',// A-Z (Indices 10-35)
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',// a-z (Indices 36-61)
	'_','$'// _ and $ (Indices 62-63)
};


///@brief Compresses a GUID string
///@details Expects a string with exactly 36 characters including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
///@returns an IFC GUID string with 22 characters, for example: "3n0m0Cc6L4xhvkpCU0k1GZ"
std::string compressGUID(const std::string& in) {
	// Remove dashes and ensure the resulting string is 32 characters of hex
	std::string hex_data;
	for (char c : in) {
		if (c != '-') {
			hex_data.push_back(c);
		}
	}
	if (hex_data.length() != 32) return "";

	std::string result;
	result.reserve(22);

	// --- Convert 32 hex chars into 16 bytes ---
	std::vector<uint8_t> binary_data;
	binary_data.reserve(16);
	for (size_t i = 0; i < 32; i += 2) {
		// Read 2 hex characters (8 bits = 1 byte)
		int h1 = base16mask[static_cast<unsigned char>(hex_data[i])];
		int h2 = base16mask[static_cast<unsigned char>(hex_data[i + 1])];
		if (h1 == -1 || h2 == -1) return ""; // Invalid hex char

		binary_data.push_back(static_cast<uint8_t>((h1 << 4) | h2));
	}

	// --- Pack 16 Bytes (128 bits) into 22 Base64 characters (132 bits) ---
	uint32_t buffer = 0;
	int bits_buffered = 0;

	for (size_t i = 0; i < 16; ++i) {
		buffer = (buffer << 8) | binary_data[i];
		bits_buffered += 8;

		// While we have enough bits for a 6-bit Base64 character
		while (bits_buffered >= 6) {
			// Extract the most significant 6 bits
			int val = (buffer >> (bits_buffered - 6)) & 0x3F; // 0x3F is 63
			result.push_back(base64Chars[val]);
			bits_buffered -= 6;
		}
	}

	// If there are any remaining bits (should be 4 bits for a 16-byte input):
	if (bits_buffered > 0) {
		// Pad with zeros to get 6 bits, then encode the last character
		int val = (buffer << (6 - bits_buffered)) & 0x3F;
		result.push_back(base64Chars[val]);
	}

	// The result must be 22 characters long
	if (result.length() != 22) return "";

	return result;
}
///@brief Decompresses an IFC GUID string
///@details Expects a string with exactly 22 characters, for example "3n0m0Cc6L4xhvkpCU0k1GZ"
///@returns GUID string with 36 characters, including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
std::string decompressGUID(const std::string& in)
{
	// --- 1. CRITICAL: Input Validation ---
	// A Base64 GUID is 22 characters for 16 binary bytes (128 bits).
	if (in.length() != 22) {
		// Return an empty string or throw an exception for invalid input length.
		return "";
	}
	if (in.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_$") != std::string::npos) {
		// Log error: Invalid Base64 character found.
		return "";
	}

	// --- Setup ---
	// GUIDs are 16 bytes. We will decode into this buffer first.
	std::vector<uint8_t> binary_data;
	binary_data.reserve(16);

	uint32_t buffer = 0;
	int bits_buffered = 0;

	// --- 2. Decode Base64 into Binary Bytes (16 bytes total) ---
	for (size_t ii = 0; ii < in.length(); ++ii) {

		// Safety Check for base64mask index and value (assuming base64mask is accessible)
		// Static cast ensures correct indexing for char/uint8_t
		int val = base64mask[static_cast<unsigned char>(in[ii])];
		if (val < 0 || val > 63) {
			return ""; // Invalid character found or mask lookup failed
		}

		// Shift the buffer left by 6 bits and OR in the new 6-bit value
		buffer = (buffer << 6) | val;
		bits_buffered += 6;

		// When we have 8 or more bits, we extract a full byte (8 bits)
		while (bits_buffered >= 8) {
			// Extract the most significant 8 bits and push them to the binary data
			// (buffer >> (bits_buffered - 8)) extracts the byte
			binary_data.push_back(static_cast<uint8_t>(buffer >> (bits_buffered - 8)));
			bits_buffered -= 8;
		}
	}

	// After the loop, the binary_data vector MUST contain 16 bytes
	if (binary_data.size() != 16) {
		// This indicates a critical error in the Base64 mask/encoding or the loop logic
		return "";
	}

	// --- 3. Format Binary Data as Dashed Hexadecimal (32 chars + 4 dashes = 36 chars) ---
	std::stringstream ss;
	ss << std::uppercase << std::hex << std::setfill('0');

	// Append 16 bytes, adding dashes at the correct GUID positions: 
	// 4-2-2-2-6 byte segments (or 8-4-4-4-12 hex segments)

	// Segment 1 (Bytes 0-3): 8 hex chars
	for (int i = 0; i < 4; ++i) ss << std::setw(2) << (int)binary_data[i];
	ss << '-';

	// Segment 2 (Bytes 4-5): 4 hex chars
	for (int i = 4; i < 6; ++i) ss << std::setw(2) << (int)binary_data[i];
	ss << '-';

	// Segment 3 (Bytes 6-7): 4 hex chars
	for (int i = 6; i < 8; ++i) ss << std::setw(2) << (int)binary_data[i];
	ss << '-';

	// Segment 4 (Bytes 8-9): 4 hex chars
	for (int i = 8; i < 10; ++i) ss << std::setw(2) << (int)binary_data[i];
	ss << '-';

	// Segment 5 (Bytes 10-15): 12 hex chars
	for (int i = 10; i < 16; ++i) ss << std::setw(2) << (int)binary_data[i];

	return ss.str();
}

///@brief Create IFC GUID string with 22 characters, for example "3n0m0Cc6L4xhvkpCU0k1GZ"
///@details Use desired character type as template parameter - char or wchar_t.
///IFC uses a different base64 character set than RFC4648 - it starts with digits
///instead of uppercase letters and uses '_' and '$' as last two characters.
std::string createBase64Uuid()
{
	std::string guid_uncompressed = createGUID32();
	std::string guid_compressed = compressGUID(guid_uncompressed);
#ifdef _DEBUG
	std::string guid_decompressed = decompressGUID(guid_compressed);
	if (guid_uncompressed.compare(guid_decompressed) != 0) {
		std::cerr << "GUID compression incorrect! uncompressed: " << guid_uncompressed << ", compressed: " << guid_compressed << std::endl;
	}
#endif
	return guid_compressed;
}
