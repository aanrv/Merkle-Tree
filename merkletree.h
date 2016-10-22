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

	/* Retrieve merkle path of element. Returns empty vector if element does not exist. */
	std::vector<MerkleNode::HashArray> getMerklePath(ItemType item) const;
private:
	/* Given a list of items and a position within a tree, calculates the hash and creates a node. */
	MerkleNode* calculateNode(int distanceFromLeaves, std::vector<MerkleTree::ItemType>& items);

	/* Given two hashes, concats them and calculates their hash */
	MerkleNode::HashArray concatHash(MerkleNode::HashArray a, MerkleNode::HashArray b) const;

	MerkleNode* head;
};

#endif	// MERKLETREE_H

