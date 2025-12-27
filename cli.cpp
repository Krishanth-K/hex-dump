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
// HACK:  Why does reinterpret_cast cast work here, and not something else???
//
// TODO: print absolute offsets instead of line numbers
// TEST: test the partial line fix
//

void hexdump(const std::vector<std::byte> &buf, size_t limit = 64,
             size_t bytesPerGroup = 2, size_t bytesPerLine = 16)
{
	int bytesWritten = 0;
	std::string asciiString;

	// creating a temp out stream, to prevent global stream poisoning
	std::ostringstream outStream;

	for (size_t i = 0; i < buf.size() && i < limit; i++)
	{
		// the "line" address
		if (i % bytesPerLine == 0)
			outStream << std::setw(8) << std::setfill('0') << std::hex << i
			          << ": ";

		// casting std::byte to int directly is wrong -> defeats the purpose of
		// std::byte. requires explicity type conversion
		int byte = std::to_integer<int>(buf[i]);
		outStream << std::setw(2) << std::setfill('0') << std::hex << byte;

		// write the ascii value if its a printable char
		if (byte >= 32 && byte <= 126)
			asciiString.push_back(static_cast<char>(byte));
		else
			asciiString.push_back('.');

		// group into pairs
		if (i % bytesPerGroup == 1)
			outStream << " ";

		// add the ascii string at the end of a line
		if (i % bytesPerLine == bytesPerLine - 1)
		{
			outStream << "  |  " << asciiString;
			outStream << "\n";

			asciiString = "";
		}
	}

	// if the last line was partial
	size_t remaining = asciiString.size();
	if (remaining > 0 && remaining < bytesPerLine)
	{
		size_t missing = bytesPerLine - remaining;

		// each byte prints as "XX", plus a space after each group
		for (size_t j = 0; j < missing; j++)
		{
			outStream << "  "; // missing hex byte

			// add group space if needed
			if ((remaining + j) % bytesPerGroup == bytesPerGroup - 1)
				outStream << " ";
		}

		outStream << "  |  " << asciiString << "\n";
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
	size_t size = file.tellg();
	file.seekg(0, std::fstream::beg);

	// read it into a buffer
	std::vector<std::byte> buffer(size);
	file.read(reinterpret_cast<char *>(buffer.data()), size);

	// hexdump the buffer
	hexdump(buffer, buffer.size());

	return 0;
}
