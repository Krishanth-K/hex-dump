#pragma once

#include "mapped_file.hpp"
#include "signature.hpp"
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

using std::cout;
using std::endl;

// convert a hex string into std::bytes
std::vector<std::byte> hexToBytes(const std::string &hex);

void print_search_results(std::vector<const Signature *> &results);

void hexdump(const std::vector<std::byte> &buf, size_t limit = 64,
             size_t bytesPerGroup = 2, size_t bytesPerLine = 16);

void hexdump(MappedFile &mapFile, size_t limit = 64, size_t bytesPerGroup = 2,
             size_t bytesPerLine = 16);
