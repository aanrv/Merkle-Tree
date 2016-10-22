#include "merklenode.h"

MerkleNode::MerkleNode(MerkleNode::HashArray hash) {
	this->hash = hash;
	left = NULL;
	right = NULL;
}

MerkleNode::~MerkleNode() {
	if (left) delete left;
	if (right) delete right;
}

MerkleNode::HashArray MerkleNode::getHash() const {
	return hash;
}

