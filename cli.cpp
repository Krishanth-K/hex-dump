#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;

// TODO: Make the error handling more robust
//
//
// TODO: print absolute offsets instead of line numbers

void hexdump(const std::vector<std::byte> &buf, size_t limit = 64)
{
	int bytesWritten = 0;

	// creating a temp out stream, to prevent global stream poisoning
	std::ostringstream outStream;

	for (size_t i = 0; i < buf.size() && i < limit; i++)
	{
		// the "line" address
		if (i % 16 == 0)
			outStream << std::setfill('0') << std::setw(8) << std::hex << i
			          << ": ";

		// casting std::byte to int directly is wrong -> defeats the purpose of
		// std::byte. requires explicity type conversion
		outStream << std::setw(2) << std::hex << std::to_integer<int>(buf[i]);
		bytesWritten++;

		// group into pairs of two
		if (bytesWritten % 2 == 0)
			outStream << " ";

		if (i % 16 == 15)
		{
			outStream << "\n";
			bytesWritten = 0;
		}
	}

	cout << outStream.str();
}

int main(int argc, char *argv[])
{
	// if (argc == 1)
	// {
	// 	cout << "[ERROR]: Enter a filepath" << endl;
	// 	return -1;
	// }
	// else if (argc > 2)
	// {
	// 	cout << "[ERROR]: Too many arguments" << endl;
	// 	return -2;
	// }
	//
	// const std::string filePath = argv[1];

	const std::string filePath = "./nets_engine.exe";

	// open the file in binary mode
	std::ifstream file(filePath, std::ios::binary);

	// find size of file
	file.seekg(0, std::fstream::end);
	int size = file.tellg();
	file.seekg(0, std::fstream::beg);

	// read it into a buffer
	std::vector<std::byte> buffer(size);
	file.read(reinterpret_cast<char *>(buffer.data()), size);

	// hexdump the buffer
	hexdump(buffer);

	return 0;
}
