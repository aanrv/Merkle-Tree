#include "merklenode.h"

MerkleNode::MerkleNode(MerkleNode::HashArray hash) {
	this->hash = hash;
	this->left = NULL;
	this->right = NULL;
}

MerkleNode::~MerkleNode() {
	if (this->left) delete this->left;
	if (this->right) delete this->right;
}

MerkleNode::HashArray MerkleNode::getHash() const {
	return this->hash;
}

