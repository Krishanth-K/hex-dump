#pragma once

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

std::vector<Signature> getSignatures();

// convert a hex string into std::bytes
std::vector<std::byte> hexToBytes(const std::string &hex);

void print_search_results(std::vector<const Signature *> &results);

void hexdump(const std::vector<std::byte> &buf, size_t limit = 64,
             size_t bytesPerGroup = 2, size_t bytesPerLine = 16);
