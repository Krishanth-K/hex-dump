#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

class TrieNode {
  public:
	std::unordered_map<std::byte, std::shared_ptr<TrieNode>> children;
	std::vector<std::vector<std::byte>> ending_patterns;
	std::shared_ptr<TrieNode> failure;

	TrieNode();
};

class AhoCorasick {
	std::shared_ptr<TrieNode> root;
	std::vector<std::vector<std::byte>> patterns{};

  public:
	AhoCorasick();
	void AddPattern(std::vector<std::byte> pattern);
	void BuildTrie();
	void AssignFailureLinks();

	// PERF: Push pattern code instead of entire patterns
	std::vector<std::pair<size_t, std::vector<std::byte>>>
	Search(const std::vector<std::byte> &data);
};
