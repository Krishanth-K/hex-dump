#pragma once

#include <string>
#include <vector>

struct Signature
{
	std::vector<std::byte> bytes;
	const std::string filetype;
	size_t offset = 0;
	int weight = 1;
};

std::vector<Signature> getSignatures();
