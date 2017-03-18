#include "merkletree.h"
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <algorithm>
#include <math.h>
#include <assert.h>

using CryptoPP::ArraySource;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::ArraySink;
using std::string;
using std::vector;
using std::array;
using std::copy;

/* Calculates item's hash */
static MerkleNode::HashArray itemHash(MerkleTree::ItemType item) {
	MerkleNode::HashArray digest;
	MerkleNode::HashFunction func;
	StringSource s(item, true, new HashFilter(func, new ArraySink(digest.data(), MerkleNode::HASHSIZE)));
	return digest;
}

/* Concats two given hashes and returns their concat hash */
static MerkleNode::HashArray concatHash(const MerkleNode::HashArray& first, const MerkleNode::HashArray& second) {
	// concat hashes
	const size_t sourceLen = MerkleNode::HASHSIZE * 2;
	array<byte, sourceLen> source;
	copy(first.begin(), first.end(), source.begin());
	copy(second.begin(), second.end(), source.begin() + MerkleNode::HASHSIZE);

	// calc hash of concated hashes
	MerkleNode::HashArray digest;
	MerkleNode::HashFunction hashFunc;
	ArraySource(source.data(), sourceLen, true, new HashFilter(hashFunc, new ArraySink(digest.data(), MerkleNode::HASHSIZE)));

	return digest;
}

/* Given a list of items and a tree height, creates a tree of given height using hashes of items as leaves */
static MerkleNode* createTree(size_t treeHeight, std::vector<MerkleTree::ItemType>& items) {
	if (treeHeight == 1 && !items.empty()) {
	// if items remain in list, create a leaf node here

		// retrieve item from list
		// TODO redo with deque, vector erase front is ineffecient
		MerkleTree::ItemType item = items.front();
		items.erase(items.begin());

		// calculate item's hash
		MerkleNode::HashArray digest = itemHash(item);

		// create merkle node with calculated hash
		return new MerkleNode(digest);
	} else if (treeHeight > 1) {
	// not a leaf

		// calculate children hashes
		MerkleNode* lChild = createTree(treeHeight - 1, items);

		// if no more elements remaining to make right tree
		// right child is leaf with same hash as right child
		// (this should happen each time the number of nodes at current depth are odd ?)
		MerkleNode* rChild;
		if (items.empty()) rChild = new MerkleNode(lChild->getHash());
		else rChild = createTree(treeHeight - 1, items);

		MerkleNode::HashArray hash = concatHash(lChild->getHash(), rChild->getHash());

		MerkleNode* thisNode = new MerkleNode(hash);
		thisNode->left = lChild;
		thisNode->right = rChild;

		return thisNode;
	} else {
	// not leaf and not ancestor of leaf
		return NULL;
	}
}

MerkleTree::MerkleTree(vector<MerkleTree::ItemType> items) {
	if (items.empty()) throw "Attempting to construct empty Merkle Tree.";

	// log2 rounded up is height of tree
	size_t treeHeight = std::ceil(log2(items.size())) + 1;
	this->head = createTree(treeHeight, items);
}

MerkleTree::~MerkleTree() {
	// TODO use smart pointers
	delete this->head;
}

MerkleNode::HashArray MerkleTree::getMerkleRoot() const {
	return this->head->getHash();
}

bool MerkleTree::itemExists(ItemType item) const {
	MerkleNode::HashArray digest = itemHash(item);

	vector<MerkleNode::HashArray> tmp;
	return this->head->findItem(digest, tmp);
}

vector<MerkleNode::HashArray> MerkleTree::getMerklePath(ItemType item) const {
	vector<MerkleNode::HashArray> stack;

	// calculate hash of item
	MerkleNode::HashArray digest = itemHash(item);

	// perform DFS to find hash and modify stack to create merkle path
	this->head->findItem(digest, stack);
	return stack;
}

vector<MerkleNode::HashArray> MerkleTree::getHashesAtLevel(size_t level) const {
	vector<MerkleNode::HashArray> hashes;
	this->head->getHashesAtLevel(level, hashes);
	return hashes;
}

