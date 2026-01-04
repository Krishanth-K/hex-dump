#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

using Pattern = std::vector<std::byte>;

struct Match
{
	size_t start_offset;
	size_t pattern_id;
};

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
	std::vector<Pattern> patterns{};

  public:
	AhoCorasick();

	Pattern &getPattern(size_t id);

	void AddPattern(Pattern pattern);
	void BuildTrie();
	void AssignFailureLinks();

	std::vector<Match> Search(const std::vector<std::byte> &data);
};
