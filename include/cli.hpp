#include "aho_corasick.hpp"
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using std::cout;
using std::endl;

struct Signature
{
	std::vector<std::byte> bytes;
	const std::string filetype;
	size_t offset = 0;
};

//

// convert a hex string into std::bytes
std::vector<std::byte> hexToBytes(const std::string &hex)
{
	std::vector<std::byte> bytes;

	auto is_hex = [](char c) {
		return std::isxdigit(static_cast<unsigned char>(c));
	};

	for (size_t i = 0; i < hex.size();)
	{
		if (!is_hex(hex[i]))
		{
			i++;
			continue;
		}

		if (i + 1 >= hex.size() || !is_hex(hex[i + 1]))
			throw std::runtime_error("invalid hex");

		uint8_t b = std::stoi(hex.substr(i, 2), nullptr, 16);
		bytes.push_back(static_cast<std::byte>(b));
		i += 2;
	}

	return bytes;
}

//

std::vector<Signature> file_signatures = {
    // ELF
    {hexToBytes("7F 45 4C 46"), "elf", 0},

    // GIF
    {hexToBytes("47 49 46 38 37 61"), "gif", 0},
    {hexToBytes("47 49 46 38 39 61"), "gif", 0},

    // TIFF
    {hexToBytes("49 49 2A 00"), "tiff", 0},
    {hexToBytes("4D 4D 00 2A"), "tiff", 0},
    {hexToBytes("49 49 2B 00"), "tiff", 0},
    {hexToBytes("4D 4D 00 2B"), "tiff", 0},

    // JPEG
    {hexToBytes("FF D8 FF DB"), "jpeg", 0},
    {hexToBytes("FF D8 FF E0"), "jpeg", 0},
    {hexToBytes("FF D8 FF EE"), "jpeg", 0},
    {hexToBytes("FF D8 FF E0 00 10 4A 46 49 46 00 01"), "jpeg", 0},

    // PNG
    {hexToBytes("89 50 4E 47 0D 0A 1A 0A"), "png", 0},

    // Windows PE (MZ)
    {hexToBytes("4D 5A"), "pe", 0},

    // ZIP family (container formats)
    {hexToBytes("50 4B 03 04"), "zip", 0},
    {hexToBytes("50 4B 05 06"), "zip", 0},

    // RAR
    {hexToBytes("52 61 72 21 1A 07 00"), "rar", 0},
    {hexToBytes("52 61 72 21 1A 07 01 00"), "rar", 0},

    // PDF
    {hexToBytes("25 50 44 46 2D"), "pdf", 0},

    // OGG
    {hexToBytes("4F 67 67 53"), "ogg", 0},

    // MP3
    {hexToBytes("49 44 33"), "mp3", 0},
    {hexToBytes("FF FB"), "mp3", 0},
    {hexToBytes("FF F3"), "mp3", 0},
    {hexToBytes("FF F2"), "mp3", 0},

    // BMP
    {hexToBytes("42 4D"), "bmp", 0},

    // FLAC
    {hexToBytes("66 4C 61 43"), "flac", 0},

    // TAR
    {hexToBytes("75 73 74 61 72 00 30 30"), "tar", 257},

    // 7z
    {hexToBytes("37 7A BC AF 27 1C"), "7z", 0},

    // GZIP
    {hexToBytes("1F 8B"), "gzip", 0},

    // Matroska / WebM
    {hexToBytes("1A 45 DF A3"), "mkv", 0},

    // RTF
    {hexToBytes("7B 5C 72 74 66 31"), "rtf", 0},

    // MP4
    {hexToBytes("66 74 79 70 69 73 6F 6D"), "mp4", 4},
    {hexToBytes("66 74 79 70 4D 53 4E 56"), "mp4", 4},
};

std::vector<Signature> getSignatures() { return file_signatures; }
