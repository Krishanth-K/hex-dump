#include "aho_corasick.hpp"
#include "cli.hpp"

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

	// const std::string filePath = "./build/nets_engine.exe";
	// const std::string filePath = "./build/test.zip";
	// const std::string filePath = "./build/L9-Quick Review of Process.pptx";
	// const std::string filePath = "./build/ramdump.raw";
	const std::string filePath = "./build/archlinux-2026.01.01-x86_64.iso";
	// const std::string filePath = "./build/LAB-3.docx";
	// const std::string filePath = "./build/ATDE-32-ATDE221253.pdf";
	// const std::string filePath = "./build/Emotet Malware _ CISA.html";

	// open the file in binary mode
	std::ifstream file(filePath, std::ios::binary);
	if (!file)
		throw std::runtime_error("Failed to open file");

	// find size of file
	file.seekg(0, std::fstream::end);
	size_t size = file.tellg();
	file.seekg(0, std::fstream::beg);

	// read it into a buffer
	std::vector<std::byte> buffer(size);
	file.read(reinterpret_cast<char *>(buffer.data()), size);

	// hexdump the buffer
	hexdump(buffer, 100);

	AhoCorasick tree = AhoCorasick();

	// load all signatures into the tree
	auto signatures = getSignatures();
	for (const auto &signature : signatures)
		tree.AddSignature(signature);

	tree.BuildTrie();
	tree.AssignFailureLinks();

	// Search for patterns
	std::vector<const Signature *> res =
	    tree.Search(std::vector(buffer.begin(), buffer.end()));
	print_search_results(res);

	return 0;
}
