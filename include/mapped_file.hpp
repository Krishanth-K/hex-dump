#pragma once

#include <cstddef>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

class MappedFile {
  public:
	std::byte *start_byte;
	size_t len;

	MappedFile(const char *filePath);
	~MappedFile();
};
