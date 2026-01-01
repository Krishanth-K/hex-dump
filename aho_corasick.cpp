#include "aho_corasick.hpp"
#include <cstddef>
#include <memory>
#include <queue>
#include <vector>

TrieNode::TrieNode() : failure(nullptr) {};

AhoCorasick::AhoCorasick() : root(std::make_shared<TrieNode>()) {};

void AhoCorasick::AddPattern(std::vector<std::byte> pattern)
{
	this->patterns.push_back(pattern);
}

// build the trie
void AhoCorasick::BuildTrie()
{
	this->root = std::make_shared<TrieNode>();

	for (const auto &pattern : this->patterns)
	{
		std::shared_ptr<TrieNode> node = this->root;

		for (const auto &byte : pattern)
		{
			if (node->children.find(byte) == node->children.end())
				node->children[byte] = std::make_shared<TrieNode>();

			node = node->children[byte];
		}
		node->ending_patterns.push_back(pattern);
	}
}

void AhoCorasick::AssignFailureLinks()
{
	std::queue<std::shared_ptr<TrieNode>> q;

	// assign the root as failure for immediate children of root
	for (const auto &[byte, child] : this->root->children)
	{
		child->failure = this->root;
		q.push(child);
	}

	while (q.size() > 0)
	{
		std::shared_ptr<TrieNode> current = q.front();
		q.pop();

		// Set the failure its for each children of a node
		for (const auto [byte, child] : current->children)
		{
			auto failure = current->failure;

			// follow failure links until you reach root or has byte as a child
			while (failure != nullptr && failure != this->root &&
			       failure->children.find(byte) == failure->children.end())
				failure = failure->failure;

			// make sure the failure node is not its own parent
			if (failure != nullptr &&
			    failure->children.find(byte) != failure->children.end())
				child->failure = failure->children[byte];
			else
				child->failure = this->root;

			// extend the output of a smaller patten in a large pattern
			// for ex: "she" also has the pattern "he". so the output of "she"
			// should also contain "he"
			if (child->failure != nullptr)
				child->ending_patterns.insert(
				    child->ending_patterns.end(),
				    child->failure->ending_patterns.begin(),
				    child->failure->ending_patterns.end());

			q.push(child);
		}
	}
}

std::vector<std::pair<size_t, std::vector<std::byte>>>
AhoCorasick::Search(const std::vector<std::byte> &data)
{
	std::vector<std::pair<size_t, std::vector<std::byte>>> results;
	auto current = this->root;

	for (size_t i = 0; i < data.size(); ++i)
	{
		std::byte b = data[i];

		while (current != this->root &&
		       current->children.find(b) == current->children.end())
			current = current->failure;

		if (current->children.find(b) != current->children.end())
			current = current->children[b];

		for (const auto &pattern : current->ending_patterns)
			results.emplace_back(i, pattern);
	}

	return results;
}
