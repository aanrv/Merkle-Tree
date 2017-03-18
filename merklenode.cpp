#include "merklenode.h"
#include <vector>

using std::vector;

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

bool MerkleNode::findItem(const MerkleNode::HashArray& hash, std::vector<MerkleNode::HashArray>& stack) const {
	if (this->hash == hash) {
	// this node is hash, return true, nothing to update on stack
		return true;
	} else if (this->left && this->right) {
	// not a leaf
		// search left tree
		if (this->left->findItem(hash, stack)) {
		// found item
			stack.push_back(this->right->getHash());
			return true;
		} else if (this->right->getHash() != this->left->getHash() && this->right->findItem(hash, stack)) {
		// not in left tree, if right tree is not duplicate of left, search in it as well
			stack.push_back(this->left->getHash());
			return true;
		}
	}
	return false;
}

void MerkleNode::getHashesAtLevel(size_t level, vector<MerkleNode::HashArray>& stack) const {
	if (level == 0) {
		stack.push_back(this->hash);
	} else {
		if (this->left) this->left->getHashesAtLevel(level - 1, stack);
		if (this->right) this->right->getHashesAtLevel(level - 1, stack);
	}
}

