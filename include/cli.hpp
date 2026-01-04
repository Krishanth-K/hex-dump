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
