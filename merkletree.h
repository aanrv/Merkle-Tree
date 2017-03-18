#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <vector>
#include <string>
#include "merklenode.h"

class MerkleTree {
public:
	typedef std::string ItemType;

	MerkleTree(std::vector<ItemType> items);
	~MerkleTree();

	/* Retrieve tree's merkle root */
	MerkleNode::HashArray getMerkleRoot() const;

	/* Checks if item's hash exists within tree */
	bool itemExists(ItemType item) const;

	/* Retrieve merkle path of element. Returns empty vector if single element or element does not exist. */
	std::vector<MerkleNode::HashArray> getMerklePath(ItemType item) const;

	/* Returns hashes from left to right at given level */
	std::vector<MerkleNode::HashArray> getHashesAtLevel(size_t level) const;
private:
	MerkleNode* head;
};

#endif	// MERKLETREE_H

