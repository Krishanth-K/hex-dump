#pragma once

#include "mapped_file.hpp"
#include "signature.hpp"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

class TrieNode {
  public:
	std::unordered_map<std::byte, std::shared_ptr<TrieNode>> children;
	std::shared_ptr<TrieNode> failure;

	// contains the pattern_id of patterns. (id -> index of pattern)
	std::vector<size_t> ending_patterns;

	TrieNode();
};

class AhoCorasick {
	std::shared_ptr<TrieNode> root;
	std::vector<const Signature *> patterns;

  public:
	AhoCorasick();

	const Signature *getSignature(size_t id);

	void AddSignature(const Signature &signature);
	void BuildTrie();
	void AssignFailureLinks();

	std::vector<const Signature *> Search(const std::vector<std::byte> &data);
	std::vector<const Signature *> Search(MappedFile &mapFile);
};
