#include "merkletree.h"
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <algorithm>
#include <math.h>

using CryptoPP::ArraySource;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::ArraySink;
using std::string;
using std::vector;
using std::array;
using std::copy;

MerkleTree::MerkleTree(vector<MerkleTree::ItemType> items) {
	// make items even if necessary
	if (items.size() % 2 != 0) items.push_back(items.back());
	// log2 rounded up is height of tree
	int treeHeight = std::ceil(log2(items.size()));
	this->head = calculateNode(treeHeight, items);
}

MerkleTree::~MerkleTree() {
	delete this->head;
}

MerkleNode::HashArray MerkleTree::getMerkleRoot() const {
	return this->head->getHash();
}

MerkleNode* MerkleTree::calculateNode(int distanceFromLeaves, std::vector<MerkleTree::ItemType>& items) {
	if (distanceFromLeaves == 0 && !items.empty()) {
		// if items remain in list, create a leaf node here

		// retrieve item from list
		MerkleTree::ItemType item = items.back();
		items.pop_back();

		// calculate item's hash
		array<byte, MerkleNode::HASHSIZE> digest;
		MerkleNode::HashFunction hashFunc;
		StringSource s(item, true, new HashFilter(hashFunc, new ArraySink(digest.data(), MerkleNode::HASHSIZE)));

		// create merkle node with calculated hash
		return new MerkleNode(digest);
	} else if (distanceFromLeaves > 0) {
		// not a leaf
		// calculate children hashes
		// (start with right child because std::vector uses pop_back()
		MerkleNode* rChild = calculateNode(distanceFromLeaves - 1, items);
		MerkleNode* lChild = calculateNode(distanceFromLeaves - 1, items);

		MerkleNode::HashArray hash = concatHash(lChild->getHash(), rChild->getHash());

		MerkleNode* thisNode = new MerkleNode(hash);
		thisNode->left = lChild;
		thisNode->right = rChild;

		return thisNode;
	} else {
		// nothing to do, empty node
		return NULL;
	}
}

MerkleNode::HashArray MerkleTree::concatHash(MerkleNode::HashArray first, MerkleNode::HashArray second) const {
	// concat hashes
	const size_t sourceLen = MerkleNode::HASHSIZE * 2;
	array<byte, sourceLen> source;
	copy(first.begin(), first.end(), source.begin());
	copy(second.begin(), second.end(), source.begin() + MerkleNode::HASHSIZE);

	// calc hash
	MerkleNode::HashArray digest;
	MerkleNode::HashFunction hashFunc;
	ArraySource(source.data(), sourceLen, true, new HashFilter(hashFunc, new ArraySink(digest.data(), MerkleNode::HASHSIZE)));

	return digest;
}

