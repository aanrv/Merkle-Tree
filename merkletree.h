#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <vector>
#include <string>
#include "merklenode.h"

class MerkleTree {
public:
	typedef std::string ItemType;

	/* Precondition: items.size() = 2^b, where b is a positive integer */
	MerkleTree(std::vector<ItemType> items);
	~MerkleTree();

	/* Retrieve tree's merkle root */
	MerkleNode::HashArray getMerkleRoot() const;

	/* Retrieve merkle path of element. Returns empty vector if single element or element does not exist. */
	std::vector<MerkleNode::HashArray> getMerklePath(ItemType item) const;

	bool itemExists(ItemType item) const;
private:
	/* Given a list of items and a position within a tree, calculates the hash and creates a node. */
	MerkleNode* createTree(size_t treeHeight, std::vector<MerkleTree::ItemType>& items);

	/* Given two hashes, concats them and calculates their hash */
	MerkleNode::HashArray concatHash(MerkleNode::HashArray first, MerkleNode::HashArray second) const;

	MerkleNode* head;
};

#endif	// MERKLETREE_H

