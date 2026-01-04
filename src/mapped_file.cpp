#include "mapped_file.hpp"
#include <stdexcept>

MappedFile::MappedFile(const char *filePath)
{
	struct stat st;

	int fd = open(filePath, O_RDONLY);
	if (fd < 0)
		throw std::runtime_error("open failed");

	if (fstat(fd, &st) < 0)
	{
		close(fd);
		throw std::runtime_error("fstat failed");
	}

	if (st.st_size == 0)
	{
		close(fd);
		throw std::runtime_error("empty file");
	}

	void *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	close(fd);

	if (data == MAP_FAILED)
		throw std::runtime_error("Map failed");

	this->start_byte = (std::byte *)data;
	this->len = st.st_size;
}

MappedFile::~MappedFile()
{
	if (this->start_byte)
		munmap(this->start_byte, this->len);
}
